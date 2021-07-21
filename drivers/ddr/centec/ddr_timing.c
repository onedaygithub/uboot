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
#include <div64.h>
#include "ddr_ctl_reg.h"
#include "ddr_phy_reg.h"

DECLARE_GLOBAL_DATA_PTR;

/* To avoid 64-bit full-divides, we factor this here */
#define ULL_2E12 2000000000000ULL
#define UL_5POW12 244140625UL
#define UL_2POW13 (1UL << 13)
#define ULL_8FS 0xFFFFFFFFULL

unsigned int ctc_get_ddr_rate(ctc5236_ddr_cfg_t *regs)
{
	unsigned int ddr_rate;
	switch(regs->ddrintf_rate)
	{
	   	case DDRINTF_RATE_2400:
			ddr_rate = 2400;
			break;
		case DDRINTF_RATE_2133:
			ddr_rate = 2133;
			break;
		case DDRINTF_RATE_1866:
			ddr_rate = 1866;
			break;
		case DDRINTF_RATE_1600:
			ddr_rate = 1600;
			break;
		case DDRINTF_RATE_1333:
			ddr_rate = 1333;
			break;
		case DDRINTF_RATE_1066:
			ddr_rate = 1066;
			break;
		case DDRINTF_RATE_800:
			ddr_rate = 800;
			break;
		default:
			ddr_rate = 1600;
			printf("Warning: DDR rate is invalid\n");
	}

	return ddr_rate*1000000;
}

unsigned int ctc_get_ddr_phy_clk_period_ps(ctc5236_ddr_cfg_t *regs)
{
	unsigned int data_rate = ctc_get_ddr_rate(regs);
	unsigned int result;

	/* Round to nearest 10ps, being careful about 64-bit multiply/divide */
	unsigned long long rem, mclk_ps = ULL_2E12;

	/* Now perform the big divide, the result fits in 32-bits */
	rem = do_div(mclk_ps, data_rate);
	result = (rem >= (data_rate >> 1)) ? mclk_ps + 1 : mclk_ps;

	return result;
}

/* Convert picoseconds into DRAM clock cycles (rounding up if needed). */
unsigned int ctc_picos_to_phyclk(ctc5236_ddr_cfg_t *regs, unsigned int picos)
{
	unsigned long long clks, clks_rem;
	unsigned long data_rate = ctc_get_ddr_rate(regs);

	/* Short circuit for zero picos */
	if (!picos)
		return 0;

	/* First multiply the time by the data rate (32x32 => 64) */
	clks = picos * (unsigned long long)data_rate;
	/*
	 * Now divide by 5^12 and track the 32-bit remainder, then divide
	 * by 2*(2^12) using shifts (and updating the remainder).
	 */
	clks_rem = do_div(clks, UL_5POW12);
	clks_rem += (clks & (UL_2POW13-1)) * UL_5POW12;
	clks >>= 13;

	/* If we had a remainder greater than the 1ps error, then round up */
	if (clks_rem > data_rate)
		clks++;

	/* Clamp to the maximum representable value */
	if (clks > ULL_8FS)
		clks = ULL_8FS;
	return (unsigned int) clks;
}

unsigned int ctc_picos_to_mcclk(ctc5236_ddr_cfg_t *regs, unsigned int picos)
{
	return  ctc_picos_to_phyclk(regs,picos) /2;
}

unsigned int ctc_mclk_to_picos(ctc5236_ddr_cfg_t *regs, unsigned int mclk)
{
	return ctc_get_ddr_phy_clk_period_ps(regs) * mclk;
}

