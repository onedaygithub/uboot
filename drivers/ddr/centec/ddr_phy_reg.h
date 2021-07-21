/*
 * (C) Copyright 2004-2017 Centec Networks (suzhou) Co., LTD.
 * Jay Cao <caoj@centecnetworks.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef __DDR_REG_H__
#define __DDR_REG_H__


#ifndef __ASSEMBLY__

/* DDR Phy Registers */
/*base:0x30600100*/
typedef struct ddrintf_phy_group0_s{
	u32	phy_vref;	 	 	/*offset:ox100*/
	u32	phy_ddr_mode;	 	/*offset:ox104*/
	u32	phy_pll_freq0;	 	/*offset:ox108*/
	u32	phy_pll_freq1;	 	/*offset:ox10c*/
	u32	phy_pll_freq2;	 	/*offset:ox110*/
	u32	phy_pll_freq3;	 	/*offset:ox114*/
	u32	phy_dll_freq0;		/*offset:ox118*/
	u32	phy_dll_freq1;	 	/*offset:ox11c*/
	u32	phy_dll_freq2;	 	/*offset:ox120*/
	u32	phy_dll_freq3;	 	/*offset:ox124*/
	u32 mlb_fast_boot_en;	/*offset:0x128*/
	u32 mlb_div_cnt;			/*offset:0x12c*/
	u32 phy_wrdata_delay;	/*offset:0x130*/
	u32 freerun_timer;	 	/*offset:0x134*/
	u32 phy_data_byte_pwrdn;		 /*offset:0x138*/
	u32 vref_ca_burst_num; 			 /*offset:0x13c*/
	u32 mlb_vref_phy_chk_sel;		 /*offset:0x140*/
	u32 mlb_vref_short;	 	/*offset:0x144*/
	u32 mlb_vref_dram_value;	 		/*offset:0x148*/
	u32 mlb_ca_vref_long;		 	/*offset:0x14c*/
	u32 phy_rddv_sel;	 	/*offset:0x150*/
	u32 mlb_mrs2_data;	 	/*offset:0x154*/
	u32 mlb_mrs1_data;	 	/*offset:0x158*/
	u32 mlb_mrs4_data;	 	/*offset:0x15c*/
	u32 mlb_mrs6_data;	 	/*offset:0x160*/
	u32 mlb_mrs8_data;	 	/*offset:0x164*/
	u32 mlb_mrd;		 	/*offset:0x168*/
	u32 mlb_oit;			 	/*offset:0x16c*/
	u32 mlb_rpall;		 	/*offset:0x170*/
	u32 mlb_rstctl_en2rst;		/*offset:0x174*/
	u32 mlb_dfi_phy_wrlat;	/*offset:0x178*/
	u32 mlb_rrd;			 	/*offset:0x17c*/
	u32 mlb_al;		 		/*offset:0x180*/
	u32 mlb_refresh_limit;		/*offset:0x184*/
	u32 phy_lpddr4_ca_odt;		 	/*offset:0x188*/
	u32 mlb_check_cnt;		/*offset:0x18c*/
	u32 mlb_ca_check_number;		/*offset:0x190*/
	u32 mlb_train_start;	 	/*offset:0x194*/
	u32 cal_offset_pu;	 	/*offset:0x198*/
	u32 phy_fdl_addr0_wr;		 	/*offset:0x19c*/
	u32 phy_fdl_addr4_wr;			/*offset:0x1a0*/
	u32 phy_fdl_addr8_wr;		 	/*offset:0x1a4*/
	u32 mlb_train_pttn_p1;			/*offset:0x1a8*/
	u32 mlb_ca_delay_bt;		 		/*offset:0x1ac*/
	u32 mlb_db0_bt_wlvl_start;		/*offset:0x1b0*/
	u32 mlb_db0_dll_wlvl_start;		/*offset:0x1b4*/
	u32 mlb_db4_dll_wlvl_start;		/*offset:0x1b8*/
	u32 mlb_db0_bt_wlvl;		 		/*offset:0x1bc*/
	u32 mlb_db0_dll_wlvl_wdqs;		/*offset:0x1c0*/
	u32 mlb_db4_dll_wlvl_wdqs;		/*offset:0x1c4*/
	u32 mlb_load_ack_cycle;		 	/*offset:0x1c8*/
	u32 db0_dll_gate_trip;		 		/*offset:0x1cc*/
	u32 db4_dll_gate_trip;		 		/*offset:0x1d0*/
	u32 db0_dll_gate_fine;		 	/*offset:0x1d4*/
	u32 db4_dll_gate_fine;		 	/*offset:0x1d8*/
	u32 mlb_train_gate_offset_minus;		 /*offset:0x1dc*/
	u32 mlb_db0_train_gate_bit_sel;		 /*offset:0x1e0*/
	u32 mlb_lpddr4_mpr0_data_0;		/*offset:0x1e4*/
	u32 mlb_lpddr4_mpr0_data_1;		/*offset:0x1e8*/
	u32 mlb_lpddr4_mpr0_data_2;		/*offset:0x1ec*/
	u32 mlb_lpddr4_mpr0_data_3;		/*offset:0x1f0*/
	u32 mlb_lpddr4_mpr1_data_0;		/*offset:0x1f4*/
	u32 mlb_lpddr4_mpr1_data_1;		/*offset:0x1f8*/
	u32 mlb_lpddr4_mpr1_data_2;		/*offset:0x1fc*/
}ddrintf_phy_group0_t;



/* ################################################################################
 * # PHY_104 Definition
 */
#define PHY_104_PHY_DDR3_MODE			(0)
#define PHY_104_PHY_DDR4_MODE			(1)
#define PHY_104_PHY_LPDDR2_MODE			 (2)
#define PHY_104_PHY_LPDDR3_MODE			 (3)
#define PHY_104_PHY_LPDDR4_MODE			 (4)
#define PHY_104_MA2T_EN					 (5)
#define PHY_104_DATA_EXCHANGE_MODE		 (6)
#define PHY_104_RDDV_TRAINING_MODE		 (7)
#define PHY_104_PHY_RDDV_SEL_OFFSET		 (8)
#define PHY_104_PLL_LOCKCNT_MODE			 (12)
#define PHY_104_DLL_LOCKCNT_MODE			 (14)
#define PHY_104_MLB_WRODT_CNT_INTG		 (16)
#define PHY_104_PHY_PLL_2ND_LOCK_MODE	 (24)
#define PHY_104_PHY_PLLPD2DIVRSTN_MODE	 (26)
#define PHY_104_MLB_VREF_CA_DISABLE		 (28)
#define PHY_104_MLB_CA_PTTN_TOGGLE_DISABLE	 (29)
#define PHY_104_MLB_CA_END_BT				 (30)
#define PHY_104_MLB_VREF_CA_RANGE_MANU	 (31)

#define PHY_104_PHY_DDR3_MODE_MASK			0x1
#define PHY_104_PHY_DDR4_MODE_MASK			0x1
#define PHY_104_PHY_LPDDR2_MODE_MASK		0x1
#define PHY_104_PHY_LPDDR3_MODE_MASK		0x1
#define PHY_104_PHY_LPDDR4_MODE_MASK		0x1
#define PHY_104_MA2T_EN_MASK					0x1
#define PHY_104_DATA_EXCHANGE_MODE_MASK	0x1
#define PHY_104_RDDV_TRAINING_MODE_MASK	0x1
#define PHY_104_PHY_RDDV_SEL_OFFSET_MASK	0x7
#define PHY_104_PLL_LOCKCNT_MODE_MASK				0x3
#define PHY_104_DLL_LOCKCNT_MODE_MASK				0x3
#define PHY_104_MLB_WRODT_CNT_INTG_MASK			0x3F
#define PHY_104_PHY_PLL_2ND_LOCK_MODE_MASK			0x3
#define PHY_104_PHY_PLLPD2DIVRSTN_MODE_MASK		0x3
#define PHY_104_MLB_VREF_CA_DISABLE_MASK			0x1
#define PHY_104_MLB_CA_PTTN_TOGGLE_DISABLE_MASK	0x1
#define PHY_104_MLB_CA_END_BT_MASK					0x1
#define PHY_104_MLB_VREF_CA_RANGE_MANU_MASK		0x1

/* ################################################################################
 * # PHY_108 Definition
 */
#define PHY_108_PHY_PLL_FREQ0		 (0)

#define PHY_108_PHY_PLL_FREQ0_MASK	0xFFFFFFFF

/* ################################################################################
 * # PHY_10C Definition
 */
#define PHY_10C_PHY_PLL_FREQ1		 (0)

#define PHY_10C_PHY_PLL_FREQ1_MASK	0xFFFFFFFF

/* ################################################################################
 * # PHY_110 Definition
 */
#define PHY_110_PHY_PLL_FREQ2		 (0)

#define PHY_110_PHY_PLL_FREQ2_MASK	0xFFFFFFFF

/* ################################################################################
 * # PHY_114 Definition
 */
#define PHY_114_PHY_PLL_FREQ3		 (0)

#define PHY_114_PHY_PLL_FREQ3_MASK	0xFFFFFFFF

/* ################################################################################
 * # PHY_118 Definition
 */
#define PHY_118_PHY_DLL_FREQ0		 (0)

#define PHY_118_PHY_DLL_FREQ0_MASK		0xFFFFFFFF

/* ################################################################################
 * # PHY_11C Definition
 */
#define PHY_11C_PHY_DLL_FREQ1		 (0)

#define PHY_11C_PHY_DLL_FREQ1_MASK		0xFFFFFFFF

/* ################################################################################
 * # PHY_120 Definition
 */
#define PHY_120_PHY_DLL_FREQ2		 (0)

#define PHY_120_PHY_DLL_FREQ2_MASK		0xFFFFFFFF

/* ################################################################################
 * # PHY_124 Definition
 */
#define PHY_124_PHY_DLL_FREQ3		 (0)

#define PHY_124_PHY_DLL_FREQ3_MASK		0xFFFFFFFF

/* ################################################################################
 * # PHY_128 Definition
 */
#define PHY_128_MLB_10US_FAST_BOOT_EN		 (0)
#define PHY_128_MLB_COMP_CLK_SEL			 (1)
#define PHY_128_MLB_PLL_START_DISABLE		 (3)
#define PHY_128_MLB_PLL_CODE_MANUAL			 (4)
#define PHY_128_MLB_CALL_PD_CNT				 (8)
#define PHY_128_MLB_INIT_EN_CNT				 (16)
#define PHY_128_MLB_PD_CNT					 (24)

#define PHY_128_MLB_10US_FAST_BOOT_EN_MASK		0x1
#define PHY_128_MLB_COMP_CLK_SEL_MASK			0x3
#define PHY_128_MLB_PLL_START_DISABLE_MASK		0x1
#define PHY_128_MLB_PLL_CODE_MANUAL_MASK		0xF
#define PHY_128_MLB_CALL_PD_CNT_MASK			0xFF
#define PHY_128_MLB_INIT_EN_CNT_MASK			0xFF
#define PHY_128_MLB_PD_CNT_MASK					0xFF

/* ################################################################################
 * # PHY_12C Definition
 */
#define PHY_12C_MLB_DIV_CNT					 (0)
#define PHY_12C_MLB_INIT_NONE_EN_CNT		 (12)
#define PHY_12C_MLB_CP_EN_CNT				 (24)

#define PHY_12C_MLB_DIV_CNT_MASK				0xFFF
#define PHY_12C_MLB_INIT_NONE_EN_CNT_MASK		0xFFF
#define PHY_12C_MLB_CP_EN_CNT_MASK				0xFF

/* ################################################################################
 * # PHY_130 Definition
 */
#define PHY_130_PHY_WRDATA_DELAY			 (0)
#define PHY_130_PHY_RC_LATCH				 (1)
#define PHY_130_PHY_CS_PWRSAVE			 (2)
#define PHY_130_PHY_ODT_N					 (4)
#define PHY_130_PHY_ODT_P					 (8)
#define PHY_130_PHY_PAD_DRVN				 (12)
#define PHY_130_PHY_PAD_DRVP				 (16)
#define PHY_130_ODT_EN_DLY				 (20)
#define PHY_130_ODT_ALWAYSON				 (23)
#define PHY_130_MLB_WRODT_EN_DLY		 (24)
#define PHY_130_MLB_WRODT_ALWAYSON		 (27)
#define PHY_130_MLB_BG_SELECT_MUX		 (28)
#define PHY_130_CLONE_ENABLE				 (31)

#define PHY_130_PHY_WRDATA_DELAY_MASK			0x1
#define PHY_130_PHY_RC_LATCH_MASK				0x1
#define PHY_130_PHY_CS_PWRSAVE_MASK				0x1
#define PHY_130_PHY_ODT_N_MASK					0x7
#define PHY_130_PHY_ODT_P_MASK					0x7
#define PHY_130_PHY_PAD_DRVN_MASK				0x7
#define PHY_130_PHY_PAD_DRVP_MASK				0x7
#define PHY_130_ODT_EN_DLY_MASK					0x7
#define PHY_130_ODT_ALWAYSON_MASK				0x1
#define PHY_130_MLB_WRODT_EN_DLY_MASK			0x7
#define PHY_130_MLB_WRODT_ALWAYSON_MASK			0x1
#define PHY_130_MLB_BG_SELECT_MUX_MASK			0x3
#define PHY_130_CLONE_ENABLE_MASK				0x1

/* ################################################################################
 * # PHY_138 Definition
 */
#define PHY_138_PHY_DATA_BYTE_PWRDN		 (0)
#define PHY_138_PHY_PREAMBLE_MODE		 (8)
#define PHY_138_PHY_ADD_MODE			 (10)
#define PHY_138_PHY_GATE_MODE			 (12)
#define PHY_138_MLB_MULTI_RANK			 (16)
#define PHY_138_PHY_FORCE_RDF_PTR		 (18)
#define PHY_138_MLB_PD_MODE				 (19)
#define PHY_138_MLB_T_LP_RESP			 (24)

#define PHY_138_PHY_DATA_BYTE_PWRDN_MASK	0xFF
#define PHY_138_PHY_PREAMBLE_MODE_MASK		0x3
#define PHY_138_PHY_ADD_MODE_MASK			0x1
#define PHY_138_PHY_GATE_MODE_MASK			0x7
#define PHY_138_MLB_MULTI_RANK_MASK			0x3
#define PHY_138_PHY_FORCE_RDF_PTR_MASK		0x1
#define PHY_138_MLB_PD_MODE_MASK			0x3
#define PHY_138_MLB_T_LP_RESP_MASK			0xFF

/* ################################################################################
 * # PHY_13C Definition
 */
#define	PHY_13C_VREF_CA_BURST_NUM			 (0)
#define	PHY_13C_VREF_CA_VALUE_START			 (8)
#define	PHY_13C_VREF_CA_VALUE_END			 (16)
#define	PHY_13C_MLB_MLB_T_CAENT				 (24)

#define	PHY_13C_VREF_CA_BURST_NUM_MASK			0x3F
#define	PHY_13C_VREF_CA_VALUE_START_MASK		0x3F
#define	PHY_13C_VREF_CA_VALUE_END_MASK			0x3F
#define	PHY_13C_MLB_MLB_T_CAENT_MASK			0xFF

/* ################################################################################
 * # PHY_140 Definition
 */
#define PHY_140_MLB_VREF_PHY_CHK_SEL		 (0)
#define PHY_140_MLB_VREF_PHY_ADJ_END		 (8)
#define PHY_140_MLB_VREF_R_T_VREDDQE		 (16)
#define PHY_140_MLB_VREF_R_T_VREDDQX		 (24)

#define PHY_140_MLB_VREF_PHY_CHK_SEL_MASK		0xF
#define PHY_140_MLB_VREF_PHY_ADJ_END_MASK		0xFF
#define PHY_140_MLB_VREF_R_T_VREDDQE_MASK		0xFF
#define PHY_140_MLB_VREF_R_T_VREDDQX_MASK		0xFF

/* ################################################################################
 * # PHY_144 Definition
 */
#define PHY_144_MLB_VREF_R_T_VREF_SHORT			 (0)
#define PHY_144_MLB_VREF_R_T_VREF_LONG			 (8)
#define PHY_144_MLB_VREF_R_T_VREF_PHY_SHORT		 (16)
#define PHY_144_MLB_VREF_R_T_VREF_PHY_LONG		 (24)

#define PHY_144_MLB_VREF_R_T_VREF_SHORT_MASK			0xff
#define PHY_144_MLB_VREF_R_T_VREF_LONG_MASK			0xff
#define PHY_144_MLB_VREF_R_T_VREF_PHY_SHORT_MASK		0xff
#define PHY_144_MLB_VREF_R_T_VREF_PHY_LONG_MASK		0xff

/* ################################################################################
 * # PHY_148 Definition
 */
