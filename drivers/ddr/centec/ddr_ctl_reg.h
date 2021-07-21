/*
 * (C) Copyright 2004-2017 Centec Networks (suzhou) Co., LTD.
 * Jay Cao <caoj@centecnetworks.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef __DDR_CTL_REG_H__
#define __DDR_CTL_REG_H__

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

/* ################################################################################
 * # CTL_00 Definition
 */
#define CTL_00_DDR_TYPE               	 (0)    
#define CTL_00_RANK_NUM               	 (4)         
#define CTL_00_BL                     		 (6)         
#define CTL_00_COL_ADDR_WIDTH    	 (8)          
#define CTL_00_ECC_ENABLE             	 (10)
#define CTL_00_ECC_ENABLE_BIT             	 BIT(10)
#define CTL_00_ECC_WR_BACK_EN        (11)              
#define CTL_00_ROW_ADDR_WIDTH      (12)                
#define CTL_00_BANK_ADDR_WIDTH    	 (16)                 
#define CTL_00_BANK_GROUP_ADDR_WIDTH  	 (20)            
#define CTL_00_ECC_ERR_SINGLE        	 (22)           
#define CTL_00_ECC_ERR_DOUBLE       	 (23)           
#define CTL_00_DDR_VERSION           	 (24)          

#define CTL_00_DDR_TYPE_MASK    			0x7         
#define CTL_00_RANK_NUM_MASK    			0x3         
#define CTL_00_BL_MASK          				0x3         
#define CTL_00_COL_ADDR_WIDTH_MASK    	0x3   
#define CTL_00_ECC_ENABLE_MASK        		0x1  
#define CTL_00_ECC_WR_BACK_EN_MASK    	0x1   
#define CTL_00_ROW_ADDR_WIDTH_MASK    	0x7  
#define CTL_00_BANK_ADDR_WIDTH_MASK   	0x1   
#define CTL_00_BANK_GROUP_ADDR_WIDTH_MASK  	0x3
#define CTL_00_ECC_ERR_SINGLE_MASK    		0x1   
#define CTL_00_ECC_ERR_DOUBLE_MASK    	0x1

/* ################################################################################
 * # CTL_04 Definition
 */
#define CTL_04_ENABLE_BIST         		 (0)
#define CTL_04_ENABLE_BIST_BIT        	BIT(0)
#define CTL_04_FORCE_SDII_NO_GNT   	 (1) 
#define CTL_04_FORCE_CKE_LOW       		BIT (2)    
#define CTL_04_FORCE_RESTN_LOW     	 BIT(3) 
#define CTL_04_DFI_RATIO           			 (4)
#define CTL_04_RD_DBI_EN           			 (6)
#define CTL_04_WR_DBI_EN           		 (7)
#define CTL_04_PRG_PD_ENTER_TIME   	 (8)
#define CTL_04_SR_ENTER_REQ        		 (16)
#define CTL_04_SR_EXIT_REQ         		 (17)
#define CTL_04_SET_DFI_INIT_START  		 (18)
#define CTL_04_ENABLE_DDRCTRL      		 (19)
#define CTL_04_SR_ENTER_TIME       		 (20)
#define CTL_04_ADDR_MODE           		 (29)
#define CTL_04_DFI_INIT_COMP           	 (31)

#define CTL_04_ENABLE_BIST_MASK        		0x1 
#define CTL_04_FORCE_SDII_NO_GNT_MASK  	0x1
#define CTL_04_FORCE_CKE_LOW_MASK      	0x1
#define CTL_04_FORCE_RESTN_LOW_MASK    	0x1 
#define CTL_04_DFI_RATIO_MASK          		0x3
#define CTL_04_RD_DBI_EN_MASK          		0x1 
#define CTL_04_WR_DBI_EN_MASK          		0x1 
#define CTL_04_PRG_PD_ENTER_TIME_MASK  	0xff
#define CTL_04_SR_ENTER_REQ_MASK       		0x1
#define CTL_04_SR_EXIT_REQ_MASK        		0x1
#define CTL_04_SET_DFI_INIT_START_MASK 	0x1
#define CTL_04_ENABLE_DDRCTRL_MASK     	0x1
#define CTL_04_SR_ENTER_TIME_MASK      		0xff
#define CTL_04_ADDR_MODE_MASK          		0x1

#define CTL_04_FORCE_CKE_LOW_SHIFT       	 (2)    
#define CTL_04_FORCE_RESTN_LOW_SHIFT     	 (3) 


/* ################################################################################
 * # CTL_08 Definition
 */
#define CTL_08_REFRESH_LIMIT         		 (0)
#define CTL_08_DFI_DRAM_CLK_DISABLE  	 (17)
#define CTL_08_DPD_ENTER_REQ         	 (18)
#define CTL_08_ARCNT_TRIG            		 (20)

