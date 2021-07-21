/*
 * ARM PL022 Module Defines
 *
 * Copyright (C) 2004-2017, 2017 ARM Semiconductor, Inc.
 * Liuht (liuht@centecnetworks.com)
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef _ARM_PL022_H_
#define _ARM_PL022_H_

/*
 * Macros to access SSP Registers with their offsets
 */
#define SSP_CR0(r)	(r + 0x000)
#define SSP_CR1(r)	(r + 0x004)
#define SSP_DR(r)	(r + 0x008)
#define SSP_SR(r)	(r + 0x00C)
#define SSP_CPSR(r)	(r + 0x010)
#define SSP_IMSC(r)	(r + 0x014)
#define SSP_RIS(r)	(r + 0x018)
#define SSP_MIS(r)	(r + 0x01C)
#define SSP_ICR(r)	(r + 0x020)
#define SSP_DMACR(r)	(r + 0x024)
#define SSP_CSR(r)	(r + 0x030) /* vendor extension */
#define SSP_ITCR(r)	(r + 0x080)
#define SSP_ITIP(r)	(r + 0x084)
#define SSP_ITOP(r)	(r + 0x088)
#define SSP_TDR(r)	(r + 0x08C)

#define SSP_PID0(r)	(r + 0xFE0)
#define SSP_PID1(r)	(r + 0xFE4)
#define SSP_PID2(r)	(r + 0xFE8)
#define SSP_PID3(r)	(r + 0xFEC)

#define SSP_CID0(r)	(r + 0xFF0)
#define SSP_CID1(r)	(r + 0xFF4)
#define SSP_CID2(r)	(r + 0xFF8)
#define SSP_CID3(r)	(r + 0xFFC)

#define SSP_DISABLED			(0)
#define SSP_ENABLED			    (1)

#define DRIVE_TX		         0
#define DO_NOT_DRIVE_TX		     1

#define RX_TRANSFER		         1
#define TX_TRANSFER		         2

/*
 * SSP DMA State - Whether DMA Enabled or Disabled
 */
#define SSP_DMA_DISABLED		(0)
#define SSP_DMA_ENABLED			(1)

/*
 * SSP DMA Control Register - SSP_DMACR
 */
/* Receive DMA Enable bit */
#define SSP_DMACR_MASK_RXDMAE		(0x1UL << 0)
/* Transmit DMA Enable bit */
#define SSP_DMACR_MASK_TXDMAE		(0x1UL << 1)

/*
 * SSP Clock Defaults
 */
#define SSP_DEFAULT_CLKRATE      0x2
#define SSP_DEFAULT_PRESCALE     0x40

/* default SCK frequency, unit: HZ */
#define PL022_SPI_DEFAULT_SCK_FREQ	10000000

/*
 * SSP Clock Prescale Register  - SSP_CPSR
 */
#define SSP_CPSR_MASK_CPSDVSR	(0xFFUL << 0)

/*
 * SSP Control Register 0  - SSP_CR0
 */
#define SSP_CR0_MASK_DSS	(0x0FUL << 0)
#define SSP_CR0_MASK_FRF	(0x3UL << 4)
#define SSP_CR0_MASK_SPO	(0x1UL << 6)
#define SSP_CR0_MASK_SPH	(0x1UL << 7)
#define SSP_CR0_MASK_SCR	(0xFFUL << 8)

/*
 * SSP Control Register 0  - SSP_CR1
 */
#define SSP_CR1_MASK_LBM	(0x1UL << 0)
#define SSP_CR1_MASK_SSE	(0x1UL << 1)
#define SSP_CR1_MASK_MS		(0x1UL << 2)
#define SSP_CR1_MASK_SOD	(0x1UL << 3)
#define SSP_CR1_MASK_BIGEND	(0x1UL << 4)


/*
 * SSP Interrupt Mask Set/Clear Register - SSP_IMSC
 */
#define SSP_IMSC_MASK_RORIM (0x1UL << 0) /* Receive Overrun Interrupt mask */
#define SSP_IMSC_MASK_RTIM  (0x1UL << 1) /* Receive timeout Interrupt mask */
#define SSP_IMSC_MASK_RXIM  (0x1UL << 2) /* Receive FIFO Interrupt mask */
#define SSP_IMSC_MASK_TXIM  (0x1UL << 3) /* Transmit FIFO Interrupt mask */