#define PHY_148_MLB_VREF_DRAM_VALUE_START		 (0)
#define PHY_148_MLB_VREF_DRAM_VALUE_END			 (8)
#define PHY_148_MLB_CA_END_STEP					 (16)
#define PHY_148_MLB_VREF_CA_VALUE_MANU			 (24)

#define PHY_148_MLB_VREF_DRAM_VALUE_START_MASK		0x3f
#define PHY_148_MLB_VREF_DRAM_VALUE_END_MAKS			0x3f
#define PHY_148_MLB_CA_END_STEP_MASK					0xff
#define PHY_148_MLB_VREF_CA_VALUE_MANU_MASK			0x3f

/* ################################################################################
 * # PHY_14C Definition
 */
#define PHY_14C_MLB_CA_VREF_R_T_VREF_LONG		 (0)
#define PHY_14C_MLB_CA_VREF_R_T_VREF_SHORT		 (8)

#define PHY_14C_MLB_CA_VREF_R_T_VREF_LONG_MASK		0xff
#define PHY_14C_MLB_CA_VREF_R_T_VREF_SHORT_MASK		0xff

/* ################################################################################
 * # PHY_150 Definition
 */
#define PHY_150_PHY_RDDV_SEL						 (0)
#define PHY_150_PHY_RDDV_SEL_MANU				 (4)
#define PHY_150_PHY_SET_RDF_PTR					 (5)
#define PHY_150_PHY_SIM_MODE						 (6)
#define PHY_150_MC_TRAINING_MODE					 (7)
#define PHY_150_PHY_PLL_BYPASS					 (8)
#define PHY_150_PHY_PLL_TESTMODE					 (9)
#define PHY_150_PHY_DLL_BYPASS					 (10)
#define PHY_150_PHY_EYE_STEP_UNIT					 (12)
#define PHY_150_PHY_DATABUSY_CNT_INTG			 (16)
#define PHY_150_MLB_MPR_USER_PTTN				 (23)
#define PHY_150_TRAINING_MANUAL_MODE			 (24)
#define PHY_150_ZQCALL_ALL_RANK					 (25)
#define PHY_150_LPDDR4_DUAL_CH					 (26)
#define PHY_150_RANK0_DFI_ODT_HIGH				 (28)
#define PHY_150_RANK1_DFI_ODT_HIGH				 (29)
#define PHY_150_RANK2_DFI_ODT_HIGH				 (30)
#define PHY_150_RANK3_DFI_ODT_HIGH				 (31)
#define PHY_150_EYE_STEP_UNIT						 (12)


#define PHY_150_PHY_RDDV_SEL_MASK						0xf
#define PHY_150_PHY_RDDV_SEL_MANU_MASK					0x1
#define PHY_150_PHY_SET_RDF_PTR_MASK					0x1
#define PHY_150_PHY_SIM_MODE_MASK						0x1
#define PHY_150_MC_TRAINING_MODE_MASK					0x1
#define PHY_150_PHY_PLL_BYPASS_MASK						0x1
#define PHY_150_PHY_PLL_TESTMODE_MASK					0x1
#define PHY_150_PHY_DLL_BYPASS_MASK						0x1
#define PHY_150_PHY_EYE_STEP_UNIT_MASK					0xf
#define PHY_150_PHY_DATABUSY_CNT_INTG_MASK				0x3f
#define PHY_150_MLB_MPR_USER_PTTN_MASK					0x1
#define PHY_150_TRAINING_MANUAL_MODE_MASK				0x1
#define PHY_150_ZQCALL_ALL_RANK_MASK						0x1
#define PHY_150_LPDDR4_DUAL_CH_MASK						0x1
#define PHY_150_RANK0_DFI_ODT_HIGH_MASK					0x1
#define PHY_150_RANK1_DFI_ODT_HIGH_MASK					0x1
#define PHY_150_RANK2_DFI_ODT_HIGH_MASK					0x1
#define PHY_150_RANK3_DFI_ODT_HIGH_MASK					0x1
#define PHY_150_EYE_STEP_UNIT_MASK						0x1f

/**0x810**/
#define PHY_150_MLB_TRAIN_DATA_USR_PTTN			 (27)

#define PHY_150_MLB_TRAIN_DATA_USR_PTTN_MASK	0x1

/* ################################################################################
 * # PHY_154 Definition
 */
#define PHY_154_MLB_MRS2_DATA			 (0)
#define PHY_154_MLB_MRS3_DATA			 (16)

#define PHY_154_MLB_MRS2_DATA_MASK			0xffff
#define PHY_154_MLB_MRS3_DATA_MASK			0xffff

/* ################################################################################
 * # PHY_158 Definition
 */
#define PHY_158_MLB_MRS1_DATA			 (0)
#define PHY_158_MLB_MRS0_DATA			 (16)

#define PHY_158_MLB_MRS1_DATA_MASK			0xffff
#define PHY_158_MLB_MRS0_DATA_MASK			0xffff

/* ################################################################################
 * # PHY_15C Definition
 */
#define PHY_15C_MLB_MRS4_DATA			 (0)
#define PHY_15C_MLB_MRS5_DATA			 (16)

#define PHY_15C_MLB_MRS4_DATA_MASK			0xffff
#define PHY_15C_MLB_MRS5_DATA_MASK			0xffff

/* ################################################################################
 * # PHY_160 Definition
 */
#define PHY_160_MLB_MRS6_DATA			 (0)
#define PHY_160_MLB_MRS7_DATA			 (16)

#define PHY_160_MLB_MRS6_DATA_MASK			0xffff
#define PHY_160_MLB_MRS7_DATA_MASK			0xffff

/* ################################################################################
 * # PHY_164 Definition
 */
#define PHY_164_MLB_MRS8_DATA			 (0)
#define PHY_164_MLB_MRS9_DATA			 (16)

#define PHY_164_MLB_MRS8_DATA_MASK			0xffff
#define PHY_164_MLB_MRS9_DATA_MASK			0xffff

/* ################################################################################
 * # PHY_168 Definition
 */
#define PHY_168_MLB_R_T_MRD				 (0)
#define PHY_168_MLB_R_T_MOD				 (16)

#define PHY_168_MLB_R_T_MRD_MASK				0xffff
#define PHY_168_MLB_R_T_MOD_MASK				0xffff

/* ################################################################################
 * # PHY_16C Definition
 */
#define PHY_16C_MLB_R_T_OIT				 (0)
#define PHY_16C_MLB_R_T_XPR				 (16)

#define PHY_16C_MLB_R_T_OIT_MASK				0xffff
#define PHY_16C_MLB_R_T_XPR_MASK				0xffff

/* ################################################################################
 * # PHY_170 Definition
 */
#define PHY_170_MLB_R_T_RPALL						 (0)
#define PHY_170_MLB_TRAIN_DPS_TRAINING_MODE			 (8)
#define PHY_170_MLB_TRAIN_PASSEND_MODE				 (9)
#define PHY_170_LB_BIST_DDR34PHY					 (12)
#define PHY_170_MLB_R_T_WR							 (16)
#define PHY_170_MLB_R_WODT_DELAY					 (24)

#define PHY_170_MLB_R_T_RPALL_MASK						0xff
#define PHY_170_MLB_TRAIN_DPS_TRAINING_MODE_MASK		0x1
#define PHY_170_MLB_TRAIN_PASSEND_MODE_MASK				0x1
#define PHY_170_LB_BIST_DDR34PHY_MASK					0x1
#define PHY_170_MLB_R_T_WR_MASK							0xff
#define PHY_170_MLB_R_WODT_DELAY_MASK					0xf

/* ################################################################################
 * # PHY_174 Definition
 */
#define PHY_174_MLB_R_RSTCTL_EN2RST				 (12)

#define PHY_174_MLB_R_RSTCTL_EN2RST_MASK			0xfffff

/* ################################################################################
 * # PHY_178 Definition
 */
#define PHY_178_MLB_R_DFI_T_PHY_WRLAT			 (0)
#define PHY_178_MLB_R_DFI_T_PHY_WRDATA			 (8)
#define PHY_178_MLB_R_RSTCTL_RST2CKE			 (12)

#define PHY_178_MLB_R_DFI_T_PHY_WRLAT_MASK			0xff
#define PHY_178_MLB_R_DFI_T_PHY_WRDATA_MASK			0xf
#define PHY_178_MLB_R_RSTCTL_RST2CKE_MASK			0xfffff

/* ################################################################################
 * # PHY_17C Definition
 */
#define PHY_17C_MLB_R_T_RRD					 (0)
#define PHY_17C_MLB_R_T_CCD					 (8)

#define PHY_17C_MLB_R_T_RRD_MASK					0xff
#define PHY_17C_MLB_R_T_CCD_MASK					0xff

/* ################################################################################
 * # PHY_180 Definition
 */
#define PHY_180_MLB_R_AL					 (0)
#define PHY_180_MLB_R_DFI_T_RDDATA_EN		 (8)
#define PHY_180_MLB_R_T_RFC					 (23)

#define PHY_180_MLB_R_AL_MASK					0xff
#define PHY_180_MLB_R_DFI_T_RDDATA_EN_MASK		0xff
#define PHY_180_MLB_R_T_RFC_MASK				0x1ff

/* ################################################################################
 * # PHY_184 Definition
 */
#define PHY_184_MLB_R_REFRESH_LIMIT			 (0)
#define PHY_184_MLB_R_REFRESH_DISABLE		 (17)
#define PHY_184_MLB_R_WODT_TIMER			 (20)
#define PHY_184_MLB_R_T_RCD					 (24)

#define PHY_184_MLB_R_REFRESH_LIMIT_MASK		0x1ffff
#define PHY_184_MLB_R_REFRESH_DISABLE_MASK		0x1
#define PHY_184_MLB_R_WODT_TIMER_MASK			0xf
#define PHY_184_MLB_R_T_RCD_MASK				0xff

/* ################################################################################
 * # PHY_188 Definition
 */
#define PHY_188_PHY_LPDDR4_CA_ODT			 (0)
#define PHY_188_CTRL_IO_DDR4_EN				 (1)
#define PHY_188_MLB_CAL_8BIT_MANUAL_MODE	 (4)
#define PHY_188_MLB_MLB_T_CACKEL			 (8)
#define PHY_188_MLB_MLB_T_CAMRD				 (16)
#define PHY_188_MLB_MLB_T_CACD_M			 (24)

#define PHY_188_PHY_LPDDR4_CA_ODT_MASK			0x1
#define PHY_188_CTRL_IO_DDR4_EN_MASK			0x1
#define PHY_188_MLB_CAL_8BIT_MANUAL_MODE_MASK	0x1
#define PHY_188_MLB_MLB_T_CACKEL_MASK			0x1f
#define PHY_188_MLB_MLB_T_CAMRD_MASK			0x3f
#define PHY_188_MLB_MLB_T_CACD_M_MASK			0x7f

/* ################################################################################
 * # PHY_18C Definition
 */
#define PHY_18C_MLB_CHECK_T_CNT			 (0)
#define PHY_18C_MLB_MLB_T_CACKEH		 (8)
#define PHY_18C_MLB_MLB_T_CAEXT			 (16)
#define PHY_18C_MLB_MLB_T_MRZ			 (24)

#define PHY_18C_MLB_CHECK_T_CNT_MASK		0xff
#define PHY_18C_MLB_MLB_T_CACKEH_MASK		0x1f
#define PHY_18C_MLB_MLB_T_CAEXT_MASK		0xff
#define PHY_18C_MLB_MLB_T_MRZ_MASK			0xff

/* ################################################################################
 * # PHY_190 Definition
 */
#define PHY_190_MLB_CA_CHECK_NUMBER			 (4)
#define PHY_190_MLB_CA_START_STEP			 (8)
#define PHY_190_MLB_CA_START_BT				 (16)
#define PHY_190_MLB_TRAIN_PASSEND_OFS		 (20)

#define PHY_190_MLB_CA_CHECK_NUMBER_MASK		0xf
#define PHY_190_MLB_CA_START_STEP_MASK			0xff
#define PHY_190_MLB_CA_START_BT_MASK			0x1
#define PHY_190_MLB_TRAIN_PASSEND_OFS_MASK		0xfff

/* ################################################################################
 * # PHY_194 Definition
 */
#define PHY_194_MLB_TRAIN_START						 (0)
#define PHY_194_CAL_DISABLE							 (1)
#define PHY_194_CAL_MANUAL_MODE						 (2)
#define PHY_194_CAL_MANUAL_PWRDN					 (3)
#define PHY_194_MLB_CAL_PCOMP_FIRST					 (4)
#define PHY_194_MLB_TRAIN_CA_PERBIT_DISABLE			 (5)
#define PHY_194_MLB_TRAIN_CA_DISABLE				 (6)
#define PHY_194_MLB_TRAIN_WRLVL_DISABLE				 (7)
#define PHY_194_MLB_TRAIN_GATE_DISABLE				 (8)
#define PHY_194_MLB_TRAIN_RD_PERBIT_DISABLE			 (9)
#define PHY_194_MLB_TRAIN_RDEYE_DISABLE				 (10)
#define PHY_194_MLB_TRAIN_RDVREF_DISABLE			 (11)
#define PHY_194_MLB_TRAIN_WR_PERBIT_DISABLE			 (12)
#define PHY_194_MLB_TRAIN_WREYE_DISABLE				 (13)
#define PHY_194_MLB_TRAIN_WRVREF_DISABLE			 (14)
#define PHY_194_MLB_TRAIN_GATE_FAST_MODE			 (15)
#define PHY_194_MLB_TRAIN_DATA_TRAINING_MODE		 (16)
#define PHY_194_MLB_TRAIN_BIT_EN					 (19)
#define PHY_194_MLB_TRAIN_WDQS_STOP_INTG			 (20)
#define PHY_194_MLB_TRAIN_WDQS_STOP_STEP			 (24)

#define PHY_194_MLB_TRAIN_START_MASK				0x1
#define PHY_194_CAL_DISABLE_MASK					0x1
#define PHY_194_CAL_MANUAL_MODE_MASK				0x1
#define PHY_194_CAL_MANUAL_PWRDN_MASK				0x1
#define PHY_194_MLB_CAL_PCOMP_FIRST_MASK			0x1
#define PHY_194_MLB_TRAIN_CA_PERBIT_DISABLE_MASK	0x1
#define PHY_194_MLB_TRAIN_CA_DISABLE_MASK			0x1
#define PHY_194_MLB_TRAIN_WRLVL_DISABLE_MASK		0x1
#define PHY_194_MLB_TRAIN_GATE_DISABLE_MASK			0x1
#define PHY_194_MLB_TRAIN_RD_PERBIT_DISABLE_MASK	0x1
#define PHY_194_MLB_TRAIN_RDEYE_DISABLE_MASK		0x1
#define PHY_194_MLB_TRAIN_RDVREF_DISABLE_MASK		0x1
#define PHY_194_MLB_TRAIN_WR_PERBIT_DISABLE_MASK	0x1
#define PHY_194_MLB_TRAIN_WREYE_DISABLE_MASK		0x1
#define PHY_194_MLB_TRAIN_WRVREF_DISABLE_MASK		0x1
#define PHY_194_MLB_TRAIN_GATE_FAST_MODE_MASK		0x1
#define PHY_194_MLB_TRAIN_DATA_TRAINING_MODE_MASK	0x7
#define PHY_194_MLB_TRAIN_BIT_EN_MASK				0x1
#define PHY_194_MLB_TRAIN_WDQS_STOP_INTG_MASK		0x7
#define PHY_194_MLB_TRAIN_WDQS_STOP_STEP_MASK		0xff

/* ################################################################################
 * # PHY_198 Definition
 */
#define PHY_198_CAL_OFFSET_PU			 (0)
#define PHY_198_CAL_OFFSET_PD			 (8)
#define PHY_198_CAL_DEFAULT_PU			 (16)
#define PHY_198_CAL_DEFAULT_PD			 (24)

#define PHY_198_CAL_OFFSET_PU_MASK		0xf
#define PHY_198_CAL_OFFSET_PD_MASK		0xf
#define PHY_198_CAL_DEFAULT_PU_MASK		0xff
#define PHY_198_CAL_DEFAULT_PD_MASK		0xff

/* ################################################################################
 * # PHY_19C Definition
 */
#define PHY_19C_PHY_FDL_ADDR0_WR			 (0)
#define PHY_19C_PHY_FDL_ADDR1_WR			 (8)
#define PHY_19C_PHY_FDL_ADDR2_WR			 (16)
#define PHY_19C_PHY_FDL_ADDR3_WR			 (24)

#define PHY_19C_PHY_FDL_ADDR0_WR_MASK		0xff
#define PHY_19C_PHY_FDL_ADDR1_WR_MASK		0xff
#define PHY_19C_PHY_FDL_ADDR2_WR_MASK		0xff
#define PHY_19C_PHY_FDL_ADDR3_WR_MASK		0xff

