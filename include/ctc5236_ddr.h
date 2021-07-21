/*
 * (C) Copyright 2004-2017 Centec Networks (suzhou) Co., LTD.
 * Jay Cao <caoj@centecnetworks.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef __CTC5236_DDR_H__
#define __CTC5236_DDR_H__

/* ddr intface bit enable */
enum ddrintf_bit_op {
	DDRINTF_BIT_CLR,   /*  clear bit  */
	DDRINTF_BIT_SET,   /*  set bit  */
};

/* ddr intface rate */
enum ddrintf_rate{
	DDRINTF_RATE_2400,
	DDRINTF_RATE_2133,
	DDRINTF_RATE_1866,
	DDRINTF_RATE_1600,
	DDRINTF_RATE_1333,
	DDRINTF_RATE_1066,
	DDRINTF_RATE_800
};

/* ddr intface type */
enum ddrintf_type {
	DDRINTF_TYPE_3,   /* DDR3 */
	DDRINTF_TYPE_4,   /* DDR4 */
};

/* ddr intface cs */
enum ddrintf_cs {
	DDRINTF_CS_1,   /* chip select 0 */
	DDRINTF_CS_2,   /* chip select 0 and chip select 1 */
	DDRINTF_CS_RSV,
	DDRINTF_CS_4    /* chip select 0/1/2/3 */
};

/* ddr intface DQ type */
enum ddrintf_dq_type {
	DDRINTF_DQ_8,   /* data 8bit */
	DDRINTF_DQ_8_ECC, /* data 8bit + 8bit ecc */
	DDRINTF_DQ_16,    /* data 16bit */
	DDRINTF_DQ_16_ECC, /* data 16bit + 8bit ecc */
};

/* ddr intface ecc enable */
enum ddrintf_ecc_en {
	DDRINTF_ECC_DISABLE,   /* disable ecc */
	DDRINTF_ECC_ENABLE, /* enable ecc */
};


/* sdram type */
enum sdram_type {
	SDRAM_TYPE_SINGLE, /* single die */
	SDRAM_TYPE_TWIN    /* twin die */
};

/* sdram size */
enum sdram_size {
	SDRAM_SIZE_512M_BIT,  /* sdram size 512M bit */
	SDRAM_SIZE_1G_BIT,  /* sdram size 1G bit */
  	SDRAM_SIZE_2G_BIT,  /* sdram size 2G bit */
	SDRAM_SIZE_4G_BIT,  /* sdram size 4G bit */
	SDRAM_SIZE_8G_BIT,  /* sdram size 8G bit */
	SDRAM_SIZE_16G_BIT  /* sdram size 16G bit */
};

/* sdram column address width */
enum sdram_col_addr_width {
  	SDRAM_COL_ADDR_WIDTH_10,  /* sdram column address width 10 */
	SDRAM_COL_ADDR_WIDTH_11,  /* sdram column address width 11 */
	SDRAM_COL_ADDR_WIDTH_12,  /* sdram column address width 12 */
	SDRAM_COL_ADDR_WIDTH_9   /* sdram column address width 9 */
};

/* sdram row address width */
enum sdram_row_addr_width {
  	SDRAM_ROW_ADDR_WIDTH_12, /* sdram row address width 12 */
	SDRAM_ROW_ADDR_WIDTH_13, /* sdram row address width 13 */
	SDRAM_ROW_ADDR_WIDTH_14, /* sdram row address width 14 */
	SDRAM_ROW_ADDR_WIDTH_15, /* sdram row address width 15 */
	SDRAM_ROW_ADDR_WIDTH_16, /* sdram row address width 16 */
	SDRAM_ROW_ADDR_WIDTH_17, /* sdram row address width 17 */
	SDRAM_ROW_ADDR_WIDTH_18  /* sdram row address width 18 */
};

/* sdram bank address width */
enum sdram_bank_addr_width {
  	SDRAM_BANK_ADDR_WIDTH_3, /* sdram bank address width 3, 8 banks */
	SDRAM_BANK_ADDR_WIDTH_2  /* sdram bank address width 2, 4 banks */
};

/* sdram bank group address width */
enum sdram_bank_group_addr_width {
  	SDRAM_BANK_GROUP_ADDR_WIDTH_0, /* sdram bank group address width 0, no bank group */
	SDRAM_BANK_GROUP_ADDR_WIDTH_1, /* sdram bank group address width 1, 2 bank group  */
	SDRAM_BANK_GROUP_ADDR_WIDTH_2, /* sdram bank group address width 2, 4 bank group */
};

