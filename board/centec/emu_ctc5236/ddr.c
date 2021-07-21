/*
 * (C) Copyright 2004-2017 Centec Networks (suzhou) Co., LTD.
 * Jay Cao <caoj@centecnetworks.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */
#include <common.h>
#include <ctc5236_ddr.h>

#include <wait_bit.h>
#include <asm/arch/cpu.h>
#include <asm/arch-ctc5236/sysctl.h>

DECLARE_GLOBAL_DATA_PTR;

/* DDR Controller Registers */
typedef struct ddrintf_ctl_group0_s {
	u32 type_cfg;			/* offset:0x0  */
	u32 bist_cfg;				/* offset:0x4  */
	u32 refresh_cfg;			/* offset:0x8  */
	u32 rddata_cfg;			/* offset:0xc  */
	u32 res1[8];				/* reserve     */
	u32 cg_mode;			/* offset:0x30 */
	u32 reserved_34;			/* offset:0x34 */
	u32 reserved_38;			/* offset:0x38 */
	u32 reserved_3c;			/* offset:0x3c */	
	u32 timing_cfg_0;			/* offset:0x40 */
	u32 timing_cfg_1;			/* offset:0x44 */	
	u32 timing_cfg_2;			/* offset:0x48 */
	u32 timing_cfg_3;			/* offset:0x4c */
	u32 timing_cfg_4;			/* offset:0x50 */
	u32 tmrs_cfg;			/* offset:0x54 */
	u32 mrs_cfg;				/* offset:0x58 */
	u32 ctrlupd_cfg;			/* offset:0x5c */
	u32 timing_cfg_5;			/* offset:0x60 */
	u32 timing_cfg_6;			/* offset:0x64 */
	u32 zqcal_cfg;			/* offset:0x68 */
	u32 res2[1];				/* reserve     */
	u32 fix_cfg;				/* offset:0x70 */
	u32 res3[6];				/* reserve     */
	u32 app_cfg;				/* offset:0x8c */
	u32 maping_cfg_0;		/* offset:0x90 */
	u32 maping_cfg_1;		/* offset:0x94 */
	u32 maping_cfg_2;		/* offset:0x98 */
	u32 maping_cfg_3;		/* offset:0x9c */
	u32 maping_cfg_4;		/* offset:0xa0 */
	u32 maping_cfg_5;		/* offset:0xa4 */
	u32 maping_cfg_6;		/* offset:0xa8 */
	u32 maping_cfg_7;		/* offset:0xac */
	u32 port_0_cfg;			/* offset:0xb0 */
	u32 port_1_cfg;			/* offset:0xb4 */
	u32 port_2_cfg;			/* offset:0xb8 */
	u32 port_3_cfg;			/* offset:0xbc */
	u32 port_4_cfg;			/* offset:0xc0 */
	u32 port_5_cfg;			/* offset:0xc4 */
	u32 port_6_cfg;			/* offset:0xc8 */
	u32 port_7_cfg;			/* offset:0xcc */
	u32 port_8_cfg;			/* offset:0xd0 */
	u32 port_9_cfg;			/* offset:0xd4 */
	u32 port_10_cfg;			/* offset:0xd8 */
	u32 port_11_cfg;			/* offset:0xdc */
	u32 port_12_cfg;			/* offset:0xe0 */
	u32 port_13_cfg;			/* offset:0xe4 */
	u32 port_14_cfg;			/* offset:0xe8 */
	u32 port_15_cfg;			/* offset:0xec */
	u32 intr_enable;			/* offset:0xf0 */
	u32 intr_status;			/* offset:0xf4 */

}ddrintf_ctl_group0_t;

#define CONFIG_SYS_CTC5236_DDRCFG_BASE 0x30600000

int ctc_init_dram(void)
{
	phys_size_t dram_size;

#if defined(CONFIG_SPL_BUILD)
        puts("Initializing DDR....\n");

	static ddrintf_ctl_group0_t *ddrintf_reg =
	(ddrintf_ctl_group0_t *)CONFIG_SYS_CTC5236_DDRCFG_BASE;
	
	u32 ret;
	/* release DDR */
	writel(0x7, &sysctl_base->SysDdrResetCtl);
	writel(0x0, &sysctl_base->SysDdrResetCtl);
	

	writel(0x01002010, &ddrintf_reg->type_cfg);
	writel(0x0008002e, &ddrintf_reg->bist_cfg);
	writel(0x00000c17, &ddrintf_reg->refresh_cfg);
	writel(0x00010609, &ddrintf_reg->rddata_cfg);

	writel(0x44121209, &ddrintf_reg->timing_cfg_0);
	writel(0x06008528, &ddrintf_reg->timing_cfg_1);
	writel(0x000b1e0b, &ddrintf_reg->timing_cfg_2);
	writel(0x06080618, &ddrintf_reg->timing_cfg_3);
	writel(0x20006008, &ddrintf_reg->timing_cfg_4);

	writel(0x0000000c, &ddrintf_reg->tmrs_cfg);
	writel(0x00040000, &ddrintf_reg->ctrlupd_cfg);

	writel(0x00001405, &ddrintf_reg->timing_cfg_5);
	writel(0x00000000, &ddrintf_reg->timing_cfg_6);
	writel(0x00001000, &ddrintf_reg->zqcal_cfg);

	writel(0x000b714c, &ddrintf_reg->app_cfg);

	writel(0x10731483, &ddrintf_reg->maping_cfg_0);
	writel(0x3ff62d49, &ddrintf_reg->maping_cfg_1);
	writel(0x1140f38d, &ddrintf_reg->maping_cfg_2);
	writel(0x165544d2, &ddrintf_reg->maping_cfg_3);

	writel(0x3f699617, &ddrintf_reg->maping_cfg_4);
	writel(0x0003ffff, &ddrintf_reg->maping_cfg_5);
	writel(0x0001d71b, &ddrintf_reg->maping_cfg_6);
	writel(0x00fdefff, &ddrintf_reg->maping_cfg_7);

	/* wati for DFI init done */
	ret = wait_for_bit(__func__, &ddrintf_reg->bist_cfg,
			   0x80000000, true, 1000, false);
	if (ret)
	{
	puts("DDR DFI init timeout!\n");
	}

	/* wait for some time then release SDII grant capability */
	udelay(100);
	clrbits_le32(&ddrintf_reg->bist_cfg, BIT(1));	
#else
        dram_size = CONFIG_SYS_SDRAM_SIZE; 
#endif
	
	gd->ram_size = dram_size;

	return 0;
}