unsigned int ctc_compute_cas_write_latency(ctc5236_ddr_cfg_t *regs)
{
	unsigned int cwl;
	const unsigned int mclk_ps = ctc_get_ddr_phy_clk_period_ps(regs);

/*
 * compute CAS write latency according to DDR4 spec
 * CWL = 9 for <= 1600MT/s
 *       10 for <= 1866MT/s
 *       11 for <= 2133MT/s
 *       12 for <= 2400MT/s
 *       14 for <= 2667MT/s
 *       16 for <= 2933MT/s
 *       18 for higher
 */
	if(DDRINTF_TYPE_4 == regs->ddrintf_type)
	{
		if (mclk_ps >= 1250)
			cwl = 9;
		else if (mclk_ps >= 1070)
			cwl = 10;
		else if (mclk_ps >= 935)
			cwl = 11;
		else if (mclk_ps >= 833)
			cwl = 12;
		else if (mclk_ps >= 750)
			cwl = 14;
		else if (mclk_ps >= 681)
			cwl = 16;
		else
			cwl = 18;
	}

/*
 * compute the CAS write latency according to DDR3 spec
 * CWL = 5 if tCK >= 2.5ns
 *       6 if 2.5ns > tCK >= 1.875ns
 *       7 if 1.875ns > tCK >= 1.5ns
 *       8 if 1.5ns > tCK >= 1.25ns
 *       9 if 1.25ns > tCK >= 1.07ns
 *       10 if 1.07ns > tCK >= 0.935ns
 *       11 if 0.935ns > tCK >= 0.833ns
 *       12 if 0.833ns > tCK >= 0.75ns
 */	
	else
	{
		if (mclk_ps >= 2500)
			cwl = 5;
		else if (mclk_ps >= 1875)
			cwl = 6;
		else if (mclk_ps >= 1500)
			cwl = 7;
		else if (mclk_ps >= 1250)
			cwl = 8;
		else if (mclk_ps >= 1070)
			cwl = 9;
		else if (mclk_ps >= 935)
			cwl = 10;
		else if (mclk_ps >= 833)
			cwl = 11;
		else if (mclk_ps >= 750)
			cwl = 12;
		else {
			cwl = 12;
			printf("Warning: CWL is out of range\n");
			}
	}

	return cwl;
}

/*
* compute the refresh cycle time according to DDR3/DDR4 spec
*	tRFC(min) = 90ns for 512Mb device
*			= 110ns for 1Gb device
*			= 160ns for 2Gb device
*			= 260ns for 4Gb device
*			= 350ns for 8Gb device
*			= 550ns for 16Gb device    //Micron SDRAM 
*/
unsigned int ctc_compute_refresh_cycle_time(ctc5236_ddr_cfg_t *regs)
{
	unsigned int rfc_ps;
	unsigned int result;

	switch(regs->sdram_size)
	{
		case SDRAM_SIZE_512M_BIT:
			rfc_ps = 90000;
			break;
		case SDRAM_SIZE_1G_BIT:
			rfc_ps = 110000;
			break;
		case SDRAM_SIZE_2G_BIT:
			rfc_ps = 160000;
			break;
		case SDRAM_SIZE_4G_BIT:
			rfc_ps = 260000;
			break;
		case SDRAM_SIZE_8G_BIT:
			rfc_ps = 350000;
			break;
		case SDRAM_SIZE_16G_BIT:
			rfc_ps = 550000;
			break;
		default :
			rfc_ps = 550000;
			printf("Warning:SDRAM SIZE error!\n");
	}

	result = ctc_picos_to_phyclk(regs,rfc_ps);

	return result;
	
}

unsigned int ctc_compute_refresh_limit_time(ctc5236_ddr_cfg_t *regs)
{
	unsigned int refi;
	unsigned int refi_ps;

	/* from DDR3/DDR4 spec REFI is 7.8 us 
	*refi  unit:mcT
	*/
	
	refi_ps = 7800000;

	refi =  ctc_picos_to_mcclk(regs,refi_ps);
	
	return refi;
}


