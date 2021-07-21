/*
 * (C) Copyright 2004-2018 Centec Networks (suzhou) Co., LTD.
 * Liuht <liuht@centecnetworks.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef __E530_24X_H
#define __E530_24X_H

#include "ctc5236_common.h"

/* DDR Controller Registers config */
/**************************************************************************/

/* DDR config */
#define CONFIG_NR_DRAM_BANKS		1
#define CONFIG_SYS_SDRAM_BASE		0x80000000
#define CONFIG_SYS_SDRAM_SIZE		0x40000000UL
#define CONFIG_SYS_LOAD_ADDR		(CONFIG_SYS_SDRAM_BASE + \
					16 * 1024 * 1024)

#undef CONFIG_RESERVE_MEMORY
#define CONFIG_RESERVE_MEMORY_MIN		DDR_RESERVE_MEMORY_1KB
#define CONFIG_RESERVE_MEMORY_START		RESERVE_MEMORY_OFFSET_1GB
#define CONFIG_RESERVE_MEMORY_SIZE 		RESERVE_MEMORY_SIZE_32MB

#undef CONFIG_SYS_DRAM_TEST		           /* memory test, takes time */
#define CONFIG_SYS_MEMTEST_START	   0x80000000	/* memtest region */
#define CONFIG_SYS_MEMTEST_END        0x80100000
/**************************************************************************/

/* PL011 Serial Configuration */
/**************************************************************************/
#define CONFIG_PL01X_SERIAL
#define CONFIG_PL011_SERIAL
#define CONFIG_SYS_SERIAL0		0x33000000
#define CONFIG_PL011_CLOCK		20000000
#define CONFIG_CONS_INDEX		0
#define CONFIG_PL01x_PORTS		{(void *)CONFIG_SYS_SERIAL0}
/**************************************************************************/

/* USB Config */
/**************************************************************************/
#define CONFIG_SYS_USB_OHCI_MAX_ROOT_PORTS	1
#define CONFIG_USB_OHCI_NEW		1
/**************************************************************************/

/* Timer config */
/**************************************************************************/
#define COUNTER_FREQUENCY		20000000 
/**************************************************************************/

/* MTD device config */
/**************************************************************************/
#define CONFIG_CMD_MTDPARTS
#define CONFIG_MTD_DEVICE		    /* needed for mtdparts commands */

#define MTDIDS_DEFAULT                  "nor0=spi-flash.0"
#define MTDPARTS_DEFAULT             	"spi-flash.0:" \
				     					"3M(u-boot)," \
				    	 				"64k(uboot parameters)," \
				     					"-(system)"
/**************************************************************************/

/* EEPROM configuration */
/**************************************************************************/			
#define CONFIG_SYS_I2C_EEPROM_ADDR		              0x57
#define CONFIG_SYS_I2C_EEPROM_ADDR_LEN		       0x02
#define CONFIG_I2C_ENV_EEPROM_BUS                           0
#define CONFIG_I2C_EEPROM                                          1
#define CONFIG_CMD_EEPROM                                         1
#define CONFIG_SYS_EEPROM_PAGE_WRITE_BITS           5  /* 32-byte page */
#define CONFIG_SYS_EEPROM_PAGE_WRITE_DELAY_MS  10
/**************************************************************************/

/* Environment settings */
/**************************************************************************/
#define CONFIG_ENV_IS_IN_SPI_FLASH
#define CONFIG_ENV_SIZE			0x10000
#define CONFIG_ENV_SECT_SIZE	0x10000
#define CONFIG_ENV_OFFSET		0x300000
#define CONFIG_ENV_OVERWRITE

#define CONFIG_ENV_SPI_CS		0
#define CONFIG_ENV_SPI_BUS		1
#define CONFIG_ENV_SPI_MAX_HZ		25000000
#define CONFIG_ENV_SPI_MODE		SPI_MODE_3
#if 0
#define CONFIG_ENV_IS_IN_EEPROM
#define CONFIG_ENV_SIZE			0x1000
#define CONFIG_ENV_OFFSET		0
#define CONFIG_ENV_OVERWRITE
#endif
/**************************************************************************/