#define CTL_08_REFRESH_LIMIT_MASK         		0xffff
#define CTL_08_DFI_DRAM_CLK_DISABLE_MASK  	0x1
#define CTL_08_DPD_ENTER_REQ_MASK         		0x1
#define CTL_08_ARCNT_TRIG_MASK            		0x1

/* ################################################################################
 * # CTL_0C Definition
 */
#define CTL_0C_RDDATA_EN    		 (0)       
#define CTL_0C_PHY_WRLAT    		 (8)       
#define CTL_0C_PHY_WRDATA   		 (16)      

#define CTL_0C_RDDATA_EN_MASK    	0x3f   
#define CTL_0C_PHY_WRLAT_MASK    	0x3f     
#define CTL_0C_PHY_WRDATA_MASK   	0x7

/* ################################################################################
 * # CTL_30 Definition
 */
#define CTL_30_IDLE_CYE_NUM  				 (10)     
#define CTL_30_CG_MODE_EN    				 (24)  
#define CTL_30_CG_MODE_LAUNCH 			 (25)       
#define CTL_30_FORCE_DFI_INIT_START_LOW 	 (26) 
#define CTL_30_FORCE_STA_CKSTPL 			 (27)         

#define CTL_30_IDLE_CYE_NUM_MASK  				0xff          
#define CTL_30_CG_MODE_EN_MASK    					0x1              
#define CTL_30_CG_MODE_LAUNCH_MASK  				0x1           
#define CTL_30_FORCE_DFI_INIT_START_LOW_MASK 	0x1  
#define CTL_30_FORCE_STA_CKSTPL_MASK         		0x1 

/* ################################################################################
 * # CTL_40 Definition
 */
#define CTL_40_RTW       	 (0)
#define CTL_40_WTR_S   	 (8)
#define CTL_40_WTR_L    	 (16)
#define CTL_40_CCD       	 (24)
#define CTL_40_CCD_S     	 (24)
#define CTL_40_CCD_L     	 (28)

#define CTL_40_RTW_MASK       	0xff
#define CTL_40_WTR_S_MASK    	0xff
#define CTL_40_WTR_L_MASK     	0xff
#define CTL_40_CCD_MASK       	0xff
#define CTL_40_CCD_S_MASK     	0xf
#define CTL_40_CCD_L_MASK     	0xf

/* ################################################################################
 * # CTL_44 Definition
 */
#define CTL_44_FAW   	 (0)        
#define CTL_44_RFC   	 (8)         
#define CTL_44_RTP   	 (24)         

#define CTL_44_FAW_MASK   	0xff           
#define CTL_44_RFC_MASK      0x3ff    
#define CTL_44_RTP_MASK      0xff 

/* ################################################################################
 * # CTL_48 Definition
 */
#define CTL_48_RCD      	 (0)     
#define CTL_48_RAS      	 (8)     
#define CTL_48_RP       	 (16)     

#define CTL_48_RCD_MASK     	0xff    
#define CTL_48_RAS_MASK     	0xff    
#define CTL_48_RP_MASK      	0xff  

/* ################################################################################
 * # CTL_4C Definition
 */
#define CTL_4C_WTP       	 (0)      
#define CTL_4C_CKESR     	 (8)     
#define CTL_4C_CKSRE     	 (16)     
#define CTL_4C_CCDDRR  	 (24)     

#define CTL_4C_WTP_MASK      	0xff      
#define CTL_4C_CKESR_MASK    	0xff       
#define CTL_4C_CKSRE_MASK    	0xff      
#define CTL_4C_CCDDRR_MASK   	0xff     

/* ################################################################################
 * # CTL_50 Definition
 */
#define CTL_50_CKSRX     	 (0)     
#define CTL_50_XS         	 (8)     
#define CTL_50_XSDLL     	 (20)     

#define CTL_50_CKSRX_MASK    	0xff       
#define CTL_50_XS_MASK       		0x3ff      
#define CTL_50_XSDLL_MASK    	0xfff      

/* ################################################################################
 * # CTL_54 Definition
 */
#define CTL_54_TMRS_WAIT    (0)     

#define CTL_54_TMRS_WAIT_MASK   0xff      

/* ################################################################################
 * # CTL_58 Definition
 */
#define CTL_58_MRS_LAUNCH        		 (1) 
#define CTL_58_MRS_RANK_ADR      	 (2)
#define CTL_58_MRS_NUM           		 (4)
#define CTL_58_MRS_OPCODE        	 (12)
#define CTL_58_MRS_ALL_RANK      	 (31)

#define CTL_58_MRS_LAUNCH_MASK    		0x1   
#define CTL_58_MRS_RANK_ADR_MASK  	0x3   
#define CTL_58_MRS_NUM_MASK       		0xf   
#define CTL_58_MRS_OPCODE_MASK    		0x3ffff    
#define CTL_58_MRS_ALL_RANK_MASK  	0x1    

/* ################################################################################
 * # CTL_5C Definition
 */