/*
 * SSP Interrupt related Macros
 */
#define DEFAULT_SSP_REG_IMSC  0x0UL
#define DISABLE_ALL_INTERRUPTS DEFAULT_SSP_REG_IMSC
#define ENABLE_ALL_INTERRUPTS ( \
	SSP_IMSC_MASK_RORIM | \
	SSP_IMSC_MASK_RTIM | \
	SSP_IMSC_MASK_RXIM | \
	SSP_IMSC_MASK_TXIM \
)

#define CLEAR_ALL_INTERRUPTS  0x3

/*
 * This macro is used to define some register default values.
 * reg is masked with mask, the OR:ed with an (again masked)
 * val shifted sb steps to the left.
 */
#define SSP_WRITE_BITS(reg, val, mask, sb) \
 ((reg) = (((reg) & ~(mask)) | (((val)<<(sb)) & (mask))))

/*
 * This macro is also used to define some default values.
 * It will just shift val by sb steps to the left and mask
 * the result with mask.
 */
#define GEN_MASK_BITS(val, mask, sb) \
 (((val)<<(sb)) & (mask))
 
/*
 * Default SSP Register Values
 */
#define DEFAULT_SSP_REG_CR0 ( \
	GEN_MASK_BITS(SSP_DATA_BITS_12, SSP_CR0_MASK_DSS, 0)	| \
	GEN_MASK_BITS(SSP_INTERFACE_MOTOROLA_SPI, SSP_CR0_MASK_FRF, 4) | \
	GEN_MASK_BITS(SSP_CLK_POL_IDLE_LOW, SSP_CR0_MASK_SPO, 6) | \
	GEN_MASK_BITS(SSP_CLK_SECOND_EDGE, SSP_CR0_MASK_SPH, 7) | \
	GEN_MASK_BITS(SSP_DEFAULT_CLKRATE, SSP_CR0_MASK_SCR, 8) \
)

#define DEFAULT_SSP_REG_CR1 ( \
	GEN_MASK_BITS(LOOPBACK_DISABLED, SSP_CR1_MASK_LBM, 0) | \
	GEN_MASK_BITS(SSP_DISABLED, SSP_CR1_MASK_SSE, 1) | \
	GEN_MASK_BITS(SSP_MASTER, SSP_CR1_MASK_MS, 2) | \
	GEN_MASK_BITS(DO_NOT_DRIVE_TX, SSP_CR1_MASK_SOD, 3) \
)

#define DEFAULT_SSP_REG_CPSR ( \
	GEN_MASK_BITS(SSP_DEFAULT_PRESCALE, SSP_CPSR_MASK_CPSDVSR, 0) \
)

#define DEFAULT_SSP_REG_DMACR (\
	GEN_MASK_BITS(SSP_DMA_DISABLED, SSP_DMACR_MASK_RXDMAE, 0) | \
	GEN_MASK_BITS(SSP_DMA_DISABLED, SSP_DMACR_MASK_TXDMAE, 1) \
)

/*
 * SSP Status Register - SSP_SR
 */
#define SSP_SR_MASK_TFE		(0x1UL << 0) /* Transmit FIFO empty */
#define SSP_SR_MASK_TNF		(0x1UL << 1) /* Transmit FIFO not full */
#define SSP_SR_MASK_RNE		(0x1UL << 2) /* Receive FIFO not empty */
#define SSP_SR_MASK_RFF		(0x1UL << 3) /* Receive FIFO full */
#define SSP_SR_MASK_BSY		(0x1UL << 4) /* Busy Flag */

/*
 * SSP Clock Parameter ranges
 */
#define CPSDVR_MIN 0x02
#define CPSDVR_MAX 0xFE
#define SCR_MIN 0x00
#define SCR_MAX 0xFF

/*
 * SSP Regs base
 */
#define SSP_REGS_BASE 0x33100000

#define SPI_CLASSIC_RATE 100000000

/*
 * The type of reading going on on this chip
 */
enum ssp_reading {
	READING_NULL,
	READING_U8,
	READING_U16,
	READING_U32
};

/**
 * The type of writing going on on this chip
 */
enum ssp_writing {
	WRITING_NULL,
	WRITING_U8,
	WRITING_U16,
	WRITING_U32
};

/**
 * whether SSP is in loopback mode or not
 */
enum ssp_loopback {
	LOOPBACK_DISABLED,
	LOOPBACK_ENABLED
};

