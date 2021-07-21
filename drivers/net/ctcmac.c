/*
 *Centec networks ethernet controller driver
 *
 *author Jqiu
 */
#include <common.h>
#include <dm.h>
#include <linux/err.h>
#include <malloc.h>
#include <miiphy.h>
#include <net.h>
#include <asm/io.h>
#include <linux/iopoll.h>
#include <wait_bit.h>
#include <ctcmac.h>
#include <switch.h>

DECLARE_GLOBAL_DATA_PTR;

#define CTC_DEBUG
#ifdef CTC_DEBUG
#define CTC_D(fmt,args...)	printf(fmt ,##args)
#else
#define CTC_D(fmt,args...)
#endif

#define CTC_GMAC0_NAME "ctcgmac0"
#define CTC_GMAC1_NAME "ctcgmac1"
#define CTC_CPU_MDIO_NAME0 "ctc_cpu_mdio0"
#define CTC_CPU_MDIO_NAME1 "ctc_cpu_mdio1"

/*channel 0: low priority recv queue*/
#define USE_LOW_RCV_QUEUE
/*channel 1: high priority recv queue*/
//#define USE_HIGH_RCV_QUEUE

#define CTC_DDR_BASE  0x80000000
#define CTC_BUFF_ALIGN  256
#define CTC_RX_BUFF_SIZE 2048
uchar *ctcmac_rx_packets[PKTBUFSRX];

#ifndef CONFIG_DM_ETH
static struct ctcmac_info_struct ctcmac_info[CTCMAC_NUM] = 
{
	{
		.id = 0,
		.dev_name = CTC_GMAC0_NAME,
		.mdio_name = CTC_CPU_MDIO_NAME,
		.reg_base = CTCMAC_0_BASE,
		.phy_addr = 0x10        
	},
	{
		.id = 1,
		.dev_name = CTC_GMAC1_NAME,
		.mdio_name = CTC_CPU_MDIO_NAME,    
		.reg_base = CTCMAC_1_BASE,
		.phy_addr = 2        
	},
};
#endif

static int _ctc_mdio_read(struct mii_dev *bus, int phyaddr, int devad, int reg)
{
	int ret = 0;
	u32 cmd = 0;
	u32 status;
	int value = 0;
	struct ctcmac_private *priv = (struct ctcmac_private *)bus->priv;

	cmd = CTCMAC_MDIO_CMD_REGAD(reg) | CTCMAC_MDIO_CMD_PHYAD(phyaddr) 
	    | CTCMAC_MDIO_CMD_OPCODE(2);

	writel(cmd, &priv->mdio_reg->MdioSocCmd0[0]);    
	writel(1, &priv->mdio_reg->MdioSocCmd0[1]);

	ret = readl_poll_timeout(&priv->mdio_reg->MdioSocStatus0, 
	    status, status & CTCMAC_MDIO_STAT(1), 10000);
	if(ret < 0)
	{
		debug("ctcmac mdio read fail 0x%x!\n", status);
		return -1;
	}

	value = (readl(&priv->mdio_reg->MdioSocStatus0) & 0xffff);

	return value;
}

static int _ctc_mdio_write(struct mii_dev *bus, int phyaddr, int devad, int reg, u16 value)
{
	int ret = 0;
	u32 cmd = 0;
	u32 tmp = 0;
	struct ctcmac_private *priv = (struct ctcmac_private *)bus->priv;

	cmd = CTCMAC_MDIO_CMD_REGAD(reg) | CTCMAC_MDIO_CMD_PHYAD(phyaddr) 
	    | CTCMAC_MDIO_CMD_OPCODE(1) | CTCMAC_MDIO_CMD_DATA(value);

	writel(cmd, &priv->mdio_reg->MdioSocCmd0[0]);    
	writel(1, &priv->mdio_reg->MdioSocCmd0[1]);

	ret = readl_poll_timeout(&priv->mdio_reg->MdioSocStatus0, 
	    tmp, tmp & CTCMAC_MDIO_STAT(1), 10000); 

	if(ret < 0)
	{
		debug("ctcmac mdio write fail!\n");
		return -1;
	}


	return 0;
}

static int _ctc_mdio_reset(struct mii_dev *bus)
{
	struct ctcmac_private *priv = (struct ctcmac_private *)bus->priv;

#if 0
	writel(1, CONFIG_SYSCTRL_BASE + CTCMAC_SYSCTRL_MDIO_RESET);
	udelay(1000);
	writel(0, CONFIG_SYSCTRL_BASE + CTCMAC_SYSCTRL_MDIO_RESET);
#endif

	writel(0x91f, &priv->mdio_reg->MdioSocCfg0);

	return 0;
}

static int ctcmac_maximize_margin_of_cmu_tempearture_ramp(void)
{
	u8 val, ctune, delta, ctune_cal;
	int tmpr = 0;

	tmpr = get_switch_temperature();
	if (0xffff == tmpr){
		printf("get temperature fail!\n");
		return -1;
	}

	ctc_mac_hss_read(0x1c, &val, 0);
	val &= 0xf8;
	val |= 0x4;
	ctc_mac_hss_write(0x1c, val, 0);
	
	/*r_pll_dlol_en  0x30[0] write 1    enable pll lol status output*/
	ctc_mac_hss_read(0x30, &val, 0);
	val |= BIT(0);
	ctc_mac_hss_write(0x30, val, 0);

	if(tmpr <= -20){
		delta = 2;	
	}else if(tmpr <= 60){
		delta = 1;
	}else{
		delta = 0;
	}
	/*read_vco_ctune     0xe0[3:0] read ctune raw value*/
	ctc_mac_hss_read(0xe0, &val, 0);
	ctune = val & 0xf;
	ctune_cal = ctune - delta;
	/*cfg_vco_byp_ctune  0x07[3:0] write (ctune - delta)*/
	ctc_mac_hss_read(0x7, &val, 0);
	val &= 0xf0;
	val |= ctune_cal;
	ctc_mac_hss_write(0x7, val, 0);
	
	/*cfg_vco_cal_byp    0x06[7]   write 1*/
	ctc_mac_hss_read(0x6, &val, 0);
	val |= BIT(7);
	ctc_mac_hss_write(0x6, val, 0);

	 /*for temperature -40~-20C, try (ctune-1) if (ctune-2) causes lol*/
	mdelay(10);
	 /*pll_lol_udl        0xe0[4]   read 0*/
	val = ctc_mac_hss_read(0xe0, &val, 0);
	if((0 != (val & BIT(4))) && (delta == 2)){
		/*cfg_vco_byp_ctune  0x07[3:0] write (ctune - 1)*/
		ctune_cal = ctune - 1;
		ctc_mac_hss_read(0x7, &val, 0);
		val &= 0xf0;
		val |= ctune_cal;
		ctc_mac_hss_write(0x7, val, 0);
	}
	/*check pll lol*/
	mdelay(10);
	/*pll_lol_udl        0xe0[4]   read 0*/
	val = ctc_mac_hss_read(0xe0, &val, 0);
	if(0 != (val & BIT(4))){
		printf("maximize margin of cmu tempearture ramp fail!\n");
		return -1;
	}

	return 0;
}

