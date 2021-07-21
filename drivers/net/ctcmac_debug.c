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

#define CTCDEBUG_DDR_BASE  0x80000000
#define CTCDEBUG_PKT_LEN 80
#define cpumacu_base ((struct CpuMacUnit_regs *)(CTCMAC_UINIT_BASE+CPUMACUNIT_REG_BASE))

/*
 * bit 0-7     : addr
 * bit 8-15   : data
 * bit 16      : read flag
 * bit 24-27 :ID 
 *                 0 cmu
 *                 3 serdes 0
 *                 4 serdes 1
 * bit 31      : valid
*/
int ctc_mac_hss_write(u8 addr, u8 data, u8 accid)
{
	int ret;
	u32 val = 0;

	accid &= 0xf;
	val = addr | (data<<8) |(accid<<24) | (1<<31);	
	writel(val, &cpumacu_base->CpuMacUnitHssRegAccCtl);

	ret = wait_for_bit(__func__, &cpumacu_base->CpuMacUnitHssRegAccResult,
 			CPU_MAC_UNIT_HSS_REG_ACC_RESULT_W0_HSS_ACC_ACK_MASK, true, 2000, false);
	if(ret){
		printf("wait for write ack CpuMacUnitHssRegAccResult:0x%x fail!\n", 
			readl(&cpumacu_base->CpuMacUnitHssRegAccResult));

		return -1;
	}

	return 0;
}

int ctc_mac_hss_read(u8 addr, u8 *data, u8 accid)
{
	int ret;
	u32 val = 0;

	accid &= 0xf;
	val = addr | (1<<16) |(accid<<24) | (1<<31);

	writel(val, &cpumacu_base->CpuMacUnitHssRegAccCtl);
	
	ret = wait_for_bit(__func__, &cpumacu_base->CpuMacUnitHssRegAccResult,
 			CPU_MAC_UNIT_HSS_REG_ACC_RESULT_W0_HSS_ACC_ACK_MASK , true, 2000, false);
	if(ret){
		printf("wait for read ack CpuMacUnitHssRegAccResult:0x%x fail!\n", 
			readl(&cpumacu_base->CpuMacUnitHssRegAccResult));

		*data = 0x0;
		return -1;
	}

	val = readl(&cpumacu_base->CpuMacUnitHssRegAccResult);
	*data = val & 0xff;
	
	return 0;
}

static void ctcmac_debug_precfg(struct CpuMac_regs *cpumac_reg)
{
	/*1. reset cpumac core*/
	writel(0x07, &cpumacu_base->CpuMacUnitResetCtl);
	writel(0x00, &cpumacu_base->CpuMacUnitResetCtl);
	mdelay(10);

	/*2. cfg init*/
	setbits_le32(&cpumac_reg->CpuMacInit, CI_INIT);
	udelay(1);

	/*3. gmac init*/

	clrbits_le32(&cpumacu_base->CpuMacUnitRefPulseCfg[1], 
	                          CPU_MAC_UNIT_REF_PULSE_CFG_W1_REF_LINK_PULSE_RST);
	ctc_mac_serdes_init(cpumacu_base, 0);
	
	/* 4. cfg sgmii */
	clrbits_le32(&cpumac_reg->CpuMacSgmiiCfg[0], CPU_MAC_SGMII_CFG_W0_CFG_MII_RX_LINK_FILTER_EN);
	setbits_le32(&cpumac_reg->CpuMacSgmiiCfg[0], CPU_MAC_SGMII_CFG_W0_CFG_TX_EVEN_IGNORE);
	setbits_le32(&cpumac_reg->CpuMacSgmiiCfg[0], CPU_MAC_SGMII_CFG_W0_CFG_FORCE_SIGNAL_DETECT);

	/* 5. cfg swap */
	setbits_le32(&cpumac_reg->CpuMacAxiCfg, CPU_MAC_AXI_CFG_W0_CFG_AXI_RD_D_WORD_SWAP_EN);
	setbits_le32(&cpumac_reg->CpuMacAxiCfg, CPU_MAC_AXI_CFG_W0_CFG_AXI_WR_D_WORD_SWAP_EN);

	/* 6. cfg gmac */
	clrbits_le32(&cpumac_reg->CpuMacGmacCfg[2], CPU_MAC_GMAC_CFG_W2_CFG_TX_STRIP_CRC_EN);

	clrbits_le32(&cpumac_reg->CpuMacSgmiiAutoNegCfg, 
			CPU_MAC_SGMII_AUTO_NEG_CFG_W0_CFG_AN_ENABLE);
	
	ctcmac_mac_enable(cpumac_reg);

}