/**
 * whether SSP is in loopback mode or not
 */
enum ssp_bigend {
	LSB_FIRST_DISABLED,
	LSB_FIRST_ENABLED
};

/**
 * enum ssp_hierarchy - whether SSP is configured as Master or Slave
 */
enum ssp_hierarchy {
	SSP_MASTER,
	SSP_SLAVE
};

/**
 * enum ssp_data_size - number of bits in one data element
 */
enum ssp_data_size {
	SSP_DATA_BITS_4 = 0x03, SSP_DATA_BITS_5, SSP_DATA_BITS_6,
	SSP_DATA_BITS_7, SSP_DATA_BITS_8, SSP_DATA_BITS_9,
	SSP_DATA_BITS_10, SSP_DATA_BITS_11, SSP_DATA_BITS_12,
	SSP_DATA_BITS_13, SSP_DATA_BITS_14, SSP_DATA_BITS_15,
	SSP_DATA_BITS_16, SSP_DATA_BITS_17, SSP_DATA_BITS_18,
	SSP_DATA_BITS_19, SSP_DATA_BITS_20, SSP_DATA_BITS_21,
	SSP_DATA_BITS_22, SSP_DATA_BITS_23, SSP_DATA_BITS_24,
	SSP_DATA_BITS_25, SSP_DATA_BITS_26, SSP_DATA_BITS_27,
	SSP_DATA_BITS_28, SSP_DATA_BITS_29, SSP_DATA_BITS_30,
	SSP_DATA_BITS_31, SSP_DATA_BITS_32
};

/**
 * enum ssp_interface - interfaces allowed for this SSP Controller
 * @SSP_INTERFACE_MOTOROLA_SPI: Motorola Interface
 * @SSP_INTERFACE_TI_SYNC_SERIAL: Texas Instrument Synchronous Serial
 * interface
 * @SSP_INTERFACE_NATIONAL_MICROWIRE: National Semiconductor Microwire
 * interface
 * @SSP_INTERFACE_UNIDIRECTIONAL: Unidirectional interface (STn8810
 * &STn8815 only)
 */
enum ssp_interface {
	SSP_INTERFACE_MOTOROLA_SPI,
	SSP_INTERFACE_TI_SYNC_SERIAL,
	SSP_INTERFACE_NATIONAL_MICROWIRE,
	SSP_INTERFACE_UNIDIRECTIONAL
};

/**
 * enum SPI Clock Polarity - clock polarity (Motorola SPI interface only)
 * @SSP_CLK_POL_IDLE_LOW: Low inactive level
 * @SSP_CLK_POL_IDLE_HIGH: High inactive level
 */
enum ssp_spi_clk_pol {
	SSP_CLK_POL_IDLE_LOW,
	SSP_CLK_POL_IDLE_HIGH
};

/**
 * enum SPI Clock Phase - clock phase (Motorola SPI interface only)
 * @SSP_CLK_FIRST_EDGE: Receive data on first edge transition (actual direction depends on polarity)
 * @SSP_CLK_SECOND_EDGE: Receive data on second edge transition (actual direction depends on polarity)
 */
enum ssp_spi_clk_phase {
	SSP_CLK_FIRST_EDGE,
	SSP_CLK_SECOND_EDGE
};

enum {
	PL022_MAX_CMD_LEN		= 5,
};

/**
 * enum ssp_clock_params - clock parameters, to set SSP clock at a
 * desired freq
 */
struct ssp_clock_params {
	u8 cpsdvsr; /* value from 2 to 254 (even only!) */
	u8 scr;	    /* value from 0 to 255 */
};

struct pl022_spi_platdata {
	uint flags;
	uint speed_hz;
    	uint dma_tx;
    	uint dma_rx;
    	uint bus_clk;
	uint num_chipselect;
    	void __iomem *virtbase;
}; 

struct pl022_spi_priv {
	uint flags;
	uint speed_hz;
    	uint dma_tx;
    	uint dma_rx;
    	uint bus_clk;
	uint num_chipselect;
   	void __iomem *virtbase;
	const void *tx;
	const void *tx_end;
	void *rx;
	void	*rx_end;
	enum ssp_reading read;
	enum ssp_writing  write;
    	u32 exp_fifo_level;
    	uint fifodepth;
    	struct ssp_clock_params clk_freq;
	int spi_mode;
};

#endif