/* ################################################################################
 * # PHY_1A0 Definition
 */
#define PHY_1A0_PHY_FDL_ADDR4_WR		 (0)
#define PHY_1A0_PHY_FDL_ADDR5_WR		 (8)
#define PHY_1A0_PHY_FDL_ADDR6_WR		 (16)
#define PHY_1A0_PHY_FDL_ADDR7_WR		 (24)

#define PHY_1A0_PHY_FDL_ADDR4_WR_MASK	0xff
#define PHY_1A0_PHY_FDL_ADDR5_WR_MASK	0xff
#define PHY_1A0_PHY_FDL_ADDR6_WR_MASK	0xff
#define PHY_1A0_PHY_FDL_ADDR7_WR_MASK	0xff

/* ################################################################################
 * # PHY_1A4 Definition
 */
#define PHY_1A4_PHY_FDL_ADDR8_WR		 (0)
#define PHY_1A4_PHY_FDL_ADDR9_WR		 (8)
#define PHY_1A4_MLB_TRAIN_8CA_PTTN_P0	 (16)

#define PHY_1A4_PHY_FDL_ADDR8_WR_MASK		0xff
#define PHY_1A4_PHY_FDL_ADDR9_WR_MASK		0xff
#define PHY_1A4_MLB_TRAIN_8CA_PTTN_P0_MASK	0xffff

/* ################################################################################
 * # PHY_1A8 Definition
 */
#define PHY_1A8_MLB_TRAIN_8CA_PTTN_P1	 (0)
#define PHY_1A8_MLB_TRAIN_2CA_PTTN_P0	 (16)
#define PHY_1A8_MLB_TRAIN_2CA_PTTN_P1	 (20)
#define PHY_1A8_MLB_CA_DELAY			 (24)

#define PHY_1A8_MLB_TRAIN_8CA_PTTN_P1_MASK	0xffff
#define PHY_1A8_MLB_TRAIN_2CA_PTTN_P0_MASK	0xf
#define PHY_1A8_MLB_TRAIN_2CA_PTTN_P1_MASK	0xf
#define PHY_1A8_MLB_CA_DELAY_MASK			0xff

/* ################################################################################
 * # PHY_1AC Definition
 */
#define PHY_1AC_MLB_CA_DELAY_BT		 (0)

#define PHY_1AC_MLB_CA_DELAY_BT_MASK	0x1

/* ################################################################################
 * # PHY_1B0 Definition
 */
#define PHY_1B0_MLB_DB0_BT_WLVL_START		 (0)
#define PHY_1B0_MLB_DB1_BT_WLVL_START		 (2)
#define PHY_1B0_MLB_DB2_BT_WLVL_START		 (4)
#define PHY_1B0_MLB_DB3_BT_WLVL_START		 (6)
#define PHY_1B0_MLB_DB4_BT_WLVL_START		 (8)
#define PHY_1B0_MLB_DB5_BT_WLVL_START		 (10)
#define PHY_1B0_MLB_DB6_BT_WLVL_START		 (12)
#define PHY_1B0_MLB_DB7_BT_WLVL_START		 (14)
#define PHY_1B0_MLB_DB8_BT_WLVL_START		 (16)

#define PHY_1B0_MLB_DB0_BT_WLVL_START_MASK	0x3
#define PHY_1B0_MLB_DB1_BT_WLVL_START_MASK	0x3
#define PHY_1B0_MLB_DB2_BT_WLVL_START_MASK	0x3
#define PHY_1B0_MLB_DB3_BT_WLVL_START_MASK	0x3
#define PHY_1B0_MLB_DB4_BT_WLVL_START_MASK	0x3
#define PHY_1B0_MLB_DB5_BT_WLVL_START_MASK	0x3
#define PHY_1B0_MLB_DB6_BT_WLVL_START_MASK	0x3
#define PHY_1B0_MLB_DB7_BT_WLVL_START_MASK	0x3
#define PHY_1B0_MLB_DB8_BT_WLVL_START_MASK	0x3

/*1B8*/
#define PHY_1B0_MLB_DB8_DLL_WLVL_START		 (24)

#define PHY_1B0_MLB_DB8_DLL_WLVL_START_MASK		0xff

/* ################################################################################
 * # PHY_1B4 Definition
 */
#define PHY_1B4_MLB_DB0_DLL_WLVL_START		 (0)
#define PHY_1B4_MLB_DB1_DLL_WLVL_START		 (8)
#define PHY_1B4_MLB_DB2_DLL_WLVL_START		 (16)
#define PHY_1B4_MLB_DB3_DLL_WLVL_START		 (24)

#define PHY_1B4_MLB_DB0_DLL_WLVL_START_MASK		0xff
#define PHY_1B4_MLB_DB1_DLL_WLVL_START_MASK		0xff
#define PHY_1B4_MLB_DB2_DLL_WLVL_START_MASK		0xff
#define PHY_1B4_MLB_DB3_DLL_WLVL_START_MASK		0xff

/* ################################################################################
 * # PHY_1B8 Definition
 */
#define PHY_1B8_MLB_DB4_DLL_WLVL_START		 (0)
#define PHY_1B8_MLB_DB5_DLL_WLVL_START		 (8)
#define PHY_1B8_MLB_DB6_DLL_WLVL_START		 (16)
#define PHY_1B8_MLB_DB7_DLL_WLVL_START		 (24)

#define PHY_1B8_MLB_DB4_DLL_WLVL_START_MASK		0xff
#define PHY_1B8_MLB_DB5_DLL_WLVL_START_MASK		0xff
#define PHY_1B8_MLB_DB6_DLL_WLVL_START_MASK		0xff
#define PHY_1B8_MLB_DB7_DLL_WLVL_START_MASK		0xff

/* ################################################################################
 * # PHY_1BC Definition
 */
#define PHY_1BC_MLB_DB0_BT_WLVL			 (0)
#define PHY_1BC_MLB_DB1_BT_WLVL			 (2)
#define PHY_1BC_MLB_DB2_BT_WLVL			 (4)
#define PHY_1BC_MLB_DB3_BT_WLVL			 (6)
#define PHY_1BC_MLB_DB4_BT_WLVL			 (8)
#define PHY_1BC_MLB_DB5_BT_WLVL			 (10)
#define PHY_1BC_MLB_DB6_BT_WLVL			 (12)
#define PHY_1BC_MLB_DB7_BT_WLVL			 (14)
#define PHY_1BC_MLB_DB8_BT_WLVL			 (16)

#define PHY_1BC_MLB_DB0_BT_WLVL_MASK	0x3
#define PHY_1BC_MLB_DB1_BT_WLVL_MASK	0x3
#define PHY_1BC_MLB_DB2_BT_WLVL_MASK	0x3
#define PHY_1BC_MLB_DB3_BT_WLVL_MASK	0x3
#define PHY_1BC_MLB_DB4_BT_WLVL_MASK	0x3
#define PHY_1BC_MLB_DB5_BT_WLVL_MASK	0x3
#define PHY_1BC_MLB_DB6_BT_WLVL_MASK	0x3
#define PHY_1BC_MLB_DB7_BT_WLVL_MASK	0x3
#define PHY_1BC_MLB_DB8_BT_WLVL_MASK	0x3
/*1c4*/
#define PHY_1BC_MLB_DB8_DLL_WLVL_WDQS		 (24)

#define PHY_1BC_MLB_DB8_DLL_WLVL_WDQS_MASK	0xff

/* ################################################################################
 * # PHY_1C0 Definition
 */
#define PHY_1C0_MLB_DB0_DLL_WLVL_WDQS		 (0)
#define PHY_1C0_MLB_DB1_DLL_WLVL_WDQS		 (8)
#define PHY_1C0_MLB_DB2_DLL_WLVL_WDQS		 (16)
#define PHY_1C0_MLB_DB3_DLL_WLVL_WDQS		 (24)

#define PHY_1C0_MLB_DB0_DLL_WLVL_WDQS_MASK	0xff
#define PHY_1C0_MLB_DB1_DLL_WLVL_WDQS_MASK	0xff
#define PHY_1C0_MLB_DB2_DLL_WLVL_WDQS_MASK	0xff
#define PHY_1C0_MLB_DB3_DLL_WLVL_WDQS_MASK	0xff

/* ################################################################################
 * # PHY_1C4 Definition
 */
#define PHY_1C4_MLB_DB4_DLL_WLVL_WDQS		 (0)
#define PHY_1C4_MLB_DB5_DLL_WLVL_WDQS		 (8)
#define PHY_1C4_MLB_DB6_DLL_WLVL_WDQS		 (16)
#define PHY_1C4_MLB_DB7_DLL_WLVL_WDQS		 (24)

#define PHY_1C4_MLB_DB4_DLL_WLVL_WDQS_MASK	0xff
#define PHY_1C4_MLB_DB5_DLL_WLVL_WDQS_MASK	0xff
#define PHY_1C4_MLB_DB6_DLL_WLVL_WDQS_MASK	0xff
#define PHY_1C4_MLB_DB7_DLL_WLVL_WDQS_MASK	0xff

/* ################################################################################
 * # PHY_1C8 Definition
 */
#define PHY_1C8_MLB_LOAD_ACK_CYCLE		 (0)
#define PHY_1C8_MLB_DFI_RDDATA_GOLD		 (8)
#define PHY_1C8_MLB_LOAD_RESP_CYCLE		 (16)
#define PHY_1C8_REFRESH_UPDATE			 (24)
#define PHY_1C8_ZQCS_UPDATE				 (25)

#define PHY_1C8_MLB_LOAD_ACK_CYCLE_MASK		0x3f
#define PHY_1C8_MLB_DFI_RDDATA_GOLD_MASK	0xff
#define PHY_1C8_MLB_LOAD_RESP_CYCLE_MASK	0x3f
#define PHY_1C8_REFRESH_UPDATE_MASK			0x1
#define PHY_1C8_ZQCS_UPDATE_MASK			0x1

/* ################################################################################
 * # PHY_1CC Definition
 */
#define PHY_1CC_DB0_DLL_GATE_TRIP		 (0)
#define PHY_1CC_DB1_DLL_GATE_TRIP		 (8)
#define PHY_1CC_DB2_DLL_GATE_TRIP		 (16)
#define PHY_1CC_DB3_DLL_GATE_TRIP		 (24)

#define PHY_1CC_DB0_DLL_GATE_TRIP_MASK		0xff
#define PHY_1CC_DB1_DLL_GATE_TRIP_MASK		0xff
#define PHY_1CC_DB2_DLL_GATE_TRIP_MASK		0xff
#define PHY_1CC_DB3_DLL_GATE_TRIP_MASK		0xff

/* ################################################################################
 * # PHY_1D0 Definition
 */
#define PHY_1D0_DB4_DLL_GATE_TRIP		 (0)
#define PHY_1D0_DB5_DLL_GATE_TRIP		 (8)
#define PHY_1D0_DB6_DLL_GATE_TRIP		 (16)
#define PHY_1D0_DB7_DLL_GATE_TRIP		 (24)

#define PHY_1D0_DB4_DLL_GATE_TRIP_MASK		0xff
#define PHY_1D0_DB5_DLL_GATE_TRIP_MASK		0xff
#define PHY_1D0_DB6_DLL_GATE_TRIP_MASK		0xff
#define PHY_1D0_DB7_DLL_GATE_TRIP_MASK		0xff

/* ################################################################################
 * # PHY_1D4 Definition
 */
#define PHY_1D4_DB0_DLL_GATE_FINE		 (0)
#define PHY_1D4_DB1_DLL_GATE_FINE		 (8)
#define PHY_1D4_DB2_DLL_GATE_FINE		 (16)
#define PHY_1D4_DB3_DLL_GATE_FINE		 (24)

#define PHY_1D4_DB0_DLL_GATE_FINE_MASK		0xff
#define PHY_1D4_DB1_DLL_GATE_FINE_MASK		0xff
#define PHY_1D4_DB2_DLL_GATE_FINE_MASK		0xff
#define PHY_1D4_DB3_DLL_GATE_FINE_MASK		0xff

/* ################################################################################
 * # PHY_1D8 Definition
 */
#define PHY_1D8_DB4_DLL_GATE_FINE		 (0)
#define PHY_1D8_DB5_DLL_GATE_FINE		 (8)
#define PHY_1D8_DB6_DLL_GATE_FINE		 (16)
#define PHY_1D8_DB7_DLL_GATE_FINE		 (24)

#define PHY_1D8_DB4_DLL_GATE_FINE_MASK		0xff
#define PHY_1D8_DB5_DLL_GATE_FINE_MASK		0xff
#define PHY_1D8_DB6_DLL_GATE_FINE_MASK		0xff
#define PHY_1D8_DB7_DLL_GATE_FINE_MASK		0xff

/* ################################################################################
 * # PHY_1DC Definition
 */
#define PHY_1DC_MLB_TRAIN_GATE_OFFSET_MINUS				 (0)
#define PHY_1DC_MLB_TRAIN_COARSE_GATE_DELAY_OFFSET		 (8)
#define PHY_1DC_MLB_TRAIN_FINE_GATE_DELAY_OFFSET		 (16)

#define PHY_1DC_MLB_TRAIN_GATE_OFFSET_MINUS_MASK			0x1
#define PHY_1DC_MLB_TRAIN_COARSE_GATE_DELAY_OFFSET_MASK		0xff
#define PHY_1DC_MLB_TRAIN_FINE_GATE_DELAY_OFFSET_MASK		0xff

/* ################################################################################
 * # PHY_1E0 Definition
 */
#define PHY_1E0_MLB_DB0_TRAIN_GATE_BIT_SEL		 (0)
#define PHY_1E0_MLB_DB1_TRAIN_GATE_BIT_SEL		 (4)
#define PHY_1E0_MLB_DB2_TRAIN_GATE_BIT_SEL		 (8)
#define PHY_1E0_MLB_DB3_TRAIN_GATE_BIT_SEL		 (12)
#define PHY_1E0_MLB_DB4_TRAIN_GATE_BIT_SEL		 (16)
#define PHY_1E0_MLB_DB5_TRAIN_GATE_BIT_SEL		 (20)
#define PHY_1E0_MLB_DB6_TRAIN_GATE_BIT_SEL		 (24)
#define PHY_1E0_MLB_DB7_TRAIN_GATE_BIT_SEL		 (28)

#define PHY_1E0_MLB_DB0_TRAIN_GATE_BIT_SEL_MASK		0xf
#define PHY_1E0_MLB_DB1_TRAIN_GATE_BIT_SEL_MASK		0xf
#define PHY_1E0_MLB_DB2_TRAIN_GATE_BIT_SEL_MASK		0xf
#define PHY_1E0_MLB_DB3_TRAIN_GATE_BIT_SEL_MASK		0xf
#define PHY_1E0_MLB_DB4_TRAIN_GATE_BIT_SEL_MASK		0xf
#define PHY_1E0_MLB_DB5_TRAIN_GATE_BIT_SEL_MASK		0xf
#define PHY_1E0_MLB_DB6_TRAIN_GATE_BIT_SEL_MASK		0xf
#define PHY_1E0_MLB_DB7_TRAIN_GATE_BIT_SEL_MASK		0xf



