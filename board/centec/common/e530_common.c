/* 
 * Author: wangyb <wangyb@centecnetworks.com>
 *
 * Copyright 2005-2018, Centec Networks (Suzhou) Co., Ltd.
 *
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
#include "e530_common.h"
#include <switch.h>

DECLARE_GLOBAL_DATA_PTR;

#ifdef MMC_ONLY_SUPPORT_1_8V
static void mmc_drive_strength_1_8v(void)
{
	u32 val = 0;
	/* clk/cmd/data0-7 drive strength 3'b001 */
	val = 0x99999999;
	ctc5236_switch_write(0x40088, 1, &val);
	val = 0x999;
	ctc5236_switch_write(0x4008c, 1, &val);
}
#else
static void mmc_drive_strength_3_3v(void)
{
	u32 val = 0;
	/* cmd/data0-7 drive strength 3'b011
	 * clk drive strength 3'b010
	 */
	val = 0x33333333;
	ctc5236_switch_write(0x40088, 1, &val);
	val = 0x323;
	ctc5236_switch_write(0x4008c, 1, &val);
}
#endif

void e530_mmc_hw_init(void)
{
#ifdef MMC_ONLY_SUPPORT_1_8V
	mmc_drive_strength_1_8v();
#else
	mmc_drive_strength_3_3v();
#endif
}

void e530_pwm_config(void)
{
	bool enabel_pwm = true;
	int channel, period_ns, duty_ns;
	struct udevice *dev;
	uclass_get_device(UCLASS_PWM, 0, &dev);

	/* period is 1ms, duty is 500us */
	period_ns = 1000 * 1000;
	duty_ns = 500 * 1000;

	for (channel=0; channel<4; channel ++){
		pwm_set_config(dev, channel, period_ns, duty_ns);
		pwm_set_enable(dev, channel, enabel_pwm);
	}
}

extern int i2c_get_cur_bus_chip(uint chip_addr, struct udevice **devp);
extern int dm_i2c_write(struct udevice *dev, uint offset, const uint8_t *buffer, int len);
int ctc_i2c_mw(int chip, long addr, uchar byte)
{
	struct udevice *dev;
	
	i2c_get_cur_bus_chip(chip, &dev);

	dm_i2c_write(dev, addr++, &byte, 1);
	
	return 0;
}

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

#ifdef CONFIG_PADCL_GPIO0_FLG

#ifndef CONFIG_PADCTL_GPIO0_NUM
#define CONFIG_PADCTL_GPIO0_NUM PADCTL_GPIO0_NUM_ALL  /* default config gpio0 num 0-7 */
#endif

#ifndef CONFIG_PADCTL_GPIO0_MSC
#define CONFIG_PADCTL_GPIO0_MSC PADCTL_GPIO0_MSC_3	/* default drive strength 3'b011 */
#endif

#ifndef CONFIG_PADCTL_GPIO0_DRV
#define CONFIG_PADCTL_GPIO0_DRV PADCTL_GPIO0_DRV_3_3v /* default mode selector 3.3v */
#endif

void gpio_drive_strength(void)
{
	u32 i;
	u32 val, driveStrength;

	ctc5236_switch_read(CONFIG_PADCTL_GPIO0_BASE, 1, &val);

	driveStrength = (CONFIG_PADCTL_GPIO0_DRV << 3 | CONFIG_PADCTL_GPIO0_MSC);
	
	switch (CONFIG_PADCTL_GPIO0_NUM) {
		case PADCTL_GPIO0_NUM_0:
			val &= (~ PADCTL_GPIO0_NUM_0_MASK);
			val |= driveStrength << PADCTL_GPIO0_NUM_0 * 4;
			break;
		case PADCTL_GPIO0_NUM_1:
			val &= (~ PADCTL_GPIO0_NUM_1_MASK);
			val |= driveStrength << PADCTL_GPIO0_NUM_1 * 4;
			break;
		case PADCTL_GPIO0_NUM_2:	
			val &= (~ PADCTL_GPIO0_NUM_2_MASK);
			val |= driveStrength << PADCTL_GPIO0_NUM_2 * 4;
			break;
		case PADCTL_GPIO0_NUM_3:	
			val &= (~ PADCTL_GPIO0_NUM_3_MASK);
			val |= driveStrength << PADCTL_GPIO0_NUM_3 * 4;
			break;
		case PADCTL_GPIO0_NUM_4:	
			val &= (~ PADCTL_GPIO0_NUM_4_MASK);
			val |= driveStrength << PADCTL_GPIO0_NUM_4 * 4;
			break;
		case PADCTL_GPIO0_NUM_5:
			val &= (~ PADCTL_GPIO0_NUM_5_MASK);
			val |= driveStrength << PADCTL_GPIO0_NUM_5 * 4;
			break;
		case PADCTL_GPIO0_NUM_6:	
			val &= (~ PADCTL_GPIO0_NUM_6_MASK);
			val |= driveStrength << PADCTL_GPIO0_NUM_6 * 4;
			break;
		case PADCTL_GPIO0_NUM_7:
			val &= (~ PADCTL_GPIO0_NUM_7_MASK);
			val |= driveStrength << PADCTL_GPIO0_NUM_7 * 4;
			break;	
		case PADCTL_GPIO0_NUM_ALL:
			val = driveStrength;
			for(i = PADCTL_GPIO0_NUM_7; i > PADCTL_GPIO0_NUM_0; i --)
				val |= val << (4 * i);
			break;
		default:
			break;	
	}
	
	ctc5236_switch_write(CONFIG_PADCTL_GPIO0_BASE, 1, &val);
}
#else
void gpio_drive_strength(void){}
#endif

void e530_board_init(void)
{
	e530_mmc_hw_init();

	e530_pwm_config();

	gpio_drive_strength();
}
