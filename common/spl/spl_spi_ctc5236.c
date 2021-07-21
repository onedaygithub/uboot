/*
 * (C) Copyright 2004-2017 Centec Networks (suzhou) Co., LTD.
 * Jay Cao <caoj@centecnetworks.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <spl.h>
#include <ctc5236image.h>
#include <spl_ctc5236.h>
#include <image-verify.h>
#include <ctcefuse.h>

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
#ifdef CONFIG_CTC_SEC_BOOT
static int qspi_secure_boot_flow(struct spl_image_info *spl_image)
{
	u8 nv;
	u8 keyindex;
	u32 data;
	u8 hash[CER_HASH_LEN], sochash[CER_HASH_LEN];
	int ret;
	ulong uboot_offset;
	//ulong getdcrc;
	ctc5236_ih_t *hdr;
	struct ctc_cert *cert;

	cert = (struct ctc_cert *)(CONFIG_SYS_TEXT_BASE);
	hdr  = (ctc5236_ih_t *)(CONFIG_SYS_TEXT_BASE+CTC5236_CERT_SIZE);
	data = (CONFIG_SYS_TEXT_BASE+CTC5236_CERT_SIZE+CTC5236_IH_SIZE);

	uboot_offset = CONFIG_SYS_SPI_SEC_CERT_OFFS;
	qspi_read_data((void *)cert, uboot_offset, CTC5236_CERT_SIZE);

	efuse_get_nv2(&nv);    
	GRTR_RETURN(nv, cert->nv);
	NEGA_RETURN(ctc_get_pubkey_hash(cert, hash));
	ctc_load_pkey_hash(sochash);
	NONZ_RETURN(memcmp(sochash, hash, CER_HASH_LEN));    
    
	qspi_read_data((void *)hdr, uboot_offset+CTC5236_CERT_SIZE, cert->img_size);
	if(cert->encrypt){
		u8 *aes_key;
		efuse_get_sbhk(&aes_key);
		ctc_aes_decrypt(aes_key, (u8 *)hdr, (u8 *)hdr, cert->img_size);
	}

	NEGA_RETURN(ctc_get_image_hash((u8 *)hdr, cert->img_size, hash));
	NEGA_RETURN(ctc_get_keyindex(cert, &keyindex));
	NEGA_RETURN(ctc_verify_sign(cert, hash, keyindex));
    
	ret = ctc5236_spl_parse_ih(spl_image, hdr);
	NEGA_RETURN(ret);

#ifdef VELOCE_PLATFORM
	/* For speeding up bootup speed on mentor veloce platform,
	 * here we don't move the rest qspi data to DDR just parse 
	 * the image hader (0x40 bytes), and then directly jump to 
	 * DDR, so need use backdoor to write image to DDR
	 */
	return 0;
#endif
	//getdcrc = ctc5236_image_get_dcrc(hdr);
	qspi_read_data((void *)spl_image->load_addr, data,
		spl_image->size);

	//ret = ctc5236_image_check_dcrc(getdcrc, spl_image);
	//ZERO_RETURN(ret);

	 return 0;
}
#else
static int qspi_normal_boot_flow(struct spl_image_info *spl_image)
{
	int ret;
	ulong uboot_offset;
	ulong getdcrc;
	ctc5236_ih_t *hdr;
	hdr = (ctc5236_ih_t *)(CONFIG_SYS_TEXT_BASE);

	uboot_offset = CONFIG_SYS_SPI_U_BOOT_OFFS;
	qspi_read_data((void *)hdr, uboot_offset, CTC5236_IH_SIZE);

	NEGA_RETURN(ctc5236_spl_parse_ih(spl_image, hdr));
#ifdef VELOCE_PLATFORM
	/* For speeding up bootup speed on mentor veloce platform,
	 * here we don't move the rest qspi data to DDR just parse 
	 * the image hader (0x40 bytes), and then directly jump to 
	 * DDR, so need use backdoor to write image to DDR
	 */
	return 0;
#endif
	getdcrc = ctc5236_image_get_dcrc(hdr);
	qspi_read_data((void *)spl_image->load_addr, uboot_offset + CTC5236_IH_SIZE,
		spl_image->size);
#ifndef CONFIG_DIRECT_QSPI
	/* While direct qspi boot, do data crc check very slowly ,
	 * for speeding up bootup here we don't check data crc
	 * when direct qspi boot mode.
	 */
	ret = ctc5236_image_check_dcrc(getdcrc, spl_image);
	ZERO_RETURN(ret);
#if 0
	printf("sram spl check data crc waiting  1.....\n");

	for(ret=0;ret< 10;ret++)
	{
		mdelay(1000);
	}

	printf("sram spl check data crc waiting  2.....\n");
#endif

#endif
	return 0;
}
#endif

static int spl_qspi_load_image(struct spl_image_info *spl_image,
            struct spl_boot_device *bootdev)
{    
#ifdef CONFIG_CTC_SEC_BOOT
	return qspi_secure_boot_flow(spl_image);
#else
	return qspi_normal_boot_flow(spl_image);
#endif

	return 0;
}

SPL_LOAD_IMAGE_METHOD("QSPI", 0, BOOT_DEVICE_SPI, spl_qspi_load_image);
