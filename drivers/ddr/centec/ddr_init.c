/*
 * (C) Copyright 2004-2017 Centec Networks (suzhou) Co., LTD.
 * Jay Cao <caoj@centecnetworks.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <wait_bit.h>
#include <asm/io.h>

#include <asm/arch/cpu.h>
#include <ctc5236_ddr.h>
#include <asm/arch-ctc5236/sysctl.h>

#include "ddr_ctl_reg.h"
#include "ddr_phy_reg.h"

DECLARE_GLOBAL_DATA_PTR;

phys_size_t ctc_ddr_phy_general_set(ctc5236_ddr_cfg_t *regs)
{
	u32 temp;
	u32 ret;
	
	writel(0x00000000, &ddrintf_phy_group0->phy_vref);
	ret = readl(&ddrintf_phy_group0->phy_vref);
	debug("0x30600100:0x%08x\n",ret);

	if(DDRINTF_TYPE_4 == regs->ddrintf_type)
	{
		/* memwr  (32'h00000104 , 1, 4'hf, 4'hf, 32'h00003062);   */
		temp = (0x0
		|(DDRINTF_BIT_SET<< PHY_104_PHY_DDR4_MODE)
		|(DDRINTF_BIT_SET<< PHY_104_MA2T_EN)
		|(DDRINTF_BIT_SET<< PHY_104_DATA_EXCHANGE_MODE)
		|(PHY_PLL_LOCKCNT_MODE_3 << PHY_104_PLL_LOCKCNT_MODE)
		);
	}
	else 
	{
		/* memwr  (32'h00000104 , 1, 4'hf, 4'hf, 32'h00003062);   */
		temp = (0x0
		|(DDRINTF_BIT_SET<< PHY_104_PHY_DDR3_MODE)
		|(DDRINTF_BIT_SET<< PHY_104_MA2T_EN)
		|(DDRINTF_BIT_SET<< PHY_104_DATA_EXCHANGE_MODE)
		|(PHY_PLL_LOCKCNT_MODE_3 << PHY_104_PLL_LOCKCNT_MODE)
		);

	}	
	//debug("[write]CTC DDR PHY :  ddrintf_type[0x104] = 0x%08x\n", temp);	
	writel(temp, &ddrintf_phy_group0->phy_ddr_mode);
	ret = readl(&ddrintf_phy_group0->phy_ddr_mode);
	debug("0x30600104:0x%08x\n",ret);	

	return 0;
}

phys_size_t ctc_ddr_phy_pll_set(ctc5236_ddr_cfg_t *regs)
{
	u32 ret;
	
	 /* memwr  (32'h00000108 , 1, 4'hf, 4'hf, 32'h1a7a0063);  */
	 writel(0x1a7a0063, &ddrintf_phy_group0->phy_pll_freq0);
	ret = readl(&ddrintf_phy_group0->phy_pll_freq0);
	debug("0x30600108:0x%08x\n",ret);	
		
	 /*memwr  (32'h0000010c , 1, 4'hf, 4'hf, 32'h5a7a0063);  */
	 writel(0x5a7a0063, &ddrintf_phy_group0->phy_pll_freq1);
	ret = readl(&ddrintf_phy_group0->phy_pll_freq1);
	debug("0x3060010c:0x%08x\n",ret);	
	
	 /* memwr  (32'h00000110 , 1, 4'hf, 4'hf, 32'h1a7a0063);  */
	 writel(0x1a7a0063, &ddrintf_phy_group0->phy_pll_freq2);
	ret = readl(&ddrintf_phy_group0->phy_pll_freq2);
	debug("0x30600110:0x%08x\n",ret);	
	
	 /* memwr  (32'h00000114 , 1, 4'hf, 4'hf, 32'h1a7a0063); */
	 writel(0x1a7a0063, &ddrintf_phy_group0->phy_pll_freq3);
	ret = readl(&ddrintf_phy_group0->phy_pll_freq3);
	debug("0x30600114:0x%08x\n",ret);	
	
	 /* memwr  (32'h00000118 , 1, 4'hf, 4'hf, 32'h20000000); */
	 writel(0x20000000, &ddrintf_phy_group0->phy_dll_freq0);
	ret = readl(&ddrintf_phy_group0->phy_dll_freq0);
	debug("0x30600118:0x%08x\n",ret);	
	
	 /* memwr  (32'h0000011c , 1, 4'hf, 4'hf, 32'h20000000); */
	 writel(0x20000000, &ddrintf_phy_group0->phy_dll_freq1);
	ret = readl(&ddrintf_phy_group0->phy_dll_freq1);
	debug("0x3060011c:0x%08x\n",ret);	
	
	 /* memwr  (32'h00000120 , 1, 4'hf, 4'hf, 32'h20000000); */
	 writel(0x20000000, &ddrintf_phy_group0->phy_dll_freq2);
	ret = readl(&ddrintf_phy_group0->phy_dll_freq2);
	debug("0x30600120:0x%08x\n",ret);	
	
	 /* memwr  (32'h00000124 , 1, 4'hf, 4'hf, 32'h20000000); */
	 writel(0x20000000, &ddrintf_phy_group0->phy_dll_freq3);
	ret = readl(&ddrintf_phy_group0->phy_dll_freq3);
	debug("0x30600124:0x%08x\n",ret);	
	
	 /*  memwr  (32'h00000128 , 1, 4'hf, 4'hf, 32'h00000008); */  
	writel(0x00000008, &ddrintf_phy_group0->mlb_fast_boot_en);
	ret = readl(&ddrintf_phy_group0->mlb_fast_boot_en);
	debug("0x30600128:0x%08x\n",ret);	
	
	 /*  memwr  (32'h0000012c , 1, 4'hf, 4'hf, 32'h00000000); */
	writel(0x00000000, &ddrintf_phy_group0->mlb_div_cnt);
	ret = readl(&ddrintf_phy_group0->mlb_div_cnt);
	debug("0x3060012c:0x%08x\n",ret);		 
	 
	return 0;
}

phys_size_t ctc_ddr_phy_trianing_set(ctc5236_ddr_cfg_t *regs)
{
	u32 temp;
	u32 ret;
	
	if(DDRINTF_TYPE_4 == regs->ddrintf_type)
	{
		temp = (0x0
		|(DDR4_PHY_ODT_MODE_4<< PHY_130_PHY_ODT_P)
		|(DDR_PHY_RON_MODE_7<< PHY_130_PHY_PAD_DRVN)
		|(DDR_PHY_RON_MODE_7<< PHY_130_PHY_PAD_DRVP)
		|(DDRINTF_BIT_CLR<< PHY_130_ODT_ALWAYSON)
		);	
	}
	else 
	{
		temp = (0x0
		|(DDR3_PHY_ODT_MODE_2<< PHY_130_PHY_ODT_N)
		|(DDR3_PHY_ODT_MODE_2<< PHY_130_PHY_ODT_P)
		|(DDR_PHY_RON_MODE_7<< PHY_130_PHY_PAD_DRVN)
		|(DDR_PHY_RON_MODE_7<< PHY_130_PHY_PAD_DRVP)
		|(DDRINTF_BIT_CLR<< PHY_130_ODT_ALWAYSON)
		);
	}
	//debug("[write]CTC DDR PHY :  phy_wrdata_delay[0x130] = 0x%08x\n", temp);
	
	writel(temp, &ddrintf_phy_group0->phy_wrdata_delay);
	ret = readl(&ddrintf_phy_group0->phy_wrdata_delay);
	debug("0x30600130:0x%08x\n",ret);		
	
	/* memwr  (32'h00000134 , 1, 4'hf, 4'hf, 32'h00000000); */
	writel(0x00000000, &ddrintf_phy_group0->freerun_timer);
	ret = readl(&ddrintf_phy_group0->freerun_timer);
	debug("0x30600134:0x%08x\n",ret);		
	
	/* memwr  (32'h00000138 , 1, 4'hf, 4'hf, 32'h00000000); */
	writel(0x00000000, &ddrintf_phy_group0->phy_data_byte_pwrdn);
	ret = readl(&ddrintf_phy_group0->phy_data_byte_pwrdn);
	debug("0x30600138:0x%08x\n",ret);		
	
	/* memwr  (32'h0000013c , 1, 4'hf, 4'hf, 32'h20000000); */
	writel(0x20000000, &ddrintf_phy_group0->vref_ca_burst_num);
	ret = readl(&ddrintf_phy_group0->vref_ca_burst_num);
	debug("0x3060013c:0x%08x\n",ret);		
	
	/* memwr  (32'h00000140 , 1, 4'hf, 4'hf, 32'h00000000); */
	writel(0x00000000, &ddrintf_phy_group0->mlb_vref_phy_chk_sel);
	ret = readl(&ddrintf_phy_group0->mlb_vref_phy_chk_sel);
	debug("0x30600140:0x%08x\n",ret);		
	
	/* memwr  (32'h00000144 , 1, 4'hf, 4'hf, 32'h00000000); */
	writel(0x00000000, &ddrintf_phy_group0->mlb_vref_short);
	ret = readl(&ddrintf_phy_group0->mlb_vref_short);
	debug("0x30600144:0x%08x\n",ret);		
	
	/* memwr  (32'h00000148 , 1, 4'hf, 4'hf, 32'h00000000); */
	writel(0x00000000, &ddrintf_phy_group0->mlb_vref_dram_value);
	ret = readl(&ddrintf_phy_group0->mlb_vref_dram_value);
	debug("0x30600148:0x%08x\n",ret);		
	
	/* memwr  (32'h0000014c , 1, 4'hf, 4'hf, 32'h00003080); */
	temp = (0x0
		| (MLB_CA_VREF_LONG<< PHY_14C_MLB_CA_VREF_R_T_VREF_LONG)
		|(MLB_CA_VREF_SHORT << PHY_14C_MLB_CA_VREF_R_T_VREF_SHORT)
		);
	//debug("[write]CTC DDR PHY :  mlb_ca_vref_long[0x14c] = 0x%08x\n", temp);
	
	writel(temp, &ddrintf_phy_group0->mlb_ca_vref_long);
	ret = readl(&ddrintf_phy_group0->mlb_ca_vref_long);
	debug("0x3060014c:0x%08x\n",ret);			
	
	/* memwr  (32'h00000150 , 1, 4'hf, 4'hf, 32'h0400f000);  */
	if(DDRINTF_TYPE_4 == regs->ddrintf_type)
	{		
		temp = (0x0
		| (PHY_EYE_STEP_UNIT<< PHY_150_PHY_EYE_STEP_UNIT)
		|(DDRINTF_BIT_SET << PHY_150_LPDDR4_DUAL_CH)
		|(DDRINTF_BIT_SET <<PHY_150_RANK0_DFI_ODT_HIGH)
		|(DDRINTF_BIT_SET <<PHY_150_RANK1_DFI_ODT_HIGH)
		|(DDRINTF_BIT_SET <<PHY_150_RANK2_DFI_ODT_HIGH)
		|(DDRINTF_BIT_SET <<PHY_150_RANK3_DFI_ODT_HIGH)
		);	
	}
	else 
	{
		temp = (0x0
		| (PHY_EYE_STEP_UNIT<< PHY_150_PHY_EYE_STEP_UNIT)
		|(DDRINTF_BIT_SET << PHY_150_ZQCALL_ALL_RANK)
		|(DDRINTF_BIT_SET << PHY_150_LPDDR4_DUAL_CH)
		|(DDRINTF_BIT_SET <<PHY_150_RANK0_DFI_ODT_HIGH)
		|(DDRINTF_BIT_SET <<PHY_150_RANK1_DFI_ODT_HIGH)
		|(DDRINTF_BIT_SET <<PHY_150_RANK2_DFI_ODT_HIGH)
		|(DDRINTF_BIT_SET <<PHY_150_RANK3_DFI_ODT_HIGH)
		);
	}
	//debug("[write]CTC DDR PHY :  phy_rddv_sel[0x150] = 0x%08x\n", temp);
	
	writel(temp, &ddrintf_phy_group0->phy_rddv_sel);
	ret = readl(&ddrintf_phy_group0->phy_rddv_sel);
	debug("0x30600150:0x%08x\n",ret);			

	return 0;
}

phys_size_t ctc_ddr_phy_sdram_mode_set(ctc5236_ddr_cfg_t *regs)
{
	u32 temp;
	u32 ret;

	
	/* bit 0-15 MLB initialize DDR3/DDR4 MRS2 setting bit 16-31 MLB initialize DDR3/DDR4 MRS3 setting */ 
	/*  memwr  (32'h00000154 , 1, 4'hf, 4'hf, 32'h00000218); */ 
	temp = (0x0
		| (ctc_set_Mode_Register2(regs)<< PHY_154_MLB_MRS2_DATA)
		|(ctc_set_Mode_Register3(regs) << PHY_154_MLB_MRS3_DATA)
		);

	//debug("[write]CTC DDR PHY :  mlb_mrs2_data[0x154] = 0x%08x\n", temp);
	writel(temp, &ddrintf_phy_group0->mlb_mrs2_data);
	ret = readl(&ddrintf_phy_group0->mlb_mrs2_data);
	debug("0x30600154:0x%08x\n",ret);
		
	 /* bit 0-15 MLB initialize DDR3/DDR4 MRS1 setting bit 16-31 MLB initialize DDR3/DDR4 MRS0 setting */  
	 /* memwr  (32'h00000158 , 1, 4'hf, 4'hf, 32'h0b400201); */
	 temp = (0x0
		| (ctc_set_Mode_Register1(regs)<< PHY_158_MLB_MRS1_DATA)
		|(ctc_set_Mode_Register0(regs) << PHY_158_MLB_MRS0_DATA)
		);

	//debug("[write]CTC DDR PHY :  mlb_mrs1_data[0x158] = 0x%08x\n", temp);
	writel(temp, &ddrintf_phy_group0->mlb_mrs1_data);
	ret = readl(&ddrintf_phy_group0->mlb_mrs1_data);
	debug("0x30600158:0x%08x\n",ret);

	if(DDRINTF_TYPE_4 == regs->ddrintf_type)
	{	
		 //  /* bit 0-15 MLB initialize DDR4 MRS4 setting bit 16-31 MLB initialize DDR4 MRS5 setting */  
		 /* memwr  (32'h0000015c , 1, 4'hf, 4'hf, 32'h04a00000); */ 
		 temp = (0x0
			| (ctc_set_Mode_Register4(regs) << PHY_15C_MLB_MRS4_DATA)
			|(ctc_set_Mode_Register5(regs)  << PHY_15C_MLB_MRS5_DATA)
			);

		//debug("[write]CTC DDR PHY :  mlb_mrs4_data[0x15c] = 0x%08x\n", temp);
		writel(temp, &ddrintf_phy_group0->mlb_mrs4_data);
		ret = readl(&ddrintf_phy_group0->mlb_mrs4_data);
		debug("0x3060015c:0x%08x\n",ret);
				
		 /* bit 0-15 MLB initialize DDR4 MRS6 setting bit 16-31 MLB initialize DDR4 MRS7 setting */  
		 /* memwr  (32'h00000160 , 1, 4'hf, 4'hf, 32'h0800084e); */
		 temp = (0x0
			|(((ctc_set_Mode_Register6(regs) & (~0x3f) )|DDR4_PHY_VREFDQ_VALUE) << PHY_160_MLB_MRS6_DATA)
			);

		//debug("[write]CTC DDR PHY :  mlb_mrs6_data[0x160] = 0x%08x\n", temp);
		writel(temp, &ddrintf_phy_group0->mlb_mrs6_data);
		ret = readl(&ddrintf_phy_group0->mlb_mrs6_data);
		debug("0x30600160:0x%08x\n",ret);
	}
	else
	{
		if((DDRINTF_CS_2 == regs->ddrintf_cs) ||(DDRINTF_CS_4 == regs->ddrintf_cs))
		{
			/* bit 0-15 MLB initialize DDR3 MRS1_RK1 setting bit 16-31 MLB initialize DDR3 MRS2_RK1 setting */
			temp = (0x0
				| (ctc_set_Mode_Register1(regs) << PHY_15C_MLB_MRS4_DATA)
				|(ctc_set_Mode_Register2(regs)  << PHY_15C_MLB_MRS5_DATA)
				);

			//debug("[write]CTC DDR PHY :  mlb_mrs4_data[0x15c] = 0x%08x\n", temp);
			writel(temp, &ddrintf_phy_group0->mlb_mrs4_data);
			ret = readl(&ddrintf_phy_group0->mlb_mrs4_data);
			debug("0x3060015c:0x%08x\n",ret);

			if(DDRINTF_CS_4 == regs->ddrintf_cs)
			{		
				/* bit 0-15 MLB initialize DDR3 MRS1_RK2 setting bit 16-31 MLB initialize DDR3 MRS2_RK2 setting */  
				temp = (0x0
					| (ctc_set_Mode_Register1(regs) << PHY_160_MLB_MRS6_DATA)
					|(ctc_set_Mode_Register2(regs) << PHY_160_MLB_MRS7_DATA)
					);

				//debug("[write]CTC DDR PHY :  mlb_mrs6_data[0x160] = 0x%08x\n", temp);
				writel(temp, &ddrintf_phy_group0->mlb_mrs6_data);
				ret = readl(&ddrintf_phy_group0->mlb_mrs6_data);
				debug("0x30600160:0x%08x\n",ret);

				/* bit 0-15 MLB initialize DDR3 MRS1_RK3 setting bit 16-31 MLB initialize DDR3 MRS2_RK3 setting */
				temp = (0x0
					| (ctc_set_Mode_Register1(regs) << PHY_164_MLB_MRS8_DATA)
					|(ctc_set_Mode_Register2(regs) << PHY_164_MLB_MRS9_DATA)
					);

				//debug("[write]CTC DDR PHY :  mlb_mrs8_data[0x164] = 0x%08x\n", temp);
				writel(temp, &ddrintf_phy_group0->mlb_mrs8_data);
				ret = readl(&ddrintf_phy_group0->mlb_mrs8_data);
				debug("0x30600164:0x%08x\n",ret);
			}
		}
	}
		
	return 0;
}