enum system_mem_size {
  	SYSTEM_MEM_256MB,  /* system memory size 256MB */
	SYSTEM_MEM_512MB,  /* system memory size 512MB */
	SYSTEM_MEM_1GB,    /* system memory size 1GB */
	SYSTEM_MEM_2GB,    /* system memory size 2GB */
	SYSTEM_MEM_4GB,    /* system memory size 4GB */
	SYSTEM_MEM_8GB     /* system memory size 8GB */
};

enum sdram_cas_latency{
	SDRAM_CAS_LATENCY_5 = 5,	/* CAS latency 5 cycles */
	SDRAM_CAS_LATENCY_6 = 6,	/* CAS latency 6 cycles */
	SDRAM_CAS_LATENCY_7 = 7,	/* CAS latency 7 cycles */
	SDRAM_CAS_LATENCY_8 = 8,	/* CAS latency 8 cycles */
	SDRAM_CAS_LATENCY_9 = 9,	/* CAS latency 9 cycles */
	SDRAM_CAS_LATENCY_10 = 10,	/* CAS latency 10 cycles */
	SDRAM_CAS_LATENCY_11 = 11,	/* CAS latency 11 cycles */
	SDRAM_CAS_LATENCY_12 = 12,	/* CAS latency 12 cycles */
	SDRAM_CAS_LATENCY_13 = 13,	/* CAS latency 13 cycles */
	SDRAM_CAS_LATENCY_14 = 14,	/* CAS latency 14 cycles */
	SDRAM_CAS_LATENCY_15 = 15,	/* CAS latency 15 cycles */
	SDRAM_CAS_LATENCY_16 = 16,	/* CAS latency 16 cycles */
	SDRAM_CAS_LATENCY_18 = 18,	/* CAS latency 18 cycles */
	SDRAM_CAS_LATENCY_20 = 20,	/* CAS latency 20 cycles */
	SDRAM_CAS_LATENCY_22 = 22,	/* CAS latency 22 cycles */
	SDRAM_CAS_LATENCY_24 = 24,	/* CAS latency 24 cycles */
};

enum sdram_page_size{
	SDRAM_PAGE_SIZE_512B,		/*SDRAM page_size 512B*/
	SDRAM_PAGE_SIZE_1K,		/*SDRAM page_size 1KB*/
	SDRAM_PAGE_SIZE_2K,		/*SDRAM page_size 2KB*/
};

enum ddrintf_wrlvl_mode{
	DDRINTF_WRLVL_AUTO_MODE,		/* auto mode write leveling  */
	DDRINTF_WRLVL_MANUAL_MODE,		/* manual mode write leveling */
};

enum ddrintf_training_mode{
	DDRINTF_AUTO_MODE_TRAIN,		/* auto mode training  */
	DDRINTF_MANUAL_MODE_TRAIN,		/* manual mode training */
};