static void ctcmac_get_pkt_stats(struct CpuMac_mems *cpumac_mem, struct ctcmac_pkt_stats *stats)
{	
	void *addr;

	addr = &cpumac_mem->CpuMacStatsRam0[0];
	stats->rx_good_ucast_bytes      +=   readq((volatile unsigned long long *)addr);
	addr = &cpumac_mem->CpuMacStatsRam0[2];
	stats->rx_good_ucast_pkt         +=   readq((volatile unsigned long long *)addr);
	addr = &cpumac_mem->CpuMacStatsRam1[0];
	stats->rx_good_mcast_bytes     +=   readq((volatile unsigned long long *)addr);
	addr = &cpumac_mem->CpuMacStatsRam1[2];
	stats->rx_good_mcast_pkt         +=   readq((volatile unsigned long long *)addr);
	addr = &cpumac_mem->CpuMacStatsRam2[0];
	stats->rx_good_bcast_bytes      +=   readq((volatile unsigned long long *)addr);
	addr = &cpumac_mem->CpuMacStatsRam2[2];
	stats->rx_good_bcast_pkt          +=   readq((volatile unsigned long long *)addr);

	addr = &cpumac_mem->CpuMacStatsRam20[0];
	stats->tx_ucast_bytes           +=   readq((volatile unsigned long long *)addr);
	addr = &cpumac_mem->CpuMacStatsRam20[2];
	stats->tx_ucast_pkt              +=   readq((volatile unsigned long long *)addr);
	addr = &cpumac_mem->CpuMacStatsRam21[0];
	stats->tx_mcast_bytes           +=   readq((volatile unsigned long long *)addr);
	addr = &cpumac_mem->CpuMacStatsRam21[2];
	stats->tx_mcast_pkt              +=   readq((volatile unsigned long long *)addr);
	addr = &cpumac_mem->CpuMacStatsRam22[0];
	stats->tx_bcast_bytes            +=   readq((volatile unsigned long long *)addr);
	addr = &cpumac_mem->CpuMacStatsRam22[2];
	stats->tx_bcast_pkt               +=   readq((volatile unsigned long long *)addr);
}

int ctcmac_get_mac_status(struct CpuMac_regs *cpumac_reg, struct CpuMac_mems *cpumac_mem, struct ctcmac_status* status)
{
	u32 val;
	
	val = readl(&cpumac_reg->CpuMacSgmiiMon[0]);
	status->link_up = val & CPU_MAC_SGMII_MON_W0_MON_LINK_STATUS;

	ctcmac_get_pkt_stats(cpumac_mem, &status->pkt_stats);
	
	if(!status->link_up){
		status->remote_speed = CTCMAC_SPEED_MAX;
		status->local_tx_speed = CTCMAC_SPEED_MAX;
		status->local_rx_speed = CTCMAC_SPEED_MAX;
		return 0;
	}
	val = readl(&cpumac_reg->CpuMacSgmiiMon[1]);
	if(((val>>10) & 3)==0x0)
	{
		status->remote_speed = CTCMAC_SPEED_10M;
	}
	else if(((val>>10) & 3)==0x1)
	{
		status->remote_speed = CTCMAC_SPEED_100M;
	}
	else if(((val>>10) & 3)==0x2)
	{
		status->remote_speed = CTCMAC_SPEED_1000M;
	}else{
		status->remote_speed = CTCMAC_SPEED_MAX;
	}

	val = readl(&cpumac_reg->CpuMacSgmiiCfg[0]);
	if(((val>>22) & 0x7f)==99)
	{
		status->local_tx_speed = CTCMAC_SPEED_10M;
	}
	else if(((val>>22) & 0x7f)==9)
	{
		status->local_tx_speed = CTCMAC_SPEED_100M;
	}
	else if(((val>>22) & 0x7f)==0)
	{
		status->local_tx_speed = CTCMAC_SPEED_1000M;
	}else{
		status->local_tx_speed = CTCMAC_SPEED_MAX;
	}

	val = readl(&cpumac_reg->CpuMacSgmiiCfg[1]);
	if(((val>>22) & 0x7f)==99)
	{
		status->local_rx_speed = CTCMAC_SPEED_10M;
	}
	else if(((val>>22) & 0x7f)==9)
	{
		status->local_rx_speed = CTCMAC_SPEED_100M;
	}
	else if(((val>>22) & 0x7f)==0)
	{
		status->local_rx_speed = CTCMAC_SPEED_1000M;
	}else{
		status->local_rx_speed = CTCMAC_SPEED_MAX;
	}
	
	return 0;
}