#define CTL_5C_CTRLUPD_SPACE  			 (0)  
#define CTL_5C_CTRLUPD_MIN    			 (8) 
#define CTL_5C_2T_MODE        			 (18)  
#define CTL_5C_DDR4_WCRC_EN   			 (19)   
#define CTL_5C_ALERT_WAIT     			 (20)   
#define CTL_5C_DDR4_WCRC_FORCE_ERR 	 (26) 

#define CTL_5C_CTRLUPD_SPACE_MASK   	0xff  
#define CTL_5C_CTRLUPD_MIN_MASK     	0x3ff
#define CTL_5C_2T_MODE_MASK         		0x1  
#define CTL_5C_DDR4_WCRC_EN_MASK    	0x1 
#define CTL_5C_ALERT_WAIT_MASK      	0x3f  
#define CTL_5C_DDR4_WCRC_FORCE_ERR_MASK 	0x1 

/* ################################################################################
 * # CTL_60 Definition
 */
#define CTL_60_CKE  	 (0)        
#define CTL_60_XP   	 (8)        

#define CTL_60_CKE_MASK    	0xff        
#define CTL_60_XP_MASK     	0xff       

/* ################################################################################
 * # CTL_64 Definition
 */
#define CTL_64_PHY_WODTLAT   		 (0)  
#define CTL_64_WODT_LEN      		 (4)   
#define CTL_64_PHY_RDN_ODTLAT  	 (8)
#define CTL_64_RDN_ODT_LEN     		 (12) 

#define CTL_64_PHY_WODTLAT_MASK   	0x7  
#define CTL_64_WODT_LEN_MASK      		0x3    
#define CTL_64_PHY_RDN_ODTLAT_MASK 	0xf 
#define CTL_64_rdn_ODT_LEN_MASK    		0x3  

/* ################################################################################
 * # CTL_68 Definition
 */
#define CTL_68_ZQCAL_WAIT    	 (0)   
#define CTL_68_ZQCAL_TYPE    	 (12)   
#define CTL_68_ZQCAL_LAUNCH  	 (13)   
#define CTL_68_ZQCAL_SPACE   	 (16)     

#define CTL_68_ZQCAL_WAIT_MASK    		0xfff    
#define CTL_68_ZQCAL_TYPE_MASK    		0x1    
#define CTL_68_ZQCAL_LAUNCH_MASK  	0x1    
#define CTL_68_ZQCAL_SPACE_MASK   		0xff     

/* ################################################################################
 * # CTL_70 Definition
 */
#define CTL_70_CS_NOM_FIX    	 (0)    
#define CTL_70_CKE_NOM_FIX   	 (4)    
#define CTL_70_ODT_NOM_FIX   	 (8)    
#define CTL_70_CS_FIXVALUE   	 (12)    
#define CTL_70_CKE_FIXVALUE  	 (16)    
#define CTL_70_ODT_FIXVALUE  	 (20)    

#define CTL_70_CS_NOM_FIX_MASK    		0xf       
#define CTL_70_CKE_NOM_FIX_MASK    	0xf      
#define CTL_70_ODT_NOM_FIX_MASK    	0xf       
#define CTL_70_CS_FIXVALUE_MASK    		0xf       
#define CTL_70_CKE_FIXVALUE_MASK    	0xf      
#define CTL_70_ODT_FIXVALUE_MASK    	0xf     

/* ################################################################################
 * # CTL_8C Definition
 */
#define CTL_8C_DYNAMIC_PRI    		 (0)     
#define CTL_8C_RW_GROUPING    		 (1)     
#define CTL_8C_BURST_GROUPING   	 (2)   
#define CTL_8C_BURST_GROUPING_CONT 	 (3)
#define CTL_8C_PAGE_POLICY         	 (4)
#define CTC_8C_TOPBOUND			 (6)
#define CTC_8C_WR_GROUP_WAITCNT  (12)
#define CTL_8C_PAGE_CLOSE_EOB      	 (15)
#define CTL_8C_WGRP_MASKAXI        	 (16)
#define CTL_8C_NONBF_WRESP         	 (17)
#define CTL_8C_WCMDQ_TH_EN         	 (18)
#define CTL_8C_PAGE_HIT_CONT       	 (19)

#define CTL_8C_DYNAMIC_PRI_MASK   		0x1      
#define CTL_8C_RW_GROUPING_MASK   	0x1         
#define CTL_8C_BURST_GROUPING_MASK   	0x1     
#define CTL_8C_BURST_GROUPING_CONT_MASK   0x1 
#define CTL_8C_PAGE_POLICY_MASK   		0x3 
#define CTC_8C_TOPBOUND_MASK			0x7
#define CTC_8C_WR_GROUP_WAITCNT_MASK	0x7
#define CTL_8C_PAGE_CLOSE_EOB_MASK   	0x1     
#define CTL_8C_WGRP_MASKAXI_MASK   	0x1       
#define CTL_8C_NONBF_WRESP_MASK   	0x1         
#define CTL_8C_WCMDQ_TH_EN_MASK   	0x1       
#define CTL_8C_PAGE_HIT_CONT_MASK   	0x1    