int ctc_mac_serdes_init(struct CpuMacUnit_regs *cpumacu_reg, u8 dfe_enable)
{
	int ret=0;
	int delay_ms = 1;

	if (dfe_enable){
		/* reset serdes */
		writel(0x4610b003, &cpumacu_reg->CpuMacUnitHssCfg[5]);
		mdelay(delay_ms);
		writel(0x4610b003, &cpumacu_reg->CpuMacUnitHssCfg[11]);
		mdelay(delay_ms);
		writel(0x83806000, &cpumacu_reg->CpuMacUnitHssCfg[0]);
		mdelay(delay_ms);
		writel(0x28061800, &cpumacu_reg->CpuMacUnitHssCfg[2]);
		mdelay(delay_ms);
		writel(0x0066c03a, &cpumacu_reg->CpuMacUnitHssCfg[6]);
		mdelay(delay_ms);
		writel(0x28061810, &cpumacu_reg->CpuMacUnitHssCfg[8]);
		mdelay(delay_ms);
		writel(0x0066c03a, &cpumacu_reg->CpuMacUnitHssCfg[12]);
		mdelay(delay_ms);
	}else{
		/* reset serdes */
		writel(0x4610a805, &cpumacu_reg->CpuMacUnitHssCfg[5]);
		mdelay(delay_ms);
		writel(0x4610a805, &cpumacu_reg->CpuMacUnitHssCfg[11]);
		mdelay(delay_ms);
		writel(0x83806000, &cpumacu_reg->CpuMacUnitHssCfg[0]);
		mdelay(delay_ms);
		writel(0x28061800, &cpumacu_reg->CpuMacUnitHssCfg[2]);
		mdelay(delay_ms);
		writel(0x0026c02a, &cpumacu_reg->CpuMacUnitHssCfg[6]);
		mdelay(delay_ms);
		writel(0x28061810, &cpumacu_reg->CpuMacUnitHssCfg[8]);
		mdelay(delay_ms);
		writel(0x0026c02a, &cpumacu_reg->CpuMacUnitHssCfg[12]);
		mdelay(delay_ms);
	}

	/* offset0 bit1 BlkRstN */
	writel(0x83806002, &cpumacu_reg->CpuMacUnitHssCfg[0]);
	mdelay(delay_ms);

	writel(0x80002309, &cpumacu_reg->CpuMacUnitHssRegAccCtl);
	mdelay(delay_ms);
	writel(0x80000842, &cpumacu_reg->CpuMacUnitHssRegAccCtl);
	mdelay(delay_ms);
	writel(0x8000ea45, &cpumacu_reg->CpuMacUnitHssRegAccCtl);
	mdelay(delay_ms);

	/* serdes 0 init */
	writel(0x83000a05, &cpumacu_reg->CpuMacUnitHssRegAccCtl);
	mdelay(delay_ms);
	writel(0x83002008, &cpumacu_reg->CpuMacUnitHssRegAccCtl);
	mdelay(delay_ms);
	writel(0x8300640f, &cpumacu_reg->CpuMacUnitHssRegAccCtl);
	mdelay(delay_ms);
	writel(0x83000214, &cpumacu_reg->CpuMacUnitHssRegAccCtl);
	mdelay(delay_ms);
	writel(0x83008015, &cpumacu_reg->CpuMacUnitHssRegAccCtl);
	mdelay(delay_ms);
	writel(0x83000116, &cpumacu_reg->CpuMacUnitHssRegAccCtl);
	mdelay(delay_ms);
	writel(0x83001817, &cpumacu_reg->CpuMacUnitHssRegAccCtl);
	mdelay(delay_ms);
	writel(0x83003018, &cpumacu_reg->CpuMacUnitHssRegAccCtl);
	mdelay(delay_ms);
	writel(0x83000e24, &cpumacu_reg->CpuMacUnitHssRegAccCtl);
	mdelay(delay_ms);
	writel(0x83008226, &cpumacu_reg->CpuMacUnitHssRegAccCtl);
	mdelay(delay_ms);
	writel(0x83001f27, &cpumacu_reg->CpuMacUnitHssRegAccCtl);
	mdelay(delay_ms);
	writel(0x83002028, &cpumacu_reg->CpuMacUnitHssRegAccCtl);
	mdelay(delay_ms);
	writel(0x83002829, &cpumacu_reg->CpuMacUnitHssRegAccCtl);
	mdelay(delay_ms);
	writel(0x8300302a, &cpumacu_reg->CpuMacUnitHssRegAccCtl);
	mdelay(delay_ms);
	writel(0x83002038, &cpumacu_reg->CpuMacUnitHssRegAccCtl);
	mdelay(delay_ms);
	writel(0x8300223a, &cpumacu_reg->CpuMacUnitHssRegAccCtl);
	mdelay(delay_ms);
	writel(0x8300523b, &cpumacu_reg->CpuMacUnitHssRegAccCtl);
	mdelay(delay_ms);
	writel(0x83002040, &cpumacu_reg->CpuMacUnitHssRegAccCtl);
	mdelay(delay_ms);
	writel(0x8300f141, &cpumacu_reg->CpuMacUnitHssRegAccCtl);
	mdelay(delay_ms);
	writel(0x8300014a, &cpumacu_reg->CpuMacUnitHssRegAccCtl);
	mdelay(delay_ms);
	writel(0x8300e693, &cpumacu_reg->CpuMacUnitHssRegAccCtl);
	mdelay(delay_ms);

	/* serdes 1 init */
	writel(0x84000a05, &cpumacu_reg->CpuMacUnitHssRegAccCtl);
	mdelay(delay_ms);
	writel(0x84002008, &cpumacu_reg->CpuMacUnitHssRegAccCtl);
	mdelay(delay_ms);
	writel(0x8400640f, &cpumacu_reg->CpuMacUnitHssRegAccCtl);
	mdelay(delay_ms);
	writel(0x84000214, &cpumacu_reg->CpuMacUnitHssRegAccCtl);
	mdelay(delay_ms);
	writel(0x84008015, &cpumacu_reg->CpuMacUnitHssRegAccCtl);
	mdelay(delay_ms);
	writel(0x84000116, &cpumacu_reg->CpuMacUnitHssRegAccCtl);
	mdelay(delay_ms);
	writel(0x84001817, &cpumacu_reg->CpuMacUnitHssRegAccCtl);
	mdelay(delay_ms);
	writel(0x84003018, &cpumacu_reg->CpuMacUnitHssRegAccCtl);
	mdelay(delay_ms);
	writel(0x84000e24, &cpumacu_reg->CpuMacUnitHssRegAccCtl);
	mdelay(delay_ms);
	writel(0x84008226, &cpumacu_reg->CpuMacUnitHssRegAccCtl);
	mdelay(delay_ms);
	writel(0x84001f27, &cpumacu_reg->CpuMacUnitHssRegAccCtl);
	mdelay(delay_ms);
	writel(0x84002028, &cpumacu_reg->CpuMacUnitHssRegAccCtl);
	mdelay(delay_ms);
	writel(0x84002829, &cpumacu_reg->CpuMacUnitHssRegAccCtl);
	mdelay(delay_ms);
	writel(0x8400302a, &cpumacu_reg->CpuMacUnitHssRegAccCtl);
	mdelay(delay_ms);
	writel(0x84002038, &cpumacu_reg->CpuMacUnitHssRegAccCtl);
	mdelay(delay_ms);
	writel(0x8400223a, &cpumacu_reg->CpuMacUnitHssRegAccCtl);
	mdelay(delay_ms);
	writel(0x8400523b, &cpumacu_reg->CpuMacUnitHssRegAccCtl);
	mdelay(delay_ms);
	writel(0x84002040, &cpumacu_reg->CpuMacUnitHssRegAccCtl);
	mdelay(delay_ms);
	writel(0x8400f141, &cpumacu_reg->CpuMacUnitHssRegAccCtl);
	mdelay(delay_ms);
	writel(0x8400014a, &cpumacu_reg->CpuMacUnitHssRegAccCtl);
	mdelay(delay_ms);
	writel(0x8400e693, &cpumacu_reg->CpuMacUnitHssRegAccCtl);
	mdelay(delay_ms);

	ctc_mac_hss_write(0x0c, 0x21, 3);
	ctc_mac_hss_write(0x0f, 0x64, 3);
	ctc_mac_hss_write(0x1a, 0x06, 3);
	ctc_mac_hss_write(0x91, 0x30, 3);
	ctc_mac_hss_write(0x48, 0x20, 3);
	ctc_mac_hss_write(0x90, 0x30, 3);
	//ctc_mac_hss_write(0x9e, 0x36, 3);
	//ctc_mac_hss_write(0x93, 0x76, 3);
	ctc_mac_hss_write(0x14, 0x01, 3);
	ctc_mac_hss_write(0x26, 0x81, 3);

	ctc_mac_hss_write(0x0c, 0x21, 4);
	ctc_mac_hss_write(0x0f, 0x64, 4);
	ctc_mac_hss_write(0x1a, 0x06, 4);
	ctc_mac_hss_write(0x91, 0x30, 4);
	ctc_mac_hss_write(0x48, 0x20, 4);
	ctc_mac_hss_write(0x90, 0x30, 4);
	//ctc_mac_hss_write(0x9e, 0x36, 4);
	//ctc_mac_hss_write(0x93, 0x76, 4);
	ctc_mac_hss_write(0x14, 0x01, 4);
	ctc_mac_hss_write(0x26, 0x81, 4);

	/* serdes post release */
	writel(0x83806003, &cpumacu_reg->CpuMacUnitHssCfg[0]);
	mdelay(delay_ms);
	writel(0x83826003, &cpumacu_reg->CpuMacUnitHssCfg[0]);
	mdelay(delay_ms);
	
	writel(0x28061801, &cpumacu_reg->CpuMacUnitHssCfg[2]);
	mdelay(delay_ms);
	writel(0x28061c01, &cpumacu_reg->CpuMacUnitHssCfg[2]);
	mdelay(delay_ms);
	writel(0x28071c01, &cpumacu_reg->CpuMacUnitHssCfg[2]);
	mdelay(delay_ms);
	
	writel(0x28061811, &cpumacu_reg->CpuMacUnitHssCfg[8]);
	mdelay(delay_ms);
	writel(0x28061c11, &cpumacu_reg->CpuMacUnitHssCfg[8]);
	mdelay(delay_ms);
	writel(0x28071c11, &cpumacu_reg->CpuMacUnitHssCfg[8]);
	mdelay(delay_ms);


	ret = wait_for_bit(__func__, &cpumacu_reg->CpuMacUnitHssMon[1],
 			CPU_MAC_UNIT_HSS_MON_W1_MON_HSS_L0_DFE_RST_DONE_MASK, true, 2000, false);
	if(ret){
		printf("wait for hss reset done fail with CpuMacUnitHssMon[1]:0x%x\n", 
			readl(&cpumacu_reg->CpuMacUnitHssMon[1]));
	}
	mdelay(delay_ms);

	ctcmac_maximize_margin_of_cmu_tempearture_ramp();

	return 0;
}