int ctcmac_cfg_internal_loopback_lm1(struct CpuMac_regs *cpumac_reg, u8 serdes_id, u8 enable)
{
	u8 val = 0;
	u8 accid = 0;

	ctcmac_debug_precfg(cpumac_reg);
	mdelay(1000);

	
	if(serdes_id == 0){
		accid = 0x3;
	}else{
		accid = 0x4;
	}
	/*reg0x06[3] cfg_tx2rx_lp_en  = 1'b1*/
	if (ctc_mac_hss_read(0x6, &val, accid) < 0){
		return -1;
	}
	if(enable)
		val |= 1<<3;
	else
		val &= ~(1<<3);
	if (ctc_mac_hss_write(0x6, val, accid)){
		return -1;
	}
	
	/*reg0x0e[5] cfg_txlb_en        = 1'b1*/
	/*reg0x0e[4] cfg_rxlb_en        = 1'b1*/
	if (ctc_mac_hss_read(0xe, &val, accid) < 0){
		return -1;
	}
	if(enable)
		val |= (3<<4);
	else
		val &= ~(3<<4);
	if (ctc_mac_hss_write(0xe, val, accid)){
		return -1;
	}

	return 0;
}

int ctcmac_cfg_external_loopback_ls2(struct CpuMac_regs *cpumac_reg, u8 serdes_id, u8 enable)
{
	u8 val = 0;
	u8 accid = 0;

	ctcmac_debug_precfg(cpumac_reg);
	mdelay(1000);

	if(serdes_id == 0){
		accid = 0x3;
	}else{
		accid = 0x4;
	}
	/*reg0x06[4] cfg_rx2tx_lp_en  = 1'b1*/
	if (ctc_mac_hss_read(0x6, &val, accid) < 0){
		return -1;
	}
	if(enable)
		val |= (1<<4);
	else
		val &= ~(1<<4);
	if (ctc_mac_hss_write(0x6, val, accid)){
		return -1;
	}
	
	/*reg0x0e[4] cfg_rxlb_en        = 1'b1*/
	if (ctc_mac_hss_read(0xe, &val, accid) < 0){
		return -1;
	}
	if(enable)
		val |= (1<<4);
	else
		val &= ~(1<<4);
	if (ctc_mac_hss_write(0xe, val, accid)){
		return -1;
	}

	/*reg0x13[2] cfg_cdrck_en        = 1'b1*/
	if (ctc_mac_hss_read(0x13, &val, accid) < 0){
		return -1;
	}
	if(enable)
		val |= (1<<2);
	else
		val &= ~(1<<2);
	if (ctc_mac_hss_write(0x13, val, accid)){
		return -1;
	}
	
	return 0;
}

