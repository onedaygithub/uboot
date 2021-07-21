/*
 * ARM PL022 Module Defines
 *
 * Copyright (C) 2004-2017, 2017 ARM Semiconductor, Inc.
 * Liuht (liuht@centecnetworks.com)
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef _CTC_QSPI_H_
#define _CTC_QSPI_H_

#define QSPI_BOOT_EN(V)            ((V) & 1)
#define QSPI_XIP_EN(V)             (((V) & 1) << 1)
#define QSPI_BOOTEN_CPHA(V)        (((V) & 1) << 4)
#define QSPI_BOOTEN_CPOL(V)        (((V) & 1) << 5)
#define QSPI_BOOT_HOLD(V)          (((V) & 1) << 6)
#define QSPI_BOOT_WP(V)            (((V) & 1) << 7)
#define QSPI_BOOT_CHIP_SEL(V)      (((V) & 7) << 8)
#define QSPI_BOOT_RXMODE(V)        (((V) & 7) << 12)
#define QSPI_BOOT_CLK_DIV(V)       (((V) & 0xff) << 16)

#define QSPI_BOOT_ADDR_CYCLE(V)    (((V) & 0x3f) << 0)
#define QSPI_BOOT_ADDR_MODE(V)     (((V) & 0x07) << 8)
#define QSPI_BOOT_CMD_MODE(V)      (((V) & 0x07) << 12)
#define QSPI_BOOT_CMD_CYCLE(V)     (((V) & 0x1f) << 16)
#define QSPI_BOOT_CMD_CODE(V)      (((V) & 0xff) << 24)

#define QSPI_BOOT_DUMMY_CYCLE(V)   (((V) & 0x3f) << 0)
#define QSPI_BOOT_DUMMY_MODE(V)    (((V) & 0x7)  << 8)

#define QSPI_BOOT_DUMMY_CODE(V)    ((V) & 0xffffffff)

#define TIMEOUT_COUNT 65535

/* default SCK frequency, unit: HZ */
#define CTC_QSPI_DEFAULT_SCK_FREQ	500000000

#define CTC_IDLE_CYCLE 0x2
#define CTC_PRE_CYCLE 0x2
#define CTC_POST_CYCLE 0x1
#define CTC_SPI_MAX_FREQUENCY 25000000


#define CMD_PAGE_PROGRAM		    0x02
#define CMD_QUAD_PAGE_PROGRAM		0x32

#define CMD_READ_ARRAY_SLOW		    0x03
#define CMD_READ_ARRAY_FAST		    0x0b
#define CMD_READ_DUAL_OUTPUT_FAST	0x3b
#define CMD_READ_DUAL_IO_FAST		0xbb
#define CMD_READ_QUAD_OUTPUT_FAST	0x6b
#define CMD_READ_QUAD_IO_FAST		0xeb

#define SPI_MODE            0x00

#define PIO_MODE_1          0x01
#define PIO_MODE_2          0x02

#define PIO_BASE(NR)        (0x20*(NR-1))
#define PIO_GO(NR)          (0x10 + PIO_BASE(NR))
#define PIO_CTRL(NR)        (0x14 + PIO_BASE(NR))
#define PIO_STEP0_CONF(NR)  (0x20 + PIO_BASE(NR))
#define PIO_STEP1_CONF(NR)  (0x24 + PIO_BASE(NR))
#define PIO_STEP2_CONF(NR)  (0x28 + PIO_BASE(NR))
#define PIO_STEP3_CONF(NR)  (0x2c + PIO_BASE(NR))

#define XIP_CTRL            0x54
#define XIP_CMD_CODE        0x60
#define XIP_CMD_CONF        0x64
#define XIP_ADDR_CONF       0x6c
#define XIP_DUMMY_CODE      0x70
#define XIP_DUMMY_CONF      0x74
#define XIP_DATA_CONF       0X78

#define PP_GO               0x80
#define PP_CTRL             0x84
#define PP_CMD_CODE         0x90
#define PP_CMD_CONF         0x94
#define PP_ADDR_CODE        0x98
#define PP_ADDR_CONF        0X9c
#define PP_DUMMY_CODE       0xa0
#define PP_DUMMY_CONF       0xa4
#define PP_DATA_CONF        0xa8

#define CTC_QSPI_TX_BUFF         0x100
#define CTC_QSPI_RX_BUFF         0x180

#define CTC_QSPI_RX_BUFFER_SIZE		0x80
#define CTC_QSPI_TX_BUFFER_SIZE		0x80

#define CTRL_IDLE_CYCLE(V)      (((V)&3)<<24)       
#define CTRL_PRE_CYCLE(V)       (((V)&3)<<22)
#define CTRL_POST_CYCLE(V)      (((V)&3)<<20)
#define CTRL_SCLK_DEFAULT(V)    (((V)&1)<<19) 
#define CTRL_SOUT3_DEFAULT(V)   (((V)&1)<<18)
#define CTRL_SOUT2_DEFAULT(V)   (((V)&1)<<17)
#define CTRL_SOUT1_DEFAULT(V)   (((V)&1)<<16)
#define CTRL_CS(V)              (((V)&0xff)<<8)
#define CTRL_DIV_SCLK(V)        (((V)&0xff)<<0)

#define QSPI_MEMORY_OFFSET       0x10000000
#define QSPI_NUM_CS 0x2


/* Common status */
#define STATUS_WIP			BIT(0)
#define STATUS_QEB_WINSPAN		BIT(1)
#define STATUS_QEB_MXIC			BIT(6)
#define STATUS_PEC			BIT(7)
#define SR_BP0				BIT(2)  /* Block protect 0 */
#define SR_BP1				BIT(3)  /* Block protect 1 */
#define SR_BP2				BIT(4)  /* Block protect 2 */

#define CMD_READ_STATUS			0x05

#define QSPI_REGS_BASE 0x10000000

#define MAX_SPEED_HZ 25000000

struct ctc_qspi_platdata {
	uint flags;
	uint speed_hz;
    uint bus_clk;
	uint num_chipselect;
    uint idlecycle;
    uint precycle;
    uint postcycle;
    uint qspi_mode;
    uint pio;
    void __iomem *virtbase;
}; 

struct ctc_cadd {
    u8  command;         /* command code, only 1 byte */
    u8  cmdlanes;        /* number of lanes used for command code */
    u32 address;         /* address code */
    u8  addrlen;         /* address code length, only support 3-bytes address mode */
    u8  addrlanes;       /* number of lanes used for address code */
    u8  dummylen;        /* dummy length */
    u8  dummylanes;      /* number of lanes used for dummy code */
    u8  datalen;         /* data length */
    u8  datalanes;       /* number of lanes used for data */
};

struct ctc_spi_transfer {
	void	*tx_buf;
	void		       *rx_buf;
	unsigned	       len;
};

struct ctc_qspi_priv {
	uint flags;
	uint speed_hz;
    uint bus_clk;
	uint num_chipselect;
    uint idlecycle;
    uint precycle;
    uint postcycle;
    uint sclkdef;
    uint clkdiv;
    uint cs;
    uint sout1def;
    uint sout2def;
    uint sout3def;
    uint qspi_mode;
    u32 tx_entry;
    u32 ctrl;
    u8  step;
    u8  pio;
    void __iomem *virtbase;
    struct ctc_cadd cadd;
    struct ctc_spi_transfer xfer[2];
};

enum ctc_qspi_mode {
	PIO_MODE1=1,
	PIO_MODE2,
	PP_MODE,
	BOOT_MODE,
	XIP_MODE,
	MODE_MAX
};

#endif

