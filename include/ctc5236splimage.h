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

#ifndef __CTC5236_SPL_IMAGE_H__
#define __CTC5236_SPL_IMAGE_H__

#define CTC5236_EFUSE_VOL_SWITCH_MDELAY   500

#define CTC5236_MMC_BOOT_START 0x0
#define CTC5236_SD_BOOT_START  0x8
#define CTC5236_IH_SIZE       0x040
#define CTC5236_IH_MAGIC	  0x28062057

#define CTC5236_CFG_UNIT         24
#define CTC5236_CFG_SIZE_MAX     0xfc0
#define CTC5236_CFG_CNT_MAX      (CTC5236_CFG_SIZE_MAX/CTC5236_CFG_UNIT)

#define CTC5236_SPL_DATA_OFFSET  0x1000

#define CTC5236_GET_CFG_SIZE(ccnt) (ccnt * CTC5236_CFG_UNIT)

#define CFG_WIDTH_BIT_OFFSET    0x0
#define CFG_WIDTH_BIT_MASK      0x7
#define CFG_ACCESS_MODE_OFFSET  0x3
#define CFG_ACCESS_MODE_MASK    0x1
#define CFG_DELAY_BIT_OFFSET    0x4
#define CFG_DELAY_BIT_MASK      0x1

typedef enum
{
    CFG_WIDTH_1BYTE=0,
    CFG_WIDTH_2BYTES,
    CFG_WIDTH_4BYTES,
    CFG_WIDTH_8BYTES,
    CFG_WIDTH_MAX
}cfg_width_t;

typedef enum
{
    CFG_ACCESS_RD=0,
    CFG_ACCESS_WR,
    CFG_ACCESS_MAX,
}cfg_access_t;

typedef enum
{
    CFG_NOT_DELAY=0,
    CFG_DELAY,
    CFG_DELAY_MAX
}cfg_delay_t;

typedef struct ctc5236_spl_cfg_s {
	__be64		reg;
	__be64		val;
	__be32		att;
    __be32		dly;
} ctc5236_spl_cfg_t;

/* CTC5236 On-chip ROM will parse this header */
typedef struct ctc5236_spl_ih_s {
	__be32		ih_magic;	    /* Image Header Magic Number  */
	__be32		ih_hcrc;	    /* Image Header CRC Checksum  */
	__be32		ih_size;	    /* Image Data Size		      */
	__be32		ih_load;	    /* Data	 Load  Address		  */
	__be32		ih_ep;		    /* Entry Point Address		  */
	__be32		ih_dcrc;	    /* Image Data CRC Checksum	  */
    __be32		ih_ccnt;        /* cfg count */
    __be32		ih_ccrc;        /* cfg CRC Checksum */
    __be32		ih_msec; 
	uint8_t		reserve[28];
} ctc5236_spl_ih_t;

#define ctc5236_image_get_hdr_l(f) \
	static inline uint32_t ctc5236_image_get_##f(const ctc5236_spl_ih_t *hdr) \
	{ \
		return uimage_to_cpu(hdr->ih_##f); \
	}

ctc5236_image_get_hdr_l(magic)		/* ctc5236_image_get_magic */
ctc5236_image_get_hdr_l(hcrc)		/* ctc5236_image_get_hcrc */
ctc5236_image_get_hdr_l(load)		/* ctc5236_image_get_load */
ctc5236_image_get_hdr_l(size)		/* ctc5236_image_get_size */
ctc5236_image_get_hdr_l(ep)		    /* ctc5236_image_get_ep */
ctc5236_image_get_hdr_l(dcrc)		/* ctc5236_image_get_dcrc */
ctc5236_image_get_hdr_l(ccnt)		/* ctc5236_image_get_ccnt */
ctc5236_image_get_hdr_l(ccrc)		/* ctc5236_image_get_ccrc */
ctc5236_image_get_hdr_l(msec)    /* ctc5236_image_get_msec */

#define ctc5236_image_set_hdr_l(f) \
	static inline void ctc5236_image_set_##f(ctc5236_spl_ih_t *hdr, uint32_t val) \
	{ \
		hdr->ih_##f = cpu_to_uimage(val); \
	}
ctc5236_image_set_hdr_l(magic)		/* ctc5236_image_set_magic */
ctc5236_image_set_hdr_l(hcrc)		/* ctc5236_image_set_hcrc */
ctc5236_image_set_hdr_l(load)		/* ctc5236_image_set_load */
ctc5236_image_set_hdr_l(size)		/* ctc5236_image_set_size */
ctc5236_image_set_hdr_l(ep)		    /* ctc5236_image_set_ep */
ctc5236_image_set_hdr_l(dcrc)		/* ctc5236_image_set_dcrc */
ctc5236_image_set_hdr_l(ccnt)		/* ctc5236_image_set_ccnt */
ctc5236_image_set_hdr_l(ccrc)	      /* ctc5236_image_set_ccrc */
ctc5236_image_set_hdr_l(msec)      /* ctc5236_image_set_msec */

#endif