int ctcmac_set_ffe(u8 serdes_id, u16 coefficient[])
{
	u32 val;
	struct CpuMac_regs *cpumac_reg;

	if(serdes_id == 0)
		cpumac_reg = (struct CpuMac_regs *)CTCMAC_0_BASE;
	else
		cpumac_reg = (struct CpuMac_regs *)CTCMAC_1_BASE;
	
	ctcmac_debug_precfg(cpumac_reg);
	mdelay(1000);

	if(serdes_id == 0){
		val = readl(&cpumacu_base->CpuMacUnitHssCfg[3]);
		val |= BIT(CPU_MAC_UNIT_HSS_CFG_W3_CFG_HSS_L0_PCS_EN_ADV_BIT)
			| BIT(CPU_MAC_UNIT_HSS_CFG_W3_CFG_HSS_L0_PCS_EN_DLY_BIT);
		val &= ~CPU_MAC_UNIT_HSS_CFG_W3_CFG_HSS_L0_PCS_TAP_ADV_MASK;
		val |= (coefficient[0]<<CPU_MAC_UNIT_HSS_CFG_W3_CFG_HSS_L0_PCS_TAP_ADV_BIT);
		val &= ~CPU_MAC_UNIT_HSS_CFG_W3_CFG_HSS_L0_PCS_TAP_DLY_MASK;
		val |= (coefficient[2]<<CPU_MAC_UNIT_HSS_CFG_W3_CFG_HSS_L0_PCS_TAP_DLY_BIT);
		writel(val, &cpumacu_base->CpuMacUnitHssCfg[3]);
		val = readl(&cpumacu_base->CpuMacUnitHssCfg[6]);
		val &= ~CPU_MAC_UNIT_HSS_CFG_W6_CFG_HSS_L0_PCS2_PMA_TX_MARGIN_MASK;
		val |= (coefficient[1]<<CPU_MAC_UNIT_HSS_CFG_W6_CFG_HSS_L0_PCS2_PMA_TX_MARGIN_BIT);
		writel(val, &cpumacu_base->CpuMacUnitHssCfg[6]);
	}else{
		val = readl(&cpumacu_base->CpuMacUnitHssCfg[9]);
		val |= BIT(CPU_MAC_UNIT_HSS_CFG_W9_CFG_HSS_L1_PCS_EN_ADV_BIT)
			| BIT(CPU_MAC_UNIT_HSS_CFG_W9_CFG_HSS_L1_PCS_EN_DLY_BIT);
		val &= ~CPU_MAC_UNIT_HSS_CFG_W9_CFG_HSS_L1_PCS_TAP_ADV_MASK;
		val |= (coefficient[0]<<CPU_MAC_UNIT_HSS_CFG_W9_CFG_HSS_L1_PCS_TAP_ADV_BIT);
		val &= ~CPU_MAC_UNIT_HSS_CFG_W9_CFG_HSS_L1_PCS_TAP_DLY_MASK;
		val |= (coefficient[2]<<CPU_MAC_UNIT_HSS_CFG_W9_CFG_HSS_L1_PCS_TAP_DLY_BIT);
		writel(val, &cpumacu_base->CpuMacUnitHssCfg[9]);
		val = readl(&cpumacu_base->CpuMacUnitHssCfg[12]);
		val &= ~CPU_MAC_UNIT_HSS_CFG_W12_CFG_HSS_L1_PCS2_PMA_TX_MARGIN_MASK;
		val |= (coefficient[1]<<CPU_MAC_UNIT_HSS_CFG_W12_CFG_HSS_L1_PCS2_PMA_TX_MARGIN_BIT);
		writel(val, &cpumacu_base->CpuMacUnitHssCfg[12]);
	}

	return 0;
	
}