unsigned int ctc_set_Mode_Register0(ctc5236_ddr_cfg_t *regs)
{
	unsigned int mr0;
	
	unsigned int dll_on;	/* DLL control for precharge PD(DDR3), 0=off, 1=on */	
	unsigned int wr = 0;	/* Write Recovery */
	unsigned int dll_rst;	/* DLL Reset */
	unsigned int mode;	/* Normal=0 or Test=1 */
	unsigned int caslat = 8;	/* CAS# latency */
	/* BT: Burst Type (0=Nibble Sequential, 1=Interleaved) */
	unsigned int bt;
	unsigned int bl = 0;	/* BL: Burst Length ,TsingMa only suppprt BL=8*/

	unsigned int wr_mclk;


	if(DDRINTF_TYPE_4 == regs->ddrintf_type)
	{
		/* DDR4 support WR 10, 12, 14, 16, 18, 20, 24 */
		static const u8 wr_table[] = {
			0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 6, 6};
			
		/* DDR4 support CAS 9, 10, 11, 12, 13, 14, 15, 16, 18, 20, 22, 24 */
		static const u8 cas_latency_table[] = {
			0, 1, 2, 3, 4, 5, 6, 7, 8, 8,
			9, 9, 10, 10, 11, 11};

		/*from DDR4 spec tWR is 15ns*/
		wr_mclk = ctc_picos_to_phyclk(regs,15000); 
		if (wr_mclk <= 24) {
			wr = wr_table[wr_mclk - 10];
		} else {
			printf("Error: unsupported write recovery for mode register wr_mclk = %d\n",
			       wr_mclk);
		}

		dll_rst = 1;	/* dll reset */
		mode = 0;	/* normal mode */


		/* look up table to get the cas latency bits */
		if (regs->sdram_cas_latency >= 9 && regs->sdram_cas_latency <= 24)
		{
			caslat = cas_latency_table[regs->sdram_cas_latency - 9];
		}
		else
		{
			printf("Error: unsupported cas latency for mode register\n");
		}

		bt = 0;	/* Nibble sequential */

		mr0 = (0
			| ((wr & 0x7) << 9)
		  	| ((dll_rst & 0x1) << 8)
		  	| ((mode & 0x1) << 7)
		  	| (((caslat >> 1) & 0x7) << 4)
		  	| ((bt & 0x1) << 3)
		  	| ((caslat & 0x1) << 2)
		  	| ((bl & 0x3) << 0)
			);
		
	}
	else	/* DDR3 */
	{
		/* DDR3 support WR 5, 6, 7, 8, 10, 12, 14,16 */
		static const u8 wr_table[] = {1, 2, 3, 4, 5, 5, 6, 6, 7, 7, 0, 0};
		
		/*
		 * DLL control for precharge PD
		 * 0=slow exit DLL off (tXPDLL)
		 * 1=fast exit DLL on (tXP)
		 */
		dll_on = 1;

		/*from DDR3 spec tWR is 15ns*/
		wr_mclk = ctc_picos_to_phyclk(regs,15000);
		if (wr_mclk <= 16) {
			wr = wr_table[wr_mclk - 5];
		} else {
			printf("Error: unsupported write recovery for mode register "
			       "wr_mclk = %d\n", wr_mclk);
		}
		
		dll_rst = 0;	/* dll no reset */
		mode = 0;	/* normal mode */

		/* look up table to get the cas latency bits */
		if (regs->sdram_cas_latency >= 5 && regs->sdram_cas_latency <= 16) {
			unsigned char cas_latency_table[] = {
				0x2,	/* 5 clocks */
				0x4,	/* 6 clocks */
				0x6,	/* 7 clocks */
				0x8,	/* 8 clocks */
				0xa,	/* 9 clocks */
				0xc,	/* 10 clocks */
				0xe,	/* 11 clocks */
				0x1,	/* 12 clocks */
				0x3,	/* 13 clocks */
				0x5,	/* 14 clocks */
				0x7,	/* 15 clocks */
				0x9,	/* 16 clocks */
			};
			caslat = cas_latency_table[regs->sdram_cas_latency - 5];
		} else {
			printf("Error: unsupported cas latency for mode register\n");
		}

		bt = 0;	/* Nibble sequential */
		mr0 = (0
			| ((dll_on & 0x1) << 12)
			| ((wr & 0x7) << 9)
		  	| ((dll_rst & 0x1) << 8)
		  	| ((mode & 0x1) << 7)
		  	| (((caslat >> 1) & 0x7) << 4)
		  	| ((bt & 0x1) << 3)
		  	| ((caslat & 0x1) << 2)
		  	| ((bl & 0x3) << 0)
			);

	}

	debug("TsingMaDDR:MR0 = 0x%08x\n",mr0);
	
	return mr0;

}

