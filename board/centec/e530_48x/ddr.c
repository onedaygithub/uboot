/*
 * (C) Copyright 2004-2017 Centec Networks (suzhou) Co., LTD.
 * Jay Cao <caoj@centecnetworks.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <ctc5236_ddr.h>

DECLARE_GLOBAL_DATA_PTR;


ctc5236_ddr_cfg_t ddr_config_e530_48x = {
	.ddrintf_rate = DDRINTF_RATE_1600,
	.ddrintf_type = DDRINTF_TYPE_3,
	.ddrintf_cs = DDRINTF_CS_2,
	.ddrintf_dq_type = DDRINTF_DQ_16_ECC,
	.ddrintf_ecc_en = DDRINTF_ECC_ENABLE,
	.ddrintf_wrlvl_mode = DDRINTF_WRLVL_MANUAL_MODE,
	.ddrintf_wrlvl_delay_db0 = 214,
	.ddrintf_wrlvl_delay_db1 = 189,
	.ddrintf_wrlvl_delay_db2 = 17,
	.ddrintf_training_mode = DDRINTF_AUTO_MODE_TRAIN,
	.sdram_cas_latency = SDRAM_CAS_LATENCY_11,
	.sdram_type = SDRAM_TYPE_SINGLE,
	.sdram_size = SDRAM_SIZE_4G_BIT,
	.sdram_col_addr_width = SDRAM_COL_ADDR_WIDTH_10,
	.sdram_row_addr_width = SDRAM_ROW_ADDR_WIDTH_15,
  	.sdram_bank_group_addr_width = SDRAM_BANK_GROUP_ADDR_WIDTH_0,
	.sdram_bank_addr_width = SDRAM_BANK_ADDR_WIDTH_3,
	.sdram_page_size = SDRAM_PAGE_SIZE_2K,
	.system_mem_size = SYSTEM_MEM_1GB,		  
};

int ctc_init_dram(void)
{
	phys_size_t dram_size;

#if defined(CONFIG_SPL_BUILD)
        puts("Initializing DDR....\n");
        dram_size = ctc_ddr_init(&ddr_config_e530_48x);
#else
        dram_size = CONFIG_SYS_SDRAM_SIZE; 
#endif
	
	gd->ram_size = dram_size;

	return 0;
}