int ctcmac_get_ffe(u8 serdes_id, u16 coefficient[])
{
	u32 val;
	
	if(serdes_id == 0){
		val = readl(&cpumacu_base->CpuMacUnitHssCfg[3]);
		coefficient[0] = (val&CPU_MAC_UNIT_HSS_CFG_W3_CFG_HSS_L0_PCS_TAP_ADV_MASK) 
			>> CPU_MAC_UNIT_HSS_CFG_W3_CFG_HSS_L0_PCS_TAP_ADV_BIT;
		coefficient[2] = (val &  CPU_MAC_UNIT_HSS_CFG_W3_CFG_HSS_L0_PCS_TAP_DLY_MASK) 
			>>CPU_MAC_UNIT_HSS_CFG_W3_CFG_HSS_L0_PCS_TAP_DLY_BIT;
		val = readl(&cpumacu_base->CpuMacUnitHssCfg[6]);
		coefficient[1] = (val &  CPU_MAC_UNIT_HSS_CFG_W6_CFG_HSS_L0_PCS2_PMA_TX_MARGIN_MASK) 
			>>CPU_MAC_UNIT_HSS_CFG_W6_CFG_HSS_L0_PCS2_PMA_TX_MARGIN_BIT;

	}else{
		val = readl(&cpumacu_base->CpuMacUnitHssCfg[9]);
		coefficient[0] = (val&CPU_MAC_UNIT_HSS_CFG_W9_CFG_HSS_L1_PCS_TAP_ADV_MASK) 
			>> CPU_MAC_UNIT_HSS_CFG_W9_CFG_HSS_L1_PCS_TAP_ADV_BIT;
		coefficient[2] = (val &  CPU_MAC_UNIT_HSS_CFG_W9_CFG_HSS_L1_PCS_TAP_DLY_MASK) 
			>>CPU_MAC_UNIT_HSS_CFG_W9_CFG_HSS_L1_PCS_TAP_DLY_BIT;
		val = readl(&cpumacu_base->CpuMacUnitHssCfg[12]);
		coefficient[1] = (val &  CPU_MAC_UNIT_HSS_CFG_W12_CFG_HSS_L1_PCS2_PMA_TX_MARGIN_MASK)
			>>CPU_MAC_UNIT_HSS_CFG_W12_CFG_HSS_L1_PCS2_PMA_TX_MARGIN_BIT;
	}

	return 0;
	
}

int ctcmac_sendpkt(struct CpuMac_regs *cpumac_reg, struct CpuMac_mems *cpumac_mem)
{
	u32 value, count;
	u32 desc_cfg_low, desc_cfg_high;
	u8 pkt[CTCDEBUG_PKT_LEN] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x08, 0x00, 0x45, 0x00,
		0x01, 0x23, 0x45, 0x67, 0x89, 0x98, 0x76, 0x54, 0x32, 0x10, 0x40, 0x00, 0x22, 0xf5, 0xc7, 0xc7,
		0xc7, 0x01, 0xc8, 0x01, 0x01, 0x01, 0xaa, 0xaa, 0xaa, 0xdd, 0xaa, 0xaa, 0xaa, 0xdd, 0xaa, 0xaa,
		0xaa, 0xdd, 0xaa, 0xaa, 0xaa, 0xdd, 0xaa, 0xaa, 0xaa, 0xdd, 0xaa, 0xaa, 0xaa, 0xdd, 0xaa, 0xaa,
		0xaa, 0xdd, 0xaa, 0xaa, 0xaa, 0xdd, 0xaa, 0xaa, 0xaa, 0xdd, 0xaa, 0xaa, 0xaa, 0xdd, 0xaa, 0xaa
	};

	flush_dcache_range((unsigned long)pkt, (unsigned long)pkt + CTCDEBUG_PKT_LEN);

	/*1. recycle free tx desc*/
	value = readl(&cpumac_reg->CpuMacDescMon[2]);
	count = ((value >>16)&0xffff);

	while(count != 0)
	{
		/*recycle one by one*/
		readl(&cpumac_mem->CpuMacDescIntf2[0]);
		readl(&cpumac_mem->CpuMacDescIntf2[1]);
		count--;
	}

	/*2. check block number in fifo*/
	value = readl(&cpumac_reg->CpuMacDescMon[1]);
	if(value & 0xffff)
	{
		printf("Error! Tx channel is blocked!\n");
		return -1;
	}
	/*3 triger DMA to transmit packet.*/
	desc_cfg_low = (u32)((u64)pkt - CTCDEBUG_DDR_BASE);
	desc_cfg_high = (u32)((((u64)pkt- CTCDEBUG_DDR_BASE)>>32)&0xff) | CDC_SOP | CDC_EOP | (CTCDEBUG_PKT_LEN<<8);
	writel(desc_cfg_low, &cpumac_mem->CpuMacDescIntf2[0]);
	writel(desc_cfg_high, &cpumac_mem->CpuMacDescIntf2[1]);

	return 0;
	
}