/* ################################################################################
 * # CTL_90 Definition
 */
#define CTL_90_COL_B0_OFS  		 (0)
#define CTL_90_COL_B1_OFS  		 (5)
#define CTL_90_COL_B2_OFS  		 (10)
#define CTL_90_COL_B3_OFS  		 (15)
#define CTL_90_COL_B4_OFS  		 (20)
#define CTL_90_COL_B5_OFS  		 (25)

#define CTL_90_COL_B0_OFS_MASK 	0x1f  
#define CTL_90_COL_B1_OFS_MASK 	0x1f 
#define CTL_90_COL_B2_OFS_MASK 	0x1f  
#define CTL_90_COL_B3_OFS_MASK 	0x1f  
#define CTL_90_COL_B4_OFS_MASK 	0x1f  
#define CTL_90_COL_B5_OFS_MASK 	0x1f  

/* ################################################################################
 * # CTL_94 Definition
 */
#define CTL_94_COL_B6_OFS 		 (0) 
#define CTL_94_COL_B7_OFS  		 (5)
#define CTL_94_COL_B8_OFS  		 (10)
#define CTL_94_COL_B9_OFS  		 (15)
#define CTL_94_COL_B10_OFS 	 (20)
#define CTL_94_COL_B11_OFS 	 (25)

#define CTL_94_COL_B6_OFS_MASK 	0x1f 
#define CTL_94_COL_B7_OFS_MASK 	0x1f  
#define CTL_94_COL_B8_OFS_MASK 	0x1f  
#define CTL_94_COL_B9_OFS_MASK 	0x1f  
#define CTL_94_COL_B10_OFS_MASK 	0x1f 
#define CTL_94_COL_B11_OFS_MASK 	0x1f 

/* ################################################################################
 * # CTL_98 Definition
 */
#define CTL_98_ROE_B0_OFS  		 (0)
#define CTL_98_ROE_B1_OFS  		 (6)
#define CTL_98_ROE_B2_OFS  		 (12)
#define CTL_98_ROE_B3_OFS  		 (18)
#define CTL_98_ROE_B4_OFS  		 (24)

#define CTL_98_ROE_B0_OFS_MASK 	0x3f  
#define CTL_98_ROE_B1_OFS_MASK 	0x3f  
#define CTL_98_ROE_B2_OFS_MASK 	0x3f  
#define CTL_98_ROE_B3_OFS_MASK 	0x3f  
#define CTL_98_ROE_B4_OFS_MASK 	0x3f  

/* ################################################################################
 * # CTL_9C Definition
 */
#define CTL_9C_ROE_B5_OFS  		 (0)
#define CTL_9C_ROE_B6_OFS  		 (6)
#define CTL_9C_ROE_B7_OFS  		 (12)
#define CTL_9C_ROE_B8_OFS  		 (18)
#define CTL_9C_ROE_B9_OFS  		 (24)

#define CTL_9C_ROE_B5_OFS_MASK 	0x3f  
#define CTL_9C_ROE_B6_OFS_MASK 	0x3f  
#define CTL_9C_ROE_B7_OFS_MASK 	0x3f  
#define CTL_9C_ROE_B8_OFS_MASK 	0x3f  
#define CTL_9C_ROE_B9_OFS_MASK 	0x3f 

/* ################################################################################
 * # CTL_A0 Definition
 */
#define CTL_A0_ROE_B10_OFS 	 (0)
#define CTL_A0_ROE_B11_OFS 	 (6)
#define CTL_A0_ROE_B12_OFS 	 (12)
#define CTL_A0_ROE_B13_OFS 	 (18)
#define CTL_A0_ROE_B14_OFS 	 (24)

#define CTL_A0_ROE_B10_OFS_MASK 	0x3f 
#define CTL_A0_ROE_B11_OFS_MASK 	0x3f 
#define CTL_A0_ROE_B12_OFS_MASK 	0x3f 
#define CTL_A0_ROE_B13_OFS_MASK 	0x3f 
#define CTL_A0_ROE_B14_OFS_MASK 	0x3f 

/* ################################################################################
 * # CTL_A4 Definition
 */
#define CTL_A4_ROE_B15_OFS 	 (0)
#define CTL_A4_ROE_B16_OFS 	 (6)
#define CTL_A4_ROE_B17_OFS 	 (12)

#define CTL_A4_ROE_B15_OFS_MASK 	0x3f 
#define CTL_A4_ROE_B16_OFS_MASK 	0x3f 
#define CTL_A4_ROE_B17_OFS_MASK 	0x3f 

/* ################################################################################
 * # CTL_A8 Definition
 */
#define CTL_A8_BANK_B0_OFS 	 (0)
#define CTL_A8_BANK_B1_OFS 	 (6)
#define CTL_A8_BANK_B2_OFS 	 (12)