/* Board DDR config */
typedef struct ctc5236_ddr_cfg_s {
       unsigned int ddrintf_rate;
	unsigned int ddrintf_type;
	unsigned int ddrintf_cs;
	unsigned int ddrintf_dq_type;
	unsigned int ddrintf_ecc_en;

	/* write leveing  parameters */
	unsigned int ddrintf_wrlvl_mode;
	unsigned int ddrintf_wrlvl_delay_db0;	/* The delay time between DQS0 and CLK in manual mode */
	unsigned int ddrintf_wrlvl_delay_db1;	/* The delay time between DQS1 and CLK in manual mode */
	unsigned int ddrintf_wrlvl_delay_db2;	/* The delay time between DQS2 and CLK in manual mode */


	/* Training parameters */
	unsigned int ddrintf_training_mode;		/*  */

	/* Gate training parameters for manual mode training */
	/* Delay of gate(coarse) */
	unsigned int ddrinitf_gate_trip_db0;		/* each bit mean delay 1/(ddr rate) ps */
	unsigned int ddrinitf_gate_trip_db1;		/* each bit mean delay 1/(ddr rate) ps */
	unsigned int ddrinitf_gate_trip_db2;		/* each bit mean delay 1/(ddr rate) ps */
	/* Delay of gate(fine) */
	unsigned int ddrinitf_gate_fine_db0;		/* each bit mean delay 1/(ddr rate)/256 ps */
	unsigned int ddrinitf_gate_fine_db1;		/* each bit mean delay 1/(ddr rate)/256 ps */
	unsigned int ddrinitf_gate_fine_db2;		/* each bit mean delay 1/(ddr rate)/256 ps */

	/* Read eye training parameters for manual mode training */
	/* DQS delay */
	unsigned int ddrintf_rdeye_dqs_db0;	/* each bit mean delay 1/(ddr rate)/256 ps */
	unsigned int ddrintf_rdeye_dqs_db1;	/* each bit mean delay 1/(ddr rate)/256 ps */
	unsigned int ddrintf_rdeye_dqs_db2;	/* each bit mean delay 1/(ddr rate)/256 ps */
	/* DQSN delay */
	unsigned int ddrintf_rdeye_dqsn_db0;	/* each bit mean delay 1/(ddr rate)/256 ps */
	unsigned int ddrintf_rdeye_dqsn_db1;	/* each bit mean delay 1/(ddr rate)/256 ps */
	unsigned int ddrintf_rdeye_dqsn_db2;	/* each bit mean delay 1/(ddr rate)/256 ps */

	/* Read vref training parameters for manual mode training */
	/* PHY vref code */
	unsigned int ddrintf_rdvref_value_db0;
	unsigned int ddrintf_rdvref_value_db1;
	unsigned int ddrintf_rdvref_value_db2;

	/* Write eye training parameters for manual mode training */
	/* DQ delay(coarse) */
	unsigned int ddrintf_wreye_coarse_db0;	/* each bit mean delay 1/(ddr rate) ps */
	unsigned int ddrintf_wreye_coarse_db1;	/* each bit mean delay 1/(ddr rate) ps */
	unsigned int ddrintf_wreye_coarse_db2;	/* each bit mean delay 1/(ddr rate) ps */
	/* DQ delay(fine) */
	unsigned int ddrintf_wreye_fine_db0;	/* each bit mean delay 1/(ddr rate)/256 ps */
	unsigned int ddrintf_wreye_fine_db1;	/* each bit mean delay 1/(ddr rate)/256 ps */
	unsigned int ddrintf_wreye_fine_db2;	/* each bit mean delay 1/(ddr rate)/256 ps */

	/* Write vref training parameters (for DDR4) for manual mode training */
	/* Vref value  */
	unsigned int ddrintf_wrvref_value;

	/* SDRAM  parameters */
	unsigned int sdram_cas_latency;
	unsigned int sdram_type;
	unsigned int sdram_size;
	unsigned int sdram_col_addr_width;
	unsigned int sdram_row_addr_width;
  	unsigned int sdram_bank_group_addr_width;
	unsigned int sdram_bank_addr_width;
	unsigned int sdram_page_size;
	unsigned int system_mem_size;	
} ctc5236_ddr_cfg_t;


/* ddr controller and phy register group offset */
#define DDRINTF_CTL_GROUP0_OFFSET 0x0
#define DDRINTF_CTL_GROUP1_OFFSET 0x700
#define DDRINTF_PHY_GROUP0_OFFSET 0x100
#define DDRINTF_PHY_GROUP1_OFFSET 0x600
#define DDRINTF_PHY_GROUP2_OFFSET 0x800

/* PLL_DDR SysDdrConfig0*/
#define DDRINTF_MC_CLOCK_CONFIG0_200		0x0370d004
#define DDRINTF_MC_CLOCK_CONFIG0_266		0x09f09024
#define DDRINTF_MC_CLOCK_CONFIG0_333		0x09f07024
#define DDRINTF_MC_CLOCK_CONFIG0_400		0x02f05004
#define DDRINTF_MC_CLOCK_CONFIG0_466		0x03705004
#define DDRINTF_MC_CLOCK_CONFIG0_533		0x07f03024
#define DDRINTF_MC_CLOCK_CONFIG0_600		0x02f03004

/* PLL_DDR SysDdrConfig1*/
#define DDRINTF_MC_CLOCK_CONFIG1_0		0x10410f09
#define DDRINTF_MC_CLOCK_CONFIG1_1		0x10411913
#define DDRINTF_MC_CLOCK_CONFIG1_2		0x1041130d

/* DDR bist cnt */
#define DDR_BIST_CNT_512MB	0x01000000
#define DDR_BIST_CNT_1GB		0x02000000
#define DDR_BIST_CNT_2GB		0x04000000
#define DDR_BIST_CNT_4GB		0x08000000
#define DDR_BIST_CNT_8GB		0x10000000