static int _ctcmac_mac_init(struct ctcmac_private *priv)
{
	/*1. reset cpumac core*/
	writel(0x07, &priv->cpumacu_reg->CpuMacUnitResetCtl);
	writel(0x00, &priv->cpumacu_reg->CpuMacUnitResetCtl);
	mdelay(10);

	/*2. cfg init*/
	setbits_le32(&priv->cpumac_reg->CpuMacInit, CI_INIT);
	udelay(1);

	if(priv->interface == PHY_INTERFACE_MODE_SGMII){
		/*3. gmac init*/

		clrbits_le32(&priv->cpumacu_reg->CpuMacUnitRefPulseCfg[1], 
		                          CPU_MAC_UNIT_REF_PULSE_CFG_W1_REF_LINK_PULSE_RST);
		ctc_mac_serdes_init(priv->cpumacu_reg, priv->dfe_enable);
	}

	/* 3. cfg sgmii */
	clrbits_le32(&priv->cpumac_reg->CpuMacSgmiiCfg[0], CPU_MAC_SGMII_CFG_W0_CFG_MII_RX_LINK_FILTER_EN);
	setbits_le32(&priv->cpumac_reg->CpuMacSgmiiCfg[0], CPU_MAC_SGMII_CFG_W0_CFG_TX_EVEN_IGNORE);

	/* 4. cfg swap */
	setbits_le32(&priv->cpumac_reg->CpuMacAxiCfg, CPU_MAC_AXI_CFG_W0_CFG_AXI_RD_D_WORD_SWAP_EN);
	setbits_le32(&priv->cpumac_reg->CpuMacAxiCfg, CPU_MAC_AXI_CFG_W0_CFG_AXI_WR_D_WORD_SWAP_EN);

	/* 5. cfg gmac */
	clrbits_le32(&priv->cpumac_reg->CpuMacGmacCfg[2], CPU_MAC_GMAC_CFG_W2_CFG_TX_STRIP_CRC_EN);

	return 0;
}