/*base:0x30600600*/
typedef struct ddrintf_phy_group1_s{
	u32 mlb_lpddr4_mpr1_data_3;		/*offset:0x600*/
	u32 mlb_lpddr4_mpr2_data_0;		/*offset:0x604*/
	u32 mlb_lpddr4_mpr2_data_1;		/*offset:0x608*/
	u32 mlb_lpddr4_mpr2_data_2;		/*offset:0x60c*/
	u32 mlb_lpddr4_mpr2_data_3;		/*offset:0x610*/
	u32 mlb_lpddr4_mpr3_data_0;		/*offset:0x614*/
	u32 mlb_lpddr4_mpr3_data_1;		/*offset:0x618*/
	u32 mlb_lpddr4_mpr3_data_2;		/*offset:0x61c*/
	u32 mlb_lpddr4_mpr3_data_3;		/*offset:0x620*/
	u32 phy_fdl_db0_dqs_rd;		 	/*offset:0x624*/
	u32 phy_fdl_db0_dq2_rd;		 	/*offset:0x628*/
	u32 phy_fdl_db0_dq6_rd;		 	/*offset:0x62c*/
	u32 phy_fdl_db1_dq0_rd;		 	/*offset:0x630*/
	u32 phy_fdl_db1_dq4_rd;		 	/*offset:0x634*/
	u32 phy_fdl_db2_dqs_rd;		 	/*offset:0x638*/
	u32 phy_fdl_db2_dq2_rd;		 	/*offset:0x63c*/
	u32 phy_fdl_db2_dq6_rd;		 	/*offset:0x640*/
	u32 phy_fdl_db3_dq0_rd;		 	/*offset:0x644*/
	u32 phy_fdl_db3_dq4_rd;		 	/*offset:0x648*/
	u32 phy_fdl_db4_dqs_rd;		 	/*offset:0x64c*/
	u32 phy_fdl_db4_dq2_rd;		 	/*offset:0x650*/
	u32 phy_fdl_db4_dq6_rd;		 	/*offset:0x654*/
	u32 phy_fdl_db5_dq0_rd;		 	/*offset:0x658*/
	u32 phy_fdl_db5_dq4_rd;		 	/*offset:0x65c*/
	u32 phy_fdl_db6_dqs_rd;		 	/*offset:0x660*/
	u32 phy_fdl_db6_dq2_rd;		 	/*offset:0x664*/
	u32 phy_fdl_db6_dq6_rd;		 	/*offset:0x668*/
	u32 phy_fdl_db7_dq0_rd;		 	/*offset:0x66c*/
	u32 phy_fdl_db7_dq4_rd;		 	/*offset:0x670*/
	u32 reserved_674;		 	/*offset:0x674*/
	u32 reserved_678;		 	/*offset:0x678*/
	u32 mlb_vref_phy_adj_start;		/*offset:0x67c*/
	u32 mlb_vref_phy_manu_db0;		/*offset:0x680*/
	u32 mlb_vref_phy_manu_db4;		/*offset:0x684*/
	u32 db0_dll_rddeye_dqs;		 	/*offset:0x688*/
	u32 db4_dll_rddeye_dqs;		 	/*offset:0x68c*/
	u32 db0_dll_rddeye_dqsn;		 	/*offset:0x690*/
	u32 db4_dll_rddeye_dqsn;		 	/*offset:0x694*/
	u32 mlb_train_deye_offset_minus;		 /*offset:0x698*/
	u32 phy_fdl_db0_dq0_wr;		 /*offset:0x69c*/
	u32 phy_fdl_db0_dq4_wr;		 /*offset:0x6a0*/
	u32 phy_fdl_db1_dqs_wr;		 /*offset:0x6a4*/
	u32 phy_fdl_db1_dq2_wr;		 /*offset:0x6a8*/
	u32 phy_fdl_db1_dq6_wr;		 /*offset:0x6ac*/
	u32 phy_fdl_db2_dq0_wr;		 /*offset:0x6b0*/
	u32 phy_fdl_db2_dq4_wr;		 /*offset:0x6b4*/
	u32 phy_fdl_db3_dqs_wr;		 /*offset:0x6b8*/
	u32 phy_fdl_db3_dq2_wr;		 /*offset:0x6bc*/
	u32 phy_fdl_db3_dq6_wr;		 /*offset:0x6c0*/
	u32 phy_fdl_db4_dq0_wr;		 /*offset:0x6c4*/
	u32 phy_fdl_db4_dq4_wr;		 /*offset:0x6c8*/
	u32 phy_fdl_db5_dqs_wr;		 /*offset:0x6cc*/
	u32 phy_fdl_db5_dq2_wr;		 /*offset:0x6d0*/
	u32 phy_fdl_db5_dq6_wr;		 /*offset:0x6d4*/
	u32 phy_fdl_db6_dq0_wr;		 /*offset:0x6d8*/
	u32 phy_fdl_db6_dq4_wr;		 /*offset:0x6dc*/
	u32 phy_fdl_db7_dqs_wr;		 /*offset:0x6e0*/
	u32 phy_fdl_db7_dq2_wr;		 /*offset:0x6e4*/
	u32 phy_fdl_db7_dq6_wr;		 /*offset:0x6e8*//*0x6ea:reserved74*/
	u32 reserved_6ec;		 /*offset:0x6ec*/
	u32 reserved_6f0;		 /*offset:0x6f0*/
	u32 mlb_vref_dram_value_manu;		 /*offset:0x6f4*/
	u32 db0_dll_wrdeye_dq;		 /*offset:0x6f8*/
	u32 db4_dll_wrdeye_dq;		 /*offset:0x6fc*/

}ddrintf_phy_group1_t; 

/* ################################################################################
 * # PHY_624 Definition
 */
#define PHY_624_PHY_FDL_DB0_DQS_RD	 (0)
#define PHY_624_PHY_FDL_DB0_DM_RD	 (8)
#define PHY_624_PHY_FDL_DB0_DQ0_RD	 (16)
#define PHY_624_PHY_FDL_DB0_DQ1_RD	 (24)

#define PHY_624_PHY_FDL_DB0_DQS_RD_MASK		0xff
#define PHY_624_PHY_FDL_DB0_DM_RD_MASK		0xff
#define PHY_624_PHY_FDL_DB0_DQ0_RD_MASK		0xff
#define PHY_624_PHY_FDL_DB0_DQ1_RD_MASK		0xff

/* ################################################################################
 * # PHY_628 Definition
 */
#define PHY_628_PHY_FDL_DB0_DQ2_RD	 (0)
#define PHY_628_PHY_FDL_DB0_DQ3_RD	 (8)
#define PHY_628_PHY_FDL_DB0_DQ4_RD	 (16)
#define PHY_628_PHY_FDL_DB0_DQ5_RD	 (24)

#define PHY_628_PHY_FDL_DB0_DQ2_RD_MASK		0xff
#define PHY_628_PHY_FDL_DB0_DQ3_RD_MASK		0xff
#define PHY_628_PHY_FDL_DB0_DQ4_RD_MASK		0xff
#define PHY_628_PHY_FDL_DB0_DQ5_RD_MASK		0xff

/* ################################################################################
 * # PHY_62C Definition
 */
#define PHY_62C_PHY_FDL_DB0_DQ6_RD	 (0)
#define PHY_62C_PHY_FDL_DB0_DQ7_RD	 (8)
#define PHY_62C_PHY_FDL_DB1_DQS_RD	 (16)
#define PHY_62C_PHY_FDL_DB1_DM_RD	 (24)

#define PHY_62C_PHY_FDL_DB0_DQ6_RD_MASK 0xff
#define PHY_62C_PHY_FDL_DB0_DQ7_RD_MASK 0xff
#define PHY_62C_PHY_FDL_DB1_DQS_RD_MASK 0xff
#define PHY_62C_PHY_FDL_DB1_DM_RD_MASK 0xff

/* ################################################################################
 * # PHY_630 Definition
 */
#define PHY_630_PHY_FDL_DB1_DQ0_RD	 (0)
#define PHY_630_PHY_FDL_DB1_DQ1_RD	 (8)
#define PHY_630_PHY_FDL_DB1_DQ2_RD	 (16)
#define PHY_630_PHY_FDL_DB1_DQ3_RD	 (24)

#define PHY_630_PHY_FDL_DB1_DQ0_RD_MASK		0xff
#define PHY_630_PHY_FDL_DB1_DQ1_RD_MASK		0xff
#define PHY_630_PHY_FDL_DB1_DQ2_RD_MASK		0xff
#define PHY_630_PHY_FDL_DB1_DQ3_RD_MASK		0xff

/* ################################################################################
 * # PHY_634 Definition
 */
#define PHY_634_PHY_FDL_DB1_DQ4_RD	 (0)
#define PHY_634_PHY_FDL_DB1_DQ5_RD	 (8)
#define PHY_634_PHY_FDL_DB1_DQ6_RD	 (16)
#define PHY_634_PHY_FDL_DB1_DQ7_RD	 (24)

#define PHY_634_PHY_FDL_DB1_DQ4_RD_MASK		0xff
#define PHY_634_PHY_FDL_DB1_DQ5_RD_MASK		0xff
#define PHY_634_PHY_FDL_DB1_DQ6_RD_MASK		0xff
#define PHY_634_PHY_FDL_DB1_DQ7_RD_MASK		0xff

/* ################################################################################
 * # PHY_638 Definition
 */
#define PHY_638_PHY_FDL_DB2_DQS_RD	 (0)
#define PHY_638_PHY_FDL_DB2_DM_RD	 (8)
#define PHY_638_PHY_FDL_DB2_DQ0_RD	 (16)
#define PHY_638_PHY_FDL_DB2_DQ1_RD	 (24)

#define PHY_638_PHY_FDL_DB2_DQS_RD_MASK		0xff
#define PHY_638_PHY_FDL_DB2_DM_RD_MASK		0xff
#define PHY_638_PHY_FDL_DB2_DQ0_RD_MASK		0xff
#define PHY_638_PHY_FDL_DB2_DQ1_RD_MASK		0xff

/* ################################################################################
 * # PHY_63C Definition
 */
#define PHY_63C_PHY_FDL_DB2_DQ2_RD	 (0)
#define PHY_63C_PHY_FDL_DB2_DQ3_RD	 (8)
#define PHY_63C_PHY_FDL_DB2_DQ4_RD	 (16)
#define PHY_63C_PHY_FDL_DB2_DQ5_RD	 (24)

#define PHY_63C_PHY_FDL_DB2_DQ2_RD_MASK		0xff
#define PHY_63C_PHY_FDL_DB2_DQ3_RD_MASK		0xff
#define PHY_63C_PHY_FDL_DB2_DQ4_RD_MASK		0xff
#define PHY_63C_PHY_FDL_DB2_DQ5_RD_MASK		0xff

/* ################################################################################
 * # PHY_640 Definition
 */
#define PHY_640_PHY_FDL_DB2_DQ6_RD	 (0)
#define PHY_640_PHY_FDL_DB2_DQ7_RD	 (8)
#define PHY_640_PHY_FDL_DB3_DQS_RD	 (16)
#define PHY_640_PHY_FDL_DB3_DM_RD	 (24)

#define PHY_640_PHY_FDL_DB2_DQ6_RD_MASK 	0xff
#define PHY_640_PHY_FDL_DB2_DQ7_RD_MASK 	0xff
#define PHY_640_PHY_FDL_DB3_DQS_RD_MASK 	0xff
#define PHY_640_PHY_FDL_DB3_DM_RD_MASK 		0xff

/* ################################################################################
 * # PHY_644 Definition
 */
#define PHY_644_PHY_FDL_DB3_DQ0_RD	 (0)
#define PHY_644_PHY_FDL_DB3_DQ1_RD	 (8)
#define PHY_644_PHY_FDL_DB3_DQ2_RD	 (16)
#define PHY_644_PHY_FDL_DB3_DQ3_RD	 (24)

#define PHY_644_PHY_FDL_DB3_DQ0_RD_MASK		0xff
#define PHY_644_PHY_FDL_DB3_DQ1_RD_MASK		0xff
#define PHY_644_PHY_FDL_DB3_DQ2_RD_MASK		0xff
#define PHY_644_PHY_FDL_DB3_DQ3_RD_MASK		0xff

/* ################################################################################
 * # PHY_648 Definition
 */
#define PHY_648_PHY_FDL_DB3_DQ4_RD	 (0)
#define PHY_648_PHY_FDL_DB3_DQ5_RD	 (8)
#define PHY_648_PHY_FDL_DB3_DQ6_RD	 (16)
#define PHY_648_PHY_FDL_DB3_DQ7_RD	 (24)

#define PHY_648_PHY_FDL_DB3_DQ4_RD_MASK		0xff
#define PHY_648_PHY_FDL_DB3_DQ5_RD_MASK		0xff
#define PHY_648_PHY_FDL_DB3_DQ6_RD_MASK		0xff
#define PHY_648_PHY_FDL_DB3_DQ7_RD_MASK		0xff

/* ################################################################################
 * # PHY_64C Definition
 */
#define PHY_64C_PHY_FDL_DB4_DQS_RD	 (0)
#define PHY_64C_PHY_FDL_DB4_DM_RD	 (8)
#define PHY_64C_PHY_FDL_DB4_DQ0_RD	 (16)
#define PHY_64C_PHY_FDL_DB4_DQ1_RD	 (24)

#define PHY_64C_PHY_FDL_DB4_DQS_RD_MASK		0xff
#define PHY_64C_PHY_FDL_DB4_DM_RD_MASK		0xff
#define PHY_64C_PHY_FDL_DB4_DQ0_RD_MASK		0xff
#define PHY_64C_PHY_FDL_DB4_DQ1_RD_MASK		0xff

/* ################################################################################
 * # PHY_650 Definition
 */
#define PHY_650_PHY_FDL_DB4_DQ2_RD	 (0)
#define PHY_650_PHY_FDL_DB4_DQ3_RD	 (8)
#define PHY_650_PHY_FDL_DB4_DQ4_RD	 (16)
#define PHY_650_PHY_FDL_DB4_DQ5_RD	 (24)

#define PHY_650_PHY_FDL_DB4_DQ2_RD_MASK		0xff
#define PHY_650_PHY_FDL_DB4_DQ3_RD_MASK		0xff
#define PHY_650_PHY_FDL_DB4_DQ4_RD_MASK		0xff
#define PHY_650_PHY_FDL_DB4_DQ5_RD_MASK		0xff

/* ################################################################################
 * # PHY_654 Definition
 */
#define PHY_654_PHY_FDL_DB4_DQ6_RD	 (0)
#define PHY_654_PHY_FDL_DB4_DQ7_RD	 (8)
#define PHY_654_PHY_FDL_DB5_DQS_RD	 (16)
#define PHY_654_PHY_FDL_DB5_DM_RD	 (24)

#define PHY_654_PHY_FDL_DB4_DQ6_RD_MASK 	0xff
#define PHY_654_PHY_FDL_DB4_DQ7_RD_MASK 	0xff
#define PHY_654_PHY_FDL_DB5_DQS_RD_MASK 	0xff
#define PHY_654_PHY_FDL_DB5_DM_RD_MASK 		0xff

/* ################################################################################
 * # PHY_658 Definition
 */
#define PHY_658_PHY_FDL_DB5_DQ0_RD	 (0)
#define PHY_658_PHY_FDL_DB5_DQ1_RD	 (8)
#define PHY_658_PHY_FDL_DB5_DQ2_RD	 (16)
#define PHY_658_PHY_FDL_DB5_DQ3_RD	 (24)

#define PHY_658_PHY_FDL_DB5_DQ0_RD_MASK		0xff
#define PHY_658_PHY_FDL_DB5_DQ1_RD_MASK		0xff
#define PHY_658_PHY_FDL_DB5_DQ2_RD_MASK		0xff
#define PHY_658_PHY_FDL_DB5_DQ3_RD_MASK		0xff

/* ################################################################################
 * # PHY_65C Definition
 */
#define PHY_65C_PHY_FDL_DB5_DQ4_RD	 (0)
#define PHY_65C_PHY_FDL_DB5_DQ5_RD	 (8)
#define PHY_65C_PHY_FDL_DB5_DQ6_RD	 (16)
#define PHY_65C_PHY_FDL_DB5_DQ7_RD	 (24)

#define PHY_65C_PHY_FDL_DB5_DQ4_RD_MASK		0xff
#define PHY_65C_PHY_FDL_DB5_DQ5_RD_MASK		0xff
#define PHY_65C_PHY_FDL_DB5_DQ6_RD_MASK		0xff
#define PHY_65C_PHY_FDL_DB5_DQ7_RD_MASK		0xff

/* ################################################################################
 * # PHY_660 Definition
 */
#define PHY_660_PHY_FDL_DB6_DQS_RD	 (0)
#define PHY_660_PHY_FDL_DB6_DM_RD	 (8)
#define PHY_660_PHY_FDL_DB6_DQ0_RD	 (16)
#define PHY_660_PHY_FDL_DB6_DQ1_RD	 (24)

#define PHY_660_PHY_FDL_DB6_DQS_RD_MASK		0xff
#define PHY_660_PHY_FDL_DB6_DM_RD_MASK		0xff
#define PHY_660_PHY_FDL_DB6_DQ0_RD_MASK		0xff
#define PHY_660_PHY_FDL_DB6_DQ1_RD_MASK		0xff

/* ################################################################################
 * # PHY_664 Definition
 */
#define PHY_664_PHY_FDL_DB6_DQ2_RD	 (0)
#define PHY_664_PHY_FDL_DB6_DQ3_RD	 (8)
#define PHY_664_PHY_FDL_DB6_DQ4_RD	 (16)
#define PHY_664_PHY_FDL_DB6_DQ5_RD	 (24)

#define PHY_664_PHY_FDL_DB6_DQ2_RD_MASK		0xff
#define PHY_664_PHY_FDL_DB6_DQ3_RD_MASK		0xff
#define PHY_664_PHY_FDL_DB6_DQ4_RD_MASK		0xff
#define PHY_664_PHY_FDL_DB6_DQ5_RD_MASK		0xff

/* ################################################################################
 * # PHY_668 Definition
 */
