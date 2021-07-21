/*
 * Centec I2C driver
 *
 * Author: Wangyb <wangyb@centecnetworks.com>
 *
 * Copyright 2005-2019, Centec Networks (Suzhou) Co., Ltd.
 *
 */
 
#include <common.h>
#include <dm.h>
#include <i2c.h>
#include <asm/io.h>
#include "ctc_i2c.h"

DECLARE_GLOBAL_DATA_PTR;

struct ctc_scl_sda_cfg {
	u32 ss_hcnt;
	u32 fs_hcnt;
	u32 ss_lcnt;
	u32 fs_lcnt;
	u32 sda_hold;
};

struct ctc_i2c {
	struct i2c_regs *regs;
	struct ctc_scl_sda_cfg *scl_sda_cfg;
};

static void ctc_i2c_enable(struct i2c_regs *i2c_base, bool enable)
{
	u32 ena = enable ? IC_ENABLE_0B : 0;
	int timeout = 100;

	do {
		writel(ena, &i2c_base->ic_enable);
		if ((readl(&i2c_base->ic_enable_status) & IC_ENABLE_0B) == ena)
			return;

		/*
		 * Wait 10 times the signaling period of the highest I2C
		 * transfer supported by the driver (for 400KHz this is
		 * 25us) as described in the DesignWare I2C databook.
		 */
		udelay(25);

	} while (timeout--);

	debug("timeout in %sabling I2C adapter\n", enable ? "en" : "dis");
}

/*
 * htime = (hcnt + 14 + 4) * 20ns;
 * ltime = (lcnt + 1 + 2) * 20ns;
 * 100khz = 10 us
 * 400khz = 2.5us
 */
static int ctc_calc_cnt(int i2c_spd)
{	int cnt;

	switch (i2c_spd){
		case IC_SPEED_MODE_STANDARD :
			cnt = 10 * 1000 / 20 / 2;
			break;
		case IC_SPEED_MODE_FAST :
			cnt = 2.5 * 1000 / 20 / 2 + 1;
			break;
		default:
			cnt = 0;
			printf("ctc i2c have no this i2c speed\n");
			break;
	}
	return cnt;
}

static unsigned int __ctc_i2c_set_bus_speed(struct i2c_regs *i2c_base,
					   struct ctc_scl_sda_cfg *scl_sda_cfg,
					   unsigned int speed)
{
	unsigned int cntl;
	unsigned int hcnt, lcnt;
	int i2c_spd;

	if (speed >= I2C_FAST_SPEED)
		i2c_spd = IC_SPEED_MODE_FAST;
	else
		i2c_spd = IC_SPEED_MODE_STANDARD;

	/* to set speed cltr must be disabled */
	ctc_i2c_enable(i2c_base, false);

	cntl = (readl(&i2c_base->ic_con) & (~IC_CON_SPD_MSK));

	switch (i2c_spd) {
	case IC_SPEED_MODE_STANDARD:
		cntl |= IC_CON_SPD_SS;
		if (scl_sda_cfg) {
			hcnt = scl_sda_cfg->ss_hcnt;
			lcnt = scl_sda_cfg->ss_lcnt;
		} else {
			hcnt = ctc_calc_cnt(IC_SPEED_MODE_STANDARD)-14-2;
			lcnt = ctc_calc_cnt(IC_SPEED_MODE_STANDARD)-1;
		}
		debug("CTC_DEBUG, SS, hcnt is %d, lcnt is %d, cntl is 0x%x\n", hcnt, lcnt, cntl);
		writel(hcnt, &i2c_base->ic_ss_scl_hcnt);
		writel(lcnt, &i2c_base->ic_ss_scl_lcnt);
		break;

	case IC_SPEED_MODE_FAST:
	default:
		cntl |= IC_CON_SPD_FS;
		if (scl_sda_cfg) {
			hcnt = scl_sda_cfg->fs_hcnt;
			lcnt = scl_sda_cfg->fs_lcnt;
		} else {
			hcnt = ctc_calc_cnt(IC_SPEED_MODE_FAST)-14-4; 
			lcnt = ctc_calc_cnt(IC_SPEED_MODE_FAST)-1-2; 
		}
		debug("CTC_DEBUG, FS, hcnt is %d, lcnt is %d, cntl is 0x%x\n", hcnt, lcnt, cntl);
		writel(hcnt, &i2c_base->ic_fs_scl_hcnt);
		writel(lcnt, &i2c_base->ic_fs_scl_lcnt);
		break;
	}

	writel(cntl, &i2c_base->ic_con);

	/* Configure SDA Hold Time if required */
	if (scl_sda_cfg)
		writel(scl_sda_cfg->sda_hold, &i2c_base->ic_sda_hold);

	/* Enable back i2c now speed set */
	ctc_i2c_enable(i2c_base, true);

	return 0;
}