#define CTL_A8_BANK_B0_OFS_MASK 	0x3f
#define CTL_A8_BANK_B1_OFS_MASK 	0x3f 
#define CTL_A8_BANK_B2_OFS_MASK 	0x3f 

/* ################################################################################
 * # CTL_AC Definition
 */
#define CTL_AC_BG_B0_OFS   	 (0)
#define CTL_AC_BG_B1_OFS   	 (6)

#define CTL_AC_BG_B0_OFS_MASK 	0x3f   
#define CTL_AC_BG_B1_OFS_MASK 	0x3f   

/* ################################################################################
 * # CTL_AC Definition
 */
#define CTL_AC_RANK_B0_OFS 	 (12)
#define CTL_AC_RANK_B1_OFS 	 (18)

#define CTL_AC_RANK_B0_OFS_MASK 	0x3f
#define CTL_AC_RANK_B1_OFS_MASK 	0x3f 

/* ################################################################################
 * # CTL_B0 Definition
 */
#define CTL_B0_PORT00_ASYNC_MODE    		 (0) 
#define CTL_B0_PORT00_RD_PAGE_HIT_EN 	 (1)
#define CTL_B0_PORT00_RD_CRITICAL_EN 		 (2)
#define CTL_B0_PORT00_RD_TIEMOUT_EN  	 (3)
#define CTL_B0_PORT00_WR_PAGE_HIT_EN 	 (4)
#define CTL_B0_PORT00_WR_CRITICAL_EN 	 (5)
#define CTL_B0_PORT00_WR_TIMEOUT_EN  	 (6)
#define CTL_B0_PRI_PORT00            			 (7)

#define CTL_B0_PORT00_ASYNC_MODE_MASK 		0x1    
#define CTL_B0_PORT00_RD_PAGE_HIT_EN_MASK 	0x1 
#define CTL_B0_PORT00_RD_CRITICAL_EN_MASK 	0x1 
#define CTL_B0_PORT00_RD_TIEMOUT_EN_MASK 	0x1  
#define CTL_B0_PORT00_WR_PAGE_HIT_EN_MASK 	0x1 
#define CTL_B0_PORT00_WR_CRITICAL_EN_MASK 	0x1 
#define CTL_B0_PORT00_WR_TIMEOUT_EN_MASK 	0x1  
#define CTL_B0_PRI_PORT00_MASK 				0xff            

/* ################################################################################
 * # CTL_B4 Definition
 */
#define CTL_B4_PORT01_ASYNC_MODE  		 (0)   
#define CTL_B4_PORT01_RD_PAGE_HIT_EN 	 (1)
#define CTL_B4_PORT01_RD_CRITICAL_EN 		 (2)
#define CTL_B4_PORT01_RD_TIEMOUT_EN  	 (3)
#define CTL_B4_PORT01_WR_PAGE_HIT_EN 	 (4)
#define CTL_B4_PORT01_WR_CRITICAL_EN 	 (5)
#define CTL_B4_PORT01_WR_TIMEOUT_EN  	 (6)
#define CTL_B4_PRI_PORT01            			 (7)

#define CTL_B4_PORT01_ASYNC_MODE_MASK 		0x1     
#define CTL_B4_PORT01_RD_PAGE_HIT_EN_MASK 	0x1 
#define CTL_B4_PORT01_RD_CRITICAL_EN_MASK 	0x1 
#define CTL_B4_PORT01_RD_TIEMOUT_EN_MASK 	0x1  
#define CTL_B4_PORT01_WR_PAGE_HIT_EN_MASK 	0x1 
#define CTL_B4_PORT01_WR_CRITICAL_EN_MASK 	0x1 
#define CTL_B4_PORT01_WR_TIMEOUT_EN_MASK 	0x1  
#define CTL_B4_PRI_PORT01_MASK 				0xff           

/* ################################################################################
 * # CTL_B8 Definition
 */
#define CTL_B8_PORT02_ASYNC_MODE     		 (0)
#define CTL_B8_PORT02_RD_PAGE_HIT_EN 	 (1)
#define CTL_B8_PORT02_RD_CRITICAL_EN 		 (2)
#define CTL_B8_PORT02_RD_TIEMOUT_EN  	 (3)
#define CTL_B8_PORT02_WR_PAGE_HIT_EN 	 (4)
#define CTL_B8_PORT02_WR_CRITICAL_EN 	 (5)
#define CTL_B8_PORT02_WR_TIMEOUT_EN  	 (6)
#define CTL_B8_PRI_PORT02            			 (7)