#define PHY_668_PHY_FDL_DB6_DQ6_RD	 (0)
#define PHY_668_PHY_FDL_DB6_DQ7_RD	 (8)
#define PHY_668_PHY_FDL_DB7_DQS_RD	 (16)
#define PHY_668_PHY_FDL_DB7_DM_RD	 (24)

#define PHY_668_PHY_FDL_DB6_DQ6_RD_MASK 	0xff
#define PHY_668_PHY_FDL_DB6_DQ7_RD_MASK 	0xff
#define PHY_668_PHY_FDL_DB7_DQS_RD_MASK 	0xff
#define PHY_668_PHY_FDL_DB7_DM_RD_MASK 		0xff

/* ################################################################################
 * # PHY_66C Definition
 */
#define PHY_66C_PHY_FDL_DB7_DQ0_RD	 (0)
#define PHY_66C_PHY_FDL_DB7_DQ1_RD	 (8)
#define PHY_66C_PHY_FDL_DB7_DQ2_RD	 (16)
#define PHY_66C_PHY_FDL_DB7_DQ3_RD	 (24)

#define PHY_66C_PHY_FDL_DB7_DQ0_RD_MASK		0xff
#define PHY_66C_PHY_FDL_DB7_DQ1_RD_MASK		0xff
#define PHY_66C_PHY_FDL_DB7_DQ2_RD_MASK		0xff
#define PHY_66C_PHY_FDL_DB7_DQ3_RD_MASK		0xff

/* ################################################################################
 * # PHY_670 Definition
 */
#define PHY_670_PHY_FDL_DB7_DQ4_RD	 (0)
#define PHY_670_PHY_FDL_DB7_DQ5_RD	 (8)
#define PHY_670_PHY_FDL_DB7_DQ6_RD	 (16)
#define PHY_670_PHY_FDL_DB7_DQ7_RD	 (24)

#define PHY_670_PHY_FDL_DB7_DQ4_RD_MASK		0xff
#define PHY_670_PHY_FDL_DB7_DQ5_RD_MASK		0xff
#define PHY_670_PHY_FDL_DB7_DQ6_RD_MASK		0xff
#define PHY_670_PHY_FDL_DB7_DQ7_RD_MASK		0xff

/* ################################################################################
 * # PHY_67C Definition
 */
#define PHY_67C_MLB_VREF_PHY_ADJ_START			 (16)
#define PHY_67C_MLB_VREF_PHY_VALUE_MANU_DB8		 (24)

#define PHY_67C_MLB_VREF_PHY_ADJ_START_MASK			0xff
#define PHY_67C_MLB_VREF_PHY_VALUE_MANU_DB8_MASK	0xff

/* ################################################################################
 * # PHY_680 Definition
 */
#define PHY_680_MLB_VREF_PHY_VALUE_MANU_DB0		 (0)
#define PHY_680_MLB_VREF_PHY_VALUE_MANU_DB1		 (8)
#define PHY_680_MLB_VREF_PHY_VALUE_MANU_DB2		 (16)
#define PHY_680_MLB_VREF_PHY_VALUE_MANU_DB3		 (24)

#define PHY_680_MLB_VREF_PHY_VALUE_MANU_DB0_MASK	0xff
#define PHY_680_MLB_VREF_PHY_VALUE_MANU_DB1_MASK	0xff
#define PHY_680_MLB_VREF_PHY_VALUE_MANU_DB2_MASK	0xff
#define PHY_680_MLB_VREF_PHY_VALUE_MANU_DB3_MASK	0xff

/* ################################################################################
 * # PHY_684 Definition
 */
#define PHY_684_MLB_VREF_PHY_VALUE_MANU_DB4		 (0)
#define PHY_684_MLB_VREF_PHY_VALUE_MANU_DB5		 (8)
#define PHY_684_MLB_VREF_PHY_VALUE_MANU_DB6		 (16)
#define PHY_684_MLB_VREF_PHY_VALUE_MANU_DB7		 (24)

#define PHY_684_MLB_VREF_PHY_VALUE_MANU_DB4_MASK	0xff
#define PHY_684_MLB_VREF_PHY_VALUE_MANU_DB5_MASK	0xff
#define PHY_684_MLB_VREF_PHY_VALUE_MANU_DB6_MASK	0xff
#define PHY_684_MLB_VREF_PHY_VALUE_MANU_DB7_MASK	0xff

/* ################################################################################
 * # PHY_688 Definition
 */
#define PHY_688_DB0_DLL_RDDEYE_DQS		 (0)
#define PHY_688_DB1_DLL_RDDEYE_DQS		 (8)
#define PHY_688_DB2_DLL_RDDEYE_DQS		 (16)
#define PHY_688_DB3_DLL_RDDEYE_DQS		 (24)

#define PHY_688_DB0_DLL_RDDEYE_DQS_MASK		0xff
#define PHY_688_DB1_DLL_RDDEYE_DQS_MASK		0xff
#define PHY_688_DB2_DLL_RDDEYE_DQS_MASK		0xff
#define PHY_688_DB3_DLL_RDDEYE_DQS_MASK		0xff

/* ################################################################################
 * # PHY_68C Definition
 */
#define PHY_68C_DB4_DLL_RDDEYE_DQS		 (0)
#define PHY_68C_DB5_DLL_RDDEYE_DQS		 (8)
#define PHY_68C_DB6_DLL_RDDEYE_DQS		 (16)
#define PHY_68C_DB7_DLL_RDDEYE_DQS		 (24)

#define PHY_68C_DB4_DLL_RDDEYE_DQS_MASK		0xff
#define PHY_68C_DB5_DLL_RDDEYE_DQS_MASK		0xff
#define PHY_68C_DB6_DLL_RDDEYE_DQS_MASK		0xff
#define PHY_68C_DB7_DLL_RDDEYE_DQS_MASK		0xff

/* ################################################################################
 * # PHY_690 Definition
 */
#define PHY_690_DB0_DLL_RDDEYE_DQSN		 (0)
#define PHY_690_DB1_DLL_RDDEYE_DQSN		 (8)
#define PHY_690_DB2_DLL_RDDEYE_DQSN		 (16)
#define PHY_690_DB3_DLL_RDDEYE_DQSN		 (24)

#define PHY_690_DB0_DLL_RDDEYE_DQSN_MASK		0xff
#define PHY_690_DB1_DLL_RDDEYE_DQSN_MASK		0xff
#define PHY_690_DB2_DLL_RDDEYE_DQSN_MASK		0xff
#define PHY_690_DB3_DLL_RDDEYE_DQSN_MASK		0xff

/* ################################################################################
 * # PHY_694 Definition
 */
#define PHY_694_DB4_DLL_RDDEYE_DQSN		 (0)
#define PHY_694_DB5_DLL_RDDEYE_DQSN		 (8)
#define PHY_694_DB6_DLL_RDDEYE_DQSN		 (16)
#define PHY_694_DB7_DLL_RDDEYE_DQSN		 (24)

#define PHY_694_DB4_DLL_RDDEYE_DQSN_MASK		0xff
#define PHY_694_DB5_DLL_RDDEYE_DQSN_MASK		0xff
#define PHY_694_DB6_DLL_RDDEYE_DQSN_MASK		0xff
#define PHY_694_DB7_DLL_RDDEYE_DQSN_MASK		0xff

/* ################################################################################
 * # PHY_698 Definition
 */
#define PHY_698_MLB_TRAIN_DEYE_OFFSET_MINUS		 (0)
#define PHY_698_MLB_TRAIN_DEYE_DELAY_OFFSET		 (8)
#define PHY_698_PHY_FDL_DB0_DQS_WR				 (16)
#define PHY_698_PHY_FDL_DB0_DM_WR				 (24)

#define PHY_698_MLB_TRAIN_DEYE_OFFSET_MINUS_MASK		0x1
#define PHY_698_MLB_TRAIN_DEYE_DELAY_OFFSET_MASK		0xff
#define PHY_698_PHY_FDL_DB0_DQS_WR_MASK				0xff
#define PHY_698_PHY_FDL_DB0_DM_WR_MASK					0xff

/* ################################################################################
 * # PHY_69C Definition
 */
#define PHY_69C_PHY_FDL_DB0_DQ0_WR				 (0)
#define PHY_69C_PHY_FDL_DB0_DQ1_WR				 (8)
#define PHY_69C_PHY_FDL_DB0_DQ2_WR				 (16)
#define PHY_69C_PHY_FDL_DB0_DQ3_WR				 (24)

#define PHY_69C_PHY_FDL_DB0_DQ0_WR_MASK			0xff
#define PHY_69C_PHY_FDL_DB0_DQ1_WR_MASK			0xff
#define PHY_69C_PHY_FDL_DB0_DQ2_WR_MASK			0xff
#define PHY_69C_PHY_FDL_DB0_DQ3_WR_MASK			0xff

/* ################################################################################
 * # PHY_6A0 Definition
 */
#define PHY_6A0_PHY_FDL_DB0_DQ4_WR				 (0)
#define PHY_6A0_PHY_FDL_DB0_DQ5_WR				 (8)
#define PHY_6A0_PHY_FDL_DB0_DQ6_WR				 (16)
#define PHY_6A0_PHY_FDL_DB0_DQ7_WR				 (24)

#define PHY_6A0_PHY_FDL_DB0_DQ4_WR_MASK			0xff
#define PHY_6A0_PHY_FDL_DB0_DQ5_WR_MASK			0xff
#define PHY_6A0_PHY_FDL_DB0_DQ6_WR_MASK			0xff
#define PHY_6A0_PHY_FDL_DB0_DQ7_WR_MASK			0xff

/* ################################################################################
 * # PHY_6A4 Definition
 */
#define PHY_6A4_PHY_FDL_DB1_DQS_WR				 (0)
#define PHY_6A4_PHY_FDL_DB1_DM_WR				 (8)
#define PHY_6A4_PHY_FDL_DB1_DQ0_WR				 (16)
#define PHY_6A4_PHY_FDL_DB1_DQ1_WR				 (24)

#define PHY_6A4_PHY_FDL_DB1_DQS_WR_MASK			0xff
#define PHY_6A4_PHY_FDL_DB1_DM_WR_MASK			0xff
#define PHY_6A4_PHY_FDL_DB1_DQ0_WR_MASK			0xff
#define PHY_6A4_PHY_FDL_DB1_DQ1_WR_MASK			0xff

/* ################################################################################
 * # PHY_6A8 Definition
 */
#define PHY_6A8_PHY_FDL_DB1_DQ2_WR				 (0)
#define PHY_6A8_PHY_FDL_DB1_DQ3_WR				 (8)
#define PHY_6A8_PHY_FDL_DB1_DQ4_WR				 (16)
#define PHY_6A8_PHY_FDL_DB1_DQ5_WR				 (24)

#define PHY_6A8_PHY_FDL_DB1_DQ2_WR_MASK			0xff
#define PHY_6A8_PHY_FDL_DB1_DQ3_WR_MASK			0xff
#define PHY_6A8_PHY_FDL_DB1_DQ4_WR_MASK			0xff
#define PHY_6A8_PHY_FDL_DB1_DQ5_WR_MASK			0xff

/* ################################################################################
 * # PHY_6AC Definition
 */
#define PHY_6AC_PHY_FDL_DB1_DQ6_WR				 (0)
#define PHY_6AC_PHY_FDL_DB1_DQ7_WR				 (8)
#define PHY_6AC_PHY_FDL_DB2_DQS_WR				 (16)
#define PHY_6AC_PHY_FDL_DB2_DM_WR				 (24)

#define PHY_6AC_PHY_FDL_DB1_DQ6_WR_MASK			0xff
#define PHY_6AC_PHY_FDL_DB1_DQ7_WR_MASK			0xff
#define PHY_6AC_PHY_FDL_DB2_DQS_WR_MASK			0xff
#define PHY_6AC_PHY_FDL_DB2_DM_WR_MASK			0xff

/* ################################################################################
 * # PHY_6B0 Definition
 */
#define PHY_6B0_PHY_FDL_DB2_DQ0_WR				 (0)
#define PHY_6B0_PHY_FDL_DB2_DQ1_WR				 (8)
#define PHY_6B0_PHY_FDL_DB2_DQ2_WR				 (16)
#define PHY_6B0_PHY_FDL_DB2_DQ3_WR				 (24)

#define PHY_6B0_PHY_FDL_DB2_DQ0_WR_MASK			0xff
#define PHY_6B0_PHY_FDL_DB2_DQ1_WR_MASK			0xff
#define PHY_6B0_PHY_FDL_DB2_DQ2_WR_MASK			0xff
#define PHY_6B0_PHY_FDL_DB2_DQ3_WR_MASK			0xff

/* ################################################################################
 * # PHY_6B4 Definition
 */
#define PHY_6B4_PHY_FDL_DB2_DQ4_WR				 (0)
#define PHY_6B4_PHY_FDL_DB2_DQ5_WR				 (8)
#define PHY_6B4_PHY_FDL_DB2_DQ6_WR				 (16)
#define PHY_6B4_PHY_FDL_DB2_DQ7_WR				 (24)

#define PHY_6B4_PHY_FDL_DB2_DQ4_WR_MASK			0xff
#define PHY_6B4_PHY_FDL_DB2_DQ5_WR_MASK			0xff
#define PHY_6B4_PHY_FDL_DB2_DQ6_WR_MASK			0xff
#define PHY_6B4_PHY_FDL_DB2_DQ7_WR_MASK			0xff

/* ################################################################################
 * # PHY_6B8 Definition
 */
#define PHY_6B8_PHY_FDL_DB3_DQS_WR				 (0)
#define PHY_6B8_PHY_FDL_DB3_DM_WR				 (8)
#define PHY_6B8_PHY_FDL_DB3_DQ0_WR				 (16)
#define PHY_6B8_PHY_FDL_DB3_DQ1_WR				 (24)

#define PHY_6B8_PHY_FDL_DB3_DQS_WR_MASK			0xff
#define PHY_6B8_PHY_FDL_DB3_DM_WR_MASK			0xff
#define PHY_6B8_PHY_FDL_DB3_DQ0_WR_MASK			0xff
#define PHY_6B8_PHY_FDL_DB3_DQ1_WR_MASK			0xff

/* ################################################################################
 * # PHY_6BC Definition
 */
#define PHY_6BC_PHY_FDL_DB3_DQ2_WR				 (0)
#define PHY_6BC_PHY_FDL_DB3_DQ3_WR				 (8)
#define PHY_6BC_PHY_FDL_DB3_DQ4_WR				 (16)
#define PHY_6BC_PHY_FDL_DB3_DQ5_WR				 (24)

#define PHY_6BC_PHY_FDL_DB3_DQ2_WR_MASK			0xff
#define PHY_6BC_PHY_FDL_DB3_DQ3_WR_MASK			0xff
#define PHY_6BC_PHY_FDL_DB3_DQ4_WR_MASK			0xff
#define PHY_6BC_PHY_FDL_DB3_DQ5_WR_MASK			0xff

/* ################################################################################
 * # PHY_6C0 Definition
 */
#define PHY_6C0_PHY_FDL_DB3_DQ6_WR				 (0)
#define PHY_6C0_PHY_FDL_DB3_DQ7_WR				 (8)
#define PHY_6C0_PHY_FDL_DB4_DQS_WR				 (16)
#define PHY_6C0_PHY_FDL_DB4_DM_WR				 (24)

#define PHY_6C0_PHY_FDL_DB3_DQ6_WR_MASK			0xff
#define PHY_6C0_PHY_FDL_DB3_DQ7_WR_MASK			0xff
#define PHY_6C0_PHY_FDL_DB4_DQS_WR_MASK			0xff
#define PHY_6C0_PHY_FDL_DB4_DM_WR_MASK			0xff

/* ################################################################################
 * # PHY_6C4 Definition
 */
#define PHY_6C4_PHY_FDL_DB4_DQ0_WR				 (0)
#define PHY_6C4_PHY_FDL_DB4_DQ1_WR				 (8)
#define PHY_6C4_PHY_FDL_DB4_DQ2_WR				 (16)
#define PHY_6C4_PHY_FDL_DB4_DQ3_WR				 (24)

#define PHY_6C4_PHY_FDL_DB4_DQ0_WR_MASK			0xff
#define PHY_6C4_PHY_FDL_DB4_DQ1_WR_MASK			0xff
#define PHY_6C4_PHY_FDL_DB4_DQ2_WR_MASK			0xff
#define PHY_6C4_PHY_FDL_DB4_DQ3_WR_MASK			0xff

/* ################################################################################
 * # PHY_6C8 Definition
 */
#define PHY_6C8_PHY_FDL_DB4_DQ4_WR				 (0)
#define PHY_6C8_PHY_FDL_DB4_DQ5_WR				 (8)
#define PHY_6C8_PHY_FDL_DB4_DQ6_WR				 (16)
#define PHY_6C8_PHY_FDL_DB4_DQ7_WR				 (24)