phys_size_t ctc_ddr_phy_timing_set(ctc5236_ddr_cfg_t *regs)
{
	u32 temp;
	u32 ret;
	u32 mod,mrd,rp,rcd,cl;
	u32 rrd;
	u32 ccd = 4;	/* for ddr3 spec  ccd = 4 and for ddr4 spec ccd_s = 4 */
	u32 cwl = ctc_compute_cas_write_latency(regs);
	u32 rfc = ctc_compute_refresh_cycle_time(regs);
	u32 refi = ctc_compute_refresh_limit_time(regs);
	
	cl = regs->sdram_cas_latency;
	rcd = rp = (cl + 1)/2;

	if(DDRINTF_TYPE_4 == regs->ddrintf_type)
	{
		/*
		*tMRD = 8nCK
		*tMOD = max(24nCK, 15ns)
		*/
		mrd = 8;
		mod = max(24U,ctc_picos_to_phyclk(regs,15000));

		/*
		* for ddr4 spec tRRD_S depends on SDRAM pagesize and DDR rate
		*
		*				  2400Mbps		  2133Mbps  		  1866Mbps		1600Mbps		
		*tRRD_S(1/2K)	Max(4nCK,3.3ns)	Max(4nCK,3.7ns)	Max(4nCK,4.2ns)	Max(4nCK,5ns)	
		*tRRD_S(1K)		Max(4nCK,3.3ns)	Max(4nCK,3.7ns)	Max(4nCK,4.2ns)	Max(4nCK,5ns)			
		*tRRD_S(2K)		Max(4nCK,5.3ns)	Max(4nCK,5.3ns)	Max(4nCK,5.3ns)	Max(4nCK,6ns)	
		*/
		unsigned int rrd_table[3][4] = {
			{3300,3700,4200,5000},
			{3300,3700,4200,5000},
			{5300,5300,5300,6000}
		};

		rrd = max(4U,ctc_picos_to_phyclk(regs,rrd_table[regs->sdram_page_size][regs->ddrintf_rate]));
	}
	else
	{
		/* for DDR3 spec
		*tMRD = 4nCK
		*tMOD = max(12nCK, 15ns)
		*/
		mrd = 4;
		mod = max(12U,ctc_picos_to_phyclk(regs,15000));

		/*
		* for ddr3 spec tRRD_S depends on SDRAM pagesize and DDR rate
		*
		*				  1600Mbps  		  1333Mbps		  1066Mbps		  800Mbps		  	
		*tRRD_S(1K)		Max(4nCK,6ns)	Max(4nCK,6ns)	Max(4nCK,7.5ns)	Max(4nCK,10ns)			
		*tRRD_S(2K)		Max(4nCK,7.5ns)	Max(4nCK,7.5ns)	Max(4nCK,10ns)	Max(4nCK,10ns)			
		*
		*/
		unsigned int rrd_table[2][4] = {
			{6000,6000,7500,10000},
			{7500,7500,10000,10000}
		};

		rrd = max(4U,ctc_picos_to_phyclk(regs,rrd_table[regs->sdram_page_size-1][regs->ddrintf_rate-3]));
	}

	/* bit 0-15 tMRD bit 16-31 tMOD */  
	/* memwr  (32'h00000168 , 1, 4'hf, 4'hf, 32'h00180008); */
	temp = (0x0
			|(mrd<< PHY_168_MLB_R_T_MRD)
			|(mod << PHY_168_MLB_R_T_MOD)
			);

	//debug("[write]CTC DDR PHY :  mlb_mrd[0x168] = 0x%08x\n", temp);
	writel(temp, &ddrintf_phy_group0->mlb_mrd);
	ret = readl(&ddrintf_phy_group0->mlb_mrd);
	debug("0x30600168:0x%08x\n",ret);	

	/* bit 0-15 tZQinit bit 16-31 tXPR */  
	if(DDRINTF_TYPE_4 == regs->ddrintf_type)
	{
		/* memwr  (32'h0000016c , 1, 4'hf, 4'hf, 32'h01500400);  */
		temp = (0x0
				| (DDR4_PHY_T_ZQINIT<< PHY_16C_MLB_R_T_OIT)
				|(DDR4_PHY_T_XPR << PHY_16C_MLB_R_T_XPR)
				);
	}
	else
	{
		/* memwr  (32'h0000016c , 1, 4'hf, 4'hf, 32'h00440100);  */
		temp = (0x0
				| (DDR3_PHY_T_ZQINIT<< PHY_16C_MLB_R_T_OIT)
				|(DDR3_PHY_T_XPR << PHY_16C_MLB_R_T_XPR)
				);
	}
	//debug("[write]CTC DDR PHY :  mlb_oit[0x16c] = 0x%08x\n", temp);
	writel(temp, &ddrintf_phy_group0->mlb_oit);
	ret = readl(&ddrintf_phy_group0->mlb_oit);
	debug("0x3060016c:0x%08x\n",ret);		
	
	/* bit 0-7 tRP for pre-charge all banks bit 8 DQS train for gate training bit 16-23 tWR bit 24-27 write to DRAM ODT signal delay */ 
	/* tWR = 15ns */
	temp = (0x0
			| (rp<< PHY_170_MLB_R_T_RPALL)
			|(ctc_picos_to_phyclk(regs,15000) << PHY_170_MLB_R_T_WR)
			);

	//debug("[write]CTC DDR PHY :  mlb_rpall[0x170] = 0x%08x\n", temp);
	writel(temp, &ddrintf_phy_group0->mlb_rpall);
	ret = readl(&ddrintf_phy_group0->mlb_rpall);
	debug("0x30600170:0x%08x\n",ret);
	
	/* bit 12-31 time for cal_done to DDR bus resrt in power on procedure */  
	/* memwr  (32'h00000174 , 1, 4'hf, 4'hf, 32'h000f0000); */
	temp = (0x0 | (PHY_MLB_RSTCTL_EN2RST<< PHY_174_MLB_R_RSTCTL_EN2RST));

	//debug("[write]CTC DDR PHY :  mlb_rstctl_en2rst[0x174] = 0x%08x\n", temp);
	writel(temp, &ddrintf_phy_group0->mlb_rstctl_en2rst);
	ret = readl(&ddrintf_phy_group0->mlb_rstctl_en2rst);
	debug("0x30600174:0x%08x\n",ret);

	if(DDRINTF_TYPE_4 == regs->ddrintf_type)
	{
		/* bit 0-7 time for DFI write command to write data enable  bit 8-11 time for DFI write data enableto  write data  bit 12-31 time for reset to CKE during power on procedure */  
		/* memwr  (32'h00000178 , 1, 4'hf, 4'hf, 32'h0001410A);*/  
		temp = (0x0
				| ((cwl-2)<< PHY_178_MLB_R_DFI_T_PHY_WRLAT)
				|( PHY_MLB_DFI_T_PHY_WRDATA<< PHY_178_MLB_R_DFI_T_PHY_WRDATA)
				|(PHY_MLB_RSTCTL_RST2CKE << PHY_178_MLB_R_RSTCTL_RST2CKE)
				);

		//debug("[write]CTC DDR PHY :  mlb_dfi_phy_wrlat[0x178] = 0x%08x\n", temp);
		writel(temp, &ddrintf_phy_group0->mlb_dfi_phy_wrlat);
		ret = readl(&ddrintf_phy_group0->mlb_dfi_phy_wrlat);
		debug("0x30600178:0x%08x\n",ret);

	}
	else
	{
		/* bit 0-7 time for DFI write command to write data enable  bit 8-11 time for DFI write data enableto  write data  bit 12-31 time for reset to CKE during power on procedure */  
		/* memwr  (32'h00000178 , 1, 4'hf, 4'hf, 32'h00258106);*/  
		temp = (0x0
				|((cwl-2)<< PHY_178_MLB_R_DFI_T_PHY_WRLAT)
				|(PHY_MLB_DFI_T_PHY_WRDATA << PHY_178_MLB_R_DFI_T_PHY_WRDATA)
				|(DDR3_PHY_MLB_RSTCTL_RST2CKE << PHY_178_MLB_R_RSTCTL_RST2CKE)
				);
		
		//debug("[write]CTC DDR PHY :  mlb_dfi_phy_wrlat[0x178] = 0x%08x\n", temp);
		writel(temp, &ddrintf_phy_group0->mlb_dfi_phy_wrlat);
		ret = readl(&ddrintf_phy_group0->mlb_dfi_phy_wrlat);
		debug("0x30600178:0x%08x\n",ret);
		
	}
		
	/* bit 0-7 tRRD bit 8-15 tCCD bit 16-23 ??? bit 24-31 ??? */  
	/* memwr  (32'h0000017c , 1, 4'hf, 4'hf, 32'h06090406);  */
	temp = (0x0
			|(rrd<< PHY_17C_MLB_R_T_RRD)
			|(ccd << PHY_17C_MLB_R_T_CCD)
			);

	//debug("[write]CTC DDR PHY :  mlb_rrd[0x17c] = 0x%08x\n", temp);
	writel(temp, &ddrintf_phy_group0->mlb_rrd);
	ret = readl(&ddrintf_phy_group0->mlb_rrd);
	debug("0x3060017c:0x%08x\n",ret);
		
	/* bit 0-7 AL bit 8-15 time for dfi read command to read data enable bit 23-31 tRFC */  
	/* memwr  (32'h00000180 , 1, 4'hf, 4'hf, 32'h40800900); */
	temp = (0x0
			| ((cl-2)<< PHY_180_MLB_R_DFI_T_RDDATA_EN)
			|((rfc & PHY_180_MLB_R_T_RFC_MASK) << PHY_180_MLB_R_T_RFC)
			);

	//debug("[write]CTC DDR PHY :  mlb_al[0x180] = 0x%08x\n", temp);
	writel(temp, &ddrintf_phy_group0->mlb_al);	
	ret = readl(&ddrintf_phy_group0->mlb_al);
	debug("0x30600180:0x%08x\n",ret);

	/* bit 0-16 time for auto-refresh interval(7.8us) bit 17 auto-refresh controller disable bit 20-23 time for write ODT period bit 24-31 tRCD */ 
	/* memwr  (32'h00000184 , 1, 4'hf, 4'hf, 32'h0b400c30); */
	temp = (0x0
			| ((refi & PHY_184_MLB_R_REFRESH_LIMIT_MASK )<< PHY_184_MLB_R_REFRESH_LIMIT)
			|(PHY_MLB_WODT_TIMER << PHY_184_MLB_R_WODT_TIMER)
			|(rcd << PHY_184_MLB_R_T_RCD)
			);

	//debug("[write]CTC DDR PHY :  mlb_refresh_limit[0x184] = 0x%08x\n", temp);
	writel(temp, &ddrintf_phy_group0->mlb_refresh_limit);
	ret = readl(&ddrintf_phy_group0->mlb_refresh_limit);
	debug("0x30600184:0x%08x\n",ret);
	
	/* bit 1-3 ????  bit 8-15 MLB CA training DRAM timing setting CACKEL */  
	if(DDRINTF_TYPE_4 == regs->ddrintf_type)
	{
		/* memwr  (32'h00000188 , 1, 4'hf, 4'hf, 32'h00001002);  */
		temp = (0x0
				| (DDRINTF_BIT_SET<< PHY_188_CTRL_IO_DDR4_EN)
				|(PHY_MLB_T_CACKEL << PHY_188_MLB_MLB_T_CACKEL)
				);
	}
	else
	{
		/* memwr  (32'h00000188 , 1, 4'hf, 4'hf, 32'h00001000);  */
		temp = (0x0
				|(PHY_MLB_T_CACKEL << PHY_188_MLB_MLB_T_CACKEL)
				);

	}
	//debug("[write]CTC DDR PHY :  phy_lpddr4_ca_odt[0x188] = 0x%08x\n", temp);
	writel(temp, &ddrintf_phy_group0->phy_lpddr4_ca_odt);
	ret = readl(&ddrintf_phy_group0->phy_lpddr4_ca_odt);
	debug("0x30600188:0x%08x\n",ret);
		
	/* memwr  (32'h0000018c , 1, 4'hf, 4'hf, 32'h05701016); */
	temp = (0x0
			|(PHY_MLB_CHECK_T_CNT<< PHY_18C_MLB_CHECK_T_CNT)
			|(PHY_MLB_MLB_T_CACKEH << PHY_18C_MLB_MLB_T_CACKEH)
			|(PHY_MLB_T_CAEXT << PHY_18C_MLB_MLB_T_CAEXT)
			|(PHY_MLB_T_MRZ << PHY_18C_MLB_MLB_T_MRZ)
			);

	//debug("[write]CTC DDR PHY :  mlb_check_cnt[0x18c] = 0x%08x\n", temp);
	writel(temp, &ddrintf_phy_group0->mlb_check_cnt);
	ret = readl(&ddrintf_phy_group0->mlb_check_cnt);
	debug("0x3060018c:0x%08x\n",ret);
	
	/* memwr  (32'h00000190 , 1, 4'hf, 4'hf, 32'h00000000); */
	writel(0x0, &ddrintf_phy_group0->mlb_ca_check_number);
	ret = readl(&ddrintf_phy_group0->mlb_ca_check_number);
	debug("0x30600190:0x%08x\n",ret);
		
	return 0;
}


phys_size_t ctc_ddr_phy_trian_ctl(ctc5236_ddr_cfg_t *regs)
{
	u32 temp;
	u32 ret;
		
	/*memwr  (32'h0000019c , 1, 4'hf, 4'hf, 32'h20202020); */
	writel(0x20202020, &ddrintf_phy_group0->phy_fdl_addr0_wr);
	ret = readl(&ddrintf_phy_group0->phy_fdl_addr0_wr);
	debug("0x3060019c:0x%08x\n",ret);	
	
	/*memwr  (32'h000001a0 , 1, 4'hf, 4'hf, 32'h20202020); */
	writel(0x20202020, &ddrintf_phy_group0->phy_fdl_addr4_wr);
	ret = readl(&ddrintf_phy_group0->phy_fdl_addr4_wr);
	debug("0x306001a0:0x%08x\n",ret);	
	
	/*memwr  (32'h000001a4 , 1, 4'hf, 4'hf, 32'h00002020); */
	writel(0x00002020, &ddrintf_phy_group0->phy_fdl_addr8_wr);
	ret = readl(&ddrintf_phy_group0->phy_fdl_addr8_wr);
	debug("0x306001a4:0x%08x\n",ret);	
	
	if(DDRINTF_TYPE_4 == regs->ddrintf_type)
	{
		/*memwr  (32'h000001a8 , 1, 4'hf, 4'hf, 32'h80002020); */
		writel(0x80002020, &ddrintf_phy_group0->mlb_train_pttn_p1);
		ret = readl(&ddrintf_phy_group0->mlb_train_pttn_p1);
		debug("0x306001a8:0x%08x\n",ret);	
	
		/*memwr  (32'h000001ac , 1, 4'hf, 4'hf, 32'h00000001);*/
		writel(0x00000001, &ddrintf_phy_group0->mlb_ca_delay_bt);
		ret = readl(&ddrintf_phy_group0->mlb_ca_delay_bt);
		debug("0x306001ac:0x%08x\n",ret);		
	}
	else
	{
		/*memwr  (32'h000001a8 , 1, 4'hf, 4'hf, 32'h80000000); */
		writel(0x80000000, &ddrintf_phy_group0->mlb_train_pttn_p1);
		ret = readl(&ddrintf_phy_group0->mlb_train_pttn_p1);
		debug("0x306001a8:0x%08x\n",ret);	
	
		/*memwr  (32'h000001ac , 1, 4'hf, 4'hf, 32'h00000000);*/
		writel(0x00000000, &ddrintf_phy_group0->mlb_ca_delay_bt);
		ret = readl(&ddrintf_phy_group0->mlb_ca_delay_bt);
		debug("0x306001ac:0x%08x\n",ret);	
	}	


	//memrd  (32'h00000170 , 1, 4'hf, 4'hf, rdata_return);
	//rdata_return[9]=1;			
	//memwr	(32'h00000170,1,4'hf,4'hf,rdata_return[31:0]);	
	temp = readl(&ddrintf_phy_group0->mlb_rpall);
		temp = (temp
		| (DDRINTF_BIT_SET<< PHY_170_MLB_TRAIN_PASSEND_MODE)
		);
	//debug("[write]CTC DDR PHY : mlb_rpall = 0x%08x\n",temp);
	writel(temp, &ddrintf_phy_group0->mlb_rpall);
	ret = readl(&ddrintf_phy_group0->mlb_rpall);
	debug("0x30600170:0x%08x\n",ret);		
	
	//memrd	(32'h00000190,1,4'hf,4'hf,rdata_return); 
	//rdata_return[31:20]=12'h100;				
	//memwr	(32'h00000190,1,4'hf,4'hf,rdata_return[31:0]);
	temp = readl(&ddrintf_phy_group0->mlb_ca_check_number);
		temp = (temp
		| (0x100<< PHY_190_MLB_TRAIN_PASSEND_OFS)
		);
	//debug("[write]CTC DDR PHY : mlb_ca_check_number = 0x%08x\n",temp);
	writel(temp, &ddrintf_phy_group0->mlb_ca_check_number);
	ret = readl(&ddrintf_phy_group0->mlb_ca_check_number);
	debug("0x30600190:0x%08x\n",ret);		

	if(DDRINTF_TYPE_4 == regs->ddrintf_type)
	{
		/*memwr  (32'h00000198,  1, 4'hf, 4'hf, 32'h08060000); */
		writel(0x08060000, &ddrintf_phy_group0->cal_offset_pu);
		ret = readl(&ddrintf_phy_group0->cal_offset_pu);
		debug("0x30600198:0x%08x\n",ret);	
	}
	else
	{
		/*memwr  (32'h00000198,  1, 4'hf, 4'hf, 32'h06080000); */
		writel(0x06080000, &ddrintf_phy_group0->cal_offset_pu);
		ret = readl(&ddrintf_phy_group0->cal_offset_pu);
		debug("0x30600198:0x%08x\n",ret);	
	}
	
	if(DDRINTF_MANUAL_MODE_TRAIN == regs->ddrintf_training_mode){
		/* manual mode gate training setting */
		/* Delay of gate(coarse) */
		temp = (0x0
			|(regs->ddrinitf_gate_trip_db0 << PHY_1CC_DB0_DLL_GATE_TRIP)
			|(regs->ddrinitf_gate_trip_db1 << PHY_1CC_DB1_DLL_GATE_TRIP)
			|(regs->ddrinitf_gate_trip_db2 << PHY_1CC_DB2_DLL_GATE_TRIP)
		);

		writel(temp, &ddrintf_phy_group0->db0_dll_gate_trip);
		ret = readl(&ddrintf_phy_group0->db0_dll_gate_trip);
		debug("0x306001cc:0x%08x\n",ret);

		/* Delay of gate(fine) */
		temp = (0x0
			|(regs->ddrinitf_gate_fine_db0 << PHY_1D4_DB0_DLL_GATE_FINE)
			|(regs->ddrinitf_gate_fine_db1 << PHY_1D4_DB1_DLL_GATE_FINE)
			|(regs->ddrinitf_gate_fine_db2 << PHY_1D4_DB2_DLL_GATE_FINE)
		);

		writel(temp, &ddrintf_phy_group0->db0_dll_gate_fine);
		ret = readl(&ddrintf_phy_group0->db0_dll_gate_fine);
		debug("0x306001d4:0x%08x\n",ret);
	}else{

		/*memwr  (32'h000001cc , 1, 4'hf, 4'hf, 32'h01010101); */
		writel(0x01010101, &ddrintf_phy_group0->db0_dll_gate_trip);
		ret = readl(&ddrintf_phy_group0->db0_dll_gate_trip);
		debug("0x306001cc:0x%08x\n",ret);
		
		/*memwr  (32'h000001d0 , 1, 4'hf, 4'hf, 32'h01010101); */
		writel(0x01010101, &ddrintf_phy_group0->db4_dll_gate_trip);
		ret = readl(&ddrintf_phy_group0->db4_dll_gate_trip);
		debug("0x306001d0:0x%08x\n",ret);

		/*memwr  (32'h000001d4 , 1, 4'hf, 4'hf, 32'h80808080); */
		writel(0x80808080, &ddrintf_phy_group0->db0_dll_gate_fine);
		ret = readl(&ddrintf_phy_group0->db0_dll_gate_fine);
		debug("0x306001d4:0x%08x\n",ret);

		/*memwr  (32'h000001d8 , 1, 4'hf, 4'hf, 32'h80808080);*/
		writel(0x80808080, &ddrintf_phy_group0->db4_dll_gate_fine);
		ret = readl(&ddrintf_phy_group0->db4_dll_gate_fine);
		debug("0x306001d8:0x%08x\n",ret);
	}
	
	if(DDRINTF_MANUAL_MODE_TRAIN == regs->ddrintf_training_mode){
		/* manual mode read eye training */
		/* DQS delay */
		temp = (0x0
			|(regs->ddrintf_rdeye_dqs_db0 << PHY_688_DB0_DLL_RDDEYE_DQS)
			|(regs->ddrintf_rdeye_dqs_db1 << PHY_688_DB1_DLL_RDDEYE_DQS)
			|(regs->ddrintf_rdeye_dqs_db2 << PHY_688_DB2_DLL_RDDEYE_DQS)
		);
		writel(temp, &ddrintf_phy_group1->db0_dll_rddeye_dqs);
		ret = readl(&ddrintf_phy_group1->db0_dll_rddeye_dqs);
		debug("0x30600688:0x%08x\n",ret);

		/* DQSN delay */
		temp = (0x0
			|(regs->ddrintf_rdeye_dqsn_db0 << PHY_690_DB0_DLL_RDDEYE_DQSN)
			|(regs->ddrintf_rdeye_dqsn_db1 << PHY_690_DB1_DLL_RDDEYE_DQSN)
			|(regs->ddrintf_rdeye_dqsn_db2 << PHY_690_DB2_DLL_RDDEYE_DQSN)
		);

		writel(temp, &ddrintf_phy_group1->db0_dll_rddeye_dqsn);
		ret = readl(&ddrintf_phy_group1->db0_dll_rddeye_dqsn);
		debug("0x30600690:0x%08x\n",ret);
	}else{
		 /* for ddr phy manu mode*/ 
		 /*memwr  (32'h00000688 , 1, 4'hf, 4'hf, 32'h80808080);*/ 
		writel(0x80808080, &ddrintf_phy_group1->db0_dll_rddeye_dqs);
		ret = readl(&ddrintf_phy_group1->db0_dll_rddeye_dqs);
		debug("0x30600688:0x%08x\n",ret);
		
		 /*memwr  (32'h0000068c , 1, 4'hf, 4'hf, 32'h80808080);*/ 
		writel(0x80808080, &ddrintf_phy_group1->db4_dll_rddeye_dqs);
		ret = readl(&ddrintf_phy_group1->db4_dll_rddeye_dqs);
		debug("0x3060068c:0x%08x\n",ret);
		
		 /*memwr  (32'h00000690 , 1, 4'hf, 4'hf, 32'h80808080);*/ 
		writel(0x80808080, &ddrintf_phy_group1->db0_dll_rddeye_dqsn);
		ret = readl(&ddrintf_phy_group1->db0_dll_rddeye_dqsn);
		debug("0x30600690:0x%08x\n",ret);
		
		 /*memwr  (32'h00000694 , 1, 4'hf, 4'hf, 32'h80808080);*/  
		writel(0x80808080, &ddrintf_phy_group1->db4_dll_rddeye_dqsn);
		ret = readl(&ddrintf_phy_group1->db4_dll_rddeye_dqsn);
		debug("0x30600694:0x%08x\n",ret);
	}

	if(DDRINTF_MANUAL_MODE_TRAIN == regs->ddrintf_training_mode){
		/* manual mode write eye training setting */

		/* DQ delay (coarse) */
		temp = (0x0
			|(regs->ddrintf_wreye_coarse_db0 << PHY_800_DB0_BT_WDQ)
			|(regs->ddrintf_wreye_coarse_db1 << PHY_800_DB1_BT_WDQ)
			|(regs->ddrintf_wreye_coarse_db2 << PHY_800_DB2_BT_WDQ)
		);
		writel(temp, &ddrintf_phy_group2->db0_bt_wdq);
		ret = readl(&ddrintf_phy_group2->db0_bt_wdq);
		debug("0x30600800:0x%08x\n",ret);

		/* DQ delay (fine-tune) */
		temp = (0x0
			|(regs->ddrintf_wreye_fine_db0 << PHY_6F8_DB0_DLL_WRDEYE_DQ)
			|(regs->ddrintf_wreye_fine_db1 << PHY_6F8_DB1_DLL_WRDEYE_DQ)
			|(regs->ddrintf_wreye_fine_db2 << PHY_6F8_DB2_DLL_WRDEYE_DQ)
		);
		writel(temp, &ddrintf_phy_group1->db0_dll_wrdeye_dq);
		ret = readl(&ddrintf_phy_group1->db0_dll_wrdeye_dq);
		debug("0x306006f8:0x%08x\n",ret);
	}else{
		/*memwr  (32'h000006f8 , 1, 4'hf, 4'hf, 32'h80808080); */
		writel(0x80808080, &ddrintf_phy_group1->db0_dll_wrdeye_dq);
		ret = readl(&ddrintf_phy_group1->db0_dll_wrdeye_dq);
		debug("0x306006f8:0x%08x\n",ret);
		
		 /*memwr  (32'h000006fc , 1, 4'hf, 4'hf, 32'h80808080);*/ 
		writel(0x80808080, &ddrintf_phy_group1->db4_dll_wrdeye_dq);
		ret = readl(&ddrintf_phy_group1->db4_dll_wrdeye_dq);
		debug("0x306006fc:0x%08x\n",ret);
		
		 /* for write dq manual mode */ 
		 /*memwr  (32'h00000800 , 1, 4'hf, 4'hf, 32'h00000000);*/
		writel(0x000000000, &ddrintf_phy_group2->db0_bt_wdq);
		ret = readl(&ddrintf_phy_group2->db0_bt_wdq);
		debug("0x30600800:0x%08x\n",ret);
	}

	/* memwr  (32'h00000194 , 1, 4'hf, 4'hf, 32'h801152e0 ); */
	temp = (0x0
		|(DDRINTF_BIT_CLR << PHY_194_MLB_TRAIN_START)
		|(DDRINTF_BIT_CLR << PHY_194_CAL_DISABLE)
		|(DDRINTF_BIT_CLR << PHY_194_CAL_MANUAL_MODE)
		|(DDRINTF_BIT_CLR << PHY_194_CAL_MANUAL_PWRDN)
		|(DDRINTF_BIT_CLR << PHY_194_MLB_CAL_PCOMP_FIRST)
		|(DDRINTF_BIT_SET << PHY_194_MLB_TRAIN_CA_PERBIT_DISABLE)
		|(DDRINTF_BIT_SET << PHY_194_MLB_TRAIN_CA_DISABLE)
		|(regs->ddrintf_wrlvl_mode << PHY_194_MLB_TRAIN_WRLVL_DISABLE)
		|(regs->ddrintf_training_mode << PHY_194_MLB_TRAIN_GATE_DISABLE)
		|(DDRINTF_BIT_SET << PHY_194_MLB_TRAIN_RD_PERBIT_DISABLE)
		|(regs->ddrintf_training_mode << PHY_194_MLB_TRAIN_RDEYE_DISABLE)
		|(regs->ddrintf_training_mode << PHY_194_MLB_TRAIN_RDVREF_DISABLE)
		|(DDRINTF_BIT_SET << PHY_194_MLB_TRAIN_WR_PERBIT_DISABLE)
		|(regs->ddrintf_training_mode << PHY_194_MLB_TRAIN_WREYE_DISABLE)
		|(regs->ddrintf_training_mode << PHY_194_MLB_TRAIN_WRVREF_DISABLE)
		|(DDRINTF_BIT_CLR << PHY_194_MLB_TRAIN_GATE_FAST_MODE)
		/* Usedata mode for gate and read eye training */
		|(DDR_TRAINING_MODE_1 << PHY_194_MLB_TRAIN_DATA_TRAINING_MODE)
		|(DDRINTF_BIT_CLR << PHY_194_MLB_TRAIN_BIT_EN)
		|(0x1 << PHY_194_MLB_TRAIN_WDQS_STOP_INTG)
		|(0x80 << PHY_194_MLB_TRAIN_WDQS_STOP_STEP)
		);
	writel(temp , &ddrintf_phy_group0->mlb_train_start);
	
	/* DDR3 don't support write vref training */
	if(DDRINTF_TYPE_3 == regs->ddrintf_type){
		setbits_le32(&ddrintf_phy_group0->mlb_train_start,BIT(14));
	}
	ret = readl(&ddrintf_phy_group0->mlb_train_start);
	debug("0x30600194:0x%08x\n",ret);
	
	return 0;
}