#define CTL_B8_PORT02_ASYNC_MODE_MASK 		0x1     
#define CTL_B8_PORT02_RD_PAGE_HIT_EN_MASK 	0x1 
#define CTL_B8_PORT02_RD_CRITICAL_EN_MASK 	0x1 
#define CTL_B8_PORT02_RD_TIEMOUT_EN_MASK 	0x1  
#define CTL_B8_PORT02_WR_PAGE_HIT_EN_MASK 	0x1 
#define CTL_B8_PORT02_WR_CRITICAL_EN_MASK 	0x1 
#define CTL_B8_PORT02_WR_TIMEOUT_EN_MASK 	0x1  
#define CTL_B8_PRI_PORT02_MASK 				0xff            

/* ################################################################################
 * # CTL_BC Definition
 */
#define CTL_BC_PORT03_ASYNC_MODE     		 (0)
#define CTL_BC_PORT03_RD_PAGE_HIT_EN 	 (1)
#define CTL_BC_PORT03_RD_CRITICAL_EN 		 (2)
#define CTL_BC_PORT03_RD_TIEMOUT_EN  	 (3)
#define CTL_BC_PORT03_WR_PAGE_HIT_EN 	 (4)
#define CTL_BC_PORT03_WR_CRITICAL_EN 	 (5)
#define CTL_BC_PORT03_WR_TIMEOUT_EN  	 (6)
#define CTL_BC_PRI_PORT03            			 (7)

#define CTL_BC_PORT03_ASYNC_MODE_MASK 		0x1      
#define CTL_BC_PORT03_RD_PAGE_HIT_EN_MASK 	0x1  
#define CTL_BC_PORT03_RD_CRITICAL_EN_MASK 	0x1  
#define CTL_BC_PORT03_RD_TIEMOUT_EN_MASK 	0x1   
#define CTL_BC_PORT03_WR_PAGE_HIT_EN_MASK 	0x1  
#define CTL_BC_PORT03_WR_CRITICAL_EN_MASK 	0x1  
#define CTL_BC_PORT03_WR_TIMEOUT_EN_MASK 	0x1   
#define CTL_BC_PRI_PORT03_MASK 				0xff            

/* ################################################################################
 * # CTL_F0 Definition
 */
#define CTL_F0_INTR_ENABLE      (0)     

#define CTL_F0_INTR_ENABLE_MASK 	0x1fffff

/* ################################################################################
 * # CTL_F4 Definition
 */
#define CTL_F4_INTR_STATUS     (0)       

#define CTL_F4_INTR_STATUS_MASK 	0x1fffff


      

/*base:0x30600700*/
typedef struct ddrintf_ctl_group1_s{
	u32 bist_sdii_cnt;			/* offset:0x700 */
	u32 bist_mode;			/* offset:0x704 */
	u32 bist_sdii_start;		/* offset:0x708 */
	u32 r1_lfsr_ini;			/* offset:0x70c */
	u32 r2_lfsr_ini;			/* offset:0x710 */
	u32 r3_lfsr_ini;			/* offset:0x714 */
	u32 r4_lfsr_ini;			/* offset:0x718 */
	u32 bist_start;			/* offset:0x71c */

}ddrintf_ctl_group1_t;

/* ################################################################################
 * # PHY_840 Definition
 */
#define CTL_700_BIST_SDII_CNT    (0)     

#define CTL_700_BIST_SDII_CNT_MASK 0xffffffff       

/* ################################################################################
 * #  CTL_704 Definition
 */
#define CTL_704_BIST_SDII_START_BANK 		 (0)
#define CTL_704_BIST_SDII_START_BG   		 (4)
#define CTL_704_BIST_SDII_START_RANK 		 (8)
#define CTL_704_R2RW2W_REQ_SPC       		 (12)
#define CTL_704_REQ_SPC_VARY         			 (16)
#define CTL_704_LOOP_WR              			 (20)
#define CTL_704_LOOP_RD              			 (21)
#define CTL_704_TST_ECC_PART         			 (24)
#define CTL_704_BIST_BURN_ENABLE     		 (27)
#define CTL_704_BIST_PAT             			 (28)
#define CTL_704_BIST_TYPE            			 (29)
#define CTL_704_FORCE_BIST_ERR       		 (30)

#define CTL_704_BIST_SDII_START_BANK_MASK 	0x7 
#define CTL_704_BIST_SDII_START_BG_MASK 		0x3   
#define CTL_704_BIST_SDII_START_RANK_MASK 	0x3 
#define CTL_704_R2RW2W_REQ_SPC_MASK 		0xf       
#define CTL_704_REQ_SPC_VARY_MASK 			0x1         
#define CTL_704_LOOP_WR_MASK 					0x1              
#define CTL_704_LOOP_RD_MASK 					0x1              
#define CTL_704_TST_ECC_PART_MASK 			0x1         
#define CTL_704_BIST_BURN_ENABLE_MASK 		0x1     
#define CTL_704_BIST_PAT_MASK 					0x1             
#define CTL_704_BIST_TYPE_MASK 				0x1            
#define CTL_704_FORCE_BIST_ERR_MASK 			0x1       

/* ################################################################################
 * # CTL_708 Definition
 */