#define PHY_6C8_PHY_FDL_DB4_DQ4_WR_MASK			0xff
#define PHY_6C8_PHY_FDL_DB4_DQ5_WR_MASK			0xff
#define PHY_6C8_PHY_FDL_DB4_DQ6_WR_MASK			0xff
#define PHY_6C8_PHY_FDL_DB4_DQ7_WR_MASK			0xff

/* ################################################################################
 * # PHY_6CC Definition
 */
#define PHY_6CC_PHY_FDL_DB5_DQS_WR				 (0)
#define PHY_6CC_PHY_FDL_DB5_DM_WR				 (8)
#define PHY_6CC_PHY_FDL_DB5_DQ0_WR				 (16)
#define PHY_6CC_PHY_FDL_DB5_DQ1_WR				 (24)

#define PHY_6CC_PHY_FDL_DB5_DQS_WR_MASK			0xff
#define PHY_6CC_PHY_FDL_DB5_DM_WR_MASK			0xff
#define PHY_6CC_PHY_FDL_DB5_DQ0_WR_MASK			0xff
#define PHY_6CC_PHY_FDL_DB5_DQ1_WR_MASK			0xff

/* ################################################################################
 * # PHY_6D0 Definition
 */
#define PHY_6D0_PHY_FDL_DB5_DQ2_WR				 (0)
#define PHY_6D0_PHY_FDL_DB5_DQ3_WR				 (8)
#define PHY_6D0_PHY_FDL_DB5_DQ4_WR				 (16)
#define PHY_6D0_PHY_FDL_DB5_DQ5_WR				 (24)

#define PHY_6D0_PHY_FDL_DB5_DQ2_WR_MASK			0xff
#define PHY_6D0_PHY_FDL_DB5_DQ3_WR_MASK			0xff
#define PHY_6D0_PHY_FDL_DB5_DQ4_WR_MASK			0xff
#define PHY_6D0_PHY_FDL_DB5_DQ5_WR_MASK			0xff

/* ################################################################################
 * # PHY_6D4 Definition
 */
#define PHY_6D4_PHY_FDL_DB5_DQ6_WR				 (0)
#define PHY_6D4_PHY_FDL_DB5_DQ7_WR				 (8)
#define PHY_6D4_PHY_FDL_DB6_DQS_WR				 (16)
#define PHY_6D4_PHY_FDL_DB6_DM_WR				 (24)

#define PHY_6D4_PHY_FDL_DB5_DQ6_WR_MASK			0xff
#define PHY_6D4_PHY_FDL_DB5_DQ7_WR_MASK			0xff
#define PHY_6D4_PHY_FDL_DB6_DQS_WR_MASK			0xff
#define PHY_6D4_PHY_FDL_DB6_DM_WR_MASK			0xff

/* ################################################################################
 * # PHY_6D8 Definition
 */
#define PHY_6D8_PHY_FDL_DB6_DQ0_WR				 (0)
#define PHY_6D8_PHY_FDL_DB6_DQ1_WR				 (8)
#define PHY_6D8_PHY_FDL_DB6_DQ2_WR				 (16)
#define PHY_6D8_PHY_FDL_DB6_DQ3_WR				 (24)

#define PHY_6D8_PHY_FDL_DB6_DQ0_WR_MASK			0xff
#define PHY_6D8_PHY_FDL_DB6_DQ1_WR_MASK			0xff
#define PHY_6D8_PHY_FDL_DB6_DQ2_WR_MASK			0xff
#define PHY_6D8_PHY_FDL_DB6_DQ3_WR_MASK			0xff

/* ################################################################################
 * # PHY_6DC Definition
 */
#define PHY_6DC_PHY_FDL_DB6_DQ4_WR				 (0)
#define PHY_6DC_PHY_FDL_DB6_DQ5_WR				 (8)
#define PHY_6DC_PHY_FDL_DB6_DQ6_WR				 (16)
#define PHY_6DC_PHY_FDL_DB6_DQ7_WR				 (24)

#define PHY_6DC_PHY_FDL_DB6_DQ4_WR_MASK			0xff
#define PHY_6DC_PHY_FDL_DB6_DQ5_WR_MASK			0xff
#define PHY_6DC_PHY_FDL_DB6_DQ6_WR_MASK			0xff
#define PHY_6DC_PHY_FDL_DB6_DQ7_WR_MASK			0xff

/* ################################################################################
 * # PHY_6E0 Definition
 */
#define PHY_6E0_PHY_FDL_DB7_DQS_WR				 (0)
#define PHY_6E0_PHY_FDL_DB7_DM_WR				 (8)
#define PHY_6E0_PHY_FDL_DB7_DQ0_WR				 (16)
#define PHY_6E0_PHY_FDL_DB7_DQ1_WR				 (24)

#define PHY_6E0_PHY_FDL_DB7_DQS_WR_MASK			0xff
#define PHY_6E0_PHY_FDL_DB7_DM_WR_MASK			0xff
#define PHY_6E0_PHY_FDL_DB7_DQ0_WR_MASK			0xff
#define PHY_6E0_PHY_FDL_DB7_DQ1_WR_MASK			0xff

/* ################################################################################
 * # PHY_6E4 Definition
 */
#define PHY_6E4_PHY_FDL_DB7_DQ2_WR				 (0)
#define PHY_6E4_PHY_FDL_DB7_DQ3_WR				 (8)
#define PHY_6E4_PHY_FDL_DB7_DQ4_WR				 (16)
#define PHY_6E4_PHY_FDL_DB7_DQ5_WR				 (24)

#define PHY_6E4_PHY_FDL_DB7_DQ2_WR_MASK			0xff
#define PHY_6E4_PHY_FDL_DB7_DQ3_WR_MASK			0xff
#define PHY_6E4_PHY_FDL_DB7_DQ4_WR_MASK			0xff
#define PHY_6E4_PHY_FDL_DB7_DQ5_WR_MASK			0xff

/* ################################################################################
 * # PHY_6E8 Definition
 */
#define PHY_6E8_PHY_FDL_DB7_DQ6_WR				 (0)
#define PHY_6E8_PHY_FDL_DB7_DQ7_WR				 (8)
#define PHY_6E8_WRVREF_BYTE_SEL					 (16)/******6EA*******/

#define PHY_6E8_PHY_FDL_DB7_DQ6_WR_MASK			0xff
#define PHY_6E8_PHY_FDL_DB7_DQ7_WR_MASK			0xff
#define PHY_6E8_WRVREF_BYTE_SEL_MASK			0xf

/* ################################################################################
 * # PHY_6F4 Definition
 */
#define PHY_6F4_MLB_VREF_DRAM_VALUE_MANU	 (0)
#define PHY_6F4_MLB_VREF_DRAM_RANGE_MANU	 (6)

#define PHY_6F4_MLB_VREF_DRAM_VALUE_MANU_MASK		0x3f
#define PHY_6F4_MLB_VREF_DRAM_RANGE_MANU_MASK		0x1

/* ################################################################################
 * # PHY_6F8 Definition
 */
#define PHY_6F8_DB0_DLL_WRDEYE_DQ		 (0)
#define PHY_6F8_DB1_DLL_WRDEYE_DQ		 (8)
#define PHY_6F8_DB2_DLL_WRDEYE_DQ		 (16)
#define PHY_6F8_DB3_DLL_WRDEYE_DQ		 (24)

#define PHY_6F8_DB0_DLL_WRDEYE_DQ_MASK		0xff
#define PHY_6F8_DB1_DLL_WRDEYE_DQ_MASK		0xff
#define PHY_6F8_DB2_DLL_WRDEYE_DQ_MASK		0xff
#define PHY_6F8_DB3_DLL_WRDEYE_DQ_MASK		0xff

/* ################################################################################
 * # PHY_6FC Definition
 */
#define PHY_6FC_DB4_DLL_WRDEYE_DQ		 (0)
#define PHY_6FC_DB5_DLL_WRDEYE_DQ		 (8)
#define PHY_6FC_DB6_DLL_WRDEYE_DQ		 (16)
#define PHY_6FC_DB7_DLL_WRDEYE_DQ		 (24)

#define PHY_6FC_DB4_DLL_WRDEYE_DQ_MASK		0xff
#define PHY_6FC_DB5_DLL_WRDEYE_DQ_MASK		0xff
#define PHY_6FC_DB6_DLL_WRDEYE_DQ_MASK		0xff
#define PHY_6FC_DB7_DLL_WRDEYE_DQ_MASK		0xff




/*base:0x30600800*/
typedef struct ddrintf_phy_group2_s{
	u32 db0_bt_wdq;				/*offset:0x800*/
	u32 mlb_train_wdq_offset_minus;		/*offset:0x804*/
	u32 lb_bist_chk_sel;			/*offset:0x808*/
	u32 rank1_rtt_nom;			/*offset:0x80c*/
	u32 mlb_train_usr_bodypttn;			/*offset:0x810*/
	u32 mlb_mpr_wdata;			/*offset:0x814*/
	u32 mlb_r_t_wtr;				/*offset:0x818*/
	u32 mlb_train_read_check_num;		/*offset:0x81c*/
	u32 db8_dll_gate_fine;		/* offset:0x820 */
	u32 db8_dll_wrdeye_dq;		/* offset:0x824 */
	u32 phy_fdl_db8_dqs_rd;		/* offset:0x828 */
	u32 phy_fdl_db8_dq2_rd;		/* offset:0x82c */
	u32 phy_fdl_db8_dq6_rd;		/* offset:0x830 */
	u32 phy_fdl_db8_dqs_wr;		/* offset:0x834 */
	u32 phy_fdl_db8_dq2_wr;		/* offset:0x838 */
	u32 phy_fdl_db8_dq6_wr;		/* offset:0x83c */
	u32 mlb_train_usr_tailpttn;		/* offset:0x840 */
	u32 ctrl_db0_chain_delay_sel;			/* offset:0x844 */

}ddrintf_phy_group2_t;

/* ################################################################################
 * # PHY_800 Definition
 */
#define PHY_800_DB0_BT_WDQ 			 (0)
#define PHY_800_DB1_BT_WDQ 			 (4)
#define PHY_800_DB2_BT_WDQ 			 (8)
#define PHY_800_DB3_BT_WDQ 			 (12)
#define PHY_800_DB4_BT_WDQ 			 (16)
#define PHY_800_DB5_BT_WDQ 			 (20)
#define PHY_800_DB6_BT_WDQ 			 (24)
#define PHY_800_DB7_BT_WDQ 			 (28)

#define PHY_800_DB0_BT_WDQ_MASK		0xf
#define PHY_800_DB1_BT_WDQ_MASK		0xf
#define PHY_800_DB2_BT_WDQ_MASK		0xf
#define PHY_800_DB3_BT_WDQ_MASK		0xf
#define PHY_800_DB4_BT_WDQ_MASK		0xf
#define PHY_800_DB5_BT_WDQ_MASK		0xf
#define PHY_800_DB6_BT_WDQ_MASK		0xf
#define PHY_800_DB7_BT_WDQ_MASK		0xf

/* ################################################################################
 * # PHY_804 Definition
 */
#define PHY_804_MLB_TRAIN_WDQ_OFFSET_MINUS		 (0)
#define PHY_804_MLB_TRAIN_WDQ_DELAY_OFFSET		 (8)
#define PHY_804_LB_BIST_START					 (16)
#define PHY_804_LB_BIST_INTERNAL				 (17)
#define PHY_804_LB_BIST_DLL_DEYE_RDQS			 (24)

#define PHY_804_MLB_TRAIN_WDQ_OFFSET_MINUS_MASK		0x1
#define PHY_804_MLB_TRAIN_WDQ_DELAY_OFFSET_MASK		0xff
#define PHY_804_LB_BIST_START_MASK					0x1
#define PHY_804_LB_BIST_INTERNAL_MASK				0x1
#define PHY_804_LB_BIST_DLL_DEYE_RDQS_MASK			0xff

/* ################################################################################
 * # PHY_808 Definition
 */
#define PHY_808_LB_BIST_CHK_SEL		 (0)
#define PHY_808_LB_BIST0_TX_EN		 (4)
#define PHY_808_LB_BIST1_TX_EN		 (5)
#define PHY_808_LB_BIST2_TX_EN		 (6)
#define PHY_808_LB_BIST3_TX_EN		 (7)
#define PHY_808_LB_BIST0_RX_EN		 (8)
#define PHY_808_LB_BIST1_RX_EN		 (9)
#define PHY_808_LB_BIST2_RX_EN		 (10)
#define PHY_808_LB_BIST3_RX_EN		 (11)
#define PHY_808_LB_BIST0_ERRINJEC	 (12)
#define PHY_808_LB_BIST1_ERRINJEC	 (13)
#define PHY_808_LB_BIST2_ERRINJEC	 (14)
#define PHY_808_LB_BIST3_ERRINJEC	 (15)
#define PHY_808_PHY_OP_SEL			 (16)
#define PHY_808_PHY_PERBIT_SEL		 (24)

#define PHY_808_LB_BIST_CHK_SEL_MASK		0xf
#define PHY_808_LB_BIST0_TX_EN_MASK		0x1
#define PHY_808_LB_BIST1_TX_EN_MASK		0x1
#define PHY_808_LB_BIST2_TX_EN_MASK		0x1
#define PHY_808_LB_BIST3_TX_EN_MASK		0x1
#define PHY_808_LB_BIST0_RX_EN_MASK		0x1
#define PHY_808_LB_BIST1_RX_EN_MASK		0x1
#define PHY_808_LB_BIST2_RX_EN_MASK		0x1
#define PHY_808_LB_BIST3_RX_EN_MASK		0x1
#define PHY_808_LB_BIST0_ERRINJEC_MASK	0x1
#define PHY_808_LB_BIST1_ERRINJEC_MASK	0x1
#define PHY_808_LB_BIST2_ERRINJEC_MASK	0x1
#define PHY_808_LB_BIST3_ERRINJEC_MASK	0x1
#define PHY_808_PHY_OP_SEL_MASK			0x3
#define PHY_808_PHY_PERBIT_SEL_MASK		0xff

/* ################################################################################
 * # PHY_80C Definition
 */
#define PHY_80C_RANK1_RTT_NOM		 (0)
#define PHY_80C_RANK2_RTT_NOM		 (4)
#define PHY_80C_RANK3_RTT_NOM		 (8)
#define PHY_80C_RANK1_RTT_WR		 (12)
#define PHY_80C_RANK2_RTT_WR		 (14)
#define PHY_80C_RANK3_RTT_WR		 (16)

#define PHY_80C_RANK1_RTT_NOM_MASK	0x7
#define PHY_80C_RANK2_RTT_NOM_MASK	0x7
#define PHY_80C_RANK3_RTT_NOM_MASK	0x7
#define PHY_80C_RANK1_RTT_WR_MASK	0x3
#define PHY_80C_RANK2_RTT_WR_MASK	0x3
#define PHY_80C_RANK3_RTT_WR_MASK	0x3

/* ################################################################################
 * # PHY_810 Definition
 */
#define PHY_810_MLB_TRAIN_USR_BODYPTTN_0		 (0)
#define PHY_810_MLB_TRAIN_USR_BODYPTTN_1		 (8)
#define PHY_810_MLB_TRAIN_USR_BODYPTTN_2		 (16)
#define PHY_810_MLB_TRAIN_USR_BODYPTTN_3		 (24)
#define PHY_810_MLB_TRAIN_USR_BODYPTTN_4		 (0)
#define PHY_810_MLB_TRAIN_USR_BODYPTTN_5		 (8)
#define PHY_810_MLB_TRAIN_USR_BODYPTTN_6		 (16)
#define PHY_810_MLB_TRAIN_USR_BODYPTTN_7		 (24)

#define PHY_810_MLB_TRAIN_USR_BODYPTTN_0_MASK	0xff
#define PHY_810_MLB_TRAIN_USR_BODYPTTN_1_MASK	0xff
#define PHY_810_MLB_TRAIN_USR_BODYPTTN_2_MASK	0xff
#define PHY_810_MLB_TRAIN_USR_BODYPTTN_3_MASK	0xff
#define PHY_810_MLB_TRAIN_USR_BODYPTTN_4_MASK	0xff
#define PHY_810_MLB_TRAIN_USR_BODYPTTN_5_MASK	0xff
#define PHY_810_MLB_TRAIN_USR_BODYPTTN_6_MASK	0xff
#define PHY_810_MLB_TRAIN_USR_BODYPTTN_7_MASK	0xff

/* ################################################################################
 * # PHY_814 Definition
 */
#define PHY_814_MLB_MPR0_WDARA		 (0)
#define PHY_814_MLB_MPR1_WDARA		 (8)
#define PHY_814_MLB_MPR2_WDARA		 (16)
#define PHY_814_MLB_MPR3_WDARA		 (24)

