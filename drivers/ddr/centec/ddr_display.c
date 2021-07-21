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

#include <div64.h>

DECLARE_GLOBAL_DATA_PTR;

#define ULL_2E12 2000000000000ULL

void print_ddr_info(void)
{
	unsigned int status;

	status = readl(&(ddrintf_ctl_group0->type_cfg));

	puts(" (DDR");
	switch (status & CTL_00_DDR_TYPE_MASK) 
	{
		case DDRINTF_TYPE_3:
			puts("3");
			break;
		case DDRINTF_TYPE_4:
			puts("4");
			break;
		default:
			puts("?");
			break;
	}
	/* TsingMa data bus width is 16-bit*/
	puts(", 16-bit");
	
	switch((status >> 4) & CTL_00_RANK_NUM_MASK)
	{
		case DDRINTF_CS_1:
			puts(", CS=1");
			break;
		case DDRINTF_CS_2:
			puts(", CS=2");
			break;
		case DDRINTF_CS_4:
			puts(", CS=4");
			break;
		default:
			puts(", CS error");
			break;		
	}



	if (status & CTL_00_ECC_ENABLE_BIT)
		puts(", ECC on)");
	else
		puts(", ECC off)");


}


void __weak detail_board_ddr_info(void)
{
	print_ddr_info();
}

void board_add_ram_info(int use_default)
{
	detail_board_ddr_info();
}



unsigned int  ctc_compute_ddr_clock_cycle_time(void)
{
	unsigned int freq_ddr,result;
	unsigned int ddr_freq_status0 = readl(&sysctl_base->SysPllDdrCfg0);
	unsigned int ddr_freq_status1 = readl(&sysctl_base->SysPllDdrCfg1);
	
	if((ddr_freq_status0 == DDRINTF_MC_CLOCK_CONFIG0_200)
		&& (ddr_freq_status1 == DDRINTF_MC_CLOCK_CONFIG1_2))
		freq_ddr = 800000000;

	if((ddr_freq_status0 == DDRINTF_MC_CLOCK_CONFIG0_266)
		&& (ddr_freq_status1 == DDRINTF_MC_CLOCK_CONFIG1_1))
		freq_ddr = 1066000000;

	if((ddr_freq_status0 == DDRINTF_MC_CLOCK_CONFIG0_333)
		&& (ddr_freq_status1 == DDRINTF_MC_CLOCK_CONFIG1_1))
		freq_ddr = 1333000000;

	if((ddr_freq_status0 == DDRINTF_MC_CLOCK_CONFIG0_400)
		&& (ddr_freq_status1 == DDRINTF_MC_CLOCK_CONFIG1_0))
		freq_ddr = 1600000000;

	if((ddr_freq_status0 == DDRINTF_MC_CLOCK_CONFIG0_466)
		&& (ddr_freq_status1 == DDRINTF_MC_CLOCK_CONFIG1_2))
		freq_ddr = 1866000000;
	
	if((ddr_freq_status0 == DDRINTF_MC_CLOCK_CONFIG0_533)
		&& (ddr_freq_status1 == DDRINTF_MC_CLOCK_CONFIG1_1))
		freq_ddr = 2133000000;

	if((ddr_freq_status0 == DDRINTF_MC_CLOCK_CONFIG0_600)
		&& (ddr_freq_status1 == DDRINTF_MC_CLOCK_CONFIG1_0))
		freq_ddr = 2400000000;

	/* Round to nearest 10ps, being careful about 64-bit multiply/divide */
	unsigned long long rem, mclk_ps = ULL_2E12;

	/* Now perform the big divide, the result fits in 32-bits */
	rem = do_div(mclk_ps, freq_ddr);
	result = (rem >= (freq_ddr >> 1)) ? mclk_ps + 1 : mclk_ps;

	return result;
}


unsigned int ctc_print_ddr_cycle_time(void)
{
	unsigned int mclk_ps;
	
	mclk_ps = ctc_compute_ddr_clock_cycle_time();
	printf("\nDDR cycle time\ntCK : %d ps\n",mclk_ps);

	return 0;
}

