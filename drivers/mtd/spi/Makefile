#
# (C) Copyright 2006
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# SPDX-License-Identifier:	GPL-2.0+
#

obj-$(CONFIG_DM_SPI_FLASH) += sf-uclass.o

ifdef CONFIG_SPL_BUILD
obj-$(CONFIG_SPL_SPI_BOOT)	+= fsl_espi_spl.o
obj-$(CONFIG_SPL_SPI_SUNXI)	+= sunxi_spi_spl.o
obj-$(CONFIG_SPL_SPI_CTC5236) += ctc_qspi_spl.o
obj-$(CONFIG_SPI_FLASH) += spi_flash.o spi_flash_ids.o sf.o
obj-$(CONFIG_SPI_FLASH_DATAFLASH) += sf_dataflash.o
else
obj-$(CONFIG_SPI_FLASH) += sf_probe.o spi_flash.o spi_flash_ids.o sf.o
obj-$(CONFIG_SPI_FLASH_DATAFLASH) += sf_dataflash.o
obj-$(CONFIG_SPI_FLASH_MTD) += sf_mtd.o
obj-$(CONFIG_SPI_FLASH_SANDBOX) += sandbox.o
endif