int ctcmac_mac_enable(struct CpuMac_regs *cpumac_reg)
{
	/* 1. release tx/rx reset */
	clrbits_le32(&cpumac_reg->CpuMacReset, CPU_MAC_RESET_W0_SOFT_RST_TX);
	clrbits_le32(&cpumac_reg->CpuMacReset, CPU_MAC_RESET_W0_SOFT_RST_RX);

	return 0;
}

static int _ctcmac_mac_disable(struct CpuMac_regs *cpumac_reg)
{
#if 0
	/* 1. cpumac unit release */
	writel(0x07, &priv->cpumacu_reg->CpuMacUnitResetCtl);
	writel(0x00, &priv->cpumacu_reg->CpuMacUnitResetCtl);
	mdelay(10);
#endif
	/* 2. reset TX/RX */
	setbits_le32(&cpumac_reg->CpuMacReset, CPU_MAC_RESET_W0_SOFT_RST_TX);
	setbits_le32(&cpumac_reg->CpuMacReset, CPU_MAC_RESET_W0_SOFT_RST_RX);

	return 0;
}

static int ctcmac_wait_for_linkup(struct ctcmac_private *priv)
{
	int timeout=3000;
	u32 mon=0;
	
	if(priv->autoneg_mode == CTCMAC_AUTONEG_DISABLE){
		/* wait for linkup */
		while(timeout--){
			mon = readl(&priv->cpumac_reg->CpuMacSgmiiMon[0]);
			if((mon & 0x100) == 0x100){
				break;
			}

			mdelay(1);
		}
		if((mon & CSM_ANST_MASK) != 0x100)
		{
			printf("Error! when phy link up, link status %d is not right.\n", mon);
			return -1;
		}
		
	}else{
		/* wait for sgmii auto nego complete */
		while(timeout--){
			mon = readl(&priv->cpumac_reg->CpuMacSgmiiMon[0]);
			if((mon & CSM_ANST_MASK) == 6){
				break;
			}

			mdelay(1);
		}
		
		if((mon & CSM_ANST_MASK) != 6)
		{
			printf("Error! when phy link up, auto-neg status %d is not right.\n", mon);
			return -1;
		}
	}

	return 0;
}

/*config mac speed depend on phy link status.*/
static int ctcmac_adjust_mac_cfg(struct ctcmac_private *priv, int speed)
{
	u32 value;
	u32 cfg_rep, cfg_smp;

	/* If not SGMII interface, just enable cpumac  */
	if(priv->interface != PHY_INTERFACE_MODE_SGMII){
		ctcmac_mac_enable(priv->cpumac_reg);
		return 0;
	}
	
	if(priv->autoneg_mode == CTCMAC_AUTONEG_DISABLE){
		clrbits_le32(&priv->cpumac_reg->CpuMacSgmiiAutoNegCfg, 
			CPU_MAC_SGMII_AUTO_NEG_CFG_W0_CFG_AN_ENABLE);
		
	}else{
		value = readl(&priv->cpumac_reg->CpuMacSgmiiAutoNegCfg);
		value &= ~CPU_MAC_SGMII_AUTO_NEG_CFG_W0_CFG_AN_MODE_MASK;
		value |= (priv->autoneg_mode<<2 | CPU_MAC_SGMII_AUTO_NEG_CFG_W0_CFG_AN_ENABLE);
		writel(value, &priv->cpumac_reg->CpuMacSgmiiAutoNegCfg);
	}

	/* update SgmiiPcs speed*/
	cfg_rep = readl(&priv->cpumac_reg->CpuMacSgmiiCfg[0]);
	cfg_smp = readl(&priv->cpumac_reg->CpuMacSgmiiCfg[1]);
	cfg_rep &= ~CSC_REP_MASK;
	cfg_smp &= ~CSC_SMP_MASK;
	if(speed == 1000)
	{        
	    cfg_rep |= CSC_1000M;
	    cfg_smp |= CSC_1000M;
	}
	else if(speed == 100)
	{
	    cfg_rep |= CSC_100M;
	    cfg_smp |= CSC_100M;
	}
	else if(speed == 10)
	{
	    cfg_rep |= CSC_10M;
	    cfg_smp |= CSC_10M;
	}
	else
	{
	    debug("Error! unsupport speed %d\n", speed);
	    return -1;
	}
	
	writel(cfg_rep, &priv->cpumac_reg->CpuMacSgmiiCfg[0]);
	writel(cfg_smp, &priv->cpumac_reg->CpuMacSgmiiCfg[1]);
	
	/* enable mac */
	ctcmac_mac_enable(priv->cpumac_reg);

	return ctcmac_wait_for_linkup(priv);
}