static void ctc_i2c_setaddress(struct i2c_regs *i2c_base, unsigned int i2c_addr)
{
	/* Disable i2c */
	ctc_i2c_enable(i2c_base, false);

	writel(i2c_addr, &i2c_base->ic_tar);

	/* Enable i2c */
	ctc_i2c_enable(i2c_base, true);
}

static void ctc_i2c_flush_rxfifo(struct i2c_regs *i2c_base)
{
	while (readl(&i2c_base->ic_status) & IC_STATUS_RFNE)
		readl(&i2c_base->ic_cmd_data);
}

static int ctc_i2c_wait_for_bb(struct i2c_regs *i2c_base)
{
	unsigned long start_time_bb = get_timer(0);

	while ((readl(&i2c_base->ic_status) & IC_STATUS_MA) ||
	       !(readl(&i2c_base->ic_status) & IC_STATUS_TFE)) {

		/* Evaluate timeout */
		if (get_timer(start_time_bb) > (unsigned long)(I2C_BYTE_TO_BB))
			return 1;
	}
	return 0;
}

static int ctc_i2c_xfer_init(struct i2c_regs *i2c_base, uchar chip)
{
	if (ctc_i2c_wait_for_bb(i2c_base))
		return 1;

	ctc_i2c_setaddress(i2c_base, chip);

	return 0;
}

static int ctc_i2c_xfer_finish(struct i2c_regs *i2c_base)
{
	ulong start_stop_det = get_timer(0);

	while (1) {
		if ((readl(&i2c_base->ic_raw_intr_stat) & IC_STOP_DET)) {
			readl(&i2c_base->ic_clr_stop_det);
			break;
		} else if (get_timer(start_stop_det) > I2C_STOPDET_TO) {
			break;
		}
	}
	
	if (ctc_i2c_wait_for_bb(i2c_base)) {
		printf("Timed out waiting for bus\n");
		return 1;
	}

	ctc_i2c_flush_rxfifo(i2c_base);

	return 0;
}

static int __ctc_i2c_read(struct i2c_regs *i2c_base, u8 chip_addr, u8 *offset, 
			int olen, u8 *data, int dlen)
{
	unsigned int active = 0;
	unsigned int flag = 0;
	unsigned long start_time_rx;

	if (ctc_i2c_xfer_init(i2c_base, chip_addr))
		return 1;
	
	if (olen > 0){
		writel(*offset | IC_RESTART, &i2c_base->ic_cmd_data);
		olen --;
		while (olen){
			offset++;
			olen--;
			writel(*offset, &i2c_base->ic_cmd_data);
		}
	}

	start_time_rx = get_timer(0);
	while (dlen) {
		if (!active) {
			if (flag == 0){
				if(dlen == 1){
					writel(IC_CMD | IC_STOP | IC_RESTART, &i2c_base->ic_cmd_data);
				}else{
					writel(IC_CMD | IC_RESTART, &i2c_base->ic_cmd_data);
				}
				flag = 1;
			}
			else if (dlen !=1){
				writel(IC_CMD, &i2c_base->ic_cmd_data);
			}
			else{
				writel(IC_CMD | IC_STOP, &i2c_base->ic_cmd_data);				
			}
			active = 1;	
		}
		if (readl(&i2c_base->ic_status) & IC_STATUS_RFNE) {
			*data++ = (uchar)readl(&i2c_base->ic_cmd_data);
			dlen--;
			start_time_rx = get_timer(0);
			active = 0;
		}else if (get_timer(start_time_rx) > I2C_BYTE_TO) {
			return -ETIMEDOUT;	
		}
	}
	
	return ctc_i2c_xfer_finish(i2c_base);
}

