/* 
 * Author: wangyb <wangyb@centecnetworks.com>
 *
 * Copyright 2005-2018, Centec Networks (Suzhou) Co., Ltd.
 *
 */
 
#ifndef _E530_COMMON_H_
#define _E530_COMMON_H_

void e530_mmc_hw_init(void);
void e530_pwm_config(void);
void gpio_drive_strength(void);
void e530_board_init(void);
int ctc_i2c_mw(int chip, long addr, uchar byte);

#ifndef CONFIG_DM_MMC
#ifdef CONFIG_MMC_SDHCI_CTC5236
int board_mmc_init(bd_t *bis);
#endif
#endif

#if defined(CONFIG_SYS_DRAM_TEST)
int testdram(void);
#endif

enum padctl_gpio0_num_e{
	PADCTL_GPIO0_NUM_0,
	PADCTL_GPIO0_NUM_1,
	PADCTL_GPIO0_NUM_2,
	PADCTL_GPIO0_NUM_3,
	PADCTL_GPIO0_NUM_4,
	PADCTL_GPIO0_NUM_5,
	PADCTL_GPIO0_NUM_6,
	PADCTL_GPIO0_NUM_7,
	PADCTL_GPIO0_NUM_ALL
};

enum padctl_gpio0_msc_e{
	PADCTL_GPIO0_MSC_0,
	PADCTL_GPIO0_MSC_1,
	PADCTL_GPIO0_MSC_2,
	PADCTL_GPIO0_MSC_3,
	PADCTL_GPIO0_MSC_4,
	PADCTL_GPIO0_MSC_5,
	PADCTL_GPIO0_MSC_6,
	PADCTL_GPIO0_MSC_7
};

enum padctl_gpio0_drv_e{
	PADCTL_GPIO0_DRV_3_3v,
	PADCTL_GPIO0_DRV_1_8v	
};

#define CONFIG_PADCTL_GPIO0_BASE 0x40080

#define PADCTL_GPIO0_NUM_0_MASK    0x0000000f
#define PADCTL_GPIO0_NUM_1_MASK    0x000000f0
#define PADCTL_GPIO0_NUM_2_MASK    0x00000f00
#define PADCTL_GPIO0_NUM_3_MASK    0x0000f000
#define PADCTL_GPIO0_NUM_4_MASK    0x000f0000
#define PADCTL_GPIO0_NUM_5_MASK    0x00f00000
#define PADCTL_GPIO0_NUM_6_MASK    0x0f000000
#define PADCTL_GPIO0_NUM_7_MASK    0xf0000000
#endif