#define PHY_814_MLB_MPR0_WDARA_MASK		0xff
#define PHY_814_MLB_MPR1_WDARA_MASK		0xff
#define PHY_814_MLB_MPR2_WDARA_MASK		0xff
#define PHY_814_MLB_MPR3_WDARA_MASK		0xff

/* ################################################################################
 * # PHY_818 Definition
 */
#define PHY_818_MLB_R_T_WTR			 (0)
#define PHY_818_MLB_R_T_RAS			 (8)
#define PHY_818_MLB_R_T_WTP			 (16)
#define PHY_818_MLB_R_T_RTP			 (24)

#define PHY_818_MLB_R_T_WTR_MASK		0xff
#define PHY_818_MLB_R_T_RAS_MASK		0xff
#define PHY_818_MLB_R_T_WTP_MASK		0xff
#define PHY_818_MLB_R_T_RTP_MASK		0xff

/* ################################################################################
 * # PHY_81C Definition
 */
#define PHY_81C_MLB_TRAIN_READ_CHECK_NUM		 (0)
#define PHY_81C_MLB_TRAIN_PERBIT_BURST_NUM		 (8)
#define PHY_81C_MLB_PERBIT_ONE_PASS_MODE		 (14)
#define PHY_81C_MLB_TRAIN_USR_DMPTTN			 (16)

#define PHY_81C_MLB_TRAIN_READ_CHECK_NUM_MASK		0xf
#define PHY_81C_MLB_TRAIN_PERBIT_BURST_NUM_MASK		0x3f
#define PHY_81C_MLB_PERBIT_ONE_PASS_MODE_MASK		0x1
#define PHY_81C_MLB_TRAIN_USR_DMPTTN_MASK			0xff

/* ################################################################################
 * # PHY_820 Definition
 */
#define PHY_820_DB8_DLL_GATE_FINE			 (0)
#define PHY_820_DB8_DLL_GATE_TRIP			 (8)
#define PHY_820_DB8_DLL_RDDEYE_DQS			 (16)
#define PHY_820_DB8_DLL_RDDEYE_DQSN			 (24)

#define PHY_820_DB8_DLL_GATE_FINE_MASK		0xff
#define PHY_820_DB8_DLL_GATE_TRIP_MASK		0xff
#define PHY_820_DB8_DLL_RDDEYE_DQS_MASK		0xff
#define PHY_820_DB8_DLL_RDDEYE_DQSN_MASK	0xff

/* ################################################################################
 * # PHY_824 Definition
 */
#define PHY_824_DB8_DLL_WRDEYE_DQ				 (0)
#define PHY_824_DB8_BT_WDQ						 (8)
#define PHY_824_MLB_DB8_TRAIN_GATE_BIT_SEL		 (12)

#define PHY_824_DB8_DLL_WRDEYE_DQ_MSK			0xff
#define PHY_824_DB8_BT_WDQ_MASK					0xf
#define PHY_824_MLB_DB8_TRAIN_GATE_BIT_SEL_MASK	0xf

/* ################################################################################
 * # PHY_828 Definition
 */
#define PHY_828_PHY_FDL_DB8_DQS_RD			 (0)
#define PHY_828_PHY_FDL_DB8_DM_RD			 (8)
#define PHY_828_PHY_FDL_DB8_DQ0_RD			 (16)
#define PHY_828_PHY_FDL_DB8_DQ1_RD			 (24)

#define PHY_828_PHY_FDL_DB8_DQS_RD_MASK		0xff
#define PHY_828_PHY_FDL_DB8_DM_RD_MASK		0xff
#define PHY_828_PHY_FDL_DB8_DQ0_RD_MASK		0xff
#define PHY_828_PHY_FDL_DB8_DQ1_RD_MASK		0xff

/* ################################################################################
 * # PHY_82C Definition
 */
#define PHY_82C_PHY_FDL_DB8_DQ2_RD			 (0)
#define PHY_82C_PHY_FDL_DB8_DQ3_RD			 (8)
#define PHY_82C_PHY_FDL_DB8_DQ4_RD			 (16)
#define PHY_82C_PHY_FDL_DB8_DQ5_RD			 (24)

#define PHY_82C_PHY_FDL_DB8_DQ2_RD_MASK		0xff
#define PHY_82C_PHY_FDL_DB8_DQ3_RD_MASK		0xff
#define PHY_82C_PHY_FDL_DB8_DQ4_RD_MASK		0xff
#define PHY_82C_PHY_FDL_DB8_DQ5_RD_MASK		0xff

/* ################################################################################
 * # PHY_830 Definition
 */
#define PHY_830_PHY_FDL_DB8_DQ6_RD			 (0)
#define PHY_830_PHY_FDL_DB8_DQ7_RD			 (8)

#define PHY_830_PHY_FDL_DB8_DQ6_RD_MASK		0xff
#define PHY_830_PHY_FDL_DB8_DQ7_RD_MASK		0xff

/* ################################################################################
 * # PHY_834 Definition
 */
#define PHY_834_PHY_FDL_DB8_DM_WR			 (8)
#define PHY_834_PHY_FDL_DB8_DQ0_WR			 (16)
#define PHY_834_PHY_FDL_DB8_DQ1_WR			 (24)

#define PHY_834_PHY_FDL_DB8_DQS_WR_MASK		0xff
#define PHY_834_PHY_FDL_DB8_DM_WR_MASK		0xff
#define PHY_834_PHY_FDL_DB8_DQ0_WR_MASK		0xff
#define PHY_834_PHY_FDL_DB8_DQ1_WR_MASK		0xff

/* ################################################################################
 * # PHY_838 Definition
 */
#define PHY_838_PHY_FDL_DB8_DQ2_WR			 (0)
#define PHY_838_PHY_FDL_DB8_DQ3_WR			 (8)
#define PHY_838_PHY_FDL_DB8_DQ4_WR			 (16)
#define PHY_838_PHY_FDL_DB8_DQ5_WR			 (24)

#define PHY_838_PHY_FDL_DB8_DQ2_WR_MASK		0xff
#define PHY_838_PHY_FDL_DB8_DQ3_WR_MASK		0xff
#define PHY_838_PHY_FDL_DB8_DQ4_WR_MASK		0xff
#define PHY_838_PHY_FDL_DB8_DQ5_WR_MASK		0xff

/* ################################################################################
 * # PHY_83C Definition
 */
#define PHY_83C_PHY_FDL_DB8_DQ6_WR			 (0)
#define PHY_83C_PHY_FDL_DB8_DQ7_WR			 (8)
#define PHY_83C_GATE_TRACKING_STEP			 (16)
#define PHY_83C_GATE_TRACKING_EN			 (24)

#define PHY_83C_PHY_FDL_DB8_DQ6_WR_MASK		0xff
#define PHY_83C_PHY_FDL_DB8_DQ7_WR_MASK		0xff
#define PHY_83C_GATE_TRACKING_STEP_MASK		0x1f	
#define PHY_83C_GATE_TRACKING_EN_MASK		0x1

/* ################################################################################
 * # PHY_840 Definition
 */
#define PHY_840_MLB_TRAIN_USR_TAILPTTN_0			 (0)
#define PHY_840_MLB_TRAIN_USR_TAILPTTN_1			 (8)
#define PHY_840_MLB_TRAIN_USR_TAILPTTN_2			 (16)
#define PHY_840_MLB_TRAIN_USR_TAILPTTN_3			 (24)
#define PHY_840_MLB_TRAIN_USR_TAILPTTN_4			 (0)
#define PHY_840_MLB_TRAIN_USR_TAILPTTN_5			 (8)
#define PHY_840_MLB_TRAIN_USR_TAILPTTN_6			 (16)
#define PHY_840_MLB_TRAIN_USR_TAILPTTN_7			 (24)

#define PHY_840_MLB_TRAIN_USR_TAILPTTN_0_MASK	0xff
#define PHY_840_MLB_TRAIN_USR_TAILPTTN_1_MASK	0xff
#define PHY_840_MLB_TRAIN_USR_TAILPTTN_2_MASK	0xff
#define PHY_840_MLB_TRAIN_USR_TAILPTTN_3_MASK	0xff
#define PHY_840_MLB_TRAIN_USR_TAILPTTN_4_MASK	0xff
#define PHY_840_MLB_TRAIN_USR_TAILPTTN_5_MASK	0xff
#define PHY_840_MLB_TRAIN_USR_TAILPTTN_6_MASK	0xff
#define PHY_840_MLB_TRAIN_USR_TAILPTTN_7_MASK	0xff

/* ################################################################################
 * # PHY_844 Definition
 */
#define PHY_844_CTRL_DB0_CHAIN_DELAY_SEL		 (0)
#define PHY_844_CTRL_DB1_CHAIN_DELAY_SEL		 (4)
#define PHY_844_CTRL_DB2_CHAIN_DELAY_SEL		 (8)
#define PHY_844_CTRL_DB3_CHAIN_DELAY_SEL		 (12)
#define PHY_844_CTRL_DB0_GATE_TRACK_SEL			 (16)
#define PHY_844_CTRL_DB1_GATE_TRACK_SEL			 (17)
#define PHY_844_CTRL_DB2_GATE_TRACK_SEL			 (18)
#define PHY_844_CTRL_DB3_GATE_TRACK_SEL			 (19)
#define PHY_844_CTRL_DB0_GATE_OE				 (20)
#define PHY_844_CTRL_DB1_GATE_OE				 (22)
#define PHY_844_CTRL_DB2_GATE_OE				 (24)
#define PHY_844_CTRL_DB3_GATE_OE				 (26)
#define PHY_844_CTRL_GATE_OE_MODE				 (28)
#define PHY_844_CTRL_NONWR_OE_VALUE				 (29)
#define PHY_844_CTRL_IO_LP4_EN					 (30)
#define PHY_844_CTRL_RSTART_DIS					 (31)

#define PHY_844_CTRL_DB0_CHAIN_DELAY_SEL_MASK		0x7
#define PHY_844_CTRL_DB1_CHAIN_DELAY_SEL_MASK		0x7
#define PHY_844_CTRL_DB2_CHAIN_DELAY_SEL_MASK		0x7
#define PHY_844_CTRL_DB3_CHAIN_DELAY_SEL_MASK		0x7
#define PHY_844_CTRL_DB0_GATE_TRACK_SEL_MASK		0x1
#define PHY_844_CTRL_DB1_GATE_TRACK_SEL_MASK		0x1
#define PHY_844_CTRL_DB2_GATE_TRACK_SEL_MASK		0x1
#define PHY_844_CTRL_DB3_GATE_TRACK_SEL_MASK		0x1
#define PHY_844_CTRL_DB0_GATE_OE_MASK				0x3
#define PHY_844_CTRL_DB1_GATE_OE_MASK				0x3
#define PHY_844_CTRL_DB2_GATE_OE_MASK				0x3
#define PHY_844_CTRL_DB3_GATE_OE_MASK				0x3
#define PHY_844_CTRL_GATE_OE_MODE_MASK				0x1
#define PHY_844_CTRL_NONWR_OE_VALUE_MASK			0x1
#define PHY_844_CTRL_IO_LP4_EN_MASK					0x1
#define PHY_844_CTRL_RSTART_DIS_MASK				0x1



/* DDR Phy read only Registers */
/*base:0x30600400*/
typedef struct ddrintf_phy_ro_group_s{
	u32	ro_phy_status;	 	 		/* offset:0x400 */
	u32	rsv1[12];	 					/* offset:0x404~0x430 */
	u32	ro_wrlvl_coarse_result;	 	/* offset:0x434 */
	u32	ro_wrlvl_fine_result;	 		/* offset:0x438 */
	u32	rsv_43c;	 					/* offset:0x43c */
	u32	ro_gate_train_result_db01;	/* offset:0x440 */
	u32	ro_gate_train_result_db23;	/* offset:0x444 */
	u32	rsv2[2];	 					/* offset:0x448~0x44c */
	u32	ro_gate_train_pass_window_db01;	 	/* offset:0x450 */
	u32	ro_gate_train_pass_window_db23;	 	/* offset:0x454 */
	u32 rsv3[2];								/* offset:0x458~0x45c */
	u32 ro_rdeye_train_rise_result;			/* offset:0x460 */
	u32 rsv_464;								/* offset:0x464 */
	u32 ro_rdeye_train_rise_pass_window;		/* offset:0x468 */
	u32 rsv_46c;		 						/* offset:0x46c */
	u32 ro_weye_train_coarse_result; 			/* offset:0x470 */
	u32 ro_weye_train_fine_result;		 		/* offset:0x474 */
	u32 rsv_478;	 							/* offset:0x478 */
	u32 ro_weye_train_pass_window;	 		/* offset:0x47c */
	u32 rsv4[2];		 						/* offset:0x480~x484 */
	u32 ro_rd_vref_result;	 				/* offset:0x488 */
	u32 rsv_48c;	 							/* offset:0x48c */
	u32 ro_dram_vref_result;	 				/* offset:0x490 */
	u32 rsv5[11];	 							/* offset:0x494~0x4bc */
	u32 ro_rdeye_train_fall_result;	 			/* offset:0x4c0 */
	u32 rsv_4c4;	 							/* offset:0x4c4 */
	u32 ro_rdeye_train_fall_pass_window;		/* offset:0x4c8 */
}ddrintf_phy_ro_group_t;


/* ################################################################################
 * # PHY_400 Definition
 */
#define PHY_400_RO_PLL_LOCK 			 	(0)
#define PHY_400_RO_DFI_INIT_COMPLETE 		(1)
#define PHY_400_RO_CAL_DONE 			 	(2)
#define PHY_400_RO_CAL_FAIL 			 	(3)
#define PHY_400_RO_CAL_RUNTIME_DONE 		(4)
#define PHY_400_RO_CAL_RUNTIME_FAIL		(5)
#define PHY_400_RO_AUTOMRS_START 		(6)
#define PHY_400_RO_AUTOMRS_DONE 			(7)
#define PHY_400_RO_CA_PERBIT_START		(8)
#define PHY_400_RO_CA_PERBIT_DONE 		(9)
#define PHY_400_RO_CA_START 				(10)
#define PHY_400_RO_CA_DONE				(11)
#define PHY_400_RO_WRLVL_START			(12)
#define PHY_400_RO_WRLVL_DONE			(13)
#define PHY_400_RO_GATE_TRAIN_START		(14)
#define PHY_400_RO_GATE_TRAIN_DONE		(15)
#define PHY_400_RO_RD_PERBIT_START		(16)
#define PHY_400_RO_RD_PERBIT_DONE		(17)
#define PHY_400_RO_RD_EYE_START			(18)
#define PHY_400_RO_RD_EYE_DONE			(19)
#define PHY_400_RO_WR_PERBIT_START		(20)
#define PHY_400_RO_WR_PERBIT_DONE		(21)
#define PHY_400_RO_WR_EYE_START			(22)
#define PHY_400_RO_WR_EYE_DONE			(23)
#define PHY_400_RO_VREF_START				(24)
#define PHY_400_RO_VREF_DONE				(25)
#define PHY_400_RO_TRAIN_START			(26)
#define PHY_400_RO_TRAIN_DONE			(27)


/* ################################################################################
 * # PHY_434 Definition
 */
#define PHY_434_RO_WRLVL_DB0_COARSE_RESULT		(16)
#define PHY_434_RO_WRLVL_DB1_COARSE_RESULT		(18)
#define PHY_434_RO_WRLVL_DB2_COARSE_RESULT		(20)


#define PHY_434_RO_WRLVL_DB0_COARSE_RESULT_MASK		0x3
#define PHY_434_RO_WRLVL_DB1_COARSE_RESULT_MASK		0x3
#define PHY_434_RO_WRLVL_DB2_COARSE_RESULT_MASK		0x3

/* ################################################################################
 * # PHY_438 Definition
 */
#define PHY_438_RO_WRLVL_DB0_FINE_RESULT		(0)
#define PHY_438_RO_WRLVL_DB1_FINE_RESULT		(8)
#define PHY_438_RO_WRLVL_DB2_FINE_RESULT		(16)


#define PHY_438_RO_WRLVL_DB0_FINE_RESULT_MASK		0xff
#define PHY_438_RO_WRLVL_DB1_FINE_RESULT_MASK		0xff
#define PHY_438_RO_WRLVL_DB2_FINE_RESULT_MASK		0xff

/* ################################################################################
 * # PHY_440 Definition
 */
#define PHY_440_RO_GATE_TRAIN_DB0_RESULT		(0)
#define PHY_440_RO_GATE_TRAIN_DB1_RESULT		(16)


#define PHY_440_RO_GATE_TRAIN_DB0_RESULT_MASK		0xffff
#define PHY_440_RO_GATE_TRAIN_DB1_RESULT_MASK		0xffff

/* ################################################################################
 * # PHY_444 Definition
 */