phys_size_t ctc_ddr_phy_write_leveling_ctl(ctc5236_ddr_cfg_t *regs)
{
	u32 temp;
	u32 ret;

	/* if you use manual mode write leveling, you must set ddrintf_wrlvl_delay_db0
	* and ddrintf_wrlvl_delay_db1, which are the delay time between DQS and CLK
	* in picoseconds,and you also need to set  ddrintf_wrlvl_delay_db2 when you use
	* ECC.
	*/
	if(DDRINTF_WRLVL_MANUAL_MODE == regs->ddrintf_wrlvl_mode){
		/* Write leveling Manual Mode DQS Coarse Delay Value */
		unsigned int wrlvl_coarse_db0 = 0;
		unsigned int wrlvl_coarse_db1 = 0;
		unsigned int wrlvl_coarse_db2 = 0;	/* db2 is defined using ECC */
		/* Write leveling Manual Mode DQS Fine Delay Value */
		unsigned int wrlvl_fine_db0 = 0;
		unsigned int wrlvl_fine_db1 = 0;
		unsigned int wrlvl_fine_db2 = 0;	/* db2 is defined using ECC */
		/* time for each coarse bit adjustment, unit:ps */
		unsigned int wrlvl_coarse_tune;	
		unsigned int db0_delay_ps = regs->ddrintf_wrlvl_delay_db0;
		unsigned int db1_delay_ps = regs->ddrintf_wrlvl_delay_db1;

		/* time for each coarse bit adjustment: time = 1/ddr rate */
		wrlvl_coarse_tune = (ctc_get_ddr_phy_clk_period_ps(regs)+1)/2;

		if(db0_delay_ps < 0 || db0_delay_ps > 4*(wrlvl_coarse_tune) ){
			printf("ERROR: Write leveling DQS0 Delay Value out of Range!\n");
			return -1;
		}
		if(db1_delay_ps < 0 || db1_delay_ps > 4*(wrlvl_coarse_tune)){
			printf("ERROR: Write leveling DQS1 Delay Value out of Range!\n");
			return -1;
		}
		
		/* write leveling coarse tune value */
		wrlvl_coarse_db0 = (db0_delay_ps)/wrlvl_coarse_tune;
		wrlvl_coarse_db1 = (db1_delay_ps)/wrlvl_coarse_tune;
		

		/* time for each fine bit adjustment: time = (1/ddr rate)/256 */
		/* write leveling fine tune value */
		wrlvl_fine_db0 = (((db0_delay_ps)%wrlvl_coarse_tune)*256 + (wrlvl_coarse_tune/2))/wrlvl_coarse_tune;
		wrlvl_fine_db1 = (((db1_delay_ps)%wrlvl_coarse_tune)*256 + (wrlvl_coarse_tune/2))/wrlvl_coarse_tune;

		/* When we use ECC, we need to configure db2 */
		if(DDRINTF_ECC_ENABLE == regs->ddrintf_ecc_en){
			unsigned int db2_delay_ps = regs->ddrintf_wrlvl_delay_db2;
			if(db1_delay_ps < 0 || db1_delay_ps > 4*(wrlvl_coarse_tune)){
				printf("ERROR: Write leveling DQS2 Delay Value out of Range!\n");
				return -1;
			}
			wrlvl_coarse_db2 = (db2_delay_ps)/wrlvl_coarse_tune;
			wrlvl_fine_db2 = (((db2_delay_ps)%wrlvl_coarse_tune)*256 + (wrlvl_coarse_tune/2))/wrlvl_coarse_tune;
		}
		
		/* Write leveling Manual Mode DQS Coarse Delay Value */
		temp = (0x0
			|((wrlvl_coarse_db0 & 0x3) << PHY_1BC_MLB_DB0_BT_WLVL)
			|((wrlvl_coarse_db1 & 0x3) << PHY_1BC_MLB_DB1_BT_WLVL)
			|((wrlvl_coarse_db2 & 0x3) << PHY_1BC_MLB_DB2_BT_WLVL)
			);
		writel(temp, &ddrintf_phy_group0->mlb_db0_bt_wlvl);
		ret = readl(&ddrintf_phy_group0->mlb_db0_bt_wlvl);
		debug("0x306001bc:0x%08x\n",ret);

		/* Write leveling Manual Mode DQS Fine Delay Value */
		temp = (0x0
			|((wrlvl_fine_db0 & 0xff) << PHY_1C0_MLB_DB0_DLL_WLVL_WDQS)
			|((wrlvl_fine_db1 & 0xff) << PHY_1C0_MLB_DB1_DLL_WLVL_WDQS)
			|((wrlvl_fine_db2 & 0xff) << PHY_1C0_MLB_DB2_DLL_WLVL_WDQS)
			);
		writel(temp, &ddrintf_phy_group0->mlb_db0_dll_wlvl_wdqs);
		ret = readl(&ddrintf_phy_group0->mlb_db0_dll_wlvl_wdqs);
		debug("0x306001c0:0x%08x\n",ret);
		
	}else{
		
		/*memwr  (32'h000001bc , 1, 4'hf, 4'hf, 32'h00000000);*/ 
		writel(0x000000000, &ddrintf_phy_group0->mlb_db0_bt_wlvl);
		ret = readl(&ddrintf_phy_group0->mlb_db0_bt_wlvl);
		debug("0x306001bc:0x%08x\n",ret);

		/*memwr  (32'h000001c0 , 1, 4'hf, 4'hf, 32'h00000000); */
		writel(0x000000000, &ddrintf_phy_group0->mlb_db0_dll_wlvl_wdqs);
		ret = readl(&ddrintf_phy_group0->mlb_db0_dll_wlvl_wdqs);
		debug("0x306001c0:0x%08x\n",ret);		
	}
	
	 /*memwr  (32'h000001b0 , 1, 4'hf, 4'hf, 32'h00000000); */
	 writel(0x000000000, &ddrintf_phy_group0->mlb_db0_bt_wlvl_start);
	ret = readl(&ddrintf_phy_group0->mlb_db0_bt_wlvl_start);
	debug("0x306001b0:0x%08x\n",ret);
	
	 /*memwr  (32'h000001b4 , 1, 4'hf, 4'hf, 32'h00000000); */
	 writel(0x000000000, &ddrintf_phy_group0->mlb_db0_dll_wlvl_start);
	ret = readl(&ddrintf_phy_group0->mlb_db0_dll_wlvl_start);
	debug("0x306001b4:0x%08x\n",ret);
	
	 /*memwr  (32'h000001b8 , 1, 4'hf, 4'hf, 32'h00000000); */
	 writel(0x000000000, &ddrintf_phy_group0->mlb_db4_dll_wlvl_start);
	ret = readl(&ddrintf_phy_group0->mlb_db4_dll_wlvl_start);
	debug("0x306001b8:0x%08x\n",ret);

	 /*memwr  (32'h000001c4 , 1, 4'hf, 4'hf, 32'h00000000);*/
	 writel(0x000000000, &ddrintf_phy_group0->mlb_db4_dll_wlvl_wdqs);
	ret = readl(&ddrintf_phy_group0->mlb_db4_dll_wlvl_wdqs);
	debug("0x306001c4:0x%08x\n",ret);

	 /* write leveling DQ golden compared data setting */
	 /*memwr  (32'h000001c8 , 1, 4'hf, 4'hf, 32'h000aff2c);*/
	 temp = (0x0
				| (PHY_MLB_LOAD_ACK_CYCLE<< PHY_1C8_MLB_LOAD_ACK_CYCLE)
				|(PHY_MLB_DFI_RDDATA_GOLD << PHY_1C8_MLB_DFI_RDDATA_GOLD)
				|(PHY_MLB_LOAD_RESP_CYCLE_WL << PHY_1C8_MLB_LOAD_RESP_CYCLE)
				|(DDRINTF_BIT_SET<< PHY_1C8_REFRESH_UPDATE)
				);
	// debug("[write]CTC DDR PHY : mlb_load_ack_cycle[0x1c8] = 0x%08x\n",temp);
	 writel(temp, &ddrintf_phy_group0->mlb_load_ack_cycle);
	ret = readl(&ddrintf_phy_group0->mlb_load_ack_cycle);
	debug("0x306001c8:0x%08x\n",ret);

	if(DDRINTF_TYPE_4 == regs->ddrintf_type)
	{
		 /*memwr  (32'h000001dc , 1, 4'hf, 4'hf, 32'h00080000);*/ 
		 temp = (0x0 | (PHY_MLB_TRAIN_FINE_GATE_DELAY<< PHY_1DC_MLB_TRAIN_FINE_GATE_DELAY_OFFSET));
	}
	else
	{
		/*memwr  (32'h000001dc , 1, 4'hf, 4'hf, 32'h00000000);*/
		temp = 0;
	}
	// debug("[write]CTC DDR PHY : mlb_train_gate_offset_minus[0x1dc] = 0x%08x\n",temp);
	 writel(temp, &ddrintf_phy_group0->mlb_train_gate_offset_minus);
	ret = readl(&ddrintf_phy_group0->mlb_train_gate_offset_minus);
	debug("0x306001dc:0x%08x\n",ret);
	
	 /*memwr  (32'h000001e0 , 1, 4'hf, 4'hf, 32'h00000000);*/
	 writel(0x000000000, &ddrintf_phy_group0->mlb_db0_train_gate_bit_sel);
	ret = readl(&ddrintf_phy_group0->mlb_db0_train_gate_bit_sel);
	debug("0x306001e0:0x%08x\n",ret);
	
	 /*	memwr  (32'h000001e4 , 1, 4'hf, 4'hf, 32'haa55aa55); 
	 *	memwr  (32'h000001e8 , 1, 4'hf, 4'hf, 32'h55aa55aa); 
	 *	memwr  (32'h000001ec , 1, 4'hf, 4'hf, 32'haaaa5555); 
	 *	memwr  (32'h000001f0 , 1, 4'hf, 4'hf, 32'h5555aaaa);
	 */
	 /*  MPR read golden data 0 */
	 writel(PHY_MLB_MPR_DATA_0, &ddrintf_phy_group0->mlb_lpddr4_mpr0_data_0);
	ret = readl(&ddrintf_phy_group0->mlb_lpddr4_mpr0_data_0);
	debug("0x306001e4:0x%08x\n",ret);
	
	 writel(PHY_MLB_MPR_DATA_1, &ddrintf_phy_group0->mlb_lpddr4_mpr0_data_1);
	ret = readl(&ddrintf_phy_group0->mlb_lpddr4_mpr0_data_1);
	debug("0x306001e8:0x%08x\n",ret);
	
	 writel(PHY_MLB_MPR_DATA_2, &ddrintf_phy_group0->mlb_lpddr4_mpr0_data_2);
	ret = readl(&ddrintf_phy_group0->mlb_lpddr4_mpr0_data_2);
	debug("0x306001ec:0x%08x\n",ret);
	
	 writel(PHY_MLB_MPR_DATA_3, &ddrintf_phy_group0->mlb_lpddr4_mpr0_data_3);
	ret = readl(&ddrintf_phy_group0->mlb_lpddr4_mpr0_data_3);
	debug("0x306001f0:0x%08x\n",ret);
	
	 /*	memwr  (32'h000001f4 , 1, 4'hf, 4'hf, 32'haa55aa55); 
	 *	memwr  (32'h000001f8 , 1, 4'hf, 4'hf, 32'h55aa55aa); 
	 *	memwr  (32'h000001fc , 1, 4'hf, 4'hf, 32'haaaa5555); 
	 *	memwr  (32'h00000600 , 1, 4'hf, 4'hf, 32'h5555aaaa);*/
	/*  MPR read golden data 1 */
	 writel(PHY_MLB_MPR_DATA_0, &ddrintf_phy_group0->mlb_lpddr4_mpr1_data_0);
	ret = readl(&ddrintf_phy_group0->mlb_lpddr4_mpr1_data_0);
	debug("0x306001f4:0x%08x\n",ret);

	 writel(PHY_MLB_MPR_DATA_1, &ddrintf_phy_group0->mlb_lpddr4_mpr1_data_1);
	ret = readl(&ddrintf_phy_group0->mlb_lpddr4_mpr1_data_1);
	debug("0x306001f8:0x%08x\n",ret);
	
	 writel(PHY_MLB_MPR_DATA_2, &ddrintf_phy_group0->mlb_lpddr4_mpr1_data_2);
	ret = readl(&ddrintf_phy_group0->mlb_lpddr4_mpr1_data_2);
	debug("0x306001fc:0x%08x\n",ret);
	
	 writel(PHY_MLB_MPR_DATA_3, &ddrintf_phy_group1->mlb_lpddr4_mpr1_data_3);
	ret = readl(&ddrintf_phy_group1->mlb_lpddr4_mpr1_data_3);
	debug("0x30600600:0x%08x\n",ret);
	
	 /* 	memwr  (32'h00000604 , 1, 4'hf, 4'hf, 32'haa55aa55); 
	  *	memwr  (32'h00000608 , 1, 4'hf, 4'hf, 32'h55aa55aa); 
	  *	memwr  (32'h0000060c , 1, 4'hf, 4'hf, 32'haaaa5555);
	  *	memwr  (32'h00000610 , 1, 4'hf, 4'hf, 32'h5555aaaa);
	  */
	 /*  MPR read golden data 2 */
	 writel(PHY_MLB_MPR_DATA_0, &ddrintf_phy_group1->mlb_lpddr4_mpr2_data_0);
	ret = readl(&ddrintf_phy_group1->mlb_lpddr4_mpr2_data_0);
	debug("0x30600604:0x%08x\n",ret);
	
	 writel(PHY_MLB_MPR_DATA_1, &ddrintf_phy_group1->mlb_lpddr4_mpr2_data_1);
	ret = readl(&ddrintf_phy_group1->mlb_lpddr4_mpr2_data_1);
	debug("0x30600608:0x%08x\n",ret);
	
	 writel(PHY_MLB_MPR_DATA_2, &ddrintf_phy_group1->mlb_lpddr4_mpr2_data_2);
	ret = readl(&ddrintf_phy_group1->mlb_lpddr4_mpr2_data_2);
	debug("0x3060060c:0x%08x\n",ret);
	
	 writel(PHY_MLB_MPR_DATA_3, &ddrintf_phy_group1->mlb_lpddr4_mpr2_data_3);
	ret = readl(&ddrintf_phy_group1->mlb_lpddr4_mpr2_data_3);
	debug("0x30600610:0x%08x\n",ret);
	
	 /*	memwr  (32'h00000614 , 1, 4'hf, 4'hf, 32'haa55aa55); 
	 *	memwr  (32'h00000618 , 1, 4'hf, 4'hf, 32'h55aa55aa); 
	 *	memwr  (32'h0000061c , 1, 4'hf, 4'hf, 32'haaaa5555); 
	 *	memwr  (32'h00000620 , 1, 4'hf, 4'hf, 32'h5555aaaa);
	 */
	 /*  MPR read golden data 3 */
	 writel(PHY_MLB_MPR_DATA_0, &ddrintf_phy_group1->mlb_lpddr4_mpr3_data_0);
	ret = readl(&ddrintf_phy_group1->mlb_lpddr4_mpr3_data_0);
	debug("0x30600614:0x%08x\n",ret);
	
	 writel(PHY_MLB_MPR_DATA_1, &ddrintf_phy_group1->mlb_lpddr4_mpr3_data_1);
	ret = readl(&ddrintf_phy_group1->mlb_lpddr4_mpr3_data_1);
	debug("0x30600618:0x%08x\n",ret);
	
	 writel(PHY_MLB_MPR_DATA_2, &ddrintf_phy_group1->mlb_lpddr4_mpr3_data_2);
	ret = readl(&ddrintf_phy_group1->mlb_lpddr4_mpr3_data_2);
	debug("0x3060061c:0x%08x\n",ret);
	
	 writel(PHY_MLB_MPR_DATA_3, &ddrintf_phy_group1->mlb_lpddr4_mpr3_data_3);
	ret = readl(&ddrintf_phy_group1->mlb_lpddr4_mpr3_data_3);
	debug("0x30600620:0x%08x\n",ret);
	 
	 /*memwr  (32'h00000624 , 1, 4'hf, 4'hf, 32'h20202020); */
	 writel(0x20202020, &ddrintf_phy_group1->phy_fdl_db0_dqs_rd);
	ret = readl(&ddrintf_phy_group1->phy_fdl_db0_dqs_rd);
	debug("0x30600624:0x%08x\n",ret);
	
	 /*memwr  (32'h00000628 , 1, 4'hf, 4'hf, 32'h20202020);*/ 	 
	 writel(0x20202020, &ddrintf_phy_group1->phy_fdl_db0_dq2_rd);
	ret = readl(&ddrintf_phy_group1->phy_fdl_db0_dq2_rd);
	debug("0x30600628:0x%08x\n",ret);
	
	 /*memwr  (32'h0000062c , 1, 4'hf, 4'hf, 32'h20202020); */
	 writel(0x20202020, &ddrintf_phy_group1->phy_fdl_db0_dq6_rd);
	ret = readl(&ddrintf_phy_group1->phy_fdl_db0_dq6_rd);
	debug("0x3060062c:0x%08x\n",ret);
	
	 /*memwr  (32'h00000630 , 1, 4'hf, 4'hf, 32'h20202020); */	 
	 writel(0x20202020, &ddrintf_phy_group1->phy_fdl_db1_dq0_rd);
	ret = readl(&ddrintf_phy_group1->phy_fdl_db1_dq0_rd);
	debug("0x30600630:0x%08x\n",ret);
	
	 /*memwr  (32'h00000634 , 1, 4'hf, 4'hf, 32'h20202020); */	
	 writel(0x20202020, &ddrintf_phy_group1->phy_fdl_db1_dq4_rd);
	ret = readl(&ddrintf_phy_group1->phy_fdl_db1_dq4_rd);
	debug("0x30600634:0x%08x\n",ret);
	
	 /*memwr  (32'h00000638 , 1, 4'hf, 4'hf, 32'h20202020); */
	 writel(0x20202020, &ddrintf_phy_group1->phy_fdl_db2_dqs_rd);
	ret = readl(&ddrintf_phy_group1->phy_fdl_db2_dqs_rd);
	debug("0x30600638:0x%08x\n",ret);
	
 	 /*memwr  (32'h0000063c , 1, 4'hf, 4'hf, 32'h20202020); */
	 writel(0x20202020, &ddrintf_phy_group1->phy_fdl_db2_dq2_rd);
	ret = readl(&ddrintf_phy_group1->phy_fdl_db2_dq2_rd);
	debug("0x3060063c:0x%08x\n",ret);
	
	/*memwr  (32'h00000640 , 1, 4'hf, 4'hf, 32'h20202020); */
	 writel(0x20202020, &ddrintf_phy_group1->phy_fdl_db2_dq6_rd);
	ret = readl(&ddrintf_phy_group1->phy_fdl_db2_dq6_rd);
	debug("0x30600640:0x%08x\n",ret);
	
	/*memwr  (32'h00000644 , 1, 4'hf, 4'hf, 32'h20202020); */
	 writel(0x20202020, &ddrintf_phy_group1->phy_fdl_db3_dq0_rd);
	ret = readl(&ddrintf_phy_group1->phy_fdl_db3_dq0_rd);
	debug("0x30600644:0x%08x\n",ret);
	
	 /*memwr  (32'h00000648 , 1, 4'hf, 4'hf, 32'h20202020); */
	 writel(0x20202020, &ddrintf_phy_group1->phy_fdl_db3_dq4_rd);
	ret = readl(&ddrintf_phy_group1->phy_fdl_db3_dq4_rd);
	debug("0x30600648:0x%08x\n",ret);
	
	/*memwr  (32'h0000064c , 1, 4'hf, 4'hf, 32'h20202020); */
	 writel(0x20202020, &ddrintf_phy_group1->phy_fdl_db4_dqs_rd);
	ret = readl(&ddrintf_phy_group1->phy_fdl_db4_dqs_rd);
	debug("0x3060064c:0x%08x\n",ret);
	
	/*memwr  (32'h00000650 , 1, 4'hf, 4'hf, 32'h20202020); */
	 writel(0x20202020, &ddrintf_phy_group1->phy_fdl_db4_dq2_rd);
	ret = readl(&ddrintf_phy_group1->phy_fdl_db4_dq2_rd);
	debug("0x30600650:0x%08x\n",ret);
	
	/*memwr  (32'h00000654 , 1, 4'hf, 4'hf, 32'h20202020); */
	 writel(0x20202020, &ddrintf_phy_group1->phy_fdl_db4_dq6_rd);
	ret = readl(&ddrintf_phy_group1->phy_fdl_db4_dq6_rd);
	debug("0x30600654:0x%08x\n",ret);
	
	 /*memwr  (32'h00000658 , 1, 4'hf, 4'hf, 32'h20202020); */
	 writel(0x20202020, &ddrintf_phy_group1->phy_fdl_db5_dq0_rd);
	ret = readl(&ddrintf_phy_group1->phy_fdl_db5_dq0_rd);
	debug("0x30600658:0x%08x\n",ret);
	
	/*memwr  (32'h0000065c , 1, 4'hf, 4'hf, 32'h20202020); */
	 writel(0x20202020, &ddrintf_phy_group1->phy_fdl_db5_dq4_rd);
	ret = readl(&ddrintf_phy_group1->phy_fdl_db5_dq4_rd);
	debug("0x3060065c:0x%08x\n",ret);
	
	/*memwr  (32'h00000660 , 1, 4'hf, 4'hf, 32'h20202020); */
	 writel(0x20202020, &ddrintf_phy_group1->phy_fdl_db6_dqs_rd);
	ret = readl(&ddrintf_phy_group1->phy_fdl_db6_dqs_rd);
	debug("0x30600660:0x%08x\n",ret);
	
	/*memwr  (32'h00000664 , 1, 4'hf, 4'hf, 32'h20202020); */
	 writel(0x20202020, &ddrintf_phy_group1->phy_fdl_db6_dq2_rd);
	ret = readl(&ddrintf_phy_group1->phy_fdl_db6_dq2_rd);
	debug("0x30600664:0x%08x\n",ret);
	
	/*memwr  (32'h00000668 , 1, 4'hf, 4'hf, 32'h20202020); */	 
	 writel(0x20202020, &ddrintf_phy_group1->phy_fdl_db6_dq6_rd);
	ret = readl(&ddrintf_phy_group1->phy_fdl_db6_dq6_rd);
	debug("0x30600668:0x%08x\n",ret);
	
	/*memwr  (32'h0000066c , 1, 4'hf, 4'hf, 32'h20202020); */	  
	 writel(0x20202020, &ddrintf_phy_group1->phy_fdl_db7_dq0_rd);
	ret = readl(&ddrintf_phy_group1->phy_fdl_db7_dq0_rd);
	debug("0x3060066c:0x%08x\n",ret);
	
	/*memwr  (32'h00000670 , 1, 4'hf, 4'hf, 32'h20202020); */	 
	 writel(0x20202020, &ddrintf_phy_group1->phy_fdl_db7_dq4_rd);
	ret = readl(&ddrintf_phy_group1->phy_fdl_db7_dq4_rd);
	debug("0x30600670:0x%08x\n",ret);

	if(DDRINTF_TYPE_4 == regs->ddrintf_type)
	{
		/*memwr  (32'h00000698 , 1, 4'hf, 4'hf, 32'h20200800); */	 
		 writel(0x20200800, &ddrintf_phy_group1->mlb_train_deye_offset_minus);
	}
	else
	{
		/*memwr  (32'h00000698 , 1, 4'hf, 4'hf, 32'h20200000); */	 
		 writel(0x20200000, &ddrintf_phy_group1->mlb_train_deye_offset_minus);
	}
	ret = readl(&ddrintf_phy_group1->mlb_train_deye_offset_minus);
	debug("0x30600698:0x%08x\n",ret);
	
	/*memwr  (32'h0000069c , 1, 4'hf, 4'hf, 32'h20202020);*/ 	 
	 writel(0x20202020, &ddrintf_phy_group1->phy_fdl_db0_dq0_wr);
	ret = readl(&ddrintf_phy_group1->phy_fdl_db0_dq0_wr);
	debug("0x3060069c:0x%08x\n",ret);
	
	/*memwr  (32'h000006a0 , 1, 4'hf, 4'hf, 32'h20202020); */	 
	 writel(0x20202020, &ddrintf_phy_group1->phy_fdl_db0_dq4_wr);
	ret = readl(&ddrintf_phy_group1->phy_fdl_db0_dq4_wr);
	debug("0x306006a0:0x%08x\n",ret);
	
	/*memwr  (32'h000006a4 , 1, 4'hf, 4'hf, 32'h20202020); */
	 writel(0x20202020, &ddrintf_phy_group1->phy_fdl_db1_dqs_wr);
	ret = readl(&ddrintf_phy_group1->phy_fdl_db1_dqs_wr);
	debug("0x306006a4:0x%08x\n",ret);
	
	/*memwr  (32'h000006a8 , 1, 4'hf, 4'hf, 32'h20202020); */	 
	 writel(0x20202020, &ddrintf_phy_group1->phy_fdl_db1_dq2_wr);
	ret = readl(&ddrintf_phy_group1->phy_fdl_db1_dq2_wr);
	debug("0x306006a8:0x%08x\n",ret);
	
	/*memwr  (32'h000006ac , 1, 4'hf, 4'hf, 32'h20202020); */
	 writel(0x20202020, &ddrintf_phy_group1->phy_fdl_db1_dq6_wr);
	ret = readl(&ddrintf_phy_group1->phy_fdl_db1_dq6_wr);
	debug("0x306006ac:0x%08x\n",ret);
	
	 /*memwr  (32'h000006b0 , 1, 4'hf, 4'hf, 32'h20202020); */
	writel(0x20202020, &ddrintf_phy_group1->phy_fdl_db2_dq0_wr);
	ret = readl(&ddrintf_phy_group1->phy_fdl_db2_dq0_wr);
	debug("0x306006b0:0x%08x\n",ret);
	
	 /*memwr  (32'h000006b4 , 1, 4'hf, 4'hf, 32'h20202020); */
	 writel(0x20202020, &ddrintf_phy_group1->phy_fdl_db2_dq4_wr);
	ret = readl(&ddrintf_phy_group1->phy_fdl_db2_dq4_wr);
	debug("0x306006b4:0x%08x\n",ret);
	
	 /*memwr  (32'h000006b8 , 1, 4'hf, 4'hf, 32'h20202020); */
	 writel(0x20202020, &ddrintf_phy_group1->phy_fdl_db3_dqs_wr);
	ret = readl(&ddrintf_phy_group1->phy_fdl_db3_dqs_wr);
	debug("0x306006b8:0x%08x\n",ret);
	
	 /*memwr  (32'h000006bc , 1, 4'hf, 4'hf, 32'h20202020); */
	 writel(0x20202020, &ddrintf_phy_group1->phy_fdl_db3_dq2_wr);
	ret = readl(&ddrintf_phy_group1->phy_fdl_db3_dq2_wr);
	debug("0x306006bc:0x%08x\n",ret);
		 
	 /*memwr  (32'h000006c0 , 1, 4'hf, 4'hf, 32'h20202020); */
	 writel(0x20202020, &ddrintf_phy_group1->phy_fdl_db3_dq6_wr);
	ret = readl(&ddrintf_phy_group1->phy_fdl_db3_dq6_wr);
	debug("0x306006c0:0x%08x\n",ret);
	
	 /*memwr  (32'h000006c4 , 1, 4'hf, 4'hf, 32'h20202020); */
	 writel(0x20202020, &ddrintf_phy_group1->phy_fdl_db4_dq0_wr);
	ret = readl(&ddrintf_phy_group1->phy_fdl_db4_dq0_wr);
	debug("0x306006c4:0x%08x\n",ret);
	
	 /*memwr  (32'h000006c8 , 1, 4'hf, 4'hf, 32'h20202020); */
	 writel(0x20202020, &ddrintf_phy_group1->phy_fdl_db4_dq4_wr);
	ret = readl(&ddrintf_phy_group1->phy_fdl_db4_dq4_wr);
	debug("0x306006c8:0x%08x\n",ret);
	
	 /*memwr  (32'h000006cc , 1, 4'hf, 4'hf, 32'h20202020); */
	 writel(0x20202020, &ddrintf_phy_group1->phy_fdl_db5_dqs_wr);
	ret = readl(&ddrintf_phy_group1->phy_fdl_db5_dqs_wr);
	debug("0x306006cc:0x%08x\n",ret);
	
	 /*memwr  (32'h000006d0 , 1, 4'hf, 4'hf, 32'h20202020); */
	 writel(0x20202020, &ddrintf_phy_group1->phy_fdl_db5_dq2_wr);
	ret = readl(&ddrintf_phy_group1->phy_fdl_db5_dq2_wr);
	debug("0x306006d0:0x%08x\n",ret);
	
	 /*memwr  (32'h000006d4 , 1, 4'hf, 4'hf, 32'h20202020);*/ 
	 writel(0x20202020, &ddrintf_phy_group1->phy_fdl_db5_dq6_wr);
	ret = readl(&ddrintf_phy_group1->phy_fdl_db5_dq6_wr);
	debug("0x306006d4:0x%08x\n",ret);
	
	 /*memwr  (32'h000006d8 , 1, 4'hf, 4'hf, 32'h20202020); */
	 writel(0x20202020, &ddrintf_phy_group1->phy_fdl_db6_dq0_wr);
	ret = readl(&ddrintf_phy_group1->phy_fdl_db6_dq0_wr);
	debug("0x306006d8:0x%08x\n",ret);
	
	 /*memwr  (32'h000006dc , 1, 4'hf, 4'hf, 32'h20202020); */
	 writel(0x20202020, &ddrintf_phy_group1->phy_fdl_db6_dq4_wr);
	ret = readl(&ddrintf_phy_group1->phy_fdl_db6_dq4_wr);
	debug("0x306006dc:0x%08x\n",ret);
	
	 /*memwr  (32'h000006e0 , 1, 4'hf, 4'hf, 32'h20202020); */
	 writel(0x20202020, &ddrintf_phy_group1->phy_fdl_db7_dqs_wr);
	ret = readl(&ddrintf_phy_group1->phy_fdl_db7_dqs_wr);
	debug("0x306006e0:0x%08x\n",ret);
	
	 /*memwr  (32'h000006e4 , 1, 4'hf, 4'hf, 32'h20202020);*/
	 writel(0x20202020, &ddrintf_phy_group1->phy_fdl_db7_dq2_wr);
	ret = readl(&ddrintf_phy_group1->phy_fdl_db7_dq2_wr);
	debug("0x306006e4:0x%08x\n",ret);
	
	 /*memwr  (32'h000006e8 , 1, 4'hf, 4'hf, 32'h00002020);*/
	 writel(0x00002020, &ddrintf_phy_group1->phy_fdl_db7_dq6_wr);
	ret = readl(&ddrintf_phy_group1->phy_fdl_db7_dq6_wr);
	debug("0x306006e8:0x%08x\n",ret);
	
	if(DDRINTF_TYPE_4 == regs->ddrintf_type)
	{
		 /*memwr  (32'h00000804 , 1, 4'hf, 4'hf, 32'h00000800);*/
		 writel(0x00000800, &ddrintf_phy_group2->mlb_train_wdq_offset_minus);
		ret = readl(&ddrintf_phy_group2->mlb_train_wdq_offset_minus);
		debug("0x30600804:0x%08x\n",ret);
	}
	
	return 0;
}