unsigned int ctc_set_Mode_Register1(ctc5236_ddr_cfg_t *regs)
{
	unsigned int mr1;

	unsigned int qoff = 0;		/* Output buffer enable 0=yes, 1=no */
	unsigned int tdqs_en = 0;	/* TDQS Enable: 0=no, 1=yes */
	unsigned int rtt;
	unsigned int wrlvl_en = 0;	/* Write level enable: 0=no, 1=yes */
	unsigned int al = 0;		/* Posted CAS# additive latency (AL) */
	unsigned int dic = 0;		/* Output driver impedance, 240/7ohm */
	unsigned int dll_en = 1;	/* DLL Enable  1=Enable (Normal),
						       			 0=Disable (Test/Debug) */

	if(DDRINTF_TYPE_4 == regs->ddrintf_type)
	{
		/*from DDR4 spec	RZQ = 240 ohm
		*RTT_NOM = 0 for RTT_NOM disable
		*		= 1 for RZQ/4
		*		= 2 for RZQ/2
		*		= 3 for RZQ/6
		*		= 4 for RZQ/1
		*		= 5 for RZQ/5
		*		= 6 for RZQ/3
		*		= 7 for RZQ/7
		*/
		
		rtt = 1;	/* 60ohm */
		mr1 = (0
			| ((qoff & 0x1) << 12)
			| ((tdqs_en & 0x1) << 11)
			| ((rtt & 0x7) << 8)
			| ((wrlvl_en & 0x1) << 7)
			| ((al & 0x3) << 3)
			| ((dic & 0x3) << 1) 
			| ((dll_en & 0x1) << 0)
		);
	}
	else	/* DDR3 */
	{
		/*from DDR3 spec	RZQ = 240 ohm
		*RTT_NOM = 0 for RTT_NOM disable
		*		= 1 for RZQ/4
		*		= 2 for RZQ/2
		*		= 3 for RZQ/6
		*		= 4 for RZQ/12
		*		= 5 for RZQ/8
		*/
		dll_en = 0;	/*for DDR3 DLL Enable  0=Enable (Normal),
						       	1=Disable (Test/Debug) */
		
		rtt = 2;	/* 120ohm */
		
		dic = 1;	/* Output driver impedance, 240/7ohm */
		
		mr1 = (0
			| ((qoff & 0x1) << 12)
			| ((tdqs_en & 0x1) << 11)
			| ((rtt & 0x4) << 7)   /* rtt field is split */
			| ((wrlvl_en & 0x1) << 7)
			| ((rtt & 0x2) << 5)   /* rtt field is split */
			| ((dic & 0x2) << 4)   /* DIC field is split */
			| ((al & 0x3) << 3)
			| ((rtt & 0x1) << 2)  /* rtt field is split */
			| ((dic & 0x1) << 1)   /* DIC field is split */
			| ((dll_en & 0x1) << 0)
			);
	}

	debug("TsingMaDDR:MR1 = 0x%08x\n",mr1);

	return mr1;
	
}