static int _ctcmac_dma_init(struct ctcmac_private *priv)
{
	int i;
	u32 desc_cfg_low, desc_cfg_high;

	memset(priv->tx_desc, 0, sizeof(struct ctcmac_desc_cfg)*TX_BUF_CNT);
	memset(priv->rx_desc0, 0, sizeof(struct ctcmac_desc_cfg)*PKTBUFSRX);
	memset(priv->rx_desc1, 0, sizeof(struct ctcmac_desc_cfg)*PKTBUFSRX);

#ifdef USE_LOW_RCV_QUEUE
	/*init rx channel 0 to test.*/
	for (i = 0; i < PKTBUFSRX; i++)
	{
		desc_cfg_low = (u32)((u64)ctcmac_rx_packets[i] - CTC_DDR_BASE);
		desc_cfg_high = ((u32)(((u64)ctcmac_rx_packets[i]-CTC_DDR_BASE) >> 32)& 0xff) | (PKTSIZE<<8);
		invalidate_dcache_range((unsigned long)ctcmac_rx_packets[i],
				(unsigned long)ctcmac_rx_packets[i] + PKTSIZE_ALIGN);
		writel(desc_cfg_low, &priv->cpumac_mem->CpuMacDescIntf0[0]);
		writel(desc_cfg_high, &priv->cpumac_mem->CpuMacDescIntf0[1]);   
	}
	priv->rx_curr_idx0 = 0;
#endif    
#ifdef USE_HIGH_RCV_QUEUE
	/*init rx channel 1 to test.*/
	for (i = 0; i < PKTBUFSRX; i++)
	{
		desc_cfg_low = (u32)ctcmac_rx_packets[i] - CTC_DDR_BASE;
		desc_cfg_high = ((u32)(((u64)ctcmac_rx_packets[i]-CTC_DDR_BASE) >> 32)& 0xff) | (PKTSIZE<<8);
		invalidate_dcache_range((unsigned long)ctcmac_rx_packets[i],
				(unsigned long)ctcmac_rx_packets[i] + PKTSIZE_ALIGN);
		writel(desc_cfg_low, &priv->cpumac_mem->CpuMacDescIntf1[0]);
		writel(desc_cfg_high, &priv->cpumac_mem->CpuMacDescIntf1[1]);  
	}
	priv->rx_curr_idx1 = 0;
#endif

	return 0;
}

static int _ctcmac_disable_dma(struct ctcmac_private *priv)
{
	/*polling all tx/rx desc to empty.*/
	return 0;
}

int ctc_mdio_init(struct ctcmac_private *priv)
{
	int retval;
	struct mii_dev *mdiodev = mdio_alloc();

	if (!mdiodev)
		return -ENOMEM;
	strncpy(mdiodev->name, priv->mdio_name, MDIO_NAME_LEN);
	mdiodev->read  = _ctc_mdio_read;
	mdiodev->write = _ctc_mdio_write;
	mdiodev->reset = _ctc_mdio_reset;
	mdiodev->priv  = priv;

	retval = mdio_register(mdiodev);
	if (retval < 0)
		return retval;

	return 0;
}

 /* Discover PHY depend on PHY ID, and init configuration. */
static int _init_phy(struct ctcmac_private *priv)
{
	u32 supported;
	struct phy_device *phydev;

	if (priv->interface == PHY_INTERFACE_MODE_SGMII){
		supported = CTCMAC_SUPPORTED;
	}else{
		supported = SUPPORTED_10baseT_Full;
	}
	
	phydev = phy_connect(priv->bus, priv->phy_addr, priv->dev, priv->interface);
	if (!phydev)
	{
		printf("In _init_phy, phy_connect failed! Not be recognized. \n");
		return -1;
	}

	phydev->supported &= supported;
	phydev->advertising = phydev->supported;
	priv->phydev = phydev;
	phy_config(phydev);

	return 0;
}

/*Init ctcmac, DMA ready to tx/rx, init phy, cfg mac depend on phy link status.*/ 
#ifndef CONFIG_DM_ETH
static int ctcmac_init(struct eth_device *dev, bd_t * bd)
#else
static int ctcmac_init(struct udevice *dev)
#endif
{
	int ret;
	struct ctcmac_private *priv = (struct ctcmac_private *)dev->priv;
	
	/* init mac*/
	_ctcmac_mac_init(priv);
	/* Init DMA */
	_ctcmac_dma_init(priv);
	/* Start up the PHY, get link status and speed/duplex. */
	ret = phy_startup(priv->phydev);
	if (ret) {
		printf("Could not initialize PHY on %s\n", dev->name);
		return ret;
	}
	/* When linkup, config mac depend on phy status */
	if(priv->phydev->link)
	{
		ctcmac_adjust_mac_cfg(priv, priv->phydev->speed);
	}

	return 0;
}