unsigned int ctc_print_cas_latency_time(unsigned int ddr_type)
{
	unsigned int temp,caslat,caslat_ps;
	unsigned int mclk_ps = ctc_compute_ddr_clock_cycle_time();
	unsigned int mr0 = (readl(&ddrintf_phy_group0->mlb_mrs1_data) >> 16);

	temp = (0x0
		|(((mr0 >> 4) & 0x7) << 1)
		|((mr0 >> 2) & 0x1)
		);

	if(DDRINTF_TYPE_3 == ddr_type)
	{
		unsigned int caslat_table[]={0,12,5,13,6,14,7,15,8,16,9,0,10,0,11};
		
		if(temp < 1 || temp == 11 || temp == 13 || temp > 14)
		{
			printf("Error:unkown cas latency!\n");
			return -1;
		}

		caslat = caslat_table[temp];
		caslat_ps = caslat * mclk_ps;

		printf("\nCAS Latency Time\ntCL : %dps(%dnCk)\n",caslat_ps,caslat);
	}
	else if(DDRINTF_TYPE_4 == ddr_type)
	{
		unsigned int caslat_table[]={9,10,11,12,13,14,15,16,18,20,22,24};
		
		if(temp < 0 || temp > 11)
		{
			printf("Error:unkown cas latency!\n");
			return -1;
		}

		caslat = caslat_table[temp];
		caslat_ps = caslat * mclk_ps;

		printf("\nCAS Latency Time\ntCL : %dps(%dnCk)\n",caslat_ps,caslat);
	}
	else
	{
		printf("Error:unkown ddr type!\n");
		return -1;
	}

	return 0;
}

unsigned int ctc_print_trcd(void)
{
	unsigned int mclk_ps = ctc_compute_ddr_clock_cycle_time();

	unsigned int trcd,trcd_ps;

	trcd = (readl(&ddrintf_ctl_group0->timing_cfg_2) & CTL_48_RCD_MASK);

	trcd_ps = trcd * mclk_ps;

	printf("\nRAS to CAS Delay Time\ntRCD : %dps(%dnCK)\n",trcd_ps,trcd);

	return 0;
}

unsigned int ctc_print_trp(void)
{
	unsigned int mclk_ps = ctc_compute_ddr_clock_cycle_time();

	unsigned int trp,trp_ps;

	trp = ((readl(&ddrintf_ctl_group0->timing_cfg_2) >> 16) & CTL_48_RP_MASK);

	trp_ps = trp * mclk_ps;

	printf("\nRow Precharge Delay Time\ntRP : %dps(%dnCK)\n",trp_ps,trp);

	return 0;
}

unsigned int ctc_print_tras(void)
{
	unsigned int mclk_ps = ctc_compute_ddr_clock_cycle_time();

	unsigned int tras,tras_ps;

	tras = ((readl(&ddrintf_ctl_group0->timing_cfg_2) >> 8) & CTL_48_RAS_MASK);

	tras_ps = tras * mclk_ps;

	printf("\nActive to Precharge Delay Time\ntRAS : %dps(%dnCK)\n",tras_ps,tras);

	return 0;
}

unsigned int ctc_print_trfc(void)
{
	unsigned int mclk_ps = ctc_compute_ddr_clock_cycle_time();

	unsigned int trfc,trfc_ps;

	trfc = ((readl(&ddrintf_ctl_group0->timing_cfg_1) >> 8) & CTL_44_RFC_MASK);

	trfc_ps = trfc * mclk_ps;

	printf("\nRefresh Recovery Delay Time\ntRFC : %dps(%dnCK)\n",trfc_ps,trfc);

	return 0;
}

unsigned int ctc_print_tfaw(void)
{
	unsigned int mclk_ps = ctc_compute_ddr_clock_cycle_time();

	unsigned int tfaw,tfaw_ps;

	tfaw = (readl(&ddrintf_ctl_group0->timing_cfg_1)  & CTL_44_FAW_MASK);

	tfaw_ps = tfaw * mclk_ps;

	printf("\nFour Activate Window Delay Time\ntFAW : %dps(%dnCK)\n",tfaw_ps,tfaw);

	return 0;
}

unsigned int ctc_print_trrd(unsigned int ddr_type)
{
	unsigned int mclk_ps = ctc_compute_ddr_clock_cycle_time();


	if(DDRINTF_TYPE_3 == ddr_type)
	{
		unsigned int trrd,trrd_ps;
		trrd = (readl(&ddrintf_phy_group0->mlb_rrd) & PHY_17C_MLB_R_T_RRD_MASK);

		trrd_ps = trrd * mclk_ps;
		printf("\nActivate to Activate Delay Time\ntRRD : %dps(%dnCK)\n",trrd_ps,trrd);
	}
	else if(DDRINTF_TYPE_4 == ddr_type)
	{
		unsigned int trrd_s,trrds_ps;
		trrd_s = (readl(&ddrintf_phy_group0->mlb_rrd) & PHY_17C_MLB_R_T_RRD_MASK);

		trrds_ps = trrd_s * mclk_ps;
		printf("\nActivate to Activate Delay Time , Different Bank Group\ntRRD_S : %dps(%dnCK)\n",trrds_ps,trrd_s);
	}
	else
	{
		printf("Error:unkown ddr type!\n");
		return -1;		
	}

	return 0;
}