unsigned int ctc_set_Mode_Register2(ctc5236_ddr_cfg_t *regs)
{
	unsigned int mr2 = 0;

	unsigned int wcrc = 0;		/* write CRC ,0 = diable,1 = enable */
	unsigned int rtt_wr;
	unsigned int cwl;			/* CAS write Latency */
	unsigned int srt = 0;	/* self-refresh temerature, normal range */
	unsigned int asr = 0;	/* auto self-refresh disable */
	unsigned int pasr = 0;	/* partial array self refresh disable */

	if(DDRINTF_TYPE_4 == regs->ddrintf_type)
	{
		rtt_wr = 0;	/* Disable RTT_WR */

		cwl = ctc_compute_cas_write_latency(regs) - 9;

		mr2 = (0
			|((wcrc & 0x1) << 12)
			|((rtt_wr & 0x3) << 9)
			|((srt & 0x3) << 6)
			|((cwl & 0x7) << 3)
			);
	}
	else	/* DDR3 */
	{
		rtt_wr = 2;	/* 120ohm */

		cwl = ctc_compute_cas_write_latency(regs) - 5;
		
		mr2 = (0
			| ((rtt_wr & 0x3) << 9)
			| ((srt & 0x1) << 7)
			| ((asr & 0x1) << 6)
			| ((cwl & 0x7) << 3)
			| ((pasr & 0x7) << 0)
			);
	}
	
	debug("TsingMaDDR:MR2 = 0x%08x\n",mr2);

	return mr2;
	
}

unsigned int ctc_set_Mode_Register3(ctc5236_ddr_cfg_t *regs)
{
	unsigned int mr3 = 0;

	unsigned int mpr = 0;	/* serial */
	unsigned int wc_lat;	/* Write CMD Latency when CRC and DM are enabled */
	unsigned int fgrm = 0;	/* normal */
	unsigned int tsr = 0;	/* Temperature sensor readout 0:disable,1:enable */
	unsigned int pda = 0;		/* Per DRAM Addressability 0:disable,1:enable */
	unsigned int geardown = 0;	/* Geardown Mode 0:1/2 rate, 1: 1/4 rate*/
	unsigned int mpr_ope = 0;	/* MPR Operation ,normal operation*/
	unsigned int mpr_page;
	unsigned int mpr_loc = 0;	/* Predefined pattern */

	const unsigned int mclk_ps = ctc_get_ddr_phy_clk_period_ps(regs);

	if(DDRINTF_TYPE_4 == regs->ddrintf_type)
	{

		if (mclk_ps >= 1250)
			wc_lat = 0;
		else if (mclk_ps >= 833)
			wc_lat = 1;
		else
			wc_lat = 2;

		mpr_ope = 0; /* normal */

		mpr_page = 0; /* page0 */

		mr3 =(0
			|((mpr & 0x3) << 11)
			|((wc_lat & 0x3) << 9)
			|((fgrm & 0x3) << 6)
			|((tsr & 0x1) << 5)
			|((pda & 0x1) << 4)
			|((geardown & 0x1) << 3)
			|((mpr_ope & 0x1) << 2)
			|((mpr_page & 0x3) << 0)
			);
	}
	else	/* DDR3 */
	{
		mr3 = (0
			| ((mpr_ope & 0x1) << 2)
			| ((mpr_loc & 0x3) << 0)
			);
	}

	debug("TsingMaDDR:MR3 = 0x%08x\n",mr3);

	return mr3;
}