/*fill packet into DMA tx ptr, send out the packet.*/
#ifndef CONFIG_DM_ETH
static int ctcmac_send(struct eth_device *dev, void *packet, int length)
#else
static int ctcmac_send(struct udevice *dev, void *packet, int length)
#endif
{
	u64 addr;
	void *new_packet = NULL;
	u32 desc_cfg_low, desc_cfg_high;
	u32 value, count;
	struct ctcmac_private *priv = (struct ctcmac_private *)dev->priv;
	
	#define PAGE_MASK ~0xFFF
	addr = (u64)packet;
	if(((u64)packet & PAGE_MASK) != (((u64)packet + length) & PAGE_MASK)){
		new_packet = memalign(CTC_BUFF_ALIGN, length);
		memcpy(new_packet, packet, length);
		addr = (u64)new_packet;
	}

	flush_dcache_range((unsigned long)addr, (unsigned long)addr + PKTSIZE_ALIGN);

	/*1. check block number in fifo*/
	value = readl(&priv->cpumac_reg->CpuMacDescMon[1]);
	if((value & 0xffff)>=TX_BUF_CNT)
	{
		printf("Error! Tx channel is blocked!\n");
		return -1;
	}
	
	/*2. triger DMA to transmit packet.*/
	desc_cfg_low = (u32)(addr - CTC_DDR_BASE);
	desc_cfg_high = (u32)(((addr- CTC_DDR_BASE)>>32)&0xff) | CDC_SOP | CDC_EOP | (length<<8);
	writel(desc_cfg_low, &priv->cpumac_mem->CpuMacDescIntf2[0]);
	writel(desc_cfg_high, &priv->cpumac_mem->CpuMacDescIntf2[1]);

	/*3. wait for send done*/
	value = readl(&priv->cpumac_reg->CpuMacDescMon[2]);
	count = ((value >>16)&0xffff);
	
	while(count == 0){
		value = readl(&priv->cpumac_reg->CpuMacDescMon[2]);
		count = ((value >>16)&0xffff);
	}

	readl(&priv->cpumac_mem->CpuMacDescIntf2[0]);
	readl(&priv->cpumac_mem->CpuMacDescIntf2[1]);
	
	if(new_packet)
		free(new_packet);
	
	return 0;
}