unsigned int ctc_print_tccd(unsigned int ddr_type)
{
	unsigned int mclk_ps = ctc_compute_ddr_clock_cycle_time();


	if(DDRINTF_TYPE_3 == ddr_type)
	{
		unsigned int tccd,tccd_ps;
		tccd = ((readl(&ddrintf_ctl_group0->timing_cfg_0) >> 24) & CTL_40_CCD_MASK);

		tccd_ps = tccd * mclk_ps;
		printf("\nCAS to CAS Delay Time \ntCCD : %dps(%dnCK)\n",tccd_ps,tccd);
	}
	else if(DDRINTF_TYPE_4 == ddr_type)
	{
		unsigned int tccd_s,tccd_l,tccds_ps,tccdl_ps;
		tccd_s = ((readl(&ddrintf_ctl_group0->timing_cfg_0) >> 24) & CTL_40_CCD_S_MASK);
		tccd_l = ((readl(&ddrintf_ctl_group0->timing_cfg_0) >> 28) & CTL_40_CCD_L_MASK);

		tccds_ps = tccd_s * mclk_ps;
		tccdl_ps = tccd_l * mclk_ps;

		printf("\nCAS to CAS Delay Time for Same Bank Group \ntCCD_L : %dps(%dnCK)\n",tccdl_ps,tccd_l);		
		printf("\nCAS to CAS Delay Time for Different Bank Group \ntCCD_S : %dps(%dnCK)\n",tccds_ps,tccd_s);

	}
	else
	{
		printf("Error:unkown ddr type!\n");
		return -1;		
	}

	return 0;
}

unsigned int ctc_print_twr(unsigned int ddr_type)
{
	unsigned int temp,wr,wr_ps;
	unsigned int mclk_ps = ctc_compute_ddr_clock_cycle_time();
	unsigned int mr0 = (readl(&ddrintf_phy_group0->mlb_mrs1_data) >> 16);

	temp = ((mr0 >> 9) & 0x7);

	if(DDRINTF_TYPE_3 == ddr_type)
	{
		unsigned int wr_table[]={16,5,6,7,8,10,12,14};
		
		if(temp < 0 || temp > 7)
		{
			printf("Error:unkown Write Recovery!\n");
			return -1;
		}

		wr = wr_table[temp];
		wr_ps = wr * mclk_ps;

		printf("\nWrite Recovery Time \ntWR : %dps(%dnCk )\n",wr_ps,wr);
	}
	else if(DDRINTF_TYPE_4 == ddr_type)
	{
		unsigned int wr_table[]={10,12,14,16,18,20,24};
		
		if(temp < 0 || temp > 6)
		{
			printf("Error:unkown Write Recovery!\n");
			return -1;
		}

		wr = wr_table[temp];
		wr_ps = wr * mclk_ps;

		printf("\nWrite Recovery Time \ntWR : %dps(%dnCk )\n",wr_ps,wr);
	}
	else
	{
		printf("Error:unkown ddr type!\n");
		return -1;
	}

	return 0;
}

unsigned int ctc_print_twtr(unsigned int ddr_type)
{
	unsigned int mclk_ps = ctc_compute_ddr_clock_cycle_time();
	
	if(DDRINTF_TYPE_3 == ddr_type)
	{
		unsigned int twtr,twtr_ps;
		twtr = ((readl(&ddrintf_ctl_group0->timing_cfg_0) >> 8) & CTL_40_WTR_S_MASK);

		twtr_ps = twtr * mclk_ps;
		printf("\nWrite command to Read command spacing \nWTR : %dps(%dnCK)\n",twtr_ps,twtr);
	}
	else if(DDRINTF_TYPE_4 == ddr_type)
	{
		unsigned int twtr_s,twtr_l,twtrs_ps,twtrl_ps;
		twtr_s = ((readl(&ddrintf_ctl_group0->timing_cfg_0) >> 8) & CTL_40_WTR_S_MASK);
		twtr_l = ((readl(&ddrintf_ctl_group0->timing_cfg_0) >> 16) & CTL_40_WTR_L_MASK);

		twtrs_ps = twtr_s * mclk_ps;
		twtrl_ps = twtr_l * mclk_ps;

		printf("\nWrite command to Read command spacing, Same Bank Group \nWTR_L : %dps(%dnCK)\n",twtrl_ps,twtr_l);		
		printf("\nWrite command to Read command spacing, Different Bank Group \nWTR_S : %dps(%dnCK)\n",twtrs_ps,twtr_s);

	}
	else
	{
		printf("Error:unkown ddr type!\n");
		return -1;		
	}


	return 0;
}


