/*
 * (C) Copyright 2004-2017 Centec Networks (suzhou) Co., LTD.
 * Jay Cao <caoj@centecnetworks.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <spl.h>

#ifdef CONFIG_SPL_OS_BOOT
#error CONFIG_SPL_OS_BOOT is not supported yet
#endif

static void qspi_init(void)
{

}

#define SPI_READ_MAX_SIZE 64

static void qspi_read_data(void *buf, u32 addr, u32 len)
{
	u8 *buf8 = buf;
	u32 chunk_len;

	while (len > 0) {
		chunk_len = len;
		if (chunk_len > SPI_READ_MAX_SIZE)
			chunk_len = SPI_READ_MAX_SIZE;
		
		memcpy((void *)buf8, (void *)addr, chunk_len);

		len  -= chunk_len;
		buf8 += chunk_len;
		addr += chunk_len;
	}
}

/*****************************************************************************/

static int spl_qspi_load_image(struct spl_image_info *spl_image,
			      struct spl_boot_device *bootdev)
{
	int err;
	struct image_header *header;
	header = (struct image_header *)(CONFIG_SYS_MONITOR_BASE);

	spl_image->flags |= SPL_COPY_PAYLOAD_ONLY;
    
	qspi_init();

	qspi_read_data((void *)header, CONFIG_SYS_SPI_U_BOOT_OFFS, 0x40);
	err = spl_parse_image_header(spl_image, header);
	if (err)
		return err;

#ifdef VELOCE_PLATFORM
    /* For speeding up bootup speed on mentor veloce platform,
     * here we don't move the rest qspi data to DDR just parse 
     * the image hader (0x40 bytes), and then directly jump to 
     * DDR, so need use backdoor to write image to DDR
     */
	return 0;
#endif
	qspi_read_data((void *)spl_image->load_addr, CONFIG_SYS_SPI_U_BOOT_OFFS + 0x40,
		       spl_image->size);

	return 0;
}

SPL_LOAD_IMAGE_METHOD("QSPI", 0, BOOT_DEVICE_SPI, spl_qspi_load_image);