static int __ctc_i2c_write(struct i2c_regs *i2c_base, u8 chip_addr, u8 *offset,
			  int olen, u8 *data, int dlen)
{
	int nb = dlen;	
	unsigned long start_time_tx;
	
	if (ctc_i2c_xfer_init(i2c_base, chip_addr))
		return 1;
	
	start_time_tx = get_timer(0);
	while(dlen) {
		if (readl(&i2c_base->ic_status) & IC_STATUS_TFNF) {
			if(--dlen == 0){
				writel(*data | IC_STOP, &i2c_base->ic_cmd_data);
			}
			else{
				writel(*data, &i2c_base->ic_cmd_data);
			}
			data++;
			start_time_tx = get_timer(0);
			
		} else if (get_timer(start_time_tx) > (nb * I2C_BYTE_TO)) {
			printf("Timed out. i2c write Failed\n");	
			return -ETIMEDOUT;
		}
	}

	return ctc_i2c_xfer_finish(i2c_base);
}

static void __ctc_i2c_init(struct i2c_regs *i2c_base, int speed, int slaveaddr)
{
	/* Disable i2c */
	ctc_i2c_enable(i2c_base, false);

	writel((IC_CON_SD | IC_CON_SPD_SS | IC_CON_MM | IC_CON_RE), &i2c_base->ic_con);
	writel(IC_RX_TL, &i2c_base->ic_rx_tl);
	writel(IC_TX_TL, &i2c_base->ic_tx_tl);
	writel(IC_STOP_DET, &i2c_base->ic_intr_mask);

	/* Enable i2c */
	ctc_i2c_enable(i2c_base, true);
}

static int ctc_i2c_xfer(struct udevice *bus, struct i2c_msg *msg,
			       int nmsgs)
{
	struct ctc_i2c *i2c = dev_get_priv(bus);
	struct i2c_msg *dmsg, *omsg, dummy;

	memset(&dummy, 0, sizeof(struct i2c_msg));
		
	/* We expect either two messages (one with an offset and one with the
	 * actucal data) or one message (just data) */
	if (nmsgs > 2 || nmsgs == 0) {
		debug("%s: Only one or two messages are supported.", __func__);
		return -1;
	}

	omsg = nmsgs == 1 ? &dummy : msg;	
	dmsg = nmsgs == 1 ? msg : msg + 1;	
	
	if (dmsg->flags & I2C_M_RD) 
		return __ctc_i2c_read(i2c->regs, dmsg->addr, omsg->buf, omsg->len, 
					dmsg->buf, dmsg->len);
	else
		return __ctc_i2c_write(i2c->regs, dmsg->addr, omsg->buf, omsg->len, 
					dmsg->buf, dmsg->len);
}

static int ctc_i2c_set_bus_speed(struct udevice *bus, unsigned int speed)
{
	struct ctc_i2c *i2c = dev_get_priv(bus);

	return __ctc_i2c_set_bus_speed(i2c->regs, i2c->scl_sda_cfg, speed);
}

