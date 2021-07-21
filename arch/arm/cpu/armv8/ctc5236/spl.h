/*
 *Copyright 2019 CENTEC NETWORKS, Inc
 *   
 *SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __CENTEC_BOARD_SPL_H
#define __CENTEC_BOARD_SPL_H

#define  CTC_REMOVE_FLASH_SIZE 102400     /*  top 100k size of flash */

struct spi_flash *ctc_spi_flash_probe(unsigned int busnum, unsigned int cs,
				  unsigned int max_hz, unsigned int spi_mode);

int ctc_spi_flash_cmd_erase_ops(struct spi_flash *flash, u32 offset, size_t len);
#endif

