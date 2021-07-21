/*
 * (C) Copyright 2004-2017 Centec Networks (suzhou) Co., LTD.
 * Liuht <liuht@centecnetworks.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <spl.h>
#include <ctc5236image.h>
#include <spl_ctc5236.h>

int ctc5236_image_check_hcrc(const ctc5236_ih_t *hdr)
{
    ulong hcrc;
    ctc5236_ih_t hdrcp;
    
    memmove((char *)&hdrcp, (char *)hdr, CTC5236_IH_SIZE);
    ctc5236_image_set_hcrc(&hdrcp, 0);
    hcrc = crc32(0, (unsigned char *)&hdrcp, CTC5236_IH_SIZE);
    
    return (hcrc == ctc5236_image_get_hcrc(hdr));
}

int ctc5236_image_check_dcrc(ulong getdcrc, struct spl_image_info *spl_image)
{
    ulong dcrc = crc32(0, (unsigned char *)spl_image->load_addr, spl_image->size);

    return (dcrc == getdcrc);
}

#ifdef CONFIG_CTC_SEC_BOOT
int ctc5236_spl_parse_ih(struct spl_image_info *spl_image, 
    const ctc5236_ih_t *hdr)
{
    spl_image->load_addr = ctc5236_image_get_load(hdr);
    spl_image->entry_point = ctc5236_image_get_ep(hdr);
    spl_image->size = ctc5236_image_get_size(hdr);

    return 0;
}
#else
int ctc5236_spl_parse_ih(struct spl_image_info *spl_image, 
    const ctc5236_ih_t *hdr)
{
    int ret = 0;
    
    if (ctc5236_image_get_magic(hdr) == CTC5236_IH_MAGIC) 
    {
        ret = ctc5236_image_check_hcrc(hdr);
        ZERO_RETURN(ret);
            
        spl_image->load_addr = ctc5236_image_get_load(hdr);
        spl_image->entry_point = ctc5236_image_get_ep(hdr);
        spl_image->size = ctc5236_image_get_size(hdr);
    } 
    else 
    {
        return -1;
    }

    return 0;
}
#endif
void ctc_memcpy_data(void *buf, u32 addr, u32 len)
{
	u8 *buf8 = buf;
	u32 chunk_len;

	while (len > 0) {
		chunk_len = len;
		if (chunk_len > 64){
			chunk_len = 64;
		}

		memcpy((void *)buf8, (void *)addr, chunk_len);

		len  -= chunk_len;
		buf8 += chunk_len;
		addr += chunk_len;
	}
}