static int ctc_i2c_probe_chip(struct udevice *bus, uint chip_addr,
				     uint chip_flags)
{
	struct ctc_i2c *i2c = dev_get_priv(bus);
	struct i2c_regs *i2c_base = i2c->regs;
	u32 tmp;
	int ret;

	/* Current Address Read */
	ret = __ctc_i2c_read(i2c_base, chip_addr, 0, 0, (uchar *)&tmp, 1);
	if (ret)
		__ctc_i2c_init(i2c_base, 0, 0);

	return ret;
}

static int ctc_i2c_probe(struct udevice *bus)
{
	struct ctc_i2c *priv = dev_get_priv(bus);
	
	priv->regs = (struct i2c_regs *)dev_get_addr_ptr(bus);

	__ctc_i2c_init(priv->regs, 0, 0);
	
	return 0;
}

static const struct dm_i2c_ops ctc_i2c_ops = {
	.xfer		= ctc_i2c_xfer,
	.probe_chip	= ctc_i2c_probe_chip,
	.set_bus_speed	= ctc_i2c_set_bus_speed,
};

static const struct udevice_id ctc_i2c_ids[] = {
	{ .compatible = "ctc,i2c" },
	{ }
};

U_BOOT_DRIVER(i2c_ctc) = {
	.name	= "ctc_i2c",
	.id	= UCLASS_I2C,
	.of_match = ctc_i2c_ids,
	.probe	= ctc_i2c_probe,
	.priv_auto_alloc_size = sizeof(struct ctc_i2c),
	.ops	= &ctc_i2c_ops,
};

/* CONFIG_DM_I2C */

struct i2c_regs *i2c_base_s;

#ifndef CTC_I2C_OWN_SLAVE_ADDR 
#define CTC_I2C_OWN_SLAVE_ADDR  0x55
#endif

int i2c_ctc_reg_slave(int dev)
{

	if (dev == 0){
		i2c_base_s = (struct i2c_regs *)CTC_I2C0_BASE;
		writel(0x1, 0x3320003c);
	}
	else if (dev == 1){
		i2c_base_s = (struct i2c_regs *)CTC_I2C1_BASE;
		writel(0x2, 0x3320003c);
	}
	else
		return -1;

	writel(0x0, 0x3320003c);

	ctc_i2c_enable(i2c_base_s, false);
	writel(0x0, &i2c_base_s->ic_tx_tl);
	writel(0x0, &i2c_base_s->ic_rx_tl);
	writel(IC_CON_RE, &i2c_base_s->ic_con);
	writel(CTC_IC_INTR_RX_FULL | CTC_IC_INTR_RD_REQ, &i2c_base_s->ic_intr_mask);
	
	printf("i2c-%d entry into i2c slave mode...\n", dev);

	return 0;
}

int i2c_ctc_send_data(int val)
{	
	int intr;
	
	writel(0x0, &i2c_base_s->ic_enable);
	writel(CTC_I2C_OWN_SLAVE_ADDR, &i2c_base_s->ic_sar);
	writel(0x1, &i2c_base_s->ic_enable);
	
	while(1){
		intr = readl(&i2c_base_s->ic_intr_stat);		
		if (intr & CTC_IC_INTR_RD_REQ)
			break;
	}
	
	readl(&i2c_base_s->ic_clr_rd_req);
	writel(val, &i2c_base_s->ic_cmd_data);
	
	printf("slave send data 0x%x\n", val);
	return 0;
}

int i2c_ctc_receive_data(void)
{
	int intr, i=0;
	int val[2]; 

	writel(0x0, &i2c_base_s->ic_enable);
	writel(CTC_I2C_OWN_SLAVE_ADDR, &i2c_base_s->ic_sar);
	writel(0x1, &i2c_base_s->ic_enable);
	
	while(1){
		intr = readl(&i2c_base_s->ic_intr_stat);
		
		if (intr & CTC_IC_INTR_RX_FULL){
			val[i] = readl(&i2c_base_s->ic_cmd_data);
			i ++;
		}
		if(i == 2)
			break;
	}
	
	printf("slave recv offest 0x%x with data 0x%x\n", val[0], val[1]);
	return val[1];
}	
