/*
 * (C) Copyright 2004-2017 Centec Networks (suzhou) Co., LTD.
 * Jay Cao <caoj@centecnetworks.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <ctc5236_ddr.h>

DECLARE_GLOBAL_DATA_PTR;

ddr_cfg_regs_t ddr_cfg_regs = {
    .type_cfg = CONFIG_SYS_DDR_TYPE,
    .bist_cfg = CONFIG_SYS_DDR_BIST,
    .refresh_cfg = CONFIG_SYS_DDR_REFRESH,
    .rddata_cfg = CONFIG_SYS_DDR_RDDATA,
    
    .timing_cfg_0 = CONFIG_SYS_DDR_TIMING_CFG_0,
    .timing_cfg_1 = CONFIG_SYS_DDR_TIMING_CFG_1,
    .timing_cfg_2 = CONFIG_SYS_DDR_TIMING_CFG_2,
    .timing_cfg_3 = CONFIG_SYS_DDR_TIMING_CFG_3,
    
    .timing_cfg_4 = CONFIG_SYS_DDR_TIMING_CFG_4,
    .tmrs_cfg = CONFIG_SYS_DDR_TMRS,
    .mrs_0_cfg = CONFIG_SYS_DDR_MRS_0_CFG,
    .mrs_1_cfg = CONFIG_SYS_DDR_MRS_1_CFG,
    .mrs_2_cfg = CONFIG_SYS_DDR_MRS_2_CFG,
    .mrs_3_cfg = CONFIG_SYS_DDR_MRS_3_CFG,
    .mrs_4_cfg = CONFIG_SYS_DDR_MRS_4_CFG,
    .mrs_5_cfg = CONFIG_SYS_DDR_MRS_5_CFG,
    .mrs_6_cfg = CONFIG_SYS_DDR_MRS_6_CFG,
    .ctrlupd_cfg = CONFIG_SYS_DDR_CTRLUPD,
    
    .timing_cfg_5 = CONFIG_SYS_DDR_TIMING_CFG_5,
    .zqcal_cfg = CONFIG_SYS_DDR_ZQCAL,

    .app_cfg = CONFIG_SYS_DDR_APP_CFG,
    
    .maping_cfg_0 = CONFIG_SYS_DDR_MAPING_CFG_0,
    .maping_cfg_1 = CONFIG_SYS_DDR_MAPING_CFG_1,
    .maping_cfg_2 = CONFIG_SYS_DDR_MAPING_CFG_2,
    .maping_cfg_3 = CONFIG_SYS_DDR_MAPING_CFG_3,
    
    .maping_cfg_4 = CONFIG_SYS_DDR_MAPING_CFG_4,
    .maping_cfg_5 = CONFIG_SYS_DDR_MAPING_CFG_5,
    .maping_cfg_6 = CONFIG_SYS_DDR_MAPING_CFG_6,
    .maping_cfg_7 = CONFIG_SYS_DDR_MAPING_CFG_7,

    .port_0_cfg = CONFIG_SYS_DDR_PORT_0_CFG,
    .port_1_cfg = CONFIG_SYS_DDR_PORT_1_CFG,
    .port_2_cfg = CONFIG_SYS_DDR_PORT_2_CFG,
    .port_3_cfg = CONFIG_SYS_DDR_PORT_3_CFG,
    .port_4_cfg = CONFIG_SYS_DDR_PORT_4_CFG,
};

int ctc_init_dram(void)
{
	phys_size_t dram_size;

#if defined(CONFIG_SPL_BUILD)
        printf("Initializing DDR....\n");
        
        ctc_ddr_init(&ddr_cfg_regs);
        dram_size = CONFIG_SYS_SDRAM_SIZE;
#else
        dram_size = CONFIG_SYS_SDRAM_SIZE; 
#endif
	
	gd->ram_size = dram_size;

	return 0;
}