phys_size_t ctc_ddr_phy_func(ctc5236_ddr_cfg_t *regs)
{
	u32 ret;
	u32 temp;
	if(DDRINTF_TYPE_4 == regs->ddrintf_type)
	{
		/*	memrd   (32'h00000140,1,4'hf,4'hf,rdata_return);
		 * 	rdata_return[15:8] = 8'h60;				 // reg_mlb_vref_phy_adj_end  
		 *	memwr   (32'h00000140,1,4'hf,4'hf,rdata_return);
		 */
		ret = readl(&ddrintf_phy_group0->mlb_vref_phy_chk_sel);
		temp = (ret
			|(PHY_MLB_VREF_PHY_ADJ_END<<PHY_140_MLB_VREF_PHY_ADJ_END)
			);
		
		//debug("[write]CTC DDR PHY : mlb_vref_phy_chk_sel[0x140] = 0x%08x\n",temp);

		writel(temp, &ddrintf_phy_group0->mlb_vref_phy_chk_sel);
		ret = readl(&ddrintf_phy_group0->mlb_vref_phy_chk_sel);
		debug("0x30600140:0x%08x\n",ret);
		
		 /*	memrd   (32'h0000067C,1,4'hf,4'hf,rdata_return);
		 * 	rdata_return[31:24] = 8'h4E;				 // reg_mlb_vref_phy_value_manu_db8  
		 *	memwr   (32'h0000067C,1,4'hf,4'hf,rdata_return);
		 */
		ret = readl(&ddrintf_phy_group1->mlb_vref_phy_adj_start);
		temp = (ret
			|(PHY_MLB_VREF_PHY_VALUE_MANU_DB8<<PHY_67C_MLB_VREF_PHY_VALUE_MANU_DB8)
			);
		
		//debug("[write]CTC DDR PHY : mlb_vref_phy_adj_start[0x67c] = 0x%08x\n",temp);

		writel(temp, &ddrintf_phy_group1->mlb_vref_phy_adj_start);	 
		ret = readl(&ddrintf_phy_group1->mlb_vref_phy_adj_start);
		debug("0x3060067c:0x%08x\n",ret);
		
		 /*	memrd   (32'h0000067C,1,4'hf,4'hf,rdata_return);
		 *	rdata_return[23:16] = 8'h40;				 // reg_mlb_vref_phy_adj_start  
		 *	memwr   (32'h0000067C,1,4'hf,4'hf,rdata_return);
		 */
		ret = readl(&ddrintf_phy_group1->mlb_vref_phy_adj_start);
		temp = (ret
			|(PHY_MLB_VREF_PHY_ADJ_START<<PHY_67C_MLB_VREF_PHY_ADJ_START)
			);
		
		//debug("[write]CTC DDR PHY : mlb_vref_phy_adj_start[0x67c] = 0x%08x\n",temp);

		writel(temp, &ddrintf_phy_group1->mlb_vref_phy_adj_start);	 
		ret = readl(&ddrintf_phy_group1->mlb_vref_phy_adj_start);
		debug("0x3060067c:0x%08x\n",ret);
		
		if(DDRINTF_MANUAL_MODE_TRAIN == regs->ddrintf_training_mode){
			/* manual mode read vref setting for DDR4*/
			temp = (0x0
				|(regs->ddrintf_rdvref_value_db0<<PHY_680_MLB_VREF_PHY_VALUE_MANU_DB0)
				|(regs->ddrintf_rdvref_value_db1<<PHY_680_MLB_VREF_PHY_VALUE_MANU_DB1)
				|(regs->ddrintf_rdvref_value_db2<<PHY_680_MLB_VREF_PHY_VALUE_MANU_DB2)
				);
			writel(temp, &ddrintf_phy_group1->mlb_vref_phy_manu_db0);	
			ret = readl(&ddrintf_phy_group1->mlb_vref_phy_manu_db0);
			debug("0x30600680:0x%08x\n",ret);
		}else{
			/* memwr   (32'h00000680,1,4'hf,4'hf,32'h4E4E4E4E);*/
			temp = (0x0
				|(PHY_MLB_VREF_PHY_VALUE_MANU_DB0<<PHY_680_MLB_VREF_PHY_VALUE_MANU_DB0)
				|(PHY_MLB_VREF_PHY_VALUE_MANU_DB1<<PHY_680_MLB_VREF_PHY_VALUE_MANU_DB1)
				|(PHY_MLB_VREF_PHY_VALUE_MANU_DB2<<PHY_680_MLB_VREF_PHY_VALUE_MANU_DB2)
				|(PHY_MLB_VREF_PHY_VALUE_MANU_DB3<<PHY_680_MLB_VREF_PHY_VALUE_MANU_DB3)
				);
			//debug("[write]CTC DDR PHY : mlb_vref_phy_manu_db0[0x680] = 0x%08x\n",temp);

			writel(temp, &ddrintf_phy_group1->mlb_vref_phy_manu_db0);	
			ret = readl(&ddrintf_phy_group1->mlb_vref_phy_manu_db0);
			debug("0x30600680:0x%08x\n",ret);

			/* memwr   (32'h00000684,1,4'hf,4'hf,32'h4E4E4E4E);*/
			temp = (0x0
				|(PHY_MLB_VREF_PHY_VALUE_MANU_DB4<<PHY_684_MLB_VREF_PHY_VALUE_MANU_DB4)
				|(PHY_MLB_VREF_PHY_VALUE_MANU_DB5<<PHY_684_MLB_VREF_PHY_VALUE_MANU_DB5)
				|(PHY_MLB_VREF_PHY_VALUE_MANU_DB6<<PHY_684_MLB_VREF_PHY_VALUE_MANU_DB6)
				|(PHY_MLB_VREF_PHY_VALUE_MANU_DB7<<PHY_684_MLB_VREF_PHY_VALUE_MANU_DB7)
				);
			//debug("[write]CTC DDR PHY : mlb_vref_phy_manu_db4[0x684] = 0x%08x\n",temp);

			writel(temp, &ddrintf_phy_group1->mlb_vref_phy_manu_db4);
			ret = readl(&ddrintf_phy_group1->mlb_vref_phy_manu_db4);
			debug("0x30600684:0x%08x\n",ret);
		}
	 	
		 /*memrd   (32'h00000148,1,4'hf,4'hf,rdata_return);
		 * rdata_return[5:0] = 6'h07;				 // reg_mlb_vref_dram_value_start  
		 *memwr   (32'h00000148,1,4'hf,4'hf,rdata_return);
		 */
		ret = readl(&ddrintf_phy_group0->mlb_vref_dram_value);
		temp = (ret
			|(PHY_MLB_VREF_DRAM_VALUE_START<<PHY_148_MLB_VREF_DRAM_VALUE_START)
			);
		
		//debug("[write]CTC DDR PHY : mlb_vref_dram_value[0x148] = 0x%08x\n",temp);

		writel(temp, &ddrintf_phy_group0->mlb_vref_dram_value);		
		ret = readl(&ddrintf_phy_group0->mlb_vref_dram_value);
		debug("0x30600148:0x%08x\n",ret);
		
		 /*	memrd   (32'h00000148,1,4'hf,4'hf,rdata_return);
		 * 	rdata_return[13:8] = 6'h2f;				 // reg_mlb_vref_dram_value_end  
		 *	memwr   (32'h00000148,1,4'hf,4'hf,rdata_return);
		 */
		ret = readl(&ddrintf_phy_group0->mlb_vref_dram_value);
		temp = (ret
			|(PHY_MLB_VREF_DRAM_VALUE_END<<PHY_148_MLB_VREF_DRAM_VALUE_END)
			);
		
		//debug("[write]CTC DDR PHY : mlb_vref_dram_value[0x148] = 0x%08x\n",temp);

		writel(temp, &ddrintf_phy_group0->mlb_vref_dram_value);
		ret = readl(&ddrintf_phy_group0->mlb_vref_dram_value);
		debug("0x30600148:0x%08x\n",ret);
		
		if(DDRINTF_MANUAL_MODE_TRAIN == regs->ddrintf_training_mode){
			/* manual mode write vref training settining */
			temp = (0x0
				|(DDRINTF_BIT_SET <<PHY_6F4_MLB_VREF_DRAM_RANGE_MANU)
				|(regs->ddrintf_wrvref_value <<PHY_6F4_MLB_VREF_DRAM_VALUE_MANU)
				);
			writel(temp, &ddrintf_phy_group1->mlb_vref_dram_value_manu);
			ret = readl(&ddrintf_phy_group1->mlb_vref_dram_value_manu);
			debug("0x306006f4:0x%08x\n",ret);
			
		}else{
			/*	memrd   (32'h000006F4,1,4'hf,4'hf,rdata_return);
			* 	rdata_return[5:0] = 6'h19;				 // reg_mlb_vref_dram_value_manu  
			*	memwr   (32'h000006F4,1,4'hf,4'hf,rdata_return);
			*/
			ret = readl(&ddrintf_phy_group1->mlb_vref_dram_value_manu);
			temp = (ret
				|(PHY_MLB_VREF_DRAM_VALUE_MANU<<PHY_6F4_MLB_VREF_DRAM_VALUE_MANU)
			);
			//debug("[write]CTC DDR PHY : mlb_vref_dram_value_manu[0x6f4] = 0x%08x\n",temp);

			writel(temp, &ddrintf_phy_group1->mlb_vref_dram_value_manu);
			ret = readl(&ddrintf_phy_group1->mlb_vref_dram_value_manu);
			debug("0x306006f4:0x%08x\n",ret);

			/*	memrd   (32'h000006F4,1,4'hf,4'hf,rdata_return);
			* 	rdata_return[6] = 1'h1;					 // reg_mlb_vref_dram_range_manu  
			*	memwr   (32'h000006F4,1,4'hf,4'hf,rdata_return);
			*/
			ret = readl(&ddrintf_phy_group1->mlb_vref_dram_value_manu);
			temp = (ret
				|(DDRINTF_BIT_SET<<PHY_6F4_MLB_VREF_DRAM_RANGE_MANU)
			);
			//debug("[write]CTC DDR PHY : mlb_vref_dram_value_manu[0x6f4] = 0x%08x\n",temp);

			writel(temp, &ddrintf_phy_group1->mlb_vref_dram_value_manu);
			ret = readl(&ddrintf_phy_group1->mlb_vref_dram_value_manu);
			debug("0x306006f4:0x%08x\n",ret);
		}
	
	}
	else
	{
		/*	memrd   (32'h00000140,1,4'hf,4'hf,rdata_return);
		 * 	rdata_return[15:8] = 8'h54;				 // reg_mlb_vref_phy_adj_end  
		 *	memwr   (32'h00000140,1,4'hf,4'hf,rdata_return);
		 */
		ret = readl(&ddrintf_phy_group0->mlb_vref_phy_chk_sel);
		temp = (ret
			|(DDR3_PHY_MLB_VREF_PHY_ADJ_END<<PHY_140_MLB_VREF_PHY_ADJ_END)
			);
		
		//debug("[write]CTC DDR PHY : mlb_vref_phy_chk_sel[0x140] = 0x%08x\n",temp);

		writel(temp, &ddrintf_phy_group0->mlb_vref_phy_chk_sel);
		ret = readl(&ddrintf_phy_group0->mlb_vref_phy_chk_sel);
		debug("0x30600140:0x%08x\n",ret);
		
		 /*	memrd   (32'h0000067C,1,4'hf,4'hf,rdata_return);
		 * 	rdata_return[31:24] = 8'h40;				 // reg_mlb_vref_phy_value_manu_db8  
		 *	memwr   (32'h0000067C,1,4'hf,4'hf,rdata_return);
		 */
		ret = readl(&ddrintf_phy_group1->mlb_vref_phy_adj_start);
		temp = (ret
			|(DDR3_PHY_MLB_VREF_PHY_VALUE_MANU_DB8<<PHY_67C_MLB_VREF_PHY_VALUE_MANU_DB8)
			);
		
		//debug("[write]CTC DDR PHY : mlb_vref_phy_adj_start[0x67c] = 0x%08x\n",temp);

		writel(temp, &ddrintf_phy_group1->mlb_vref_phy_adj_start);	 
		ret = readl(&ddrintf_phy_group1->mlb_vref_phy_adj_start);
		debug("0x3060067c:0x%08x\n",ret);
		
		 /*	memrd   (32'h0000067C,1,4'hf,4'hf,rdata_return);
		 *	rdata_return[23:16] = 8'h40;				 // reg_mlb_vref_phy_adj_start  
		 *	memwr   (32'h0000067C,1,4'hf,4'hf,rdata_return);
		 */
		ret = readl(&ddrintf_phy_group1->mlb_vref_phy_adj_start);
		temp = (ret
			|(DDR3_PHY_MLB_VREF_PHY_ADJ_START<<PHY_67C_MLB_VREF_PHY_ADJ_START)
			);
		
		//debug("[write]CTC DDR PHY : mlb_vref_phy_adj_start[0x67c] = 0x%08x\n",temp);

		writel(temp, &ddrintf_phy_group1->mlb_vref_phy_adj_start);	 
		ret = readl(&ddrintf_phy_group1->mlb_vref_phy_adj_start);
		debug("0x3060067c:0x%08x\n",ret);
		
		if(DDRINTF_MANUAL_MODE_TRAIN == regs->ddrintf_training_mode){
			/* manual mode read vref training settining */
			temp = (0x0
				|(regs->ddrintf_rdvref_value_db0<<PHY_680_MLB_VREF_PHY_VALUE_MANU_DB0)
				|(regs->ddrintf_rdvref_value_db1<<PHY_680_MLB_VREF_PHY_VALUE_MANU_DB1)
				|(regs->ddrintf_rdvref_value_db2<<PHY_680_MLB_VREF_PHY_VALUE_MANU_DB2)
				);
			writel(temp, &ddrintf_phy_group1->mlb_vref_phy_manu_db0);	
			ret = readl(&ddrintf_phy_group1->mlb_vref_phy_manu_db0);
			debug("0x30600680:0x%08x\n",ret);
			
		}else{
			/* memwr   (32'h00000680,1,4'hf,4'hf,32'h40404040);*/
			temp = (0x0
				|(DDR3_PHY_MLB_VREF_PHY_VALUE_MANU_DB0<<PHY_680_MLB_VREF_PHY_VALUE_MANU_DB0)
				|(DDR3_PHY_MLB_VREF_PHY_VALUE_MANU_DB1<<PHY_680_MLB_VREF_PHY_VALUE_MANU_DB1)
				|(DDR3_PHY_MLB_VREF_PHY_VALUE_MANU_DB2<<PHY_680_MLB_VREF_PHY_VALUE_MANU_DB2)
				|(DDR3_PHY_MLB_VREF_PHY_VALUE_MANU_DB3<<PHY_680_MLB_VREF_PHY_VALUE_MANU_DB3)
				);
			
			//debug("[write]CTC DDR PHY : mlb_vref_phy_manu_db0[0x680] = 0x%08x\n",temp);
			writel(temp, &ddrintf_phy_group1->mlb_vref_phy_manu_db0);	
			ret = readl(&ddrintf_phy_group1->mlb_vref_phy_manu_db0);
			debug("0x30600680:0x%08x\n",ret);
			
			/* memwr   (32'h00000684,1,4'hf,4'hf,32'h40404040);*/
			temp = (0x0
				|(DDR3_PHY_MLB_VREF_PHY_VALUE_MANU_DB4<<PHY_684_MLB_VREF_PHY_VALUE_MANU_DB4)
				|(DDR3_PHY_MLB_VREF_PHY_VALUE_MANU_DB5<<PHY_684_MLB_VREF_PHY_VALUE_MANU_DB5)
				|(DDR3_PHY_MLB_VREF_PHY_VALUE_MANU_DB6<<PHY_684_MLB_VREF_PHY_VALUE_MANU_DB6)
				|(DDR3_PHY_MLB_VREF_PHY_VALUE_MANU_DB7<<PHY_684_MLB_VREF_PHY_VALUE_MANU_DB7)
				);
			
			//debug("[write]CTC DDR PHY : mlb_vref_phy_manu_db4[0x684] = 0x%08x\n",temp);
			writel(temp, &ddrintf_phy_group1->mlb_vref_phy_manu_db4);
			ret = readl(&ddrintf_phy_group1->mlb_vref_phy_manu_db4);
			debug("0x30600684:0x%08x\n",ret);		
		}
	 	
		 /*memrd   (32'h00000148,1,4'hf,4'hf,rdata_return);
		 * rdata_return[5:0] = 6'h0;				 // reg_mlb_vref_dram_value_start  
		 *memwr   (32'h00000148,1,4'hf,4'hf,rdata_return);
		 */
		ret = readl(&ddrintf_phy_group0->mlb_vref_dram_value);
		temp = (ret
			|(DDR3_PHY_MLB_VREF_DRAM_VALUE_START<<PHY_148_MLB_VREF_DRAM_VALUE_START)
			);
		
		//debug("[write]CTC DDR PHY : mlb_vref_dram_value[0x148] = 0x%08x\n",temp);

		writel(temp, &ddrintf_phy_group0->mlb_vref_dram_value);		
		ret = readl(&ddrintf_phy_group0->mlb_vref_dram_value);
		debug("0x30600148:0x%08x\n",ret);
		
		 /*	memrd   (32'h00000148,1,4'hf,4'hf,rdata_return);
		 * 	rdata_return[13:8] = 6'h0;				 // reg_mlb_vref_dram_value_end  
		 *	memwr   (32'h00000148,1,4'hf,4'hf,rdata_return);
		 */
		ret = readl(&ddrintf_phy_group0->mlb_vref_dram_value);
		temp = (ret
			|(DDR3_PHY_MLB_VREF_DRAM_VALUE_END<<PHY_148_MLB_VREF_DRAM_VALUE_END)
			);
		
		//debug("[write]CTC DDR PHY : mlb_vref_dram_value[0x148] = 0x%08x\n",temp);

		writel(temp, &ddrintf_phy_group0->mlb_vref_dram_value);
		ret = readl(&ddrintf_phy_group0->mlb_vref_dram_value);
		debug("0x30600148:0x%08x\n",ret);
		
		 /*	memrd   (32'h000006F4,1,4'hf,4'hf,rdata_return);
		 * 	rdata_return[5:0] = 6'h0;				 // reg_mlb_vref_dram_value_manu  
		 *	memwr   (32'h000006F4,1,4'hf,4'hf,rdata_return);
		 */
		ret = readl(&ddrintf_phy_group1->mlb_vref_dram_value_manu);
		temp = (ret
			|(DDR3_PHY_MLB_VREF_DRAM_VALUE_MANU<<PHY_6F4_MLB_VREF_DRAM_VALUE_MANU)
			);
		
		//debug("[write]CTC DDR PHY : mlb_vref_dram_value_manu[0x6f4] = 0x%08x\n",temp);

		writel(temp, &ddrintf_phy_group1->mlb_vref_dram_value_manu);
		ret = readl(&ddrintf_phy_group1->mlb_vref_dram_value_manu);
		debug("0x306006f4:0x%08x\n",ret);
		
		 /*	memrd   (32'h000006F4,1,4'hf,4'hf,rdata_return);
		 * 	rdata_return[6] = 1'h0;					 // reg_mlb_vref_dram_range_manu  
		 *	memwr   (32'h000006F4,1,4'hf,4'hf,rdata_return);
		 */
		ret = readl(&ddrintf_phy_group1->mlb_vref_dram_value_manu);
		temp = (ret
			|(DDRINTF_BIT_CLR<<PHY_6F4_MLB_VREF_DRAM_RANGE_MANU)
			);
		
		//debug("[write]CTC DDR PHY : mlb_vref_dram_value_manu[0x6f4] = 0x%08x\n",temp);

		writel(temp, &ddrintf_phy_group1->mlb_vref_dram_value_manu);
		ret = readl(&ddrintf_phy_group1->mlb_vref_dram_value_manu);
		debug("0x306006f4:0x%08x\n",ret);
	
	}

	 /*	memrd   (32'h00000150,1,4'hf,4'hf,rdata_return);
	 * 	rdata_return[15:12] = 4'h0;				 // reg_eye_step_unit  
	 *	memwr   (32'h00000150,1,4'hf,4'hf,rdata_return);
	 */
	ret = readl(&ddrintf_phy_group0->phy_rddv_sel);
	temp = ((ret & (~(0xf << 12)))
		|((PHY_PHY_EYE_STEP_UNIT&PHY_150_PHY_EYE_STEP_UNIT_MASK)<<PHY_150_PHY_EYE_STEP_UNIT)
		);
	
	//debug("[write]CTC DDR PHY : phy_rddv_sel[0x150] = 0x%08x\n",temp);

	writel(temp, &ddrintf_phy_group0->phy_rddv_sel);
	ret = readl(&ddrintf_phy_group0->phy_rddv_sel);
	debug("0x30600150:0x%08x\n",ret);
	
	 /*	memrd   (32'h00000168,1,4'hf,4'hf,rdata_return);
	 * 	rdata_return[15:0] = 16'h8;				 // reg_mlb_r_t_mrd  
	 *	memwr   (32'h00000168,1,4'hf,4'hf,rdata_return);
	 */
	ret = readl(&ddrintf_phy_group0->mlb_mrd);
	temp = ((ret & (~(0xffff)))
		|(PHY_MLB_R_T_MRD<<PHY_168_MLB_R_T_MRD)
		);
	
	//debug("[write]CTC DDR PHY : mlb_mrd[0x168] = 0x%08x\n",temp);

	writel(temp, &ddrintf_phy_group0->mlb_mrd);
	ret = readl(&ddrintf_phy_group0->mlb_mrd);
	debug("0x30600168:0x%08x\n",ret);

	if(DDRINTF_TYPE_4 == regs->ddrintf_type)
	{
		 /*	memrd   (32'h00000168,1,4'hf,4'hf,rdata_return);
		 * 	rdata_return[31:16] = 16'h18;				 // reg_mlb_r_t_mod  
		 *	memwr   (32'h00000168,1,4'hf,4'hf,rdata_return);
		 */
		ret = readl(&ddrintf_phy_group0->mlb_mrd);
		temp = ((ret & (~(0xffff << 16)))
			|(PHY_MLB_R_T_MOD<<PHY_168_MLB_R_T_MOD)
			);
	}
	else	
	{
		/* DDR3 */
		 /*	memrd   (32'h00000168,1,4'hf,4'hf,rdata_return);
		 * 	rdata_return[31:16] = 16'h8;				 // reg_mlb_r_t_mod  
		 *	memwr   (32'h00000168,1,4'hf,4'hf,rdata_return);
		 */
		ret = readl(&ddrintf_phy_group0->mlb_mrd);
		temp = ((ret & (~(0xffff << 16)))
			|(DDR3_PHY_MLB_R_T_MOD<<PHY_168_MLB_R_T_MOD)
			);
	}
	//debug("[write]CTC DDR PHY : mlb_mrd[0x168] = 0x%08x\n",temp);	
	writel(temp, &ddrintf_phy_group0->mlb_mrd);
	ret = readl(&ddrintf_phy_group0->mlb_mrd);
	debug("0x30600168:0x%08x\n",ret);
	
	 /*	memrd   (32'h0000016C,1,4'hf,4'hf,rdata_return);
	 * 	rdata_return[15:0] = 16'h400;				 // reg_mlb_r_t_oit  
	 *	memwr   (32'h0000016C,1,4'hf,4'hf,rdata_return);
	 */
	ret = readl(&ddrintf_phy_group0->mlb_oit);
	temp = ((ret & (~(0xffff)))
		|(PHY_MLB_R_T_OIT<<PHY_16C_MLB_R_T_OIT)
		);
	
	//debug("[write]CTC DDR PHY : mlb_oit[0x16c] = 0x%08x\n",temp);

	writel(temp, &ddrintf_phy_group0->mlb_oit);
	ret = readl(&ddrintf_phy_group0->mlb_oit);
	debug("0x3060016c:0x%08x\n",ret);

	if(DDRINTF_TYPE_4 == regs->ddrintf_type)
	{
		 /*	memrd   (32'h0000016C,1,4'hf,4'hf,rdata_return);
		 * 	rdata_return[31:16] = 16'h150;				 // reg_mlb_r_t_xpr  
		 *	memwr   (32'h0000016C,1,4'hf,4'hf,rdata_return);
		 */
		ret = readl(&ddrintf_phy_group0->mlb_oit);
		temp = ((ret & (~(0xffff << 16)))
			|(PHY_MLB_R_T_XPR<<PHY_16C_MLB_R_T_XPR)
			);	
	}
	else
	{
		 /*	memrd   (32'h0000016C,1,4'hf,4'hf,rdata_return);
		 * 	rdata_return[31:16] = 16'he0;				 // reg_mlb_r_t_xpr  
		 *	memwr   (32'h0000016C,1,4'hf,4'hf,rdata_return);
		 */
		ret = readl(&ddrintf_phy_group0->mlb_oit);
		temp = ((ret & (~(0xffff << 16)))
			|(DDR3_PHY_MLB_R_T_XPR<<PHY_16C_MLB_R_T_XPR)
			);	
	}

	//debug("[write]CTC DDR PHY : mlb_oit[0x16c] = 0x%08x\n",temp);

	writel(temp, &ddrintf_phy_group0->mlb_oit);	 
	ret = readl(&ddrintf_phy_group0->mlb_oit);
	debug("0x3060016c:0x%08x\n",ret);
	
	 /*	memrd   (32'h00000174,1,4'hf,4'hf,rdata_return);
	 * 	rdata_return[31:12] = 20'h13880;				 // reg_mlb_r_rstctl_en2rst  
	 *	memwr   (32'h00000174,1,4'hf,4'hf,rdata_return);
	 */
	ret = readl(&ddrintf_phy_group0->mlb_rstctl_en2rst);
	temp = ((ret&(0xfff))
		|(PHY_MLB_R_RSTCTL_EN2RST<<PHY_174_MLB_R_RSTCTL_EN2RST)
		);
	
	//debug("[write]CTC DDR PHY : mlb_rstctl_en2rst[0x174] = 0x%08x\n",temp);

	writel(temp, &ddrintf_phy_group0->mlb_rstctl_en2rst);	 	
	ret = readl(&ddrintf_phy_group0->mlb_rstctl_en2rst);
	debug("0x30600174:0x%08x\n",ret);
	
	 /*	memrd   (32'h00000178,1,4'hf,4'hf,rdata_return);
	 * 	rdata_return[31:12] = 20'h30D40;				 // reg_mlb_r_rstctl_rst2cke  
	 *	memwr   (32'h00000178,1,4'hf,4'hf,rdata_return);
	 */
	ret = readl(&ddrintf_phy_group0->mlb_dfi_phy_wrlat);
	temp = ((ret&(0xfff))
		|(PHY_MLB_R_RSTCTL_RST2CKE <<PHY_178_MLB_R_RSTCTL_RST2CKE)
		);
	
	//debug("[write]CTC DDR PHY : mlb_dfi_phy_wrlat[0x178] = 0x%08x\n",temp);

	writel(temp, &ddrintf_phy_group0->mlb_dfi_phy_wrlat);	 
	ret = readl(&ddrintf_phy_group0->mlb_dfi_phy_wrlat);
	debug("0x30600178:0x%08x\n",ret);
	
	 /*	memrd   (32'h00000194,1,4'hf,4'hf,rdata_return);
	 * 	rdata_return[1] = 1'h0;					 // reg_cal_disable  
	 *	memwr   (32'h00000194,1,4'hf,4'hf,rdata_return);
	 */
	ret = readl(&ddrintf_phy_group0->mlb_train_start);
	temp = (ret
		|(DDRINTF_BIT_CLR<<PHY_194_CAL_DISABLE)
		);
	
	//debug("[write]CTC DDR PHY : mlb_train_start[0x194] = 0x%08x\n",temp);

	writel(temp, &ddrintf_phy_group0->mlb_train_start);	 
	ret = readl(&ddrintf_phy_group0->mlb_train_start);
	debug("0x30600194:0x%08x\n",ret);
	
	 /*	memrd   (32'h00000194,1,4'hf,4'hf,rdata_return);
	 * 	rdata_return[2] = 1'h0;					 // reg_cal_manual_mode  
	 *	memwr   (32'h00000194,1,4'hf,4'hf,rdata_return);
	 */
	ret = readl(&ddrintf_phy_group0->mlb_train_start);
	temp = (ret
		|(DDRINTF_BIT_CLR<<PHY_194_CAL_MANUAL_MODE)
		);
	
	//debug("[write]CTC DDR PHY : mlb_train_start[0x194] = 0x%08x\n",temp);

	writel(temp, &ddrintf_phy_group0->mlb_train_start);	 
	ret = readl(&ddrintf_phy_group0->mlb_train_start);
	debug("0x30600194:0x%08x\n",ret);
	
	 /*	memrd   (32'h00000194,1,4'hf,4'hf,rdata_return);
	 * 	rdata_return[15] = 1'h0;					 // reg_mlb_train_gate_fast_mode  
	 *	memwr   (32'h00000194,1,4'hf,4'hf,rdata_return);
	 */
	ret = readl(&ddrintf_phy_group0->mlb_train_start);
	temp = (ret
		|(DDRINTF_BIT_CLR<<PHY_194_MLB_TRAIN_GATE_FAST_MODE)
		);
	
	//debug("[write]CTC DDR PHY : mlb_train_start[0x194] = 0x%08x\n",temp);

	writel(temp, &ddrintf_phy_group0->mlb_train_start);	 
	ret = readl(&ddrintf_phy_group0->mlb_train_start);
	debug("0x30600194:0x%08x\n",ret);
	
	 /*	memrd   (32'h00000104,1,4'hf,4'hf,rdata_return);
	 * 	rdata_return[13:12] = 2'h2;				 // reg_pll_lockcnt_mode  
	 *	memwr   (32'h00000104,1,4'hf,4'hf,rdata_return);
	 */
	ret = readl(&ddrintf_phy_group0->phy_ddr_mode);
	temp = ((ret & (~(0x3 << 12)))
		|((PHY_PLL_LOCKCNT_MODE&PHY_104_PLL_LOCKCNT_MODE_MASK)<<PHY_104_PLL_LOCKCNT_MODE)
		);
	
	//debug("[write]CTC DDR PHY : phy_ddr_mode[0x104] = 0x%08x\n",temp);

	writel(temp, &ddrintf_phy_group0->phy_ddr_mode);
	ret = readl(&ddrintf_phy_group0->phy_ddr_mode);
	debug("0x30600104:0x%08x\n",ret);
	
	 /*	memrd   (32'h00000104,1,4'hf,4'hf,rdata_return);
	 * 	rdata_return[25:24] = 2'h0;				 // reg_phy_pll_2nd_lock_mode  
	 *	memwr   (32'h00000104,1,4'hf,4'hf,rdata_return);
	 */
	ret = readl(&ddrintf_phy_group0->phy_ddr_mode);
	temp = (ret
		|(PHY_PLL_2ND_LOCK_MODE<<PHY_104_PHY_PLL_2ND_LOCK_MODE)
		);
	
	//debug("[write]CTC DDR PHY : phy_ddr_mode[0x104] = 0x%08x\n",temp);

	writel(temp, &ddrintf_phy_group0->phy_ddr_mode);	 
	ret = readl(&ddrintf_phy_group0->phy_ddr_mode);
	debug("0x30600104:0x%08x\n",ret);
	
	 /*	memrd   (32'h00000104,1,4'hf,4'hf,rdata_return);
	 * 	rdata_return[27:26] = 2'h3;				 // reg_phy_pllpd2divrstn_mode  
	 *	memwr   (32'h00000104,1,4'hf,4'hf,rdata_return);
	 */
	ret = readl(&ddrintf_phy_group0->phy_ddr_mode);
	temp = (ret
		|(PHY_PLLPD2DIVRSTN_MODE<<PHY_104_PHY_PLLPD2DIVRSTN_MODE)
		);
	
	//debug("[write]CTC DDR PHY : phy_ddr_mode[0x104] = 0x%08x\n",temp);

	writel(temp, &ddrintf_phy_group0->phy_ddr_mode);	 
	ret = readl(&ddrintf_phy_group0->phy_ddr_mode);
	debug("0x30600104:0x%08x\n",ret);
	
	 /*	memrd   (32'h00000150,1,4'hf,4'hf,rdata_return);
	 * 	rdata_return[6] = 1'h0;					 // reg_phy_sim_mode  
	 *	memwr   (32'h00000150,1,4'hf,4'hf,rdata_return);
	 */
	ret = readl(&ddrintf_phy_group0->phy_rddv_sel);
	temp = (ret
		|(DDRINTF_BIT_CLR<<PHY_150_PHY_SIM_MODE)
		);
	
	//debug("[write]CTC DDR PHY : phy_rddv_sel[0x150] = 0x%08x\n",temp);

	writel(temp, &ddrintf_phy_group0->phy_rddv_sel);
	ret = readl(&ddrintf_phy_group0->phy_rddv_sel);
	debug("0x30600150:0x%08x\n",ret);

	return 0;
}


