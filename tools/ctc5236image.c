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
#include <ctc5236image.h>

static ctc5236_ih_t ctc5236_header;

static int ctc5236_image_check_image_types(uint8_t type)
{
	if (type == IH_TYPE_CTC5236IMAGE)
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
	ctc5236_ih_t header;
	ctc5236_ih_t *hdr = &header;

	/*
	 * create copy of header so that we can blank out the
	 * checksum field for checking - this can't be done
	 * on the PROT_READ mapped data.
	 */
	memcpy(hdr, ptr, sizeof(ctc5236_ih_t));

	if (be32_to_cpu(hdr->ih_magic) != IH_MAGIC) {
		debug("%s: Bad Magic Number: \"%s\" is no valid image\n",
		      params->cmdname, params->imagefile);
		return -FDT_ERR_BADMAGIC;
	}

	data = (const unsigned char *)hdr;
	len  = sizeof(ctc5236_ih_t);

	checksum = be32_to_cpu(hdr->ih_hcrc);
	hdr->ih_hcrc = cpu_to_be32(0);	/* clear for re-calculation */

	if (crc32(0, data, len) != checksum) {
		debug("%s: ERROR: \"%s\" has bad header checksum!\n",
		      params->cmdname, params->imagefile);
		return -FDT_ERR_BADSTATE;
	}

	data = (const unsigned char *)ptr + sizeof(ctc5236_ih_t);
	len  = image_size - sizeof(ctc5236_ih_t) ;

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
	uint32_t checksum;
	uint32_t imagesize;

	ctc5236_ih_t * hdr = (ctc5236_ih_t *)ptr;

	checksum = crc32(0,
			(const unsigned char *)(ptr +
				sizeof(ctc5236_ih_t)),
			sbuf->st_size - sizeof(ctc5236_ih_t));
    
    imagesize = sbuf->st_size - sizeof(image_header_t);

    memset(ptr, 0, sizeof(ctc5236_ih_t));
	/* Build new header */
    ctc5236_image_set_magic(hdr, CTC5236_IH_MAGIC);
    ctc5236_image_set_size(hdr, imagesize);
    ctc5236_image_set_load(hdr, params->addr);
    ctc5236_image_set_ep(hdr, params->ep);
    ctc5236_image_set_dcrc(hdr, checksum);
    ctc5236_image_set_hcrc(hdr, 0);

	checksum = crc32(0, (const unsigned char *)hdr,
				sizeof(ctc5236_ih_t));

    hdr->ih_hcrc = cpu_to_be32(checksum);
}

static void ctc5236_image_print_contents(const void *ptr)
{

}

/*
 * Default image type parameters definition
 */
U_BOOT_IMAGE_TYPE(
	ctc5236image,
	"Centec 5236 Image support",
	sizeof(ctc5236_ih_t),
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