unsigned int ctc_set_Mode_Register4(ctc5236_ddr_cfg_t *regs)
{
	unsigned int mr4;

	unsigned int wp = 0;  /* Write Preamble;0 = Disable 1 = Enable */
	unsigned int rp = 0;	/* Read Preamble 0 = Disable 1 = Enable*/
	unsigned int rp_train = 0; /* Read Preamble Training Mode 0 = Disable 1 = Enable*/
	unsigned int sra = 0;	/* Self Refresh Abort 0 = Disable 1 = Enable*/
	unsigned int cal = 0;	/* CS to CMD/ADDR Latency Mode, 0  disable*/
	unsigned int ivm = 0;	/* Internal Vref Monitor */
	unsigned int tcr = 0;	/* Temperature Controlled Refresh Mode 0 = Disable 1 = Enable*/
	unsigned int tcr_range = 0; /* Temperature Controlled Refresh Range 0 normal , 1 extended*/
	unsigned int mpd = 0; /* Maximum Power Down Mode0 = Disable 1 = Enable*/

	mr4 = (0
		|((wp & 0x1) << 12)
		|((rp & 0x1) << 11)
		|((rp_train &0x1) << 10)
		|((sra & 0x1) << 9)
		|((cal & 0x7) << 6)
		|((ivm & 0x1) << 4)
		|((tcr & 0x1) << 3)
		|((tcr_range & 0x1) << 2)
		|((mpd & 0x1) << 1)
		);

	debug("TsingMaDDR:MR4 = 0x%08x\n",mr4);

	return mr4;
}

unsigned int ctc_set_Mode_Register5(ctc5236_ddr_cfg_t *regs)
{
	unsigned int mr5;

	unsigned int rdbi = 0;	/* Read DBI 0 = Disable 1 = Enable*/
	unsigned int wdbi = 0;	/* Write DBI 0 = Disable 1 = Enable*/
	unsigned int dm;	/* Data Mask */
	unsigned int cappe = 0;	/* CA parity Persistent Error 0 = Disable 1 = Enable*/
	unsigned int rtt_park;
	unsigned int odtib;		/* ODT Input Buffer during Power Down mode*/
	unsigned int capes = 0;	/* C/A Parity Error Status 0 = Clear 1 = Error*/
	unsigned int crce = 0;	/* CRC Error Clear 0 = Clear 1 = Error*/
	unsigned int pl;	/* C/A Parity Latency Mode */

	dm = 1;	/* Data Mask enable */
	
	rtt_park = 0;	/* Disable RTT_PARK */

	odtib = 1;	/* ODT input buffer is deactivated */

	pl = 0; /* disable */

	mr5 = (0
		|((rdbi & 0x1) << 12)
		|((wdbi & 0x1) << 11)
		|((dm & 0x1) << 10)
		|((cappe & 0x1) << 9)
		|((rtt_park & 0x7) << 6)
		|((odtib & 0x1) << 5)
		|((capes & 0x1) << 4)
		|((crce & 0x1) << 3)
		|((pl & 0x7) << 0)
		);

	debug("TsingMaDDR:MR5 = 0x%08x\n",mr5);

	return mr5;
}

unsigned int ctc_set_Mode_Register6(ctc5236_ddr_cfg_t *regs)
{
	unsigned int mr6;

	unsigned int ccd_l;
	unsigned int vrefdq_train;
	unsigned int vrefdq_train_range;
	unsigned int vrefdq_train_value;
	unsigned int ret;
	
	const unsigned int mclk_ps = ctc_get_ddr_phy_clk_period_ps(regs);

	if(mclk_ps >= 1500)
		ccd_l = 0;
	else if(mclk_ps >= 1250)
		ccd_l = 1;
	else if(mclk_ps >= 833)
		ccd_l = 2;
	else
		ccd_l = 3;

	vrefdq_train = 0;	/* Normal operation Mode */

	vrefdq_train_range = 1;	/* range 2 */

	/* get dram vref training result */
	ret = readl(&ddrintf_phy_ro_group->ro_dram_vref_result);
	vrefdq_train_value = (ret & PHY_490_RO_RDVERF_TRAIN_RESULT_MASK);

	mr6 = (0
		|((ccd_l & 0x7) << 10)
		|((vrefdq_train & 0x1) << 7)
		|((vrefdq_train_range & 0x1) << 6)
		|((vrefdq_train_value & 0x3f) << 0)
		);

	debug("TsingMaDDR:MR6 = 0x%08x\n",mr6);
	
	return mr6;
}