unsigned int ctc_dispaly_odt(unsigned int ddr_type)
{
	unsigned int mr1 = readl(&ddrintf_phy_group0->mlb_mrs1_data);
	unsigned int mr2 = readl(&ddrintf_phy_group0->mlb_mrs2_data);
	unsigned int mr5 = readl(&ddrintf_phy_group0->mlb_mrs4_data);
	unsigned int rtt_nom,rtt_wr;

	printf("\nDDR ODT:\n");
	
	if(DDRINTF_TYPE_3 == ddr_type)
	{
		rtt_nom = (0x0
			|(((mr1 >> 9) & 0x1) << 2)
			|(((mr1 >> 6) & 0x1) << 1)
			|((mr1 >> 2) & 0x1)
			);
		rtt_wr =((mr2 >> 9) & 0x3);

		switch(rtt_nom)
		{
			case 0:
				printf("RTT_NOM : disable \n");
				break;
			case 1:
				printf("RTT_NOM : 60ohm \n");
				break;
			case 2:
				printf("RTT_NOM : 120ohm \n");
				break;
			case 3:
				printf("RTT_NOM : 40ohm \n");
				break;				
			case 4:
				printf("RTT_NOM : 20ohm \n");
				break;
			case 5:
				printf("RTT_NOM : 30ohm \n");
				break;
			default:
				printf("Error: unknown RTT_NOM \n");
		}

		switch(rtt_wr)
		{
			case 0:
				printf("Dynamic ODT Off \n");
				break;
			case 1:
				printf("RTT_WR : 60ohm \n");
				break;
			case 2:
				printf("RTT_WR : 120ohm \n");
				break;
			default:
				printf("Error: unknown RTT_WR \n");
		}

	}
	else if(DDRINTF_TYPE_4 == ddr_type)
	{
		unsigned int rtt_park;
		
		rtt_nom = ((mr1 >> 8) & 0x7);
		rtt_wr =((mr2 >> 9) & 0x3);
		rtt_park = ((mr5 >> 22) & 0x7);

		switch(rtt_nom)
		{
			case 0:
				printf("RTT_NOM : disable \n");
				break;
			case 1:
				printf("RTT_NOM : 60ohm \n");
				break;
			case 2:
				printf("RTT_NOM : 120ohm \n");
				break;
			case 3:
				printf("RTT_NOM : 40ohm \n");
				break;				
			case 4:
				printf("RTT_NOM : 240ohm \n");
				break;
			case 5:
				printf("RTT_NOM : 48ohm \n");
				break;
			case 6:
				printf("RTT_NOM : 80ohm \n");
				break;
			case 7:
				printf("RTT_NOM : 34ohm \n");
				break;
			default:
				printf("Error: unknown RTT_NOM \n");
		}
		switch(rtt_wr)
		{
			case 0:
				printf("Dynamic ODT Off \n");
				break;
			case 1:
				printf("RTT_WR : 120ohm \n");
				break;
			case 2:
				printf("RTT_WR : 240ohm \n");
				break;
			default:
				printf("Error: unknown RTT_WR \n");
		}

		switch(rtt_park)
		{
			case 0:
				printf("RTT_PARK Disable \n");
				break;
			case 1:
				printf("RTT_PARK : 60ohm \n");
				break;
			case 2:
				printf("RTT_PARK : 120ohm \n");
				break;
			case 3:
				printf("RTT_PARK : 40ohm \n");
				break;				
			case 4:
				printf("RTT_PARK : 240ohm \n");
				break;
			case 5:
				printf("RTT_PARK : 48ohm \n");
				break;
			case 6:
				printf("RTT_PARK : 80ohm \n");
				break;
			case 7:
				printf("RTT_PARK : 34ohm \n");
				break;
			default:
				printf("Error: unknown RTT_PARK \n");
		}
		
	}
	else
	{
		printf("Error:unkown ddr type!\n");
		return -1;
	}
	return 0;
}

unsigned int ctc_dispaly_mr(unsigned int ddr_type)
{
	unsigned int status,mr0,mr1,mr2,mr3;
	
	status = readl(&ddrintf_phy_group0->mlb_mrs1_data);
	mr0 = ((status >> PHY_158_MLB_MRS0_DATA) & PHY_158_MLB_MRS0_DATA_MASK);
	mr1 = ((status >> PHY_158_MLB_MRS1_DATA) & PHY_158_MLB_MRS1_DATA_MASK);

	status = readl(&ddrintf_phy_group0->mlb_mrs2_data);
	mr2 = ((status >> PHY_154_MLB_MRS2_DATA) & PHY_154_MLB_MRS2_DATA_MASK);
	mr3 = ((status >> PHY_154_MLB_MRS3_DATA) & PHY_154_MLB_MRS3_DATA_MASK);
	
	printf("\nMode Register:\n");
	
	if(DDRINTF_TYPE_3 == ddr_type)
	{
		printf("MR0: 0x%04x\n",mr0);
		printf("MR1: 0x%04x\n",mr1);
		printf("MR2: 0x%04x\n",mr2);
		printf("MR3: 0x%04x\n",mr3);
	}
	else if(DDRINTF_TYPE_4 == ddr_type)
	{
		unsigned int mr4,mr5,mr6;

		status = readl(&ddrintf_phy_group0->mlb_mrs4_data);
		mr4 = ((status >> PHY_15C_MLB_MRS4_DATA) & PHY_15C_MLB_MRS4_DATA_MASK);
		mr5 = ((status >> PHY_15C_MLB_MRS5_DATA) & PHY_15C_MLB_MRS5_DATA_MASK);
		
		mr6 = (readl(&ddrintf_phy_group0->mlb_mrs6_data) & PHY_160_MLB_MRS6_DATA_MASK);
		
		printf("MR0: 0x%04x\n",mr0);
		printf("MR1: 0x%04x\n",mr1);
		printf("MR2: 0x%04x\n",mr2);
		printf("MR3: 0x%04x\n",mr3);
		printf("MR4: 0x%04x\n",mr4);
		printf("MR5: 0x%04x\n",mr5);
		printf("MR6: 0x%04x\n",mr6);
	}
	else
	{
		printf("Error:unkown ddr type!\n");
		return -1;	
	}
	return 0;
}