#define PHY_444_RO_GATE_TRAIN_DB2_RESULT		(0)


#define PHY_444_RO_GATE_TRAIN_DB2_RESULT_MASK		0xffff

/* ################################################################################
 * # PHY_450 Definition
 */
#define PHY_450_RO_GATE_TRAIN_DB0_PASS_WINDOW		(0)
#define PHY_450_RO_GATE_TRAIN_DB1_PASS_WINDOW		(16)


#define PHY_450_RO_GATE_TRAIN_DB0_PASS_WINDOW_MASK		0xffff
#define PHY_450_RO_GATE_TRAIN_DB1_PASS_WINDOW_MASK		0xffff

/* ################################################################################
 * # PHY_454 Definition
 */
#define PHY_454_RO_GATE_TRAIN_DB2_PASS_WINDOW		(0)


#define PHY_454_RO_GATE_TRAIN_DB2_PASS_WINDOW_MASK		0xffff

/* ################################################################################
 * # PHY_460 Definition
 */
#define PHY_460_RO_RDEYE_TRAIN_DB0_RISE_RESULT		(0)
#define PHY_460_RO_RDEYE_TRAIN_DB1_RISE_RESULT		(8)
#define PHY_460_RO_RDEYE_TRAIN_DB2_RISE_RESULT		(16)


#define PHY_460_RO_RDEYE_TRAIN_DB0_RISE_RESULT_MASK		0xff
#define PHY_460_RO_RDEYE_TRAIN_DB1_RISE_RESULT_MASK		0xff
#define PHY_460_RO_RDEYE_TRAIN_DB2_RISE_RESULT_MASK		0xff

/* ################################################################################
 * # PHY_468 Definition
 */
#define PHY_468_RO_RDEYE_TRAIN_DB0_RISE_PASS_WINDOW		(0)
#define PHY_468_RO_RDEYE_TRAIN_DB1_RISE_PASS_WINDOW		(8)
#define PHY_468_RO_RDEYE_TRAIN_DB2_RISE_PASS_WINDOW		(16)

#define PHY_468_RO_RDEYE_TRAIN_DB0_RISE_PASS_WINDOW_MASK		0xff
#define PHY_468_RO_RDEYE_TRAIN_DB1_RISE_PASS_WINDOW_MASK		0xff
#define PHY_468_RO_RDEYE_TRAIN_DB2_RISE_PASS_WINDOW_MASK		0xff

/* ################################################################################
 * # PHY_470 Definition
 */
#define PHY_470_RO_WREYE_TRAIN_DB0_COARSE_RESULT		(0)
#define PHY_470_RO_WREYE_TRAIN_DB1_COARSE_RESULT		(4)
#define PHY_470_RO_WREYE_TRAIN_DB2_COARSE_RESULT		(8)

#define PHY_470_RO_WREYE_TRAIN_DB0_COARSE_RESULT_MASK		0xf
#define PHY_470_RO_WREYE_TRAIN_DB1_COARSE_RESULT_MASK		0xf
#define PHY_470_RO_WREYE_TRAIN_DB2_COARSE_RESULT_MASK		0xf

/* ################################################################################
 * # PHY_474 Definition
 */
#define PHY_474_RO_WREYE_TRAIN_DB0_FINE_RESULT		(0)
#define PHY_474_RO_WREYE_TRAIN_DB1_FINE_RESULT		(8)
#define PHY_474_RO_WREYE_TRAIN_DB2_FINE_RESULT		(16)

#define PHY_474_RO_WREYE_TRAIN_DB0_FINE_RESULT_MASK		0xff
#define PHY_474_RO_WREYE_TRAIN_DB1_FINE_RESULT_MASK		0xff
#define PHY_474_RO_WREYE_TRAIN_DB2_FINE_RESULT_MASK		0xff

/* ################################################################################
 * # PHY_47c Definition
 */
#define PHY_47C_RO_WREYE_TRAIN_DB0_PASS_WINDOW		(0)
#define PHY_47C_RO_WREYE_TRAIN_DB1_PASS_WINDOW		(8)
#define PHY_47C_RO_WREYE_TRAIN_DB2_PASS_WINDOW		(16)

#define PHY_47C_RO_WREYE_TRAIN_DB0_PASS_WINDOW_MASK		0xff
#define PHY_47C_RO_WREYE_TRAIN_DB1_PASS_WINDOW_MASK		0xff
#define PHY_47C_RO_WREYE_TRAIN_DB2_PASS_WINDOW_MASK		0xff

/* ################################################################################
 * # PHY_488 Definition
 */
#define PHY_488_RO_RDVERF_TRAIN_DB0_RESULT		(0)
#define PHY_488_RO_RDVERF_TRAIN_DB1_RESULT		(8)
#define PHY_488_RO_RDVERF_TRAIN_DB2_RESULT		(16)

#define PHY_488_RO_RDVERF_TRAIN_DB0_RESULT_MASK		0xff
#define PHY_488_RO_RDVERF_TRAIN_DB1_RESULT_MASK		0xff
#define PHY_488_RO_RDVERF_TRAIN_DB2_RESULT_MASK		0xff


/* ################################################################################
 * # PHY_490 Definition
 */
#define PHY_490_RO_DRAM_VERF_TRAIN_RESULT		(0)


#define PHY_490_RO_RDVERF_TRAIN_RESULT_MASK		0x3f



/* ################################################################################
 * # PHY_4C0 Definition
 */
#define PHY_4C0_RO_RDEYE_TRAIN_DB0_FALL_RESULT		(0)
#define PHY_4C0_RO_RDEYE_TRAIN_DB1_FALL_RESULT		(8)
#define PHY_4C0_RO_RDEYE_TRAIN_DB2_FALL_RESULT		(16)


#define PHY_4C0_RO_RDEYE_TRAIN_DB0_FALL_RESULT_MASK		0xff
#define PHY_4C0_RO_RDEYE_TRAIN_DB1_FALL_RESULT_MASK		0xff
#define PHY_4C0_RO_RDEYE_TRAIN_DB2_FALL_RESULT_MASK		0xff

/* ################################################################################
 * # PHY_4C8 Definition
 */
#define PHY_4C8_RO_RDEYE_TRAIN_DB0_RISE_PASS_WINDOW		(0)
#define PHY_4C8_RO_RDEYE_TRAIN_DB1_RISE_PASS_WINDOW		(8)
#define PHY_4C8_RO_RDEYE_TRAIN_DB2_RISE_PASS_WINDOW		(16)

#define PHY_4C8_RO_RDEYE_TRAIN_DB0_RISE_PASS_WINDOW_MASK		0xff
#define PHY_4C8_RO_RDEYE_TRAIN_DB1_RISE_PASS_WINDOW_MASK		0xff
#define PHY_4C8_RO_RDEYE_TRAIN_DB2_RISE_PASS_WINDOW_MASK		0xff






#define PHY_PLL_LOCKCNT_MODE 0x2

enum phy_pll_lockcnt_mode {
  	PHY_PLL_LOCKCNT_MODE_0,  /* 0x27100 MC_CLK cycles */
	PHY_PLL_LOCKCNT_MODE_1,  /* 0x13880 MC_CLK cycles */
	PHY_PLL_LOCKCNT_MODE_2,  /* 0x3ffff MC_CLK cycles */
	PHY_PLL_LOCKCNT_MODE_3   /* 0x3ffff MC_CLK cycles */
};

/* ddr3 IO ODT Resistor(Rtt) setting  */
enum ddr3_phy_odt_mode {
  	DDR3_PHY_ODT_MODE_0,  /* ODT disabled */
	DDR3_PHY_ODT_MODE_1,  /* 120 Ohm */
	DDR3_PHY_ODT_MODE_2,  /* 60 Ohm */
	DDR3_PHY_ODT_MODE_3,   /* 40 Ohm */
	DDR3_PHY_ODT_MODE_4,   /* 30 Ohm */
	DDR3_PHY_ODT_MODE_5,   /* 24 Ohm */
	DDR3_PHY_ODT_MODE_6,   /* 20 Ohm */
	DDR3_PHY_ODT_MODE_7,   /* 17 Ohm */
};

/* ddr4 IO ODT Resistor(Rtt) setting  */
enum ddr4_phy_odt_mode {
  	DDR4_PHY_ODT_MODE_0,  /* ODT disabled */
	DDR4_PHY_ODT_MODE_1,  /* 240 Ohm */
	DDR4_PHY_ODT_MODE_2,  /* 120 Ohm */
	DDR4_PHY_ODT_MODE_3,   /* 80 Ohm */
	DDR4_PHY_ODT_MODE_4,   /* 60 Ohm */
	DDR4_PHY_ODT_MODE_5,   /* 48 Ohm */
	DDR4_PHY_ODT_MODE_6,   /* 40 Ohm */
	DDR4_PHY_ODT_MODE_7,   /* 34 Ohm */
};

/* ddr phy RON  setting for ZQ=240 ohm  */
enum ddr_phy_ron_mode {
  	DDR_PHY_RON_MODE_0,  /* ODT disabled */
	DDR_PHY_RON_MODE_1,  /* 240 Ohm */
	DDR_PHY_RON_MODE_2,  /* 120 Ohm */
	DDR_PHY_RON_MODE_3,   /* 80 Ohm */
	DDR_PHY_RON_MODE_4,   /* 60 Ohm */
	DDR_PHY_RON_MODE_5,   /* 48 Ohm */
	DDR_PHY_RON_MODE_6,   /* 40 Ohm */
	DDR_PHY_RON_MODE_7,   /* 34 Ohm */
};

enum ddr_training_mode{
	DDR_TRAINING_MODE_0,	/* Use data for gate and read eye training */
	DDR_TRAINING_MODE_1,	/* Usedata mode for gate and read eye training */
	DDR_TRAINING_MODE_2,	/* Use dqs for gate training */
	DDR_TRAINING_MODE_3,	/* Use MPR for gate and read eye training */
};

#define MLB_CA_VREF_LONG   0x80
#define MLB_CA_VREF_SHORT 0x30

#define PHY_EYE_STEP_UNIT 0xf

#define DDR3_PHY_T_ZQINIT 0x0100
#define DDR3_PHY_T_XPR 0x0044

#define DDR4_PHY_T_ZQINIT 0x0400
#define DDR4_PHY_T_XPR 0x0150

#define PHY_MLB_RSTCTL_EN2RST 0xf0

#define DDR3_PHY_MLB_DFI_T_PHY_WRLAT		0x06
#define DDR3_PHY_MLB_RSTCTL_RST2CKE			0x258

#define PHY_MLB_DFI_T_PHY_WRLAT			0x0a
#define PHY_MLB_DFI_T_PHY_WRDATA			0x01
#define PHY_MLB_RSTCTL_RST2CKE			0x14

#define DDR4_PHY_MLB_T_RRD			0x04
#define DDR3_PHY_MLB_T_RRD			0x06
#define PHY_MLB_T_CCD			0x04

#define DDR3_PHY_MLB_DFI_T_RDDATA_EN		0x9

#define PHY_MLB_DFI_T_RDDATA_EN		0x10
#define PHY_MLB_T_RFC		0x14a

#define DDR3_PHY_MLB_T_RCD				0x0B

#define PHY_MLB_REFRESH_LIMIT			0x1248
#define PHY_MLB_WODT_TIMER			0x4
#define PHY_MLB_T_RCD					0x09

#define PHY_MLB_T_CACKEL			0x10

#define PHY_MLB_CHECK_T_CNT			0x16
#define PHY_MLB_MLB_T_CACKEH		0x10
#define PHY_MLB_T_CAEXT			0x70
#define PHY_MLB_T_MRZ			0x05

#define PHY_MLB_LOAD_ACK_CYCLE		0x2c
#define PHY_MLB_DFI_RDDATA_GOLD		0xff
#define PHY_MLB_LOAD_RESP_CYCLE		0x0a
#define PHY_MLB_LOAD_RESP_CYCLE_WL	0x1c	

#define PHY_MLB_TRAIN_FINE_GATE_DELAY		0x8

#define PHY_MLB_MPR_DATA_0 0xaa55aa55
#define PHY_MLB_MPR_DATA_1 0x55aa55aa
#define PHY_MLB_MPR_DATA_2 0xaaaa5555
#define PHY_MLB_MPR_DATA_3 0x5555aaaa

#define DDR3_PHY_MLB_VREF_PHY_ADJ_END 0x54

#define PHY_MLB_VREF_PHY_ADJ_END 0x60

#define DDR3_PHY_MLB_VREF_PHY_VALUE_MANU_DB8	0x40
#define DDR3_PHY_MLB_VREF_PHY_ADJ_START	0x2d

#define DDR3_PHY_MLB_VREF_PHY_VALUE_MANU_DB0	0x40
#define DDR3_PHY_MLB_VREF_PHY_VALUE_MANU_DB1	0x40
#define DDR3_PHY_MLB_VREF_PHY_VALUE_MANU_DB2	0x40
#define DDR3_PHY_MLB_VREF_PHY_VALUE_MANU_DB3	0x40
#define DDR3_PHY_MLB_VREF_PHY_VALUE_MANU_DB4	0x40
#define DDR3_PHY_MLB_VREF_PHY_VALUE_MANU_DB5	0x40
#define DDR3_PHY_MLB_VREF_PHY_VALUE_MANU_DB6	0x40
#define DDR3_PHY_MLB_VREF_PHY_VALUE_MANU_DB7	0x40

#define DDR3_PHY_MLB_VREF_DRAM_VALUE_START	0x0
#define DDR3_PHY_MLB_VREF_DRAM_VALUE_END		0x0

#define PHY_MLB_VREF_PHY_VALUE_MANU_DB8	0x4e
#define PHY_MLB_VREF_PHY_ADJ_START	0x40

#define PHY_MLB_VREF_PHY_VALUE_MANU_DB0	0x4e
#define PHY_MLB_VREF_PHY_VALUE_MANU_DB1	0x4e
#define PHY_MLB_VREF_PHY_VALUE_MANU_DB2	0x4e
#define PHY_MLB_VREF_PHY_VALUE_MANU_DB3	0x4e
#define PHY_MLB_VREF_PHY_VALUE_MANU_DB4	0x4e
#define PHY_MLB_VREF_PHY_VALUE_MANU_DB5	0x4e
#define PHY_MLB_VREF_PHY_VALUE_MANU_DB6	0x4e
#define PHY_MLB_VREF_PHY_VALUE_MANU_DB7	0x4e

#define PHY_MLB_VREF_DRAM_VALUE_START	0x07
#define PHY_MLB_VREF_DRAM_VALUE_END		0x2f

#define DDR3_PHY_MLB_VREF_DRAM_VALUE_MANU 0X0

#define PHY_MLB_VREF_DRAM_VALUE_MANU 	0x27

#define PHY_PHY_EYE_STEP_UNIT  0

#define DDR3_PHY_MLB_R_T_MOD	0x8

#define PHY_MLB_R_T_MRD	0x8
#define PHY_MLB_R_T_MOD	0x18

#define DDR3_PHY_MLB_R_T_XPR	0xE0

#define PHY_MLB_R_T_OIT	0x400
#define PHY_MLB_R_T_XPR	0x150

#define PHY_MLB_R_RSTCTL_EN2RST	0x272AB
#define PHY_MLB_R_RSTCTL_RST2CKE	0x4E557

#define PHY_PLL_2ND_LOCK_MODE	0x0
#define PHY_PLLPD2DIVRSTN_MODE	0x3

#define PHY_MLB_TRAIN_WDQS_STOP_INTG 	0x1
#define PHY_MLB_TRAIN_WDQS_STOP_STEP 	0x80

#define DDR4_PHY_VREFDQ_VALUE 0x27


#define DDRINTF_PHY_GROUP0_OFFSET 	0x100
#define DDRINTF_PHY_GROUP1_OFFSET 	0x600
#define DDRINTF_PHY_GROUP2_OFFSET 	0x800
#define DDRINTF_PHY_RO_GROUP_OFFSET 0x400

#define ddrintf_phy_group0	(( ddrintf_phy_group0_t *)(CONFIG_SYS_CTC5236_DDRCFG_BASE + DDRINTF_PHY_GROUP0_OFFSET))
#define ddrintf_phy_group1	(( ddrintf_phy_group1_t *)(CONFIG_SYS_CTC5236_DDRCFG_BASE + DDRINTF_PHY_GROUP1_OFFSET))
#define ddrintf_phy_group2	(( ddrintf_phy_group2_t *)(CONFIG_SYS_CTC5236_DDRCFG_BASE + DDRINTF_PHY_GROUP2_OFFSET))
#define ddrintf_phy_ro_group	(( ddrintf_phy_ro_group_t *)(CONFIG_SYS_CTC5236_DDRCFG_BASE + DDRINTF_PHY_RO_GROUP_OFFSET))

#endif
#endif