phys_size_t ctc_ddr_ctl_config(ctc5236_ddr_cfg_t *regs)
{
	u32 ret;
	u32 temp;
	u32 refi;
	u32 cl,cwl;
	u32 rtw,ccd_s,wtr_s;
	u32 ccd_l = 0;
	u32 wtr_l = 0;
	u32 faw,rfc,rtp;
	u32 rcd,ras,rp;
	u32 wtp;
	u32 tmrs_wait;
	u32 topbound;

	const unsigned int mclk_ps = ctc_get_ddr_phy_clk_period_ps(regs);
	unsigned int data_rate = ctc_get_ddr_rate(regs);
	
	rfc = ctc_compute_refresh_cycle_time(regs);
	refi = ctc_compute_refresh_limit_time(regs);
	cwl = ctc_compute_cas_write_latency(regs);

	rcd = rp = cl = regs->sdram_cas_latency;

	/* DDR3:rtw = RL+ tCCD + 2tCK - WL
	*		CCD = 4nCK
	*
	*   DDR4:rtw = RL + BL/2 - WL + 2tCK 
	*		BL = 8
	*/
	rtw = cl -cwl+6; 

	/* for faster clock, need more time for data setup */
	rtw += (data_rate/1000000 > 1900) ? 3 : 2;
	
	if(DDRINTF_TYPE_4 == regs->ddrintf_type)
	{
		/*
		* wtr_l = WL + BL/2 + tWTR_L
		* 
		* wtr_s = max{(WL+BL/2+tWTR_S), (wtr_l-ccd_s)}
		*
		* tWTR_S = max(2nCK,2.5ns)	tWTR_L = max(4nCK,7.5ns)
		*
		* CCD_L = 6 for 2133/2400Mbps
      		*	      = 5 for 1600/1866Mbps
		* CCD_S = 4nCK
		*/
		
		ccd_s = 4;

		if (mclk_ps >= 1070)
			ccd_l = 5;
		else
			ccd_l = 6;

		wtr_l = cwl + 4 +max(4U,ctc_picos_to_phyclk(regs,7500));

		wtr_s = max((cwl + 4 + max(2U,ctc_picos_to_phyclk(regs,2500))),(wtr_l - ccd_s));

		/*
		* RAS = 32ns for 2400Mbps
    		*	  = 33ns for 2133Mbps
    		*	  = 34ns for 1866Mbps
		*	  = 35ns for 1600Mbps
		*/
		if (mclk_ps >= 1250)
			ras = ctc_picos_to_phyclk(regs,35000);
		else if (mclk_ps >= 1070)
			ras = ctc_picos_to_phyclk(regs,34000);
		else if (mclk_ps >= 935)
			ras = ctc_picos_to_phyclk(regs,33000);
		else
			ras = ctc_picos_to_phyclk(regs,32000);

		/*
		* tmrs_wait = max(tMRD, tMOD)
		* tMRD = 8nCK, tMOD = max(24nCK, 15ns) 
		*/
		tmrs_wait = max(8U,max(24U,ctc_picos_to_phyclk(regs,15000)));

		/* faw time depends on SDRAM pagesize and DDR rate */
		unsigned int faw_table[3][4] = {
				{13000,15000,17000,20000},
				{21000,21000,23000,25000},
				{30000,30000,30000,35000},};
		faw = faw_table[regs->sdram_page_size][regs->ddrintf_rate];
		
		
	}
	else
	{
		/*
		* wtr = WL + BL/2 + tWTR
		*
		* tWTR = max(4nCK,7.5ns)
		*
		* CCD = 4nCK
		*/
		
		ccd_s = 4;

		wtr_s = cwl + 4 +max(4U,ctc_picos_to_phyclk(regs,7500));

		/*
		* RAS = 35ns for 1600Mbps
    		*	  = 36ns for 1333Mbps
    		*	  = 37.5ns for 1066Mbps
		*	  = 37.5ns for 800Mbps
		*/
		if (mclk_ps >= 2500)
			ras = ctc_picos_to_phyclk(regs,37500);
		else if (mclk_ps >= 1875)
			ras = ctc_picos_to_phyclk(regs,37500);
		else if (mclk_ps >= 1500)
			ras = ctc_picos_to_phyclk(regs,36000);
		else
			ras = ctc_picos_to_phyclk(regs,35000);

		/*
		* tmrs_wait = max(tMRD, tMOD)
		* tMRD = 4nCK, tMOD = max(12nCK, 15ns) 
		*/
		tmrs_wait = max(4U,max(12U,ctc_picos_to_phyclk(regs,15000)));

		/* faw time depends on SDRAM pagesize and DDR rate */
		unsigned int faw_table[2][4] = {
				{30000,30000,37500,40000},
				{40000,45000,50000,50000},};
				
		faw = faw_table[regs->sdram_page_size-1][regs->ddrintf_rate-3];
		
	}

	/* tRTP = max(4nCK,7.5ns) */
	rtp = max(4U,ctc_picos_to_phyclk(regs,7500));

	/* tWTP= WL + BL/2 + tWR  tWR = 15ns*/
	wtp = cwl +4 + ctc_picos_to_phyclk(regs,15000);
	
	switch(regs->system_mem_size)
	{
		case SYSTEM_MEM_256MB:
			topbound = 0x3;
			break;
		case SYSTEM_MEM_512MB:
			topbound = 0x4;
			break;
		case SYSTEM_MEM_1GB:
			topbound = 0x5;
			break;
		case SYSTEM_MEM_2GB:
			topbound = 0x6;
			break;
		case SYSTEM_MEM_4GB:
			topbound = 0x7;
			break;

		case SYSTEM_MEM_8GB:
			topbound = 0x0;
			break;
		default:
			return -1;
	}


	/* mem_wr (32'h00000000, 1, 4'hf, 4'hf 32'h01113431); */
	temp = (0x0
	| (regs->ddrintf_type<< CTL_00_DDR_TYPE)
	| (regs->ddrintf_cs<< CTL_00_RANK_NUM)
	| (CTL_00_DDR_BL_8<< CTL_00_BL)
	| (regs->sdram_col_addr_width << CTL_00_COL_ADDR_WIDTH)
	| (regs->ddrintf_ecc_en<< CTL_00_ECC_ENABLE)
	| (regs->sdram_row_addr_width<< CTL_00_ROW_ADDR_WIDTH)
	| (regs->sdram_bank_addr_width<< CTL_00_BANK_ADDR_WIDTH)
	| (regs->sdram_bank_group_addr_width<< CTL_00_BANK_GROUP_ADDR_WIDTH)
	);
	//debug("[write]CTC DDR CTL : type_cfg[0x00] = 0x%08x\n",temp);
	
	writel(temp, &ddrintf_ctl_group0->type_cfg);	
	ret = readl(&ddrintf_ctl_group0->type_cfg);
	debug("0x30600000:0x%08x\n",ret);

	/* mem_wr (32'h00000004, 1, 4'hf, 4'hf 32'h0008001e) */
	temp = (0x0
	| (DDRINTF_BIT_SET<< CTL_04_FORCE_SDII_NO_GNT)
	| (DDRINTF_BIT_SET<< CTL_04_FORCE_CKE_LOW_SHIFT)
	| (DDRINTF_BIT_SET<< CTL_04_FORCE_RESTN_LOW_SHIFT)
	| (CTL_04_DFI_RATIO_1_2<< CTL_04_DFI_RATIO)
	| (DDRINTF_BIT_SET<< CTL_04_ENABLE_DDRCTRL)	
	);
	//debug("[write]CTC DDR CTL : bist_cfg[0x04] = 0x%08x\n",temp);
			
	writel(temp, &ddrintf_ctl_group0->bist_cfg);
	ret = readl(&ddrintf_ctl_group0->bist_cfg);
	debug("0x30600004:0x%08x\n",ret);

	/* mem_wr (32'h00000008, 1, 4'hf, 4'hf 32'h00000c17);  */
	temp = (0x0
	| ((refi & CTL_08_REFRESH_LIMIT_MASK)<< CTL_08_REFRESH_LIMIT)
	);
	//debug("[write]CTC DDR CTL : refresh_cfg[0x08] = 0x%08x\n",temp);
	
	writel(temp, &ddrintf_ctl_group0->refresh_cfg);
	ret = readl(&ddrintf_ctl_group0->refresh_cfg);
	debug("0x30600008:0x%08x\n",ret);

	/* mem_wr (32'h0000000c, 1, 4'hf, 4'hf 32'h00010a10) */
	temp = (0x0
	| ((cl-2)<< CTL_0C_RDDATA_EN)
	| ((cwl-2)<< CTL_0C_PHY_WRLAT)
	| (CTL_PHY_WRDATA<< CTL_0C_PHY_WRDATA)
	);

	//debug("[write]CTC DDR CTL : rddata_cfg[0x0c] = 0x%08x\n",temp);
			
	writel(temp, &ddrintf_ctl_group0->rddata_cfg);
	ret = readl(&ddrintf_ctl_group0->rddata_cfg);
	debug("0x3060000c:0x%08x\n",ret);	

	/*mem_wr (32'h00000030, 1, 4'hf, 4'hf 32'h00000000);*/
	writel(0x0, &ddrintf_ctl_group0->cg_mode);
	ret = readl(&ddrintf_ctl_group0->cg_mode);
	debug("0x30600030:0x%08x\n",ret);

	/* mem_wr (32'h00000040, 1, 4'hf, 4'hf 32'h6419130b); */
	temp = (0x0
	| (rtw<< CTL_40_RTW)
	| (wtr_s<< CTL_40_WTR_S)
	| (wtr_l<< CTL_40_WTR_L)
	| (ccd_s<< CTL_40_CCD_S)
	| (ccd_l<< CTL_40_CCD_L)
	);
	//debug("[write]CTC DDR CTL : timing_cfg_0[0x40] = 0x%08x\n",temp);
			
	writel(temp, &ddrintf_ctl_group0->timing_cfg_0);
	ret = readl(&ddrintf_ctl_group0->timing_cfg_0);
	debug("0x30600040:0x%08x\n",ret);
	
	/* mem_wr (32'h00000044, 1, 4'hf, 4'hf 32'h0b098424); */
	temp = (0x0
	| (ctc_picos_to_phyclk(regs,faw)<< CTL_44_FAW)
	| ((rfc & CTL_44_RFC_MASK) << CTL_44_RFC)
	| (rtp<< CTL_44_RTP)
	);
	//debug("[write]CTC DDR CTL : timing_cfg_1[0x44] = 0x%08x\n",temp);
	
	writel(temp, &ddrintf_ctl_group0->timing_cfg_1);	
	ret = readl(&ddrintf_ctl_group0->timing_cfg_1);
	debug("0x30600044:0x%08x\n",ret);

	/* mem_wr (32'h00000048, 1, 4'hf, 4'hf 32'h00122712); */
	temp = (0x0
	| (rcd<< CTL_48_RCD)
	| (ras<< CTL_48_RAS)
	| (rp<< CTL_48_RP)
	);
	//debug("[write]CTC DDR CTL : timing_cfg_2[0x48] = 0x%08x\n",temp);
			
	writel(temp, &ddrintf_ctl_group0->timing_cfg_2);
	ret = readl(&ddrintf_ctl_group0->timing_cfg_2);
	debug("0x30600048:0x%08x\n",ret);
	
	
	if(DDRINTF_TYPE_4 == regs->ddrintf_type)
	{		
		/* mem_wr (32'h0000004c, 1, 4'hf, 4'hf 32'h060c0922); */
		temp = (0x0
		| (wtp<< CTL_4C_WTP)
		| (CTL_CKESR<< CTL_4C_CKESR)
		| (CTL_CKSRE<< CTL_4C_CKSRE)
		| (CTL_CCDDRR<< CTL_4C_CCDDRR)
		);
		//debug("[write]CTC DDR CTL : timing_cfg_3[0x4c] = 0x%08x\n",temp);
				
		writel(temp, &ddrintf_ctl_group0->timing_cfg_3);
		ret = readl(&ddrintf_ctl_group0->timing_cfg_3);
		debug("0x3060004c:0x%08x\n",ret);
	
		/* mem_wr (32'h00000050, 1, 4'hf, 4'hf 32'h3000cc0c); */
		temp = (0x0
		| (CTL_CKSRX<< CTL_50_CKSRX)
		| (CTL_XS<< CTL_50_XS)
		| (CTL_XSDLL<< CTL_50_XSDLL)
		);
		//debug("[write]CTC DDR CTL : timing_cfg_4[0x50] = 0x%08x\n",temp);
				
		writel(temp, &ddrintf_ctl_group0->timing_cfg_4);
		ret = readl(&ddrintf_ctl_group0->timing_cfg_4);
		debug("0x30600050:0x%08x\n",ret);
		
		/* mem_wr (32'h00000054, 1, 4'hf, 4'hf 32'h0000001d); */
		temp = (0x0
		| (tmrs_wait<< CTL_54_TMRS_WAIT)
		);
		//debug("[write]CTC DDR CTL : tmrs_cfg[0x54] = 0x%08x\n",temp);
				
		writel(temp, &ddrintf_ctl_group0->tmrs_cfg);		
		ret = readl(&ddrintf_ctl_group0->tmrs_cfg);
		debug("0x30600054:0x%08x\n",ret);
		
		/* mem_wr (32'h0000005c, 1, 4'hf, 4'hf 32'h01440000); */
		temp = (0x0
		| (DDRINTF_BIT_SET<< CTL_5C_2T_MODE)
		| (CTL_ALERT_WAIT<< CTL_5C_ALERT_WAIT)
		);
		//debug("[write]CTC DDR CTL : ctrlupd_cfg[0x5c] = 0x%08x\n",temp);
				
		writel(temp, &ddrintf_ctl_group0->ctrlupd_cfg);
		ret = readl(&ddrintf_ctl_group0->ctrlupd_cfg);
		debug("0x3060005c:0x%08x\n",ret);
		
		/* mem_wr (32'h00000060, 1, 4'hf, 4'hf 32'h00000806); */
		temp = (0x0
		| (CTL_CKE<< CTL_60_CKE)
		| (CTL_XP<< CTL_60_XP)
		);
		//debug("[write]CTC DDR CTL : timing_cfg_5[0x60] = 0x%08x\n",temp);
				
		writel(temp, &ddrintf_ctl_group0->timing_cfg_5);	
		ret = readl(&ddrintf_ctl_group0->timing_cfg_5);
		debug("0x30600060:0x%08x\n",ret);		
		
	}
	else
	{	/* DDR3 */	
		/* mem_wr (32'h0000004c, 1, 4'hf, 4'hf 32'h06080618); */
		temp = (0x0
		| ( wtp<< CTL_4C_WTP)
		| ( DDR3_CTL_CKESR<< CTL_4C_CKESR)
		| ( DDR3_CTL_CKSRE<< CTL_4C_CKSRE)
		| ( DDR3_CTL_CCDDRR<< CTL_4C_CCDDRR)
		);
		//debug("[write]CTC DDR CTL : timing_cfg_3[0x4c] = 0x%08x\n",temp);
				
		writel(temp, &ddrintf_ctl_group0->timing_cfg_3);
		ret = readl(&ddrintf_ctl_group0->timing_cfg_3);
		debug("0x3060004c:0x%08x\n",ret);
	
		/* mem_wr (32'h00000050, 1, 4'hf, 4'hf 32'h20006008); */
		temp = (0x0
		| ( DDR3_CTL_CKSRX<< CTL_50_CKSRX)
		| ( DDR3_CTL_XS<< CTL_50_XS)
		| ( DDR3_CTL_XSDLL<< CTL_50_XSDLL)
		);
		//debug("[write]CTC DDR CTL : timing_cfg_4[0x50] = 0x%08x\n",temp);
				
		writel(temp, &ddrintf_ctl_group0->timing_cfg_4);
		ret = readl(&ddrintf_ctl_group0->timing_cfg_4);
		debug("0x30600050:0x%08x\n",ret);
		
		/* mem_wr (32'h00000054, 1, 4'hf, 4'hf 32'h0000000c); */
		temp = (0x0
		| (tmrs_wait<< CTL_54_TMRS_WAIT)
		);
		//debug("[write]CTC DDR CTL : tmrs_cfg[0x54] = 0x%08x\n",temp);
				
		writel(temp, &ddrintf_ctl_group0->tmrs_cfg);		
		ret = readl(&ddrintf_ctl_group0->tmrs_cfg);
		debug("0x30600054:0x%08x\n",ret);
		
		/* mem_wr (32'h0000005c, 1, 4'hf, 4'hf 32'h00040000); */
		temp = (0x0
		| (DDRINTF_BIT_SET<< CTL_5C_2T_MODE)
		);
		//debug("[write]CTC DDR CTL : ctrlupd_cfg[0x5c] = 0x%08x\n",temp);
				
		writel(temp, &ddrintf_ctl_group0->ctrlupd_cfg);
		ret = readl(&ddrintf_ctl_group0->ctrlupd_cfg);
		debug("0x3060005c:0x%08x\n",ret);
		
		/* mem_wr (32'h00000060, 1, 4'hf, 4'hf 32'h00001405); */
		temp = (0x0
		| (DDR3_CTL_CKE<< CTL_60_CKE)
		| (DDR3_CTL_XP<< CTL_60_XP)
		);
		//debug("[write]CTC DDR CTL : timing_cfg_5[0x60] = 0x%08x\n",temp);
				
		writel(temp, &ddrintf_ctl_group0->timing_cfg_5);	
		ret = readl(&ddrintf_ctl_group0->timing_cfg_5);
		debug("0x30600060:0x%08x\n",ret);
		
	} 

	temp = (0x0
		|((cl-cwl) << CTL_64_PHY_RDN_ODTLAT)
		);
	writel(temp, &ddrintf_ctl_group0->timing_cfg_6);		
	ret = readl(&ddrintf_ctl_group0->timing_cfg_6);
	debug("0x30600064:0x%08x\n",ret);
	
	if(DDRINTF_TYPE_4 == regs->ddrintf_type)
	{
		/* mem_wr (32'h00000068, 1, 4'hf, 4'hf 32'h00000400); */
		temp = (0x0
		| (CTL_ZQCAL_WAIT<< CTL_68_ZQCAL_WAIT)
		);
	}
	else
	{
		/* mem_wr (32'h00000068, 1, 4'hf, 4'hf 32'h00001000); */
		temp = (0x0
		| (DDRINTF_BIT_SET<< CTL_68_ZQCAL_TYPE)
		);
	}
	//debug("[write]CTC DDR CTL : zqcal_cfg[0x68] = 0x%08x\n",temp);
			
	writel(temp, &ddrintf_ctl_group0->zqcal_cfg);
	ret = readl(&ddrintf_ctl_group0->zqcal_cfg);
	debug("0x30600068:0x%08x\n",ret);
	
	/* mem_wr (32'h0000008c, 1, 4'hf, 4'hf 32'h000b7148); */
	temp = (0x0
		| (DDRINTF_BIT_SET<< CTL_8C_RW_GROUPING)
		| (DDRINTF_BIT_SET<< CTL_8C_BURST_GROUPING)	
		| (DDRINTF_BIT_SET<< CTL_8C_BURST_GROUPING_CONT)
		| (topbound<< CTC_8C_TOPBOUND)
		| (WR_GROUP_WAITCNT<< CTC_8C_WR_GROUP_WAITCNT)
		| (DDRINTF_BIT_SET<< CTL_8C_WGRP_MASKAXI)
		| (DDRINTF_BIT_SET<< CTL_8C_NONBF_WRESP)
		| (DDRINTF_BIT_SET<< CTL_8C_PAGE_HIT_CONT)	
		);
	//debug("[write]CTC DDR CTL : app_cfg[0x8c] = 0x%08x\n",temp);
			
	writel(temp, &ddrintf_ctl_group0->app_cfg);
	ret = readl(&ddrintf_ctl_group0->app_cfg);
	debug("0x3060008c:0x%08x\n",ret);
	
	return 0;
}