/*recv DMA packet, call net API to process it.*/
#ifndef CONFIG_DM_ETH
static int ctcmac_recv(struct eth_device *dev)
{
	u32 value, count, rcvlen, index;
	u32 desc_cfg_low, desc_cfg_high;
	struct ctcmac_private *priv = (struct ctcmac_private *)dev->priv;
#ifdef USE_HIGH_RCV_QUEUE    
	/*rx channel 1 high prio*/
	while (1) 
	{
		/*First process rx channel 1*/
		value = readl(&priv->cpumac_reg->CpuMacDescMon[2]);
		count = (value & 0xffff);
		if(count == 0)
		{
		    break;
		}
		while(count)
		{
			/*get recv packet*/
			index = priv->rx_curr_idx1;
			value = readl(&priv->cpumac_mem->CpuMacDescIntf1[0]);
			value = readl(&priv->cpumac_mem->CpuMacDescIntf1[1]);
			if((value & 0x01000000)||((value &0x00c00000) != 0x00c00000)
			{
				printf("Recv error packet, drop it. desc 0x%x\n", value);
			}
			else
			{                
				rcvlen = (value >> 8)&0x3fff;
				net_process_received_packet(ctcmac_rx_packets[index], rcvlen-4);
			}
			desc_cfg_low = (u32)ctcmac_rx_packets[index];
			desc_cfg_high = ((u32)((u64)ctcmac_rx_packets[index] >> 32)& 0xff) | (PKTSIZE<<8);
			writel(desc_cfg_low, &priv->cpumac_mem->CpuMacDescIntf1[0]);
			writel(desc_cfg_high, &priv->cpumac_mem->CpuMacDescIntf1[1]); 
			priv->rx_curr_idx1 = (index+1)%PKTBUFSRX;
			count--;
		}
	}
#endif
#ifdef USE_LOW_RCV_QUEUE
	while (1) 
	{
		/*First process rx channel 0*/
		value = readl(&priv->cpumac_reg->CpuMacDescMon[1]);
		count = ((value>>16) & 0xffff);
		if(count == 0)
		{
		    break;
		}
	    
		while(count)
		{
			/*get recv packet*/
			index = priv->rx_curr_idx0;
			value = readl(&priv->cpumac_mem->CpuMacDescIntf0[0]);
			value = readl(&priv->cpumac_mem->CpuMacDescIntf0[1]);
			if((value & 0x01000000)||((value &0x00c00000) != 0x00c00000)
			{
				printf("Low queue recv error packet, drop it. desc 0x%x\n", value);
			}
			else
			{                
				rcvlen = (value >> 8)&0x3fff;
				net_process_received_packet(ctcmac_rx_packets[index], rcvlen-4);
			}

			desc_cfg_low = (u32)ctcmac_rx_packets[index];
			desc_cfg_high = ((u32)((u64)ctcmac_rx_packets[index] >> 32)& 0xff) | (PKTSIZE<<8);
			writel(desc_cfg_low, &priv->cpumac_mem->CpuMacDescIntf0[0]);
			writel(desc_cfg_high, &priv->cpumac_mem->CpuMacDescIntf0[1]);           
			priv->rx_curr_idx0 = (index+1)%PKTBUFSRX;
			count--;
		}
	}
#endif    
	return 0;
}
#else
static int ctcmac_recv(struct udevice *dev, int flags, uchar **packetp)
{
	u32 count, rcvlen, value;
	int ret = -1;
	struct ctcmac_private *priv = (struct ctcmac_private *)(dev->priv);
#ifdef USE_HIGH_RCV_QUEUE    
	/*process rx channel 1*/
	value = readl(&priv->cpumac_reg->CpuMacDescMon[2]);
	count = (value & 0xffff);
	if(count != 0)
	{  
		value = readl(&priv->cpumac_mem->CpuMacDescIntf1[0]);
		value = readl(&priv->cpumac_mem->CpuMacDescIntf1[1]);
		if((value & 0x01000000)||((value &0x00c00000) != 0x00c00000))
		{
			printf("Recv error packet, drop it. desc 0x%x\n", value);
			ret = 0;
		}
		else
		{
			invalidate_dcache_range((unsigned long)ctcmac_rx_packets[priv->rx_curr_idx1],
						(unsigned long)ctcmac_rx_packets[priv->rx_curr_idx1] + PKTSIZE_ALIGN);
			*packetp = ctcmac_rx_packets[priv->rx_curr_idx1];
			rcvlen = (value >> 8)&0x3fff;
			ret = rcvlen - 4;
			return ret;
		}
	}
#endif    
#ifdef USE_LOW_RCV_QUEUE
	/*process rx channel 0*/
	value = readl(&priv->cpumac_reg->CpuMacDescMon[1]);
	count = ((value>>16) & 0xffff);
	if(count != 0)
	{  
		value = readl(&priv->cpumac_mem->CpuMacDescIntf0[0]);
		value = readl(&priv->cpumac_mem->CpuMacDescIntf0[1]);
		if((value & 0x01000000)||((value &0x00c00000) != 0x00c00000))
		{
			printf("Recv error packet, drop it. desc 0x%x\n", value);
			ret = 0;
		}
		else
		{     
			invalidate_dcache_range((unsigned long)ctcmac_rx_packets[priv->rx_curr_idx0],
					(unsigned long)ctcmac_rx_packets[priv->rx_curr_idx0] + PKTSIZE_ALIGN);
			*packetp = ctcmac_rx_packets[priv->rx_curr_idx0];
			rcvlen = (value >> 8)&0x3fff;
			ret = rcvlen - 4;
			return ret;
		}    
	}
#endif    
    return ret;
}

static int ctcmac_free_pkt(struct udevice *dev, uchar *packet, int length)
{
	struct ctcmac_private *priv = (struct ctcmac_private *)dev->priv;
	u32 desc_cfg_low, desc_cfg_high;
	u32 count, value;
	u8 index;
        
#ifdef USE_HIGH_RCV_QUEUE
	/*recycle rx desc 1*/
	/*check availnum*/
	value = readl(&priv->cpumac_reg->CpuMacDescMon[0]);
	count = ((value>>16) & 0xffff);
	if(count < PKTBUFSRX)
	{
		index = priv->rx_curr_idx1;
		desc_cfg_low = (u32)ctcmac_rx_packets[index];
		desc_cfg_high = ((u32)((u64)ctcmac_rx_packets[index] >> 32)& 0xff) | (PKTSIZE<<8);
		writel(desc_cfg_low, &priv->cpumac_mem->CpuMacDescIntf1[0]);
		writel(desc_cfg_high, &priv->cpumac_mem->CpuMacDescIntf1[1]); 
		priv->rx_curr_idx1 = (priv->rx_curr_idx1 + 1)%PKTBUFSRX;
	}
	else
	{
		printf("RX channel 1 is blocked!!\n");
	}
#endif
#ifdef USE_LOW_RCV_QUEUE
	/*recycle rx desc 0*/
	/*check availnum*/
	value = readl(&priv->cpumac_reg->CpuMacDescMon[0]);
	count = (value & 0xffff);
	if(count < PKTBUFSRX)
	{
		index = priv->rx_curr_idx0;
		desc_cfg_low = (u32)((u64)ctcmac_rx_packets[index]-CTC_DDR_BASE);
		desc_cfg_high = ((u32)(((u64)ctcmac_rx_packets[index]-CTC_DDR_BASE) >> 32)& 0xff) | (PKTSIZE<<8);
		invalidate_dcache_range((unsigned long)ctcmac_rx_packets[index],
				(unsigned long)ctcmac_rx_packets[index] + PKTSIZE_ALIGN);

		writel(desc_cfg_low, &priv->cpumac_mem->CpuMacDescIntf0[0]);
		writel(desc_cfg_high, &priv->cpumac_mem->CpuMacDescIntf0[1]); 
		priv->rx_curr_idx0 = (priv->rx_curr_idx0 + 1)%PKTBUFSRX;
	}
	else
	{
		printf("RX channel 0 is blocked!!\n");
	}
#endif
    return 0;
}
#endif

/*Stop interface.*/
#ifndef CONFIG_DM_ETH
static void ctcmac_halt(struct eth_device *dev)
#else
static void ctcmac_halt(struct udevice *dev)
#endif
{
	struct ctcmac_private *priv = (struct ctcmac_private *)dev->priv;
	/* Shut down the PHY, as needed */
	phy_shutdown(priv->phydev);
	/*disable mac*/   
	_ctcmac_mac_disable(priv->cpumac_reg);
	/*disable dma*/
	_ctcmac_disable_dma(priv);        

	return ;
}
#ifndef CONFIG_DM_ETH
static int ctcmac_register(bd_t *bis, struct ctcmac_info_struct* ctcmac_info)
{
	struct eth_device *dev;	
	struct ctcmac_private *priv;
	int i;

	dev = (struct eth_device *)malloc(sizeof *dev);
	if (NULL == dev)
	{
		printf("%s: Malloc dev fail, not enough memory!\n", __func__);
		return -1;
	}
	memset(dev, 0, sizeof(*dev));
	priv = (struct ctcmac_private *)malloc(sizeof(*priv));
	if (NULL == priv)
	{        
		printf("%s: Malloc priv fail, not enough memory!\n", __func__);
		free(dev);
		return -1;
	}
	memset(priv, 0, sizeof(*priv));
	/*Init priv data*/
	priv->mdio_base = ctcmac_info->mdio_base;
	priv->phy_addr = ctcmac_info->phy_addr;
	priv->cpumac_reg = ctcmac_info->reg_base;
	priv->cpumac_mem = ctcmac_info->mem_base;
	priv->id = ctcmac_info->id;
	priv->bus = miiphy_get_dev_by_name(ctcmac_info->mdio_name);
	priv->dev = dev;

	/*register dev*/
	strcpy(dev->name, ctcmac_info->devname);
	dev->iobase = 0;    
	dev->priv = (void *)priv;
	dev->init = ctcmac_init;
	dev->halt = ctcmac_halt;
	dev->send = ctcmac_send;
	dev->recv = ctcmac_recv;
	for (i = 0; i < 6; i++)
		dev->enetaddr[i] = 0;    
	eth_register(dev);
	  
	/* gmac cfg */


	/* initialize PHY */
	return _init_phy(priv);
}

/*called by board init*/
int ctcmac_eth_init(bd_t *bis)
{
	int ret, i, count=0;
	ret = 0;

	/* init mdio */
	ret = ctc_mdio_init();
	if(ret < 0)
	{
		debug("ctc_mdio_init fail, ret %d\n", ret);
		return -1;
	}

	/* init cpu mac */
	for(i=0; i<CTCMAC_NUM; i++)
	{
		ret = ctcmac_register(bis, ctcmac_info[i]);
		if(ret == 0)
		{
			count ++;
		}
	}

	return ret;
}
#else
int ctcmac_alloc_rx_buff(void)
{
	int i = 0;

	for(i=0; i<PKTBUFSRX; i++){
		ctcmac_rx_packets[i] = memalign(CTC_BUFF_ALIGN, CTC_RX_BUFF_SIZE);
		if(!ctcmac_rx_packets[i])
			return -1;
		ctcmac_rx_packets[i] = (uchar *)((u64)ctcmac_rx_packets[i]+CTC_BUFF_ALIGN);
	}
	return 0;
}
int ctcmac_probe(struct udevice *dev)
{
	const char *ctype, *automode, *dfe;
	struct ctcmac_private *priv = dev_get_priv(dev);
	struct eth_pdata *pdata = dev_get_platdata(dev);
	int offset = 0;
	uint reg;
	int ret;

	priv->dev = dev;
	pdata->iobase     = (phys_addr_t)dev_get_addr_index(dev, 1);
	priv->cpumacu_reg = (struct CpuMacUnit_regs *)(pdata->iobase+CPUMACUNIT_REG_BASE);
	pdata->iobase     = (phys_addr_t)dev_get_addr_index(dev, 0);
	priv->cpumac_reg  = (struct CpuMac_regs *)(pdata->iobase+CPUMAC_REG_BASE);
	priv->cpumac_mem  = (struct CpuMac_mems *)(pdata->iobase+CPUMAC_MEM_BASE);
	
	ctype = fdt_getprop(gd->fdt_blob, dev_of_offset(dev), "phy-connection-type", NULL);
	if(ctype && !strncmp(ctype, "mii", 3)){
		priv->interface = PHY_INTERFACE_MODE_MII;
	}else{
		priv->interface = PHY_INTERFACE_MODE_SGMII;
	}
	
	automode = fdt_getprop(gd->fdt_blob, dev_of_offset(dev), "auto-nego-mode", NULL);
	if(automode && !strncmp(automode, "disable", 7)){
		priv->autoneg_mode = CTCMAC_AUTONEG_DISABLE;
	}else if(automode && !strncmp(automode, "sgmii-mac", 9)){
		priv->autoneg_mode = CTCMAC_AUTONEG_MAC_M;
	}else if(automode && !strncmp(automode, "sgmii-phy", 9)){
		priv->autoneg_mode = CTCMAC_AUTONEG_PHY_M;
	}else if(automode && !strncmp(automode, "1000base-x", 10)){
		priv->autoneg_mode = CTCMAC_AUTONEG_1000BASEX_M;
	}else{
		priv->autoneg_mode = CTCMAC_AUTONEG_MAC_M;
	}

	dfe = fdt_getprop(gd->fdt_blob, dev_of_offset(dev), "dfe", NULL);
	if(dfe && !strncmp(dfe, "enable", 6)){
		priv->dfe_enable = 1;
	}else{
		priv->dfe_enable = 0;
	}
	
	offset = fdtdec_lookup_phandle(gd->fdt_blob, dev_of_offset(dev), "phy-handle");
	if (offset > 0) {
		priv->phy_addr = fdtdec_get_uint(gd->fdt_blob, offset, "reg", 0);
		priv->mdio_index = fdtdec_get_uint(gd->fdt_blob, offset, "index", 0);
		offset = fdt_parent_offset(gd->fdt_blob, offset);    
		if (offset > 0) {
			reg = fdtdec_get_addr(gd->fdt_blob, offset, "reg");
			priv->mdio_reg = (struct MdioSoc_regs *)reg;
		}
		if(priv->mdio_index == 0)
			priv->mdio_name = CTC_CPU_MDIO_NAME0;
		else
			priv->mdio_name = CTC_CPU_MDIO_NAME1;

		ret = ctc_mdio_init(priv);
		if(ret < 0)
		{
			printf("Mdio init fail %d!\n", ret);
			return -1;
		}
		priv->bus = miiphy_get_dev_by_name(priv->mdio_name);
	}

	if(ctcmac_alloc_rx_buff()){
		printf("cpumac alloc rx buffer fail!\n");
		return -1;
	}
	
	return _init_phy(priv);
}

int ctcmac_remove(struct udevice *dev)
{
	struct ctcmac_private *priv = dev->priv;
	
	free(priv->phydev);
	mdio_unregister(priv->bus);
	mdio_free(priv->bus);

	return 0;
}


static const struct eth_ops ctcmac_ops = {
	.start    = ctcmac_init,
	.send     = ctcmac_send,
	.recv     = ctcmac_recv,
	.free_pkt = ctcmac_free_pkt,
	.stop     = ctcmac_halt,
};


static const struct udevice_id ctcmac_ids[] = {
	{ .compatible = "ctc,mac" },
	{ }
};

U_BOOT_DRIVER(eth_ctcmac) = {
	.name = "ctc_mac",
	.id = UCLASS_ETH,
	.of_match = ctcmac_ids,
	.probe = ctcmac_probe,
	.remove = ctcmac_remove,
	.ops = &ctcmac_ops,
	.priv_auto_alloc_size = sizeof(struct ctcmac_private),
	.platdata_auto_alloc_size = sizeof(struct eth_pdata),
	.flags = DM_FLAG_ALLOC_PRIV_DMA,
};
#endif