unsigned int ctc_print_ddr_addr_info(void)
{
	unsigned int temp;
	unsigned int column = 0;	/* SDRAM column num */
	unsigned int row = 0;		/* SDRAM row num */
	unsigned int bank = 0;		/* SDRAM bank num */
	unsigned int bg = 0;		/* SDRAM bankgroup num,for DDR3 bg = 0 */
	unsigned int rank = 0;		/* chip select num */
	unsigned int status = readl(&ddrintf_ctl_group0->type_cfg);

	printf("\nDDR Address Info:\n");

	temp = ((status >> CTL_00_RANK_NUM) & CTL_00_RANK_NUM_MASK);
	if(temp >=0 && temp <= 3)
		rank = temp + 1;
	else
		printf("error:unkown rank num!\n");

	temp = ((status >> CTL_00_COL_ADDR_WIDTH) & CTL_00_COL_ADDR_WIDTH_MASK);
	if (SDRAM_COL_ADDR_WIDTH_9 == temp)
		column = 9;
	else if(temp >= 0 && temp < 3)
		column = temp + 10;
	else
		printf("error:unkown column!\n");

	temp = ((status >> CTL_00_ROW_ADDR_WIDTH) & CTL_00_ROW_ADDR_WIDTH_MASK);

	if(temp <=6 && temp >=0)
		row = temp + 12;
	else
		printf("error:unkown row!\n");

	temp = ((status >> CTL_00_BANK_ADDR_WIDTH) & CTL_00_BANK_ADDR_WIDTH_MASK);

	if(temp)
		bank = 2;
	else
		bank = 3;

	temp = ((status >> CTL_00_BANK_GROUP_ADDR_WIDTH) & CTL_00_BANK_GROUP_ADDR_WIDTH_MASK);

	if(0 == temp)
		bg = 0;
	else if(1 == temp)
		bg = 2;
	else if(2 == temp)
		bg = 4;
	else
		printf("error:unkown bank group!\n");

	printf("column	: %d\n",column);
	printf("row	: %d\n",row);
	printf("bank	: %d\n",bank);
	printf("bank group : %d\n",bg);
	printf("chip select: %d\n",rank);
	
	return 0;
}