#define CTL_708_BIST_SDII_START_COL  		 (0)
#define CTL_708_BIST_SDII_START_ROW  		 (12)

#define CTL_708_BIST_SDII_START_COL_MASK 		0xfff  
#define CTL_708_BIST_SDII_START_ROW_MASK 	0x3ffff  

/* ################################################################################
 * # CTL_70C Definition
 */
#define CTL_70C_R1_LFSR_INI    		 (0)   

#define CTL_70C_R1_LFSR_INT_MASK 	0xffffffff           

/* ################################################################################
 * # CTL_710 Definition
 */
#define CTL_710_R2_LFSR_INI   	 	 (0)   

#define CTL_710_R2_LFSR_INI_MASK 	0xffffffff           

/* ################################################################################
 * # CTL_714 Definition
 */
#define CTL_714_R3_LFSR_INI     		 (0)      

#define CTL_714_R3_LFSR_INI_MASK 	0xffffffff           

/* ################################################################################
 * # CTL_718 Definition
 */
#define CTL_718_R4_LFSR_INI    		 (0)       

#define CTL_718_R4_LFSR_INI_MASK 	0xffffffff            

/* ################################################################################
 * # CTL_71C Definition
 */
#define CTL_71C_BIST_START   		 BIT(0)
#define CTL_71C_BIST_RESULT   		 BIT(4) 
#define CTL_71C_BIST_START_SHIFT   		(0)

#define CTL_71C_BIST_START_MASK 	0x1   




#define 	CTL_00_DDR_BL_8 0x0  
#define 	CTL_00_DDR_BL_16 0x1 

#define 	CTL_04_DFI_RATIO_1_1 0x0
#define 	CTL_04_DFI_RATIO_1_2 0x1
#define 	CTL_04_DFI_RATIO_1_4 0x2

#define 	CTL_58_MRS_RANK_0 0x0
#define 	CTL_58_MRS_RANK_1 0x1
#define 	CTL_58_MRS_RANK_2 0x2
#define 	CTL_58_MRS_RANK_3 0x3

#define 	CTL_58_MRS_NUM_0 0x0
#define 	CTL_58_MRS_NUM_1 0x1
#define 	CTL_58_MRS_NUM_2 0x2
#define 	CTL_58_MRS_NUM_3 0x3
#define 	CTL_58_MRS_NUM_4 0x4
#define 	CTL_58_MRS_NUM_5 0x5
#define 	CTL_58_MRS_NUM_6 0x6

#define CTL_5C_ALERT_WAIT_1600 0x14
#define CTL_5C_ALERT_WAIT_2400 0x1a

#define CTL_64_WODT_LEN_6 0x0
#define CTL_64_WODT_LEN_7 0x1
#define CTL_64_WODT_LEN_8 0x2

#define CTL_64_RDN_ODT_LEN_6 0x0
#define CTL_64_RDN_ODT_LEN_7 0x1
#define CTL_64_RDN_ODT_LEN_8 0x2

#define CTL_68_ZQCAL_TYPE_CS 0x0
#define CTL_68_ZQCAL_TYPE_CL 0x1


/* 0x0c */
#define CTL_PHY_WRDATA	0x1


/*0x4c*/
#define  DDR3_CTL_CKESR  0x06
#define  DDR3_CTL_CKSRE 0x08
#define  DDR3_CTL_CCDDRR 0x06

#define CTL_CKESR  0x09
#define CTL_CKSRE 0x0c
#define CTL_CCDDRR 0x06

/*0x50*/
#define  DDR3_CTL_CKSRX 0x08
#define  DDR3_CTL_XS  0x60
#define  DDR3_CTL_XSDLL 0x200

#define CTL_CKSRX 0x0c
#define CTL_XS  0xcc
#define CTL_XSDLL 0x300

/*0x54*/
#define DDR3_CTL_TMRS_WAIT 0x0c

#define CTL_TMRS_WAIT 0x1d

/*0x5c*/
#define CTL_ALERT_WAIT 0x14

/*0x60*/
#define  DDR3_CTL_CKE 0x05
#define  DDR3_CTL_XP 0x14

#define CTL_CKE 0x06
#define CTL_XP 0x08

/*0x68*/
#define DDR3_CTL_ZQCAL_WAIT 0x200

#define CTL_ZQCAL_WAIT 0x400

/*0x8c*/
#define WR_GROUP_WAITCNT 0x7

/* address mapping */

/*0x90*/
#define CTL_COL_B0_OFS(colum,offset)  ((colum>0) ? (0x0 + (offset)) : (0x1f))
#define CTL_COL_B1_OFS(colum,offset)  ((colum>1) ? (0x1 + (offset)) : (0x1f))
#define CTL_COL_B2_OFS(colum,offset)  ((colum>2) ? (0x2 + (offset)) : (0x1f))
#define CTL_COL_B3_OFS(colum,offset)  ((colum>3) ? (0x3 + (offset)) : (0x1f))
#define CTL_COL_B4_OFS(colum,offset)  ((colum>4) ? (0x4 + (offset)) : (0x1f))
#define CTL_COL_B5_OFS(colum,offset)  ((colum>5) ? (0x5 + (offset)) : (0x1f))

