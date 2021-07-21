/*
 * (C) Copyright 2004-2017 Centec Networks (suzhou) Co., LTD.
 * Jay Cao <caoj@centecnetworks.com>
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
#include <dm.h>
#include <pwm.h>
#include "../common/e530_common.h"

DECLARE_GLOBAL_DATA_PTR;

extern int cmd_i2c_set_bus_num(unsigned int busnum);
void e530_24x_i2c_cfg(void)
{
	int i2cbus = 0;
	cmd_i2c_set_bus_num(i2cbus);

	/* release i2c bridge */
	ctc_i2c_mw(0x36, 0x08, 0xff);
	ctc_i2c_mw(0x74, 0x0, 0x10);
	/* Lcm cfg i2c to gpio for 24x2c */
	ctc_i2c_mw(0x21, 0x18, 0x0);
	ctc_i2c_mw(0x21, 0x19, 0xff);
	ctc_i2c_mw(0x21, 0x1a, 0xff);
	ctc_i2c_mw(0x21, 0x1b, 0x0);
	ctc_i2c_mw(0x21, 0x1c, 0xff);
	ctc_i2c_mw(0x21, 0x21, 0xff);
	ctc_i2c_mw(0x21, 0x22, 0xff);
	ctc_i2c_mw(0x21, 0x24, 0xff);
	ctc_i2c_mw(0x21, 0x08, 0x0);
	ctc_i2c_mw(0x21, 0x0b, 0x0);

	ctc_i2c_mw(0x22, 0x18, 0x0);
	ctc_i2c_mw(0x22, 0x19, 0xff);
	ctc_i2c_mw(0x22, 0x1a, 0x9f);
	ctc_i2c_mw(0x22, 0x1b, 0xff);
	ctc_i2c_mw(0x22, 0x1c, 0xff);
	ctc_i2c_mw(0x22, 0x20, 0xff);
	ctc_i2c_mw(0x22, 0x21, 0xff);
	ctc_i2c_mw(0x22, 0x22, 0xff);
	ctc_i2c_mw(0x22, 0x23, 0xff);
	ctc_i2c_mw(0x22, 0x24, 0xff);
	ctc_i2c_mw(0x22, 0x09, 0x0);
	ctc_i2c_mw(0x22, 0x0a, 0x60);
}

int board_init(void)
{
#ifdef CONFIG_DIRECT_QSPI
	/* Address remap */
	writel(0x2, CONFIG_SYS_CTC5236_GPV_BASE); 
#endif

#ifndef CONFIG_DIRECT_QSPI
	/* QSPI: exit boot mode */
	writel(0x0, &sysctl_base->SysQspiBootCfg0);
#endif

	e530_24x_i2c_cfg();

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

int board_late_init(void)
{
	/* TODO */
 	return 0;
}

int checkboard(void)
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
