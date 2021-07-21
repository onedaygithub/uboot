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
#include "../common/e530_common.h"

DECLARE_GLOBAL_DATA_PTR;

int board_init(void)
{
#ifndef CONFIG_DIRECT_QSPI
	/* QSPI: exit boot mode */
	writel(0x0, &sysctl_base->SysQspiBootCfg0);
#endif

	return 0;
}

int board_early_init_f(void)
{

	/* eMMC/SD: config card detect and write protect signal from sysctl reg not from pad,
	 *  for emulation and veloce don't have those signal.
	 */
	writel(0x034000a7, &sysctl_base->SysMshCfg);

	/* QSPI: disable qspi secure feature  */
	writel(0x1, CONFIG_SYS_CTC5236_GPV_BASE + 0xc);
	writel(0x2, CONFIG_SYS_CTC5236_GPV_BASE );
	
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