phys_size_t ctc_ddr_ctl_address_mapping(ctc5236_ddr_cfg_t *regs)
{
	u32 temp;
	u32 dq,colum,row,bank,bg,rank;
	u32 colum_offset,row_offset,bank_offset,bg_offset,rank_offset;
	u32 ret;

	if( DDRINTF_DQ_8 == regs->ddrintf_dq_type || DDRINTF_DQ_8_ECC == regs->ddrintf_dq_type)
	{
		dq = 0;
	}
	else
	{
		dq = 1;
	}

	switch(regs->sdram_col_addr_width)
	{
		case SDRAM_COL_ADDR_WIDTH_10:
			colum = 10;
			break;
		case SDRAM_COL_ADDR_WIDTH_11:
			colum = 11;
			break;
		case SDRAM_COL_ADDR_WIDTH_12:
			colum = 12;
			break;
		case SDRAM_COL_ADDR_WIDTH_9:
			colum = 9;
			break;
		default :
			printf("Error: unsupported colum for mode register\n");
			return 1;
	}

	switch(regs->sdram_row_addr_width)
	{
		case SDRAM_ROW_ADDR_WIDTH_12:
			row = 12;
			break;
		case SDRAM_ROW_ADDR_WIDTH_13:
			row = 13;
			break;
		case SDRAM_ROW_ADDR_WIDTH_14:
			row = 14;
			break;
		case SDRAM_ROW_ADDR_WIDTH_15:
			row = 15;
			break;
		case SDRAM_ROW_ADDR_WIDTH_16:
			row = 16;
			break;
		case SDRAM_ROW_ADDR_WIDTH_17:
			row = 17;
			break;
		case SDRAM_ROW_ADDR_WIDTH_18:
			row = 18;
			break;
		default :
			printf("Error: unsupported row for mode register\n");
			return 1;
			
	}

	switch(regs->sdram_bank_addr_width)
	{
		case SDRAM_BANK_ADDR_WIDTH_3:
			bank = 3;
			break;
		case SDRAM_BANK_ADDR_WIDTH_2:
			bank = 2;
			break;
		default :
			printf("Error: unsupported bank for mode register\n");
			return 1;
	}

	switch(regs->sdram_bank_group_addr_width)
	{
		case SDRAM_BANK_GROUP_ADDR_WIDTH_0:
			bg = 0;
			break;
		case SDRAM_BANK_GROUP_ADDR_WIDTH_1:
			bg = 1;
			break;
		case SDRAM_BANK_GROUP_ADDR_WIDTH_2:
			bg = 2;
			break;
		default :
			printf("Error: unsupported bankgroup for mode register\n");
			return 1;
	}
	if(DDRINTF_CS_1 == regs->ddrintf_cs)
	{
		rank = 0;
	}
	else
	{
		rank = 1;
	}	

	colum_offset = dq;
	row_offset = dq+colum+bank+bg;
	bank_offset = colum+dq;
	bg_offset = dq+colum+bank;
	rank_offset = dq+colum+row+bank+bg;
	
	/* mem_wr (32'h00000090, 1, 4'hf, 4'hf 32'h0c520c41); */
	temp = (0x0
	| (CTL_COL_B0_OFS(colum,colum_offset) << CTL_90_COL_B0_OFS)
	| (CTL_COL_B1_OFS(colum,colum_offset) << CTL_90_COL_B1_OFS)
	| (CTL_COL_B2_OFS(colum,colum_offset) << CTL_90_COL_B2_OFS)
	| (CTL_COL_B3_OFS(colum,colum_offset) << CTL_90_COL_B3_OFS)
	| (CTL_COL_B4_OFS(colum,colum_offset) << CTL_90_COL_B4_OFS)
	| (CTL_COL_B5_OFS(colum,colum_offset) << CTL_90_COL_B5_OFS)
	);
	//debug("[write]CTC DDR CTL : maping_cfg_0[0x90] = 0x%08x\n",temp);
				
	writel(temp, &ddrintf_ctl_group0->maping_cfg_0);
	ret = readl(&ddrintf_ctl_group0->maping_cfg_0);
	debug("0x30600090:0x%08x\n",ret);
	
	/* mem_wr (32'h00000094, 1, 4'hf, 4'hf 32'h3ff52507); */
	temp = (0x0
	| (CTL_COL_B6_OFS(colum,colum_offset) << CTL_94_COL_B6_OFS)
	| (CTL_COL_B7_OFS(colum,colum_offset) << CTL_94_COL_B7_OFS)
	| (CTL_COL_B8_OFS(colum,colum_offset) << CTL_94_COL_B8_OFS)
	| (CTL_COL_B9_OFS(colum,colum_offset) << CTL_94_COL_B9_OFS)
	| (CTL_COL_B10_OFS(colum,colum_offset) << CTL_94_COL_B10_OFS)
	| (CTL_COL_B11_OFS(colum,colum_offset) << CTL_94_COL_B11_OFS)
	); 
	//debug("[write]CTC DDR CTL : maping_cfg_1[0x94] = 0x%08x\n",temp);
			
	writel(temp, &ddrintf_ctl_group0->maping_cfg_1);	
	ret = readl(&ddrintf_ctl_group0->maping_cfg_1);
	debug("0x30600094:0x%08x\n",ret);

	
	/* mem_wr (32'h00000098, 1, 4'hf, 4'hf 32'h0f38d30b); */
	temp = (0x0
	| (CTL_ROE_B0_OFS(row,row_offset) << CTL_98_ROE_B0_OFS)
	| (CTL_ROE_B1_OFS(row,row_offset) << CTL_98_ROE_B1_OFS)
	| (CTL_ROE_B2_OFS(row,row_offset) << CTL_98_ROE_B2_OFS)
	| (CTL_ROE_B3_OFS(row,row_offset) << CTL_98_ROE_B3_OFS)
	| (CTL_ROE_B4_OFS(row,row_offset) << CTL_98_ROE_B4_OFS)
	);
	//debug("[write]CTC DDR CTL : maping_cfg_2[0x98] = 0x%08x\n",temp);
			
	writel(temp, &ddrintf_ctl_group0->maping_cfg_2);	
	ret = readl(&ddrintf_ctl_group0->maping_cfg_2);
	debug("0x30600098:0x%08x\n",ret);
	
	/* mem_wr (32'h0000009c, 1, 4'hf, 4'hf 32'h144d2450); */
	temp = (0x0
	| (CTL_ROE_B5_OFS(row,row_offset) << CTL_9C_ROE_B5_OFS)
	| (CTL_ROE_B6_OFS(row,row_offset) << CTL_9C_ROE_B6_OFS)
	| (CTL_ROE_B7_OFS(row,row_offset) << CTL_9C_ROE_B7_OFS)
	| (CTL_ROE_B8_OFS(row,row_offset) << CTL_9C_ROE_B8_OFS)
	| (CTL_ROE_B9_OFS(row,row_offset) << CTL_9C_ROE_B9_OFS)
	);
	//debug("[write]CTC DDR CTL : maping_cfg_3[0x9c] = 0x%08x\n",temp);
			
	writel(temp, &ddrintf_ctl_group0->maping_cfg_3);	
	ret = readl(&ddrintf_ctl_group0->maping_cfg_3);
	debug("0x3060009c:0x%08x\n",ret);
	
	/* mem_wr (32'h000000a0, 1, 4'hf, 4'hf 32'h19617595); */
	temp = (0x0
	| (CTL_ROE_B10_OFS(row,row_offset) << CTL_A0_ROE_B10_OFS)
	| (CTL_ROE_B11_OFS(row,row_offset) << CTL_A0_ROE_B11_OFS)
	| (CTL_ROE_B12_OFS(row,row_offset) << CTL_A0_ROE_B12_OFS)
	| (CTL_ROE_B13_OFS(row,row_offset) << CTL_A0_ROE_B13_OFS)
	| (CTL_ROE_B14_OFS(row,row_offset) << CTL_A0_ROE_B14_OFS)
	);
	//debug("[write]CTC DDR CTL : maping_cfg_4[0xa0] = 0x%08x\n",temp);
			
	writel(temp, &ddrintf_ctl_group0->maping_cfg_4);	
	ret = readl(&ddrintf_ctl_group0->maping_cfg_4);
	debug("0x306000a0:0x%08x\n",ret);
	
	/* mem_wr (32'h000000a4, 1, 4'hf, 4'hf 32'h0003ffda); */
	temp = (0x0
	| (CTL_ROE_B15_OFS(row,row_offset) << CTL_A4_ROE_B15_OFS)
	| (CTL_ROE_B16_OFS(row,row_offset) << CTL_A4_ROE_B16_OFS)
	| (CTL_ROE_B17_OFS(row,row_offset) << CTL_A4_ROE_B17_OFS)
	);
	//debug("[write]CTC DDR CTL : maping_cfg_5[0xa4] = 0x%08x\n",temp);
			
	writel(temp, &ddrintf_ctl_group0->maping_cfg_5);	
	ret = readl(&ddrintf_ctl_group0->maping_cfg_5);
	debug("0x306000a4:0x%08x\n",ret);

	/* mem_wr (32'h000000a8, 1, 4'hf, 4'hf 32'h0003f71b); */	
	temp = (0x0
	| (CTL_BANK_B0_OFS(bank,bank_offset) << CTL_A8_BANK_B0_OFS)
	| (CTL_BANK_B1_OFS(bank,bank_offset) << CTL_A8_BANK_B1_OFS)
	| (CTL_BANK_B2_OFS(bank,bank_offset) << CTL_A8_BANK_B2_OFS)
	);
	//debug("[write]CTC DDR CTL : maping_cfg_6[0xa8] = 0x%08x\n",temp);
			
	writel(temp, &ddrintf_ctl_group0->maping_cfg_6);	
	ret = readl(&ddrintf_ctl_group0->maping_cfg_6);
	debug("0x306000a8:0x%08x\n",ret);

	/* mem_wr (32'h000000ac, 1, 4'hf, 4'hf 32'h0081f79d); */
	temp = (0x0
	| (CTL_BG_B0_OFS(bg,bg_offset)<< CTL_AC_BG_B0_OFS)
	| (CTL_BG_B1_OFS(bg,bg_offset)<< CTL_AC_BG_B1_OFS)
	| (CTL_RANK_B0_OFS(rank,rank_offset)<< CTL_AC_RANK_B0_OFS)
	| (CTL_RANK_B1_OFS(rank,rank_offset)<< CTL_AC_RANK_B1_OFS)
	);
	//debug("[write]CTC DDR CTL : maping_cfg_7[0xac] = 0x%08x\n",temp);
			
	writel(temp, &ddrintf_ctl_group0->maping_cfg_7);	

	//writel(0x00fff75c, &ddrintf_ctl_group0->maping_cfg_7);
	ret = readl(&ddrintf_ctl_group0->maping_cfg_7);
	debug("0x306000ac:0x%08x\n",ret);
		

	/* mem_wr (32'h000000b0, 1, 4'hf, 4'hf 32'h0000005b); */
	temp = (0x0
	| (DDRINTF_BIT_SET<< CTL_B0_PORT00_ASYNC_MODE)
	| (DDRINTF_BIT_SET<< CTL_B0_PORT00_RD_PAGE_HIT_EN)
	| (DDRINTF_BIT_SET<< CTL_B0_PORT00_RD_TIEMOUT_EN)
	| (DDRINTF_BIT_SET<< CTL_B0_PORT00_WR_PAGE_HIT_EN)
	| (DDRINTF_BIT_SET<< CTL_B0_PORT00_WR_TIMEOUT_EN)
	);
	//debug("[write]CTC DDR CTL : port_0_cfg[0xb0] = 0x%08x\n",temp);
			
	writel(temp, &ddrintf_ctl_group0->port_0_cfg);	
	ret = readl(&ddrintf_ctl_group0->port_0_cfg);
	debug("0x306000b0:0x%08x\n",ret);
		
	/* mem_wr (32'h000000b4, 1, 4'hf, 4'hf 32'h0000005b); */
	temp = (0x0
	| (DDRINTF_BIT_SET<< CTL_B0_PORT00_ASYNC_MODE)
	| (DDRINTF_BIT_SET<< CTL_B0_PORT00_RD_PAGE_HIT_EN)
	| (DDRINTF_BIT_SET<< CTL_B0_PORT00_RD_TIEMOUT_EN)
	| (DDRINTF_BIT_SET<< CTL_B0_PORT00_WR_PAGE_HIT_EN)
	| (DDRINTF_BIT_SET<< CTL_B0_PORT00_WR_TIMEOUT_EN)
	);
	//debug("[write]CTC DDR CTL : port_1_cfg[0xb4] = 0x%08x\n",temp);
			
	writel(temp, &ddrintf_ctl_group0->port_1_cfg);	
	ret = readl(&ddrintf_ctl_group0->port_1_cfg);
	debug("0x306000b4:0x%08x\n",ret);
	
	/* mem_wr (32'h000000b8, 1, 4'hf, 4'hf 32'h0000005b); */
	temp = (0x0
	| (DDRINTF_BIT_SET<< CTL_B0_PORT00_ASYNC_MODE)
	| (DDRINTF_BIT_SET<< CTL_B0_PORT00_RD_PAGE_HIT_EN)
	| (DDRINTF_BIT_SET<< CTL_B0_PORT00_RD_TIEMOUT_EN)
	| (DDRINTF_BIT_SET<< CTL_B0_PORT00_WR_PAGE_HIT_EN)
	| (DDRINTF_BIT_SET<< CTL_B0_PORT00_WR_TIMEOUT_EN)
	);
	//debug("[write]CTC DDR CTL : port_2_cfg[0xb8] = 0x%08x\n",temp);
			
	writel(temp, &ddrintf_ctl_group0->port_2_cfg);	
	ret = readl(&ddrintf_ctl_group0->port_2_cfg);
	debug("0x306000b8:0x%08x\n",ret);


	/* mem_wr (32'h000000bc, 1, 4'hf, 4'hf 32'h0000005b); */
	temp = (0x0
	| (DDRINTF_BIT_SET<< CTL_B0_PORT00_ASYNC_MODE)
	| (DDRINTF_BIT_SET<< CTL_B0_PORT00_RD_PAGE_HIT_EN)
	| (DDRINTF_BIT_SET<< CTL_B0_PORT00_RD_TIEMOUT_EN)
	| (DDRINTF_BIT_SET<< CTL_B0_PORT00_WR_PAGE_HIT_EN)
	| (DDRINTF_BIT_SET<< CTL_B0_PORT00_WR_TIMEOUT_EN)
	);
	//debug("[write]CTC DDR CTL : port_3_cfg[0xbc] = 0x%08x\n",temp);
			
	writel(temp, &ddrintf_ctl_group0->port_3_cfg);
	ret = readl(&ddrintf_ctl_group0->port_3_cfg);
	debug("0x306000bc:0x%08x\n",ret);
	
	if(DDRINTF_TYPE_4 == regs->ddrintf_type)
	{
		/* memrd	(32'h00000704,1,4'hf,4'hf,rdata_return);
		*  memwr	(32'h00000704,1,4'hf,4'hf,(rdata_return[31:0] | 32'h1000000));
		*/
		temp = readl(&ddrintf_ctl_group1->bist_mode);
		temp = (temp
		| (DDRINTF_BIT_SET<< CTL_704_TST_ECC_PART)
		);
		//debug("[write]CTC DDR CTL : bist_mode[0x704] = 0x%08x\n",temp);
				
		writel(temp, &ddrintf_ctl_group1->bist_mode);	
		ret = readl(&ddrintf_ctl_group1->bist_mode);
		debug("0x30600704:0x%08x\n",ret);
	}

	/* enable DDR interrupt */
	writel(0x1000f, &ddrintf_ctl_group0->intr_enable);

	return 0;
}

