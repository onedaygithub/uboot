/*
 * (C) Copyright 2017 Centec
 *
 * (C) Copyright 2017-2018
 * Centec Software Engineering
 * Liuht, liuht@centecnetworks.com
 *
 *
 * All rights reserved.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include "imagetool.h"
#include "mkimage.h"

#include <image.h>
#include <u-boot/crc.h>
#include <ctc5236splimage.h>

static ctc5236_spl_cfg_t ctc5236_cfg[] = {

{0x33200018, 0xffffffff,  CFG_WIDTH_4BYTES|(CFG_ACCESS_WR<<CFG_ACCESS_MODE_OFFSET), 1},
{0x33200018, 0x00000000,  CFG_WIDTH_4BYTES|(CFG_ACCESS_WR<<CFG_ACCESS_MODE_OFFSET), 0},

{0x30600000, 0x01002010,  CFG_WIDTH_4BYTES|(CFG_ACCESS_WR<<CFG_ACCESS_MODE_OFFSET), 0},
{0x30600004, 0x0008002e,  CFG_WIDTH_4BYTES|(CFG_ACCESS_WR<<CFG_ACCESS_MODE_OFFSET), 0},
{0x30600008, 0x00000c17,  CFG_WIDTH_4BYTES|(CFG_ACCESS_WR<<CFG_ACCESS_MODE_OFFSET), 0},
{0x3060000c, 0x00010609,  CFG_WIDTH_4BYTES|(CFG_ACCESS_WR<<CFG_ACCESS_MODE_OFFSET), 0},

{0x30600040, 0x44121209,  CFG_WIDTH_4BYTES|(CFG_ACCESS_WR<<CFG_ACCESS_MODE_OFFSET), 0},
{0x30600044, 0x06008528,  CFG_WIDTH_4BYTES|(CFG_ACCESS_WR<<CFG_ACCESS_MODE_OFFSET), 0},
{0x30600048, 0x000b1e0b,  CFG_WIDTH_4BYTES|(CFG_ACCESS_WR<<CFG_ACCESS_MODE_OFFSET), 0},
{0x3060004c, 0x06080618,  CFG_WIDTH_4BYTES|(CFG_ACCESS_WR<<CFG_ACCESS_MODE_OFFSET), 0},

{0x30600050, 0x20006008,  CFG_WIDTH_4BYTES|(CFG_ACCESS_WR<<CFG_ACCESS_MODE_OFFSET), 0},
{0x30600054, 0x0000000c,  CFG_WIDTH_4BYTES|(CFG_ACCESS_WR<<CFG_ACCESS_MODE_OFFSET), 0},
{0x3060005c, 0x00040000,  CFG_WIDTH_4BYTES|(CFG_ACCESS_WR<<CFG_ACCESS_MODE_OFFSET), 0},

{0x30600060, 0x00001405,  CFG_WIDTH_4BYTES|(CFG_ACCESS_WR<<CFG_ACCESS_MODE_OFFSET), 0},
{0x30600064, 0x00000000,  CFG_WIDTH_4BYTES|(CFG_ACCESS_WR<<CFG_ACCESS_MODE_OFFSET), 0},
{0x30600068, 0x00001000,  CFG_WIDTH_4BYTES|(CFG_ACCESS_WR<<CFG_ACCESS_MODE_OFFSET), 0},

{0x3060008c, 0x000b714c,  CFG_WIDTH_4BYTES|(CFG_ACCESS_WR<<CFG_ACCESS_MODE_OFFSET), 0},


{0x30600090, 0x10731483,  CFG_WIDTH_4BYTES|(CFG_ACCESS_WR<<CFG_ACCESS_MODE_OFFSET), 0},
{0x30600094, 0x3ff62d49,  CFG_WIDTH_4BYTES|(CFG_ACCESS_WR<<CFG_ACCESS_MODE_OFFSET), 0},
{0x30600098, 0x1140f38d,  CFG_WIDTH_4BYTES|(CFG_ACCESS_WR<<CFG_ACCESS_MODE_OFFSET), 0},
{0x3060009c, 0x165544d2,  CFG_WIDTH_4BYTES|(CFG_ACCESS_WR<<CFG_ACCESS_MODE_OFFSET), 0},


{0x306000a0, 0x3f699617,  CFG_WIDTH_4BYTES|(CFG_ACCESS_WR<<CFG_ACCESS_MODE_OFFSET), 0},
{0x306000a4, 0x0003ffff,  CFG_WIDTH_4BYTES|(CFG_ACCESS_WR<<CFG_ACCESS_MODE_OFFSET), 0},
{0x306000a8, 0x0001d71b,  CFG_WIDTH_4BYTES|(CFG_ACCESS_WR<<CFG_ACCESS_MODE_OFFSET), 0},
{0x306000ac, 0x00fdefff,  CFG_WIDTH_4BYTES|(CFG_ACCESS_WR<<CFG_ACCESS_MODE_OFFSET), 1000000},

{0x30600004, 0x0008002c,  CFG_WIDTH_4BYTES|(CFG_ACCESS_WR<<CFG_ACCESS_MODE_OFFSET), 0},

};

static char ctc5236_header[4096];


static int ctc5236_image_check_image_types(uint8_t type)
{
	if (type == IH_TYPE_CTC5236SPLIMAGE)
		return EXIT_SUCCESS;
	else
		return EXIT_FAILURE;
}

static int ctc5236_image_check_params(struct image_tool_params *params)
{
	return	((params->dflag && (params->fflag || params->lflag)) ||
		(params->fflag && (params->dflag || params->lflag)) ||
		(params->lflag && (params->dflag || params->fflag)));
}

static int ctc5236_image_verify_header(unsigned char *ptr, int image_size,
			struct image_tool_params *params)
{
	uint32_t len;
	const unsigned char *data;
	uint32_t checksum;
	ctc5236_spl_ih_t header;
	ctc5236_spl_ih_t *hdr = &header;

	/*
	 * create copy of header so that we can blank out the
	 * checksum field for checking - this can't be done
	 * on the PROT_READ mapped data.
	 */
	memcpy(hdr, ptr, sizeof(ctc5236_spl_ih_t));

	if (be32_to_cpu(hdr->ih_magic) != IH_MAGIC) {
		debug("%s: Bad Magic Number: \"%s\" is no valid image\n",
		      params->cmdname, params->imagefile);
		return -FDT_ERR_BADMAGIC;
	}

	data = (const unsigned char *)hdr;
	len  = sizeof(ctc5236_spl_ih_t);

	checksum = be32_to_cpu(hdr->ih_hcrc);
	hdr->ih_hcrc = cpu_to_be32(0);	/* clear for re-calculation */

	if (crc32(0, data, len) != checksum) {
		debug("%s: ERROR: \"%s\" has bad header checksum!\n",
		      params->cmdname, params->imagefile);
		return -FDT_ERR_BADSTATE;
	}

	data = (const unsigned char *)ptr + sizeof(ctc5236_spl_ih_t);
	len  = image_size - sizeof(ctc5236_spl_ih_t) ;

	checksum = be32_to_cpu(hdr->ih_dcrc);
	if (crc32(0, data, len) != checksum) {
		debug("%s: ERROR: \"%s\" has corrupted data!\n",
		      params->cmdname, params->imagefile);
		return -FDT_ERR_BADSTRUCTURE;
	}
	return 0;
}