/* PCIE Configs */
/**************************************************************************/
#define CONFIG_CMD_PCI
#define CONFIG_PCI_SCAN_SHOW
/**************************************************************************/

/* WatchDog config */
/**************************************************************************/
#define CONFIG_WDT0
#define CONFIG_WDT0_BASE                0x33500000
#define CONFIG_WDT1_BASE                0x33501000
#ifdef CONFIG_WDT0
#define CONFIG_WDT_BASE			CONFIG_WDT0_BASE
#else
#define CONFIG_WDT_BASE			CONFIG_WDT1_BASE
#endif
#define CONFIG_WDT_CLOCK_KHZ		1000
#define CONFIG_HW_WATCHDOG_TIMEOUT_MS	10
/**************************************************************************/

/* emmc configs */
/**************************************************************************/
#define CTC5236_SDHC_MIN_FREQ	        400000
#define CONFIG_EMMC_BASE                0x30400000
#define CONFIG_EMMC_MAX_CLK             100000000 
#define CONFIG_SUPPORT_EMMC_BOOT
#define CTC5236_SDHCI_HOST_CAPS			MMC_MODE_8BIT 
#define CTC5236_SDHCI_HOST_ADMA64
/**************************************************************************/

/* I2C SYSMON (LM77) config */
/**************************************************************************/
#define CONFIG_DTT_LM77
#define CONFIG_CMD_DTT
#define CONFIG_SYS_I2C_DTT_ADDR 0x48
#define CONFIG_SYS_DTT_BUS_NUM	0	/* The I2C bus for DTT		*/
#define CONFIG_DTT_SENSORS	{ 0 }
/**************************************************************************/

/* I2C RTC config */
/**************************************************************************/
#define CONFIG_SYS_RTC_BUS_NUM 0
/**************************************************************************/

/* i2c config */
/**************************************************************************/
#define IC_CLK			50
/**************************************************************************/

/* cpumac config */
/**************************************************************************/
#define CONFIG_CTC_MAC
#define CONFIG_UDP_CHECKSUM 1
/**************************************************************************/

/* hardware independent config */
/**************************************************************************/
#define CONFIG_SYS_MALLOC_LEN		      (CONFIG_ENV_SIZE + 512 * 1024)

#define CONFIG_IMAGE_FORMAT_LEGACY

#ifdef CONFIG_SYSRESET
#define CONFIG_PANIC_HANG		/* do not reset board on panic */
#endif
/**************************************************************************/

/* PWM Gpio Multi */
/**************************************************************************/
#define PWM_GpioMulti_MASK 0x3fcff
#define PWM_GpioMulti 0xaaa80200
/**************************************************************************/
#define CONFIG_SYS_ALT_MEMTEST

/* Debug config */
/**************************************************************************/
#define CONFIG_CMD_SF_TEST
/**************************************************************************/

/* set bootm uimage size */
/**************************************************************************/
#define CONFIG_SYS_BOOTM_LEN 0x20000000               /*  512M*/
/**************************************************************************/

/* board lata init config */
/**************************************************************************/
#define CONFIG_BOARD_LATE_INIT
/**************************************************************************/

/* extra env settings config */
/**************************************************************************/
#define CONFIG_EXTRA_ENV_SETTINGS  "uImageboot=bootm ${loadaddr}#e530-48x\0" \
									"uImageload=ext4load mmc 0:2 ${loadaddr} uImage\0" \
									"product_series=e530\0" \
									"board_type=48x\0" \
									"serialno=N/A\0" \
									"eth=ethernet@33410000\0" \
									"eth1=ethernet@33420000\0" \
									"ethact=ethernet@33410000\0" \
									"mtdparts=" MTDPARTS_DEFAULT "\0"
/**************************************************************************/

/* spi gpio multi config */
/**************************************************************************/
#define SPI_GpioMulti 0x451
/**************************************************************************/
#endif