phys_size_t ctc_ddr_ctl_sdram_mode_set(ctc5236_ddr_cfg_t *regs)
{
	u32 ret;
	u32 temp;

	/* MR3*/
	/* memwr (32'h00000058 , 1, 4'hf, 4'hf, 32'h80000032); */
	temp =(0x0
	|(DDRINTF_BIT_SET<< CTL_58_MRS_LAUNCH)
	|(CTL_58_MRS_NUM_3<< CTL_58_MRS_NUM)
	|(ctc_set_Mode_Register3(regs)<< CTL_58_MRS_OPCODE)
	|(DDRINTF_BIT_SET<< CTL_58_MRS_ALL_RANK)
	);
	//debug("[write]CTC DDR CTL : mrs_cfg[0x58 mr3] = 0x%08x\n",temp);
			
	writel(temp, &ddrintf_ctl_group0->mrs_cfg);
	ret = readl(&ddrintf_ctl_group0->mrs_cfg);
	debug("0x30600058_MR3:0x%08x\n",ret);
	
	/* wait for MRW complete */
	ret = wait_for_bit(__func__, &ddrintf_ctl_group0->mrs_cfg,
	 			0x2, false, 100, false);
	if (ret)
	{
		puts("MR3 launch timeout!\n");
	}
	
	if(DDRINTF_TYPE_4 == regs->ddrintf_type)
	{
		/* Frist MR6 setting */
		temp =(0x0
		|(DDRINTF_BIT_SET<< CTL_58_MRS_LAUNCH)	
		|(CTL_58_MRS_NUM_6<< CTL_58_MRS_NUM)
		|((ctc_set_Mode_Register6(regs)|(BIT(7)))<< CTL_58_MRS_OPCODE)
		|(DDRINTF_BIT_SET<< CTL_58_MRS_ALL_RANK)
		);
			
		writel(temp, &ddrintf_ctl_group0->mrs_cfg);
		ret = readl(&ddrintf_ctl_group0->mrs_cfg);
		debug("0x30600058_MR6:0x%08x\n",ret);
		
		/* wait for MRW complete */
		ret = wait_for_bit(__func__, &ddrintf_ctl_group0->mrs_cfg,
		 			0x2, false, 100, false);
		if (ret)
		{
			puts("Frist MR6 launch timeout!\n");
		}

		/* Second MR6 setting */
		temp =(0x0
		|(DDRINTF_BIT_SET<< CTL_58_MRS_LAUNCH)	
		|(CTL_58_MRS_NUM_6<< CTL_58_MRS_NUM)
		|(ctc_set_Mode_Register6(regs)<< CTL_58_MRS_OPCODE)
		|(DDRINTF_BIT_SET<< CTL_58_MRS_ALL_RANK)
		);
			
		writel(temp, &ddrintf_ctl_group0->mrs_cfg);
		ret = readl(&ddrintf_ctl_group0->mrs_cfg);
		debug("0x30600058_MR6:0x%08x\n",ret);
		
		/* wait for MRW complete */
		ret = wait_for_bit(__func__, &ddrintf_ctl_group0->mrs_cfg,
		 			0x2, false, 100, false);
		if (ret)
		{
			puts("Second MR6 launch timeout!\n");
		}

		/* MR5*/		
		/* memwr  (32'h00000058 , 1, 4'hf, 4'hf, 32'h804a0052); */
		temp =(0x0
		|(DDRINTF_BIT_SET<< CTL_58_MRS_LAUNCH)	
		|(CTL_58_MRS_NUM_5<< CTL_58_MRS_NUM)
		|(ctc_set_Mode_Register5(regs)<< CTL_58_MRS_OPCODE)
		|(DDRINTF_BIT_SET<< CTL_58_MRS_ALL_RANK)
		);
		//debug("[write]CTC DDR CTL : mrs_cfg[0x58 mr5] = 0x%08x\n",temp);
				
		writel(temp, &ddrintf_ctl_group0->mrs_cfg);
		ret = readl(&ddrintf_ctl_group0->mrs_cfg);
		debug("0x30600058_MR5:0x%08x\n",ret);
		
		/* wait for MRW complete */
		ret = wait_for_bit(__func__, &ddrintf_ctl_group0->mrs_cfg,
		 			0x2, false, 100, false);
		if (ret)
		{
			puts("MR launch timeout!\n");
		}

		/* MR4*/
		/* memwr  (32'h00000058 , 1, 4'hf, 4'hf, 32'h80000042); */
		temp =(0x0
		|(DDRINTF_BIT_SET<< CTL_58_MRS_LAUNCH)	
		|(CTL_58_MRS_NUM_4<< CTL_58_MRS_NUM)
		|(ctc_set_Mode_Register4(regs)<< CTL_58_MRS_OPCODE)
		|(DDRINTF_BIT_SET<< CTL_58_MRS_ALL_RANK)
		);
		//debug("[write]CTC DDR CTL : mrs_cfg[0x58 mr4] = 0x%08x\n",temp);
				
		writel(temp, &ddrintf_ctl_group0->mrs_cfg);
		ret = readl(&ddrintf_ctl_group0->mrs_cfg);
		debug("0x30600058_MR4:0x%08x\n",ret);
		
		/* wait for MRW complete */
		ret = wait_for_bit(__func__, &ddrintf_ctl_group0->mrs_cfg,
		 			0x2, false, 100, false);
		if (ret)
		{
			puts("MR4 launch timeout!\n");
		}

	}

	/* MR2*/
	/* memwr  (32'h00000058 , 1, 4'hf, 4'hf, 32'h80218022); */
	temp =(0x0
	|(DDRINTF_BIT_SET<< CTL_58_MRS_LAUNCH)	
	|(CTL_58_MRS_NUM_2<< CTL_58_MRS_NUM)
	|(ctc_set_Mode_Register2(regs)<< CTL_58_MRS_OPCODE)
	|(DDRINTF_BIT_SET<< CTL_58_MRS_ALL_RANK)
	);
	//debug("[write]CTC DDR CTL : mrs_cfg[0x58 mr2] = 0x%08x\n",temp);
			
	writel(temp, &ddrintf_ctl_group0->mrs_cfg);
	ret = readl(&ddrintf_ctl_group0->mrs_cfg);
	debug("0x30600058_MR2:0x%08x\n",ret);
	
	/* wait for MRW complete */
	ret = readl(&ddrintf_ctl_group0->mrs_cfg);
	ret = wait_for_bit(__func__, &ddrintf_ctl_group0->mrs_cfg,
	 			0x2, false, 100, false);
	if (ret)
	{
		puts("MR2 launch timeout!\n");
	}		

	/* MR1*/
	/* memwr  (32'h00000058 , 1, 4'hf, 4'hf, 32'h80201012); */
	temp =(0x0
	|(DDRINTF_BIT_SET<< CTL_58_MRS_LAUNCH)	
	|(CTL_58_MRS_NUM_1<< CTL_58_MRS_NUM)
	|(ctc_set_Mode_Register1(regs)<< CTL_58_MRS_OPCODE)
	|(DDRINTF_BIT_SET<< CTL_58_MRS_ALL_RANK)
	);
	//debug("[write]CTC DDR CTL : mrs_cfg[0x58 mr1] = 0x%08x\n",temp);
			
	writel(temp, &ddrintf_ctl_group0->mrs_cfg);	
	ret = readl(&ddrintf_ctl_group0->mrs_cfg);
	debug("0x30600058_MR1:0x%08x\n",ret);
	
	/* wait for MRW complete */
	ret = wait_for_bit(__func__, &ddrintf_ctl_group0->mrs_cfg,
	 			0x2, false, 100, false);
	if (ret)
	{
		puts("MR1 launch timeout!\n");
	}

	/* MR0*/
	/* memwr  (32'h00000058 , 1, 4'hf, 4'hf, 32'h80b40002); */
	temp =(0x0
	|(DDRINTF_BIT_SET<< CTL_58_MRS_LAUNCH)	
	|(CTL_58_MRS_NUM_0<< CTL_58_MRS_NUM)
	|(ctc_set_Mode_Register0(regs)<< CTL_58_MRS_OPCODE)
	|(DDRINTF_BIT_SET<< CTL_58_MRS_ALL_RANK)
	);
	//debug("[write]CTC DDR CTL : mrs_cfg[0x58 mr0] = 0x%08x\n",temp);
			
	writel(temp, &ddrintf_ctl_group0->mrs_cfg);
	ret = readl(&ddrintf_ctl_group0->mrs_cfg);
	debug("0x30600058_MR0:0x%08x\n",ret);
	
	/* wait for MRW complete */
	ret = wait_for_bit(__func__, &ddrintf_ctl_group0->mrs_cfg,
	 			0x2, false, 100, false);
	if (ret)
	{
		puts("MR0 launch timeout!\n");
	}

	return 0;
}


