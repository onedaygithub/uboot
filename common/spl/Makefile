#
# (C) Copyright 2012
# Texas Instruments Incorporated - http://www.ti.com/
# Aneesh V <aneesh@ti.com>
#
# SPDX-License-Identifier:	GPL-2.0+
#
# Based on common/Makefile.
#

ifdef CONFIG_SPL_BUILD
obj-$(CONFIG_SPL_FRAMEWORK) += spl.o
obj-$(CONFIG_SPL_LOAD_FIT) += spl_fit.o
obj-$(CONFIG_SPL_NOR_SUPPORT) += spl_nor.o
ifndef CONFIG_SPL_UBI
obj-$(CONFIG_SPL_NAND_SUPPORT) += spl_nand.o
obj-$(CONFIG_SPL_ONENAND_SUPPORT) += spl_onenand.o
endif
obj-$(CONFIG_SPL_UBI) += spl_ubi.o
obj-$(CONFIG_SPL_NET_SUPPORT) += spl_net.o
ifndef CONFIG_SPL_CTC5236
obj-$(CONFIG_SPL_MMC_SUPPORT) += spl_mmc.o
obj-$(CONFIG_SPL_SPI_LOAD) += spl_spi.o
obj-$(CONFIG_SPL_YMODEM_SUPPORT) += spl_ymodem.o
else
obj-y += spl_ctc5236.o
obj-$(CONFIG_SPL_MMC_SUPPORT) += spl_mmc_ctc5236.o
obj-$(CONFIG_SPL_SPI_CTC5236) += spl_spi_ctc5236.o
obj-$(CONFIG_SPL_YMODEM_SUPPORT) += spl_ymodem_ctc5236.o
endif
obj-$(CONFIG_SPL_USB_SUPPORT) += spl_usb.o
obj-$(CONFIG_SPL_FAT_SUPPORT) += spl_fat.o
obj-$(CONFIG_SPL_EXT_SUPPORT) += spl_ext.o
obj-$(CONFIG_SPL_SATA_SUPPORT) += spl_sata.o
obj-$(CONFIG_SPL_DFU_SUPPORT) += spl_dfu.o
obj-$(CONFIG_SPL_RAM_SUPPORT) += spl_ram.o
endif