unsigned int ctc_print_ddr_training_result_info(ctc5236_ddr_cfg_t *regs)
{
	unsigned int train_mode,train_status;
	unsigned int result0,result1;
	unsigned int db0_tune,db1_tune;
	unsigned int temp;

	const unsigned int mclk_ps = (ctc_get_ddr_phy_clk_period_ps(regs)+1)/2;

	printf("----------------training result information-----------------\n");
	printf("\n");

	/* training type and training status */	
	train_mode = readl(&ddrintf_phy_group0->mlb_train_start);
	train_status = readl(&ddrintf_phy_ro_group->ro_phy_status);

	printf("training status value:\n0x30600400:0x%08x\n\n",train_status);

	/* write leveling result */
	printf("----------------write leveling------------------------------\n");
	if(0x0 == (train_mode & BIT(7))){
		/* auto write leveling result */
		printf("write leveling mode : auto\n");
		
		/* auto training result in reg 0x434 & 0x438 */
		result0 = readl(&ddrintf_phy_ro_group->ro_wrlvl_coarse_result);
		printf("coarse result(0x434) : 0x%08x\n",result0);
		
		result1 = readl(&ddrintf_phy_ro_group->ro_wrlvl_fine_result);
		printf("fine   result(0x438) : 0x%08x\n",result1);

		db0_tune = ((result0>> 16) & 0x3) * mclk_ps + (result1 & 0xff)*mclk_ps/256;
		db1_tune = ((result0>> 18) & 0x3) * mclk_ps + ((result1 >> 8) & 0xff)*mclk_ps/256;
		printf("db0_delay: %dps   db1_delay: %dps",db0_tune,db1_tune);

		/* For TsingMa SoC,db2 is ecc */
		if(DDRINTF_ECC_ENABLE== regs->ddrintf_ecc_en){
			unsigned int db2_tune;
			db2_tune = ((result0>> 20) & 0x3) * mclk_ps + ((result1 >> 16) & 0xff)*mclk_ps/256;
			printf("  db2_delay: %dps\n",db2_tune);
		}else{
			printf("\n");
		}
	}else{
		/* manual write leveling result */
		printf("write leveling mode : manual\n");

		/* manual mode training setting in reg 0x1bc & 0x1c0 */
		result0 = readl(&ddrintf_phy_group0->mlb_db0_bt_wlvl);
		printf("coarse delay setting(0x1bc) : 0x%08x\n",result0);
		
		result1 = readl(&ddrintf_phy_group0->mlb_db0_dll_wlvl_wdqs);
		printf("fine   delay setting(0x1c0) : 0x%08x\n",result1);

		db0_tune = (result0 & 0x3) * mclk_ps + (result1 & 0xff)*mclk_ps/256;
		db1_tune = ((result0>> 2) & 0x3) * mclk_ps + ((result1 >> 8) & 0xff)*mclk_ps/256;
		printf("db0_delay: %dps   db1_delay: %dps",db0_tune,db1_tune);

		/* For TsingMa SoC,db2 is ecc */
		if(DDRINTF_ECC_ENABLE== regs->ddrintf_ecc_en){
			unsigned int db2_tune;
			db2_tune = ((result0>> 4) & 0x3) * mclk_ps + ((result1 >> 16) & 0xff)*mclk_ps/256;
			printf("  db2_delay: %dps\n",db2_tune);
		}else{
			printf("\n");
		}
	}

	printf("\n");

	/* gate training result */
	printf("----------------gate training-------------------------------\n");
	if(0x0 == (train_mode & BIT(8))){
		/* auto gate training result */
		printf("gate training mode : auto \n");
		
		result0 = readl(&ddrintf_phy_ro_group->ro_gate_train_result_db01);
		printf("db01 result(0x440): 0x%08x\n",result0);
		
		temp = readl(&ddrintf_phy_ro_group->ro_gate_train_pass_window_db01);
		printf("db01 pass window(0x450): 0x%08x\n",temp);
		
		if(DDRINTF_ECC_ENABLE == regs->ddrintf_ecc_en){
			result1 = readl(&ddrintf_phy_ro_group->ro_gate_train_result_db23);
			printf("db2 result(0x444): 0x%08x\n",result1);
			
			temp = readl(&ddrintf_phy_ro_group->ro_gate_train_pass_window_db23);
			printf("db2 pass window(0x450): 0x%08x\n",temp);
		}

		db0_tune = ((result0 >> 8) & 0xff) * mclk_ps + (result0 & 0xff) * mclk_ps/256;
		db1_tune = ((result0 >> 24) & 0xff) * mclk_ps + ((result0 >> 16) & 0xff) * mclk_ps/256;
		printf("db0: %dps   db1: %dps",db0_tune,db1_tune);

		if(DDRINTF_ECC_ENABLE == regs->ddrintf_ecc_en){
			unsigned int db2_tune;
			db2_tune = ((result1 >> 8) & 0xff) * mclk_ps + (result1 & 0xff) * mclk_ps/256;
			printf("  db2: %dps\n",db2_tune);

		}else{
			printf("\n");
		}

	}else{
		/* manual gate training result */
		printf("gate training mode : manual \n");

		result0 = readl(&ddrintf_phy_group0->db0_dll_gate_trip);
		printf("coarse setting: 0x%08x\n",result0);
		
		result1 = readl(&ddrintf_phy_group0->db0_dll_gate_fine);
		printf("fine   setting: 0x%08x\n",result1);

		db0_tune = (result0 & 0xff) * mclk_ps + (result1 & 0xff) * mclk_ps/256;
		db1_tune = ((result0 >> 8) & 0xff) * mclk_ps + ((result1 >> 8) & 0xff) * mclk_ps/256;
		printf("db0: %dps   db1: %dps",db0_tune,db1_tune);
		
		if(DDRINTF_ECC_ENABLE == regs->ddrintf_ecc_en){
			unsigned int db2_tune;
			db2_tune = ((result0 >> 16) & 0xff) * mclk_ps + ((result1 >> 16) & 0xff) * mclk_ps/256;
			printf("  db2: %dps\n",db2_tune);
		}else{
			printf("\n");
		}

	}

	printf("\n");

	/* read eye training result */
	printf("----------------read eye training---------------------------\n");
	if(0 == (train_mode & BIT(10))){
		printf("read eye training mode: auto\n");

		/* rising edge result */
		result0 = readl(&ddrintf_phy_ro_group->ro_rdeye_train_rise_result);
		printf("rising edge result(0x460): 0x%08x\n",result0);
		
		result1 = readl(&ddrintf_phy_ro_group->ro_rdeye_train_rise_pass_window);
		printf("rising edge pass window(0x468): 0x%08x\n",result1);

		db0_tune = (result0 & 0xff) * mclk_ps / 256;
		db1_tune = ((result0 >> 8) & 0xff) * mclk_ps / 256;
		printf("db0_rising: %dps   db1_rising: %dps",db0_tune,db1_tune);

		if(DDRINTF_ECC_ENABLE == regs->ddrintf_ecc_en){
			unsigned int db2_tune;
			db2_tune = ((result0 >> 16) & 0xff) * mclk_ps/256;
			printf("  db2_rising: %dps\n",db2_tune);

		}else{
			printf("\n");
		}
		
		printf("\n");
		
		/* falling edge result */
		result0 = readl(&ddrintf_phy_ro_group->ro_rdeye_train_fall_result);
		printf("falling edge result(0x4c0): 0x%08x\n",result0);
		
		result1 = readl(&ddrintf_phy_ro_group->ro_rdeye_train_fall_pass_window);
		printf("falling edge pass window(0x4c8): 0x%08x\n",result1);

		db0_tune = (result0 & 0xff) * mclk_ps/256;
		db1_tune = ((result0 >> 8) & 0xff) * mclk_ps/256;
		printf("db0_falling: %dps   db1_falling: %dps",db0_tune,db1_tune);

		if(DDRINTF_ECC_ENABLE == regs->ddrintf_ecc_en){
			unsigned int db2_tune;
			db2_tune = ((result0 >> 16) & 0xff) * mclk_ps/256;
			printf("  db2_falling: %dps\n",db2_tune);

		}else{
			printf("\n");
		}

	}else{
		printf("read eye training mode: manual\n");

		result0 = readl(&ddrintf_phy_group1->db0_dll_rddeye_dqs);
		printf("dqs delay setting(0x688): 0x%08x\n",result0);
		
		result1 = readl(&ddrintf_phy_group1->db0_dll_rddeye_dqsn);
		printf("dqsn delay setting(0x690): 0x%08x\n",result1);

		/* dqs delay */
		db0_tune = (result0 & 0xff) * mclk_ps/256;
		db1_tune = ((result0 >> 8) & 0xff) * mclk_ps/256;
		printf("db0_dqs: %dps   db1_dqs: %dps",db0_tune,db1_tune);

		if(DDRINTF_ECC_ENABLE == regs->ddrintf_ecc_en){
			unsigned int db2_tune;
			db2_tune = ((result0 >> 16) & 0xff) * mclk_ps/256;
			printf("  db2_dqs: %dps\n",db2_tune);

		}else{
			printf("\n");
		}
		
		printf("\n");
		
		/* dqsn delay */
		db0_tune = (result1 & 0xff) * mclk_ps/256;
		db1_tune = ((result1 >> 8) & 0xff) * mclk_ps/256;
		printf("db0_dqsn: %dps   db1_dqsn: %dps",db0_tune,db1_tune);

		if(DDRINTF_ECC_ENABLE == regs->ddrintf_ecc_en){
			unsigned int db2_tune;
			db2_tune = ((result1 >> 16) & 0xff) * mclk_ps/256;
			printf("  db2_dqsn: %dps\n",db2_tune);

		}else{
			printf("\n");
		}
	}
	
	printf("\n");
	
	/* read vref training result */
	printf("----------------read vref training--------------------------\n");

	if(0 == (train_mode & BIT(11))){
		printf("read vref training mode: auto\n");
		
		result0 = readl(&ddrintf_phy_ro_group->ro_rd_vref_result);
		printf("training result(0x488): 0x%08x\n",result0);
		
		db0_tune = result0 & 0xff;
		db1_tune = (result0 >> 8) & 0xff;
		printf("db0: 0x%x  db1: 0x%x",db0_tune,db1_tune);

		if(DDRINTF_ECC_ENABLE == regs->ddrintf_ecc_en){
			unsigned int db2_tune;
			
			db2_tune = (result0 >> 16) & 0xff;
			printf("  db2: 0x%x\n",db2_tune);
			
		}else{
			printf("\n");
		}
		
	}else{
		printf("read vref training mode: manual\n");
		
		result0 = readl(&ddrintf_phy_group1->mlb_vref_phy_manu_db0);
		printf("manual setting(0x680): 0x%08x\n",result0);

		db0_tune = result0 & 0xff;
		db1_tune = (result0 >> 8) & 0xff;
		printf("db0: 0x%x  db1: 0x%x",db0_tune,db1_tune);

		if(DDRINTF_ECC_ENABLE == regs->ddrintf_ecc_en){
			unsigned int db2_tune;
			
			db2_tune = (result0 >> 16) & 0xff;
			printf("  db2: 0x%x\n",db2_tune);
			
		}else{
			printf("\n");
		}
	}

	printf("\n");
	
	/* write eye training result */
	printf("----------------write eye training--------------------------\n");
	if(0 == (train_mode & BIT(13))){
		/* auto write eye training */
		printf("write eye training mode: auto\n\n");

		result0 = readl(&ddrintf_phy_ro_group->ro_weye_train_coarse_result);
		printf("coarse result(0x470): 0x%08x\n",result0);
		
		result1 = readl(&ddrintf_phy_ro_group->ro_weye_train_fine_result);
		printf("fine   result(0x474): 0x%08x\n",result1);
		
		temp = readl(&ddrintf_phy_ro_group->ro_weye_train_pass_window);
		printf("pass   window(0x47c): 0x%08x\n",temp);

		db0_tune = (result0 & 0xf) * mclk_ps + (result1 & 0xff) * mclk_ps / 256;
		db1_tune = ((result0>>4) & 0xf) * mclk_ps + ((result1>>8) & 0xff) * mclk_ps / 256;
		printf("db0: %dps  db1: %dps",db0_tune,db1_tune);

		if(DDRINTF_ECC_ENABLE == regs->ddrintf_ecc_en){
			unsigned int db2_tune;
			
			db2_tune = ((result0>>8) & 0xf) * mclk_ps + ((result1>>16) & 0xff) * mclk_ps / 256;
			printf("  db2: %dps\n",db2_tune);
			
		}else{
			printf("\n");
		}

	}else{
		/* manual write eye training */
		printf("write eye training mode: manual\n");

		result1 = readl(&ddrintf_phy_group2->db0_bt_wdq);
		printf("coarse delay setting(0x800): 0x%08x\n",result1);
		
		result0 = readl(&ddrintf_phy_group1->db0_dll_wrdeye_dq);
		printf("fine   delay setting(0x6f8): 0x%08x\n",result0);
		
		db0_tune = (result1 & 0xf) * mclk_ps + (result0 & 0xff) * mclk_ps / 256;
		db1_tune = ((result1>>4) & 0xf) * mclk_ps + ((result0>>8) & 0xff) * mclk_ps / 256;
		printf("db0: %dps  db1: %dps",db0_tune,db1_tune);

		if(DDRINTF_ECC_ENABLE == regs->ddrintf_ecc_en){
			unsigned int db2_tune;
			
			db2_tune = ((result1>>8) & 0xf) * mclk_ps + ((result0>>16) & 0xff) * mclk_ps / 256;
			printf("  db2: %dps\n",db2_tune);
			
		}else{
			printf("\n");
		}
	}
	
	printf("\n");
	
	/* only DDR4 support write vref training */
	if(DDRINTF_TYPE_4 == regs->ddrintf_type)
	{
		/* write vref training result */
		printf("----------------write vref training-------------------------\n");

		if(0 == (train_mode & BIT(14))){
			printf("write vref training mode: auto\n");
		}else{
			printf("write vref training mode: manual\n");
		}
		result0 = readl(&ddrintf_phy_group1->mlb_vref_dram_value_manu);
		if((result0 >> 6)& 0x1)
		{
			printf("vrefdq range : range2\n");
		}else{
			printf("vrefdq range : range1\n");
		}
		
		result1 = readl(&ddrintf_phy_ro_group->ro_dram_vref_result);
		printf("vrefdq value : 0x%x\n",result1);
		
	}
	
	return 0;
}

