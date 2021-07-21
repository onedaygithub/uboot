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

#if 0
static const struct pl01x_serial_platdata serial0 = {
	.base = CONFIG_SYS_SERIAL0,
	.type = TYPE_PL011,
	.clock = CONFIG_PL011_CLOCK,
};

U_BOOT_DEVICE(ctc5236_serial0) = {
	.name = "serial_pl01x",
	.platdata = &serial0,
};
#endif

DECLARE_GLOBAL_DATA_PTR;

int board_init(void)
{
	/* TODO: Jay Cao */
	printf("board_init\n");
#ifndef CONFIG_DIRECT_QSPI
	/* QSPI: exit boot mode */
	writel(0x0, &sysctl_base->SysQspiBootCfg0);
#endif

	return 0;
}

int board_early_init_f(void)
{

	/* eMMC/SD: config card detect and write protect signal from sysctl reg not from pad,
	 * for emulation and veloce don't have those signal.
	 */
	writel(0x034000a7, &sysctl_base->SysMshCfg);

        /* QSPI: disable qspi secure feature  */
	writel(0x1, CONFIG_SYS_CTC5236_GPV_BASE + 0xc);

	/* TODO: Jay Cao */
	return 0;
}

void reset_cpu(ulong addr)
{
	u32 load;
    
#define SP805_REG_WDOGLOAD         0x000
#define SP805_REG_WDOGVAL          0x004
#define SP805_REG_WDOGCTRL         0x008

	setbits_le32(&sysctl_base->SysWdt0ResetEnCtl, SYS_WDT0_RESET_EN_CTL_W0_WDT0_RST_EN_WDT0);
	setbits_le32(&sysctl_base->SysWdt0ResetEnCtl, SYS_WDT0_RESET_EN_CTL_W0_WDT0_RST_EN_WDT1);
	setbits_le32(&sysctl_base->SysWdt0ResetEnCtl, SYS_WDT0_RESET_EN_CTL_W0_WDT0_RST_EN_CPU0_CORE);
	setbits_le32(&sysctl_base->SysWdt0ResetEnCtl, SYS_WDT0_RESET_EN_CTL_W0_WDT0_RST_EN_CPU1_CORE);
	writel(1, &sysctl_base->SysWdt0Cnt);

	setbits_le32(&sysctl_base->SysWdt1ResetEnCtl, SYS_WDT1_RESET_EN_CTL_W0_WDT1_RST_EN_WDT0);
	setbits_le32(&sysctl_base->SysWdt1ResetEnCtl, SYS_WDT1_RESET_EN_CTL_W0_WDT1_RST_EN_WDT1);
	setbits_le32(&sysctl_base->SysWdt1ResetEnCtl, SYS_WDT1_RESET_EN_CTL_W0_WDT1_RST_EN_CPU0_CORE);
	setbits_le32(&sysctl_base->SysWdt1ResetEnCtl, SYS_WDT1_RESET_EN_CTL_W0_WDT1_RST_EN_CPU1_CORE);
	writel(1, &sysctl_base->SysWdt1Cnt);
        
	/* disable watchdog */
	writel(0, CONFIG_WDT_BASE+SP805_REG_WDOGCTRL);

	/* calculate the timeout value, and set */
	load = (CONFIG_WDT_CLOCK_KHZ*CONFIG_HW_WATCHDOG_TIMEOUT_MS) - 1;
	writel(load, (CONFIG_WDT_BASE + SP805_REG_WDOGLOAD));

	/* enable watchdog */
	 writel(3, CONFIG_WDT_BASE+SP805_REG_WDOGCTRL);   
}

int checkboard(void)
{
	printf("Board: Centec CTC5236 (TsingMa) Veloce Board\n");
	return 0;
}

int board_late_init(void)
{
	/* TODO: Jay Cao */
	printf("board_late_init\n");

	return 0;
}

int board_eth_init(bd_t *bis)
{
	/* TODO: Jay Cao */
	return 0;
}

#ifdef CONFIG_PCI
void pci_init_board(void)
{
	/* TODO: Jay Cao */
	printf("pci_init_board\n");
}
#endif

#if defined(CONFIG_SYS_DRAM_TEST)
int testdram(void)
{
	u64 *pstart = (u64 *) CONFIG_SYS_MEMTEST_START;
	u64 *pend = (u64 *) CONFIG_SYS_MEMTEST_END;
	u64 *p;

	printf("Testing DRAM from 0x%08x to 0x%08x\n",
		CONFIG_SYS_MEMTEST_START,
		CONFIG_SYS_MEMTEST_END);

	printf("DRAM test phase 1:\n");
	for (p = pstart; p < pend; p++)
		*p = 0xaaaaaaaa;

	for (p = pstart; p < pend; p++) {
		if (*p != 0xaaaaaaaa) {
			printf("DRAM test fails at: %llx\n", (u64) p);
			return 1;
		}
	}

	printf("DRAM test phase 2:\n");
	for (p = pstart; p < pend; p++)
		*p = 0x55555555;

	for (p = pstart; p < pend; p++) {
		if (*p != 0x55555555) {
			printf("DRAM test fails at: %llx\n", (u64) p);
			return 1;
		}
	}

	printf("DRAM test passed.\n");
	return 0;
}
#endif

#ifndef CONFIG_DM_MMC
extern int ctc5236_sdhci_init(void);
int board_mmc_init(bd_t *bis)
{
#ifdef CONFIG_MMC_SDHCI_CTC5236
    return ctc5236_sdhci_init();
#else
    return 0;
#endif
}
#endif