static void ctc5236_image_set_header(void *ptr, struct stat *sbuf, int ifd,
				struct image_tool_params *params)
{
    uint32_t cfgcount;
	uint32_t dchecksum, cchecksum, hchecksum;
	uint32_t imagesize;
	ctc5236_spl_ih_t * hdr = (ctc5236_spl_ih_t *)ptr;

    //cfgcount = sizeof(ctc5236_cfg)/sizeof(ctc5236_spl_cfg_t);
    cfgcount = 0;
    
	dchecksum = crc32(0,
			(const unsigned char *)(ptr + CTC5236_SPL_DATA_OFFSET),
			sbuf->st_size - CTC5236_SPL_DATA_OFFSET);

    memcpy((void*)(ptr + CTC5236_IH_SIZE), (void*)ctc5236_cfg, sizeof(ctc5236_cfg));

	cchecksum = crc32(0,
			(const unsigned char *)(ptr + CTC5236_IH_SIZE), sizeof(ctc5236_cfg));
    
    imagesize = sbuf->st_size - CTC5236_SPL_DATA_OFFSET;
    
    memset(ptr, 0, sizeof(ctc5236_spl_ih_t));
	/* Build new header */
    ctc5236_image_set_magic(hdr, CTC5236_IH_MAGIC);
    ctc5236_image_set_size(hdr, imagesize);
    ctc5236_image_set_load(hdr, params->addr);
    ctc5236_image_set_ep(hdr, params->ep);
    ctc5236_image_set_dcrc(hdr, dchecksum);
    ctc5236_image_set_hcrc(hdr, 0);
    ctc5236_image_set_ccnt(hdr, cfgcount);
    ctc5236_image_set_ccrc(hdr, cchecksum);
    ctc5236_image_set_msec(hdr, CTC5236_EFUSE_VOL_SWITCH_MDELAY);

	hchecksum = crc32(0, (const unsigned char *)hdr,
				sizeof(ctc5236_spl_ih_t));

    hdr->ih_hcrc = cpu_to_be32(hchecksum);
}

static void ctc5236_image_print_contents(const void *ptr)
{

}

/*
 * Default image type parameters definition
 */
U_BOOT_IMAGE_TYPE(
	ctc5236splimage,
	"Centec 5236 Image support",
	CTC5236_SPL_DATA_OFFSET,
	(void *)&ctc5236_header,
	ctc5236_image_check_params,
	ctc5236_image_verify_header,
	ctc5236_image_print_contents,
	ctc5236_image_set_header,
	NULL,
	ctc5236_image_check_image_types,
	NULL,
	NULL
);