unsigned int ctc_ddr_register_dump(void)
{
	unsigned int i,ret;
	unsigned int *addr = (unsigned int *)CONFIG_SYS_CTC5236_DDRCFG_BASE;

	for(i=0;i<600;i++)
	{
		ret = readl(addr);
		printf("0x%8p:0x%08x\n",addr,ret);
		addr ++;
	}
	return 0;
}

unsigned int ctc_print_ddr_debug_info(ctc5236_ddr_cfg_t *regs)
{
	unsigned int waitdelay = 2;
	unsigned int dump_flag = 0;
	unsigned long ts;
	ctc_print_ddr_addr_info();	
	ctc_print_ddr_training_result_info(regs);

	printf("\nHit any key to dump ddr register : %2d ", waitdelay);
	
	/*
	 * Check if key already pressed
	 */
	if (tstc()) {	/* we got a key press	*/
		(void) getc();  /* consume input	*/
		puts("\b\b\b 0");
		/*dump register*/
		ctc_ddr_register_dump();
		dump_flag = 1;
	}

	while ((waitdelay > 0) && (!dump_flag)) {
		--waitdelay;
		/* delay 1000 ms */
		ts = get_timer(0);
		do {
			if (tstc()) {	/* we got a key press	*/
				dump_flag = 1;
				waitdelay = 0;	/* no more delay	*/

				(void) getc();  /* consume input	*/
				/*dump register*/
				ctc_ddr_register_dump();
				break;
			}
			udelay(10000);
		} while (!dump_flag && get_timer(ts) < 1000);

		printf("\b\b\b%2d ", waitdelay);
	}
	
	return 0;
}