/*0x94*/
#define CTL_COL_B6_OFS(colum,offset)  ((colum>6) ? (0x6 + (offset)) : (0x1f))
#define CTL_COL_B7_OFS(colum,offset)  ((colum>7) ? (0x7 + (offset)) : (0x1f))
#define CTL_COL_B8_OFS(colum,offset)  ((colum>8) ? (0x8 + (offset)) : (0x1f))
#define CTL_COL_B9_OFS(colum,offset)  ((colum>9) ? (0x9 + (offset)) : (0x1f))
#define CTL_COL_B10_OFS(colum,offset)  ((colum>10) ? (0xa + (offset)) : (0x1f))
#define CTL_COL_B11_OFS(colum,offset)  ((colum>11) ? (0xb + (offset)) : (0x1f))

/*0x98*/
#define CTL_ROE_B0_OFS(row,offset) ((row > 0) ? (0x0 + (offset)) : (0x3f))
#define CTL_ROE_B1_OFS(row,offset) ((row > 1) ? (0x1 + (offset)) : (0x3f))
#define CTL_ROE_B2_OFS(row,offset) ((row > 2) ? (0x2 + (offset)) : (0x3f))
#define CTL_ROE_B3_OFS(row,offset) ((row > 3) ? (0x3 + (offset)) : (0x3f))
#define CTL_ROE_B4_OFS(row,offset) ((row > 4) ? (0x4 + (offset)) : (0x3f))

/*0x9c*/
#define CTL_ROE_B5_OFS(row,offset) ((row > 5) ? (0x5 + (offset)) : (0x3f))
#define CTL_ROE_B6_OFS(row,offset) ((row > 6) ? (0x6 + (offset)) : (0x3f))
#define CTL_ROE_B7_OFS(row,offset) ((row > 7) ? (0x7 + (offset)) : (0x3f))
#define CTL_ROE_B8_OFS(row,offset) ((row > 8) ? (0x8 + (offset)) : (0x3f))
#define CTL_ROE_B9_OFS(row,offset) ((row > 9) ? (0x9 + (offset)) : (0x3f))

/*0xa0*/
#define CTL_ROE_B10_OFS(row,offset) ((row > 10) ? (0xa + (offset)) : (0x3f))
#define CTL_ROE_B11_OFS(row,offset) ((row > 11) ? (0xb + (offset)) : (0x3f))
#define CTL_ROE_B12_OFS(row,offset) ((row > 12) ? (0xc + (offset)) : (0x3f))
#define CTL_ROE_B13_OFS(row,offset) ((row > 13) ? (0xd + (offset)) : (0x3f))
#define CTL_ROE_B14_OFS(row,offset) ((row > 14) ? (0xe + (offset)) : (0x3f))

/*0xa4*/
#define CTL_ROE_B15_OFS(row,offset) ((row > 15) ? (0xf + (offset)) : (0x3f))
#define CTL_ROE_B16_OFS(row,offset) ((row > 16) ? (0x10 + (offset)) : (0x3f))
#define CTL_ROE_B17_OFS(row,offset) ((row > 17) ? (0x11 + (offset)) : (0x3f))

/*0xa8*/
#define CTL_BANK_B0_OFS(bank,offset) ((bank > 0) ? (0x0 + (offset)) : (0x3f))
#define CTL_BANK_B1_OFS(bank,offset) ((bank > 1) ? (0x1 + (offset)) : (0x3f))
#define CTL_BANK_B2_OFS(bank,offset) ((bank > 2) ? (0x2 + (offset)) : (0x3f))

/*0xac*/
#define CTL_BG_B0_OFS(bankgroup,offset) ((bankgroup > 0) ? (0x0 + (offset)) : (0x3f))
#define CTL_BG_B1_OFS(bankgroup,offset) ((bankgroup > 1) ? (0x1 + (offset)) : (0x3f))

#define CTL_RANK_B0_OFS(rank,offset) ((rank > 0) ? (0x0 + (offset)) : (0x3f))
#define CTL_RANK_B1_OFS(rank,offset) ((rank > 1) ? (0x1 + (offset)) : (0x3f))


#define DDRINTF_CTL_GROUP0_OFFSET 0x0
#define DDRINTF_CTL_GROUP1_OFFSET 0x700

#define ddrintf_ctl_group0	(( ddrintf_ctl_group0_t *)(CONFIG_SYS_CTC5236_DDRCFG_BASE + DDRINTF_CTL_GROUP0_OFFSET))
#define ddrintf_ctl_group1	(( ddrintf_ctl_group1_t *)(CONFIG_SYS_CTC5236_DDRCFG_BASE + DDRINTF_CTL_GROUP1_OFFSET))

#endif
