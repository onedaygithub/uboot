/*
 *(C) Copyright 2004-2017 Centec Networks (suzhou) Co., LTD.
 * Matt gu <guy@centecnetworks.com>
 *    * SPDX-License-Identifier:	GPL-2.0
 *     */

#include <common.h>
#include <dm.h>
#include <errno.h>
#include <sysreset.h>
#include <asm/io.h>
#include <asm/arch/sysctl.h>
#include <asm/arch/cpu.h>
#include <dm/lists.h>


int ctc_sysreset_request(struct udevice *dev, enum sysreset_t type)
{
	u32 load;
	
	#define SP805_REG_WDOGLOAD         0x000
	#define SP805_REG_WDOGVAL          0x004
	#define SP805_REG_WDOGCTRL         0x008
	
	writel(0x0, &sysctl_base->SysWdt0ResetEnCtl);
#ifdef CONFIG_TARGET_EMU_CTC5236
	/* QSPI: speed up qspi boot clock div from 40 to 10 */
	writel(0x000a11f1, &sysctl_base->SysQspiBootCfg0);

	setbits_le32(&sysctl_base->SysWdt0ResetEnCtl, SYS_WDT0_RESET_EN_CTL_W0_WDT0_RST_EN_WDT0);
	setbits_le32(&sysctl_base->SysWdt0ResetEnCtl, SYS_WDT0_RESET_EN_CTL_W0_WDT0_RST_EN_WDT1);
	setbits_le32(&sysctl_base->SysWdt0ResetEnCtl, SYS_WDT0_RESET_EN_CTL_W0_WDT0_RST_EN_CPU0_CORE);
	setbits_le32(&sysctl_base->SysWdt0ResetEnCtl, SYS_WDT0_RESET_EN_CTL_W0_WDT0_RST_EN_CPU1_CORE);
	
	setbits_le32(&sysctl_base->SysWdt1ResetEnCtl, SYS_WDT1_RESET_EN_CTL_W0_WDT1_RST_EN_WDT0);
	setbits_le32(&sysctl_base->SysWdt1ResetEnCtl, SYS_WDT1_RESET_EN_CTL_W0_WDT1_RST_EN_WDT1);
	setbits_le32(&sysctl_base->SysWdt1ResetEnCtl, SYS_WDT1_RESET_EN_CTL_W0_WDT1_RST_EN_CPU0_CORE);
	setbits_le32(&sysctl_base->SysWdt1ResetEnCtl, SYS_WDT1_RESET_EN_CTL_W0_WDT1_RST_EN_CPU1_CORE);
	writel(1, &sysctl_base->SysWdt0Cnt);
	writel(1, &sysctl_base->SysWdt1Cnt);
#endif

	/* disable watchdog */
	writel(0, CONFIG_WDT_BASE+SP805_REG_WDOGCTRL);

	/* calculate the timeout value, and set */
	load = (CONFIG_WDT_CLOCK_KHZ*CONFIG_HW_WATCHDOG_TIMEOUT_MS)/2 - 1;
	writel(load, (CONFIG_WDT_BASE + SP805_REG_WDOGLOAD));

	/* enable watchdog */
	writel(3, CONFIG_WDT_BASE+SP805_REG_WDOGCTRL); 

	return -EINPROGRESS;
}

static struct sysreset_ops ctc_sysreset = {
	.request	= ctc_sysreset_request,
};

static const struct udevice_id ctc_wdt_sysreset_ids[] = {
	{ .compatible = "centec,ctc_wdt_reset" },
	{ }
};

U_BOOT_DRIVER(sysreset_ctc) = {
	.name	= "ctc_sysreset",
	.id	= UCLASS_SYSRESET,
	.ops	= &ctc_sysreset,
	.of_match	= ctc_wdt_sysreset_ids,
};