phys_size_t ctc_ddr_ctl_zqcal_init(ctc5236_ddr_cfg_t *regs)
{
	u32 temp;
	u32 ret;

	if(DDRINTF_TYPE_4 == regs->ddrintf_type)
	{
		/*  memwr  (32'h00000068 , 1, 4'hf, 4'hf, 32'h00003400); */
		temp = (0x0
		| (CTL_ZQCAL_WAIT<< CTL_68_ZQCAL_WAIT)
		| (DDRINTF_BIT_SET<< CTL_68_ZQCAL_TYPE)
		| (DDRINTF_BIT_SET<< CTL_68_ZQCAL_LAUNCH)
		);
	}
	else
	{
		/* DDR3 */
		/*  memwr  (32'h00000068 , 1, 4'hf, 4'hf, 32'h00003200); */
		temp = (0x0
		| (DDR3_CTL_ZQCAL_WAIT<< CTL_68_ZQCAL_WAIT)
		| (DDRINTF_BIT_SET<< CTL_68_ZQCAL_TYPE)
		| (DDRINTF_BIT_SET<< CTL_68_ZQCAL_LAUNCH)
		);
	}
	//debug("[write]CTC DDR CTL : zqcal_cfg[0x68] = 0x%08x\n",temp);
			
	writel(temp, &ddrintf_ctl_group0->zqcal_cfg);
	ret = readl(&ddrintf_ctl_group0->zqcal_cfg);
	debug("0x30600068:0x%08x\n",ret);
	
	/* wait for ZQCAL init complete */
	ret = wait_for_bit(__func__, &ddrintf_ctl_group0->zqcal_cfg,
				0x2000, false, 1, false);
	if (ret)
	{
		puts("ZQCAL launch timeout!\n");
	}	
	
	return 0;
}

phys_size_t ctc_do_bist(ctc5236_ddr_cfg_t *regs,unsigned long long start,unsigned int cnt)
{
	unsigned int temp,ret;

	unsigned int cloum,row,bank,bg,rank;

	if(SDRAM_COL_ADDR_WIDTH_9 ==regs->sdram_col_addr_width)
		cloum = 9;
	else
		cloum = regs->sdram_col_addr_width + 10;
	
	row = regs->sdram_row_addr_width + 12;
	
	if(regs->sdram_bank_addr_width)
		bank = 2;
	else
		bank = 3;
	
	bg = regs->sdram_bank_group_addr_width;
	
	if(DDRINTF_CS_4 == regs->ddrintf_cs)
		rank = 3;
	else
		rank = regs->ddrintf_cs;
	
	if(DDRINTF_ECC_ENABLE == regs->ddrintf_ecc_en)
	{
		/* Disable ECC */
		clrbits_le32(&ddrintf_ctl_group0->type_cfg, CTL_00_ECC_ENABLE_BIT);
	}
	/* Enable BIST */
	setbits_le32(&ddrintf_ctl_group0->bist_cfg, CTL_04_ENABLE_BIST_BIT);

	/* BIST setting */
	writel(cnt, &ddrintf_ctl_group1->bist_sdii_cnt);
	
	temp = (0x0
		|(((start>>(1+cloum))&((1<<bank)-1))<< CTL_704_BIST_SDII_START_BANK)
		|(((start>>(1+cloum+bank))&((1<<bg)-1)) << CTL_704_BIST_SDII_START_BG)
		|(((start>>(1+cloum+row+bank+bg))&((1<<rank)-1)) << CTL_704_BIST_SDII_START_RANK)
		|(regs->ddrintf_ecc_en << CTL_704_TST_ECC_PART)
		|(DDRINTF_BIT_SET << CTL_704_BIST_PAT)
		);
	
	writel(temp, &ddrintf_ctl_group1->bist_mode);
	
	temp = (0x0
		|(((start>>1) & ((1 << cloum)-1))<< CTL_708_BIST_SDII_START_COL)
		|(((start >> (1+cloum+bank+bg)) & ((1 << row)-1))<< CTL_708_BIST_SDII_START_ROW)
		);
	writel(temp, &ddrintf_ctl_group1->bist_sdii_start);

	/* BIST pattern */
	writel(DDR_BIST_PATTERN, &ddrintf_ctl_group1->r1_lfsr_ini);
	writel(DDR_BIST_PATTERN, &ddrintf_ctl_group1->r2_lfsr_ini);
	writel(DDR_BIST_PATTERN, &ddrintf_ctl_group1->r3_lfsr_ini);
	writel(DDR_BIST_PATTERN, &ddrintf_ctl_group1->r4_lfsr_ini);
	
	/* BIST start */
	temp = (0x0
	| (DDRINTF_BIT_SET<< CTL_71C_BIST_START_SHIFT)
	);
	debug("[write]CTC DDR CTL : bist_start = 0x%08x\n",temp);
	writel(temp, &ddrintf_ctl_group1->bist_start);

	/* wait for BIST done */
	ret = wait_for_bit(__func__, &ddrintf_ctl_group1->bist_start,
				0x1, false, 10000, false);
	if (ret)
	{
		puts("BIST test timeout!\n");
	}

	ret = readl( &ddrintf_ctl_group1->bist_start);
	if((ret & 0x10) != 0)
	{
		debug("BIST test PASS !\n");
	}
	else
	{
		printf("ERROR:BIST test fail !\n");
		ctc_print_ddr_debug_info(regs);
	}

	/* exist BIST mode */
	if(DDRINTF_ECC_ENABLE == regs->ddrintf_ecc_en)
	{
		setbits_le32(&ddrintf_ctl_group0->type_cfg, CTL_00_ECC_ENABLE_BIT);
	}
	clrbits_le32(&ddrintf_ctl_group0->bist_cfg, CTL_04_ENABLE_BIST_BIT);
	
	return 0;
}

#ifdef CONFIG_RESERVE_MEMORY
phys_size_t ctc_reserved_memory_relocate(ctc5236_ddr_cfg_t *regs,void *addr,unsigned int size)
{
	unsigned int i,n;
	unsigned long long buff[CONFIG_RESERVE_MEMORY_MIN];
	unsigned long long * reload_addr = (unsigned long long *)CONFIG_SYS_SDRAM_BASE;

	/* copy reserved memory to 0x80000000 */
	for(i = 0; i < (size/sizeof(buff)); i++)
	{
		if(DDRINTF_ECC_ENABLE == regs->ddrintf_ecc_en){
		/* disable ecc */
		clrbits_le32(&ddrintf_ctl_group0->type_cfg, CTL_00_ECC_ENABLE_BIT);
		}
		
		for(n = 0 ; n < CONFIG_RESERVE_MEMORY_MIN; n ++)
		{
			buff[n]=readq(addr);
			addr += 8;
		}

		if(DDRINTF_ECC_ENABLE == regs->ddrintf_ecc_en){
		/* enable ecc */
		setbits_le32(&ddrintf_ctl_group0->type_cfg, CTL_00_ECC_ENABLE_BIT);
		}
		
		for(n = 0 ; n < CONFIG_RESERVE_MEMORY_MIN; n ++)
		{
			writeq(buff[n],reload_addr);
			reload_addr ++;
		}
		udelay(1);
	}
	
	return 0;
}

#endif

phys_size_t ctc_sdram_init(ctc5236_ddr_cfg_t *regs)
{

	unsigned	int cnt;
	unsigned long long start_addr,mem_size;

	switch(regs->system_mem_size)
	{
		case SYSTEM_MEM_512MB:
			cnt = DDR_BIST_CNT_512MB;
			mem_size = DDR_MEMORY_SIZE_512MB;
			break;
		case SYSTEM_MEM_1GB:
			cnt = DDR_BIST_CNT_1GB;
			mem_size = DDR_MEMORY_SIZE_1GB;
			break;
		case SYSTEM_MEM_2GB:
			cnt = DDR_BIST_CNT_2GB;
			mem_size = DDR_MEMORY_SIZE_2GB;
			break;
		case SYSTEM_MEM_4GB:
			cnt = DDR_BIST_CNT_4GB;
			mem_size = DDR_MEMORY_SIZE_4GB;
			break;
		case SYSTEM_MEM_8GB:
			cnt = DDR_BIST_CNT_8GB;
			mem_size = DDR_MEMORY_SIZE_8GB;
			break;
		default:
			printf("ERROR:SYSTEM MEMORY ERROR!\n");
			return -1;
	}

	/* first time to do init memory */
	start_addr = 0x0;
	ctc_do_bist(regs,start_addr,cnt);


#ifdef CONFIG_RESERVE_MEMORY

	void * reserve_addr = (void *)CONFIG_RESERVE_MEMORY_START;

	debug("reserve_addr is %#llx\n",(unsigned long long)reserve_addr);

	/* save reserved_memory data */
	ctc_reserved_memory_relocate(regs,reserve_addr,CONFIG_RESERVE_MEMORY_SIZE);

#endif

	start_addr = mem_size / 2;

	debug("second start addr is:0x%llx\n",start_addr);

	/* second time to init memory  */
	ctc_do_bist(regs,start_addr,cnt);

#ifdef CONFIG_RESERVE_MEMORY
	/* write back */
	unsigned int  i;
	unsigned long long ret;
	unsigned long long  * reload_addr = (unsigned long long *)CONFIG_SYS_SDRAM_BASE;
	
	for(i = 0;i < (CONFIG_RESERVE_MEMORY_SIZE/8); i++)
	{
		ret=readq(reload_addr);
		reload_addr ++;
		
		writeq(ret,reserve_addr);	
		reserve_addr += 8;
	}

#endif

	return 0;

}

phys_size_t ctc_ddr_phy_init(ctc5236_ddr_cfg_t *regs)
{
	ctc_ddr_phy_general_set(regs);

	ctc_ddr_phy_pll_set(regs);

	ctc_ddr_phy_trianing_set(regs);

	ctc_ddr_phy_sdram_mode_set(regs);

	ctc_ddr_phy_timing_set(regs);

	ctc_ddr_phy_trian_ctl(regs);

	ctc_ddr_phy_write_leveling_ctl(regs);

	ctc_ddr_phy_func(regs);
	
	return 0;
	
}

phys_size_t ctc_ddr_init(ctc5236_ddr_cfg_t *regs)
{
	u32 ret;
	u32 temp;
	u32 ddr_rate;

	/* assert all reset singal */
	writel(0x7, &sysctl_base->SysDdrResetCtl); 

	ddr_rate = regs->ddrintf_rate;

	/* ddr interface clock setting */
	ctc_ddrclk_set(ddr_rate);

	/* wait clock stable */
	udelay(100);
	
	/* de-assert ddrCfgHReset /ddrIntfAReset */
	clrbits_le32(&sysctl_base->SysDdrResetCtl, SYS_DDR_RESET_CTL_W0_CFG_DDR_CFG_RESET);
	clrbits_le32(&sysctl_base->SysDdrResetCtl, SYS_DDR_RESET_CTL_W0_CFG_DDR_AXI_RESET);
	ret = readl(&sysctl_base->SysDdrResetCtl);
	debug("DDRreset:0x%08x\n",ret);
	
	ctc_ddr_phy_init(regs);

	ctc_ddr_ctl_config(regs);

	ctc_ddr_ctl_address_mapping(regs);

	/* mem_wr (32'h00000004, 1, 4'hf, 4'hf 32'h0008001e) */
	temp = (0x0
	| (DDRINTF_BIT_SET<< CTL_04_FORCE_SDII_NO_GNT)
	| (DDRINTF_BIT_SET<< CTL_04_FORCE_CKE_LOW_SHIFT)
	| (DDRINTF_BIT_SET<< CTL_04_FORCE_RESTN_LOW_SHIFT)
	| (CTL_04_DFI_RATIO_1_2<< CTL_04_DFI_RATIO)
	| (DDRINTF_BIT_SET<< CTL_04_ENABLE_DDRCTRL)	
	);
	//debug("[write]CTC DDR CTL : bist_cfg[0x04] = 0x%08x\n",temp);
			
	writel(temp, &ddrintf_ctl_group0->bist_cfg);	
	ret = readl(&ddrintf_ctl_group0->bist_cfg);
	debug("0x30600004:0x%08x\n",ret);
	
	/* de-assert ddrMcResetBar */
	clrbits_le32(&sysctl_base->SysDdrResetCtl, SYS_DDR_RESET_CTL_W0_CFG_DDR_MC_RESET);

	/* release FORCE_RESETN_LOW */
	/*memrd	(32'h00000004,1,4'hf,4'hf,rdata_return);
	 *memwr	(32'h00000004,1,4'hf,4'hf,(rdata_return[31:0] & ~32'h8));
	*/
	clrbits_le32(&ddrintf_ctl_group0->bist_cfg, CTL_04_FORCE_RESTN_LOW);
	ret = readl(&ddrintf_ctl_group0->bist_cfg);
	debug("0x30600004:0x%08x\n",ret);
	/* wait 0.5 ms*/
	mdelay(1);
	
	/* release FORCE_CKE_LOW  */
	/*memrd	(32'h00000004,1,4'hf,4'hf,rdata_return);
	 *memwr	(32'h00000004,1,4'hf,4'hf,(rdata_return[31:0] & ~32'h4));
	*/
	clrbits_le32(&ddrintf_ctl_group0->bist_cfg, CTL_04_FORCE_CKE_LOW);
	ret = readl(&ddrintf_ctl_group0->bist_cfg);


	/* wait for dfi init complete */
	/*rdata_return = 0;
	 *while (rdata_return[31] != 1'b1) begin	//init_done, mrs_done
	 *memrd	(32'h00000004,1,4'hf,4'hf,rdata_return);
	 *end
	 */
	
	ret = wait_for_bit(__func__, &ddrintf_ctl_group0->bist_cfg,
	 			0x80000000, true, 1000, false);
	if (ret)
	{
		puts("DDR DFI init timeout!\n");
	
	}
	ret = readl(&ddrintf_ctl_group0->bist_cfg);
	debug("init done:0x%08x\n",ret);

	/* enable odt */
	setbits_le32( &ddrintf_phy_group0->phy_wrdata_delay, BIT(23));
	
	if(DDRINTF_ECC_DISABLE == regs->ddrintf_ecc_en)
	{
		/* data exchange mode disable  */
		clrbits_le32(&ddrintf_phy_group0->phy_ddr_mode, BIT(6));
	}

	/* FORCE_RESETN_LOW */
	/*memrd	(32'h00000004,1,4'hf,4'hf,rdata_return);
	 *memwr	(32'h00000004,1,4'hf,4'hf,(rdata_return[31:0] | 32'h8));
	*/
	setbits_le32(&ddrintf_ctl_group0->bist_cfg, CTL_04_FORCE_RESTN_LOW);
	ret = readl(&ddrintf_ctl_group0->bist_cfg);
	debug("0x30600004:0x%08x\n",ret);
	/* wait 0.5 ms*/
	mdelay(1);
	
	/*FORCE_CKE_LOW  */
	/*memrd	(32'h00000004,1,4'hf,4'hf,rdata_return);
	 *memwr	(32'h00000004,1,4'hf,4'hf,(rdata_return[31:0] |32'h4));
	*/
	setbits_le32(&ddrintf_ctl_group0->bist_cfg, CTL_04_FORCE_CKE_LOW);
	ret = readl(&ddrintf_ctl_group0->bist_cfg);
	debug("0x30600004:0x%08x\n",ret);
	mdelay(1);

	/* release FORCE_RESETN_LOW */
	/*memrd	(32'h00000004,1,4'hf,4'hf,rdata_return);
	 *memwr	(32'h00000004,1,4'hf,4'hf,(rdata_return[31:0] & ~32'h8));
	*/
	clrbits_le32(&ddrintf_ctl_group0->bist_cfg, CTL_04_FORCE_RESTN_LOW);
	ret = readl(&ddrintf_ctl_group0->bist_cfg);
	debug("0x30600004:0x%08x\n",ret);
	/* wait 0.5 ms*/
	mdelay(1);
	
	/* release FORCE_CKE_LOW  */
	/*memrd	(32'h00000004,1,4'hf,4'hf,rdata_return);
	 *memwr	(32'h00000004,1,4'hf,4'hf,(rdata_return[31:0] & ~32'h4));
	*/
	clrbits_le32(&ddrintf_ctl_group0->bist_cfg, CTL_04_FORCE_CKE_LOW);
	ret = readl(&ddrintf_ctl_group0->bist_cfg);
	debug("0x30600004:0x%08x\n",ret);
	ctc_ddr_ctl_sdram_mode_set(regs);

	ctc_ddr_ctl_zqcal_init(regs);

	/* release SDII grant capability */
	ret = readl(&ddrintf_ctl_group0->bist_cfg);
	
	temp =(ret
		| (DDRINTF_BIT_CLR<< CTL_04_FORCE_SDII_NO_GNT)
		);
	
	//debug("[write]CTC DDR CTL : bist_cfg = 0x%08x\n",temp);
	
	writel(0x00080011, &ddrintf_ctl_group0->bist_cfg);
	ret = readl(&ddrintf_ctl_group0->bist_cfg);
	debug("0x30600004:0x%08x\n",ret);		

	ctc_sdram_init(regs);

	return 0;
}