/* DDR bist pattern */
#define DDR_BIST_PATTERN	0x0

/* DDR memory size */
#define DDR_MEMORY_SIZE_512MB	0x20000000
#define DDR_MEMORY_SIZE_1GB		0x40000000
#define DDR_MEMORY_SIZE_2GB		0x80000000
#define DDR_MEMORY_SIZE_4GB		0x100000000
#define DDR_MEMORY_SIZE_8GB		0x200000000

/* DDR reserve memory  */
#define DDR_RESERVE_MEMORY_1KB	128
#define DDR_RESERVE_MEMORY_4KB	512

#define RESERVE_MEMORY_OFFSET_1GB	0xbe000000ULL
#define RESERVE_MEMORY_OFFSET_2GB	0xfe000000ULL
#define RESERVE_MEMORY_OFFSET_4GB	0x17e000000ULL

#define RESERVE_MEMORY_SIZE_16KB		0x4000
#define RESERVE_MEMORY_SIZE_64KB		0x10000
#define RESERVE_MEMORY_SIZE_128KB	0x20000
#define RESERVE_MEMORY_SIZE_512KB	0x80000
#define RESERVE_MEMORY_SIZE_1MB		0x100000
#define RESERVE_MEMORY_SIZE_2MB		0x200000
#define RESERVE_MEMORY_SIZE_4MB		0x400000
#define RESERVE_MEMORY_SIZE_8MB		0x800000
#define RESERVE_MEMORY_SIZE_16MB		0x1000000
#define RESERVE_MEMORY_SIZE_32MB		0x2000000


void board_add_ram_info(int use_default);
phys_size_t ctc_ddrclk_set(u32 ddr_rate);
phys_size_t ctc_ddr_init(ctc5236_ddr_cfg_t *regs);
int ctc_init_dram(void);
unsigned int ctc_get_ddr_rate(ctc5236_ddr_cfg_t *regs);
unsigned int ctc_get_ddr_phy_clk_period_ps(ctc5236_ddr_cfg_t *regs);
unsigned int ctc_picos_to_phyclk(ctc5236_ddr_cfg_t *regs, unsigned int picos);
unsigned int ctc_compute_refresh_cycle_time(ctc5236_ddr_cfg_t *regs);
unsigned int ctc_compute_refresh_limit_time(ctc5236_ddr_cfg_t *regs);
unsigned int ctc_compute_cas_write_latency(ctc5236_ddr_cfg_t *regs);

unsigned int ctc_set_Mode_Register0(ctc5236_ddr_cfg_t *regs);
unsigned int ctc_set_Mode_Register1(ctc5236_ddr_cfg_t *regs);
unsigned int ctc_set_Mode_Register2(ctc5236_ddr_cfg_t *regs);
unsigned int ctc_set_Mode_Register3(ctc5236_ddr_cfg_t *regs);
unsigned int ctc_set_Mode_Register4(ctc5236_ddr_cfg_t *regs);
unsigned int ctc_set_Mode_Register5(ctc5236_ddr_cfg_t *regs);
unsigned int ctc_set_Mode_Register6(ctc5236_ddr_cfg_t *regs);

unsigned int ctc_dispaly_odt(unsigned int ddr_type);
unsigned int ctc_print_ddr_cycle_time(void);
unsigned int ctc_print_cas_latency_time(unsigned int ddr_type);
unsigned int ctc_print_trcd(void);
unsigned int ctc_print_trp(void);
unsigned int ctc_print_tras(void);
unsigned int ctc_print_trfc(void);
unsigned int ctc_print_tfaw(void);
unsigned int ctc_print_trrd(unsigned int ddr_type);
unsigned int ctc_print_tccd(unsigned int ddr_type);
unsigned int ctc_print_twr(unsigned int ddr_type);	
unsigned int ctc_print_twtr(unsigned int ddr_type);
unsigned int ctc_dispaly_mr(unsigned int ddr_type);
unsigned int ctc_print_ddr_addr_info(void);
unsigned int ctc_print_ddr_training_result_info(ctc5236_ddr_cfg_t *regs);
unsigned int ctc_ddr_register_dump(void);
unsigned int ctc_print_ddr_debug_info(ctc5236_ddr_cfg_t *regs);

#endif

