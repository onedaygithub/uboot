/*
 * (C) Copyright 2017 Centec
 *
 * (C) Copyright 2017-2018
 * Liuht Software Engineering, liuht@centecnetworks.com.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 ********************************************************************
 * NOTE: This header file defines an interface to U-Boot. Including
 * this (unmodified) header file in another file is considered normal
 * use of U-Boot, and does *not* fall under the heading of "derived
 * work".
 ********************************************************************
 */

#ifndef __CTC5236_IMAGE_H__
#define __CTC5236_IMAGE_H__

/* CTC5236 On-chip ROM will parse this header */
typedef struct ctc5236_ih_s {
	__be32		ih_magic;	    /* Image Header Magic Number  */
	__be32		ih_hcrc;	    /* Image Header CRC Checksum  */
	__be32		ih_size;	    /* Image Data Size		      */
	__be32		ih_load;	    /* Data	 Load  Address		  */
	__be32		ih_ep;		    /* Entry Point Address		  */
	__be32		ih_dcrc;	    /* Image Data CRC Checksum	  */
	uint8_t		reserve[40];	
} ctc5236_ih_t;

#define CTC5236_MMC_BOOT_START 0x0
#define CTC5236_SD_BOOT_START  0x8
#define CTC5236_IH_SIZE     0x40
#define CTC5236_IH_MAGIC	0x28062057	/* Image Magic Number		*/

#define ctc5236_image_get_hdr_l(f) \
	static inline uint32_t ctc5236_image_get_##f(const ctc5236_ih_t *hdr) \
	{ \
		return uimage_to_cpu(hdr->ih_##f); \
	}

ctc5236_image_get_hdr_l(magic)		/* ctc5236_image_get_magic */
ctc5236_image_get_hdr_l(hcrc)		/* ctc5236_image_get_hcrc */
ctc5236_image_get_hdr_l(load)		/* ctc5236_image_get_load */
ctc5236_image_get_hdr_l(size)		/* ctc5236_image_get_size */
ctc5236_image_get_hdr_l(ep)		    /* ctc5236_image_get_ep */
ctc5236_image_get_hdr_l(dcrc)		/* ctc5236_image_get_dcrc */

#define ctc5236_image_set_hdr_l(f) \
	static inline void ctc5236_image_set_##f(ctc5236_ih_t *hdr, uint32_t val) \
	{ \
		hdr->ih_##f = cpu_to_uimage(val); \
	}
ctc5236_image_set_hdr_l(magic)		/* ctc5236_image_set_magic */
ctc5236_image_set_hdr_l(hcrc)		/* ctc5236_image_set_hcrc */
ctc5236_image_set_hdr_l(load)		/* ctc5236_image_set_load */
ctc5236_image_set_hdr_l(size)		/* ctc5236_image_set_size */
ctc5236_image_set_hdr_l(ep)		    /* ctc5236_image_set_ep */
ctc5236_image_set_hdr_l(dcrc)		/* ctc5236_image_set_dcrc */

enum {
	CTC_SPL_PASS,
    CTC_SPL_IO_ERROR,
	CTC_SPL_MAGIC_ERROR, 
	CTC_SPL_HCRC_ERROR,			
	CTC_SPL_DCRC_ERROR,						
};

int ctc5236_image_check_dcrc(ulong getdcrc, struct spl_image_info *spl_image);
int ctc5236_image_check_hcrc(const ctc5236_ih_t *hdr);

#endif
