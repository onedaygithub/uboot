/*
 * Copyright 2005-2019, Centec Networks (Suzhou) Co., Ltd.
 * Author: Wangyb <wangyb@centecnetworks.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <dm/platdata.h>
#include <dm/platform_data/serial_pl01x.h>
#include <asm/io.h>
#include <asm/arch/cpu.h>
#include <asm/arch/sysctl.h>
#include <malloc.h>
#include <sdhci.h>
#include <linux/mtd/rawnand.h>
#include "../common/e530_common.h"

DECLARE_GLOBAL_DATA_PTR;

extern int cmd_i2c_set_bus_num(unsigned int busnum);
void e530_48s_p_i2c_cfg(void)
{
	int i2cbus = 0;
	cmd_i2c_set_bus_num(i2cbus);

	ctc_i2c_mw(0x36, 0x4, 0xff);
	
	/* PPU I2C BGR reset */
	ctc_i2c_mw(0x36, 0x5, 0xff);
	
	/* Phy reset */
	ctc_i2c_mw(0x36, 0x7, 0xff);

	/* phy interrupt enable */
	ctc_i2c_mw(0x36, 0xa, 0x0);
	ctc_i2c_mw(0x36, 0xb, 0x0);
	ctc_i2c_mw(0x36, 0xc, 0x0);
	ctc_i2c_mw(0x36, 0xd, 0x0);
	
	/* SFP Tx enable */
	ctc_i2c_mw(0x36, 0xe, 0x0);
	ctc_i2c_mw(0x36, 0xf, 0x0);
	ctc_i2c_mw(0x36, 0x10, 0x0);
	ctc_i2c_mw(0x36, 0x11, 0x0);
	ctc_i2c_mw(0x36, 0x12, 0x0);
	ctc_i2c_mw(0x36, 0x13, 0x0);
	ctc_i2c_mw(0x36, 0x14, 0x0);
}

int board_init(void)
{
#ifndef CONFIG_DIRECT_QSPI
	/* QSPI: exit boot mode */
	writel(0x0, &sysctl_base->SysQspiBootCfg0);
#endif

	e530_48s_p_i2c_cfg();

	e530_board_init();	
	return 0;
}

int board_early_init_f(void)
{

	/* eMMC/SD: config card detect and write protect signal from sysctl reg not from pad,
	 *  for emulation and veloce don't have those signal.
	 */
	writel(0x3400027, &sysctl_base->SysMshCfg);

	/* QSPI: disable qspi secure feature  */
	writel(0x1, CONFIG_SYS_CTC5236_GPV_BASE + 0xc);

	return 0;
}

int checkboard(void)
{
	/* TODO */
	return 0;
}

int board_late_init(void)
{	
	/* TODO */
	return 0;
}

int board_eth_init(bd_t *bis)
{
	/* TODO */
	return 0;
}

#ifdef CONFIG_PCI
void pci_init_board(void)
{
	/* TODO */
}
#endif

int board_nand_init(struct nand_chip *nand)
{
	/* TODO */
	return 0;
}
