/*
 * (C) Copyright 2000-2004
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * (C) Copyright 2011
 * Texas Instruments, <www.ti.com>
 *
 * Matt Porter <mporter@ti.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#include <common.h>
#include <spl.h>
#include <xyzModem.h>
#include <asm/u-boot.h>
#include <asm/utils.h>
#include <ctc5236image.h>
#include <spl_ctc5236.h>
#include <image-verify.h>
#include <ctcefuse.h>

#define BUF_SIZE 1024

static int getcymodem(void) {
    if (tstc())
        return (getc());
    return -1;
}

#ifdef CONFIG_CTC_SEC_BOOT
static int ymodem_secure_boot_flow(struct spl_image_info *spl_image)
{
    u8 nv;
    u8 keyindex;
    u32 data;
    u8 hash[CER_HASH_LEN], sochash[CER_HASH_LEN];
    int cert_blk_cnt;
    int err, res, size = 0;
    connection_info_t info;
    //ulong getdcrc;
    ulong uboot_offset;
    char *buf;
    ctc5236_ih_t *hdr;
    struct ctc_cert *cert;

    printf("Please download image through UART with ymodem protocol.\n");
    printf("The UART parameter is :\n");
    printf("BaudRate : 115200\n");
    printf("DataBit  : 8\n");
    printf("Parity   : none\n");
    printf("StopBit  : 1\n");
    printf("FlowCtrl : none\n");

    buf  = (char *)CONFIG_SYS_TEXT_BASE;
    cert = (struct ctc_cert *)(CONFIG_SYS_TEXT_BASE);
    hdr  = (ctc5236_ih_t *)(CONFIG_SYS_TEXT_BASE+CTC5236_CERT_SIZE);
    data = (CONFIG_SYS_TEXT_BASE+CTC5236_CERT_SIZE+CTC5236_IH_SIZE);
    
    info.mode = xyzModem_ymodem;
    res = xyzModem_stream_open(&info, &err);
    NONZ_GOTO_END(res, end_stream);

    uboot_offset = CONFIG_SPL_PAD_TO + CTC5236_CERT_SIZE*2;
    /* skip spl image */
    while(uboot_offset>0){
        res = xyzModem_stream_read(buf, BUF_SIZE, &err);
        NOGT_GOTO_END(res, 0, end_stream);
        uboot_offset -= BUF_SIZE;
    }

    /* Cert occupy 3 block */
    cert_blk_cnt = 3;
    while(cert_blk_cnt){
        res = xyzModem_stream_read(buf, BUF_SIZE, &err);
        buf += res;
        cert_blk_cnt--;
    }

    efuse_get_nv2(&nv);    
    GRTR_RETURN(nv, cert->nv);

    NEGA_RETURN(ctc_get_pubkey_hash(cert, hash));
    ctc_load_pkey_hash(sochash);
    NONZ_RETURN(memcmp(sochash, hash, CER_HASH_LEN));

    /* read total image */
    size = res+BUF_SIZE/2;
    while (((res = xyzModem_stream_read((void *)buf, BUF_SIZE, &err)) > 0)
        && (size<cert->img_size)) {
        size += res;
        buf += res;
    }
    
    if(cert->encrypt){
        u8 *aes_key;
        efuse_get_sbhk(&aes_key);
        ctc_aes_decrypt(aes_key, (u8 *)hdr, (u8 *)hdr, cert->img_size);
    }

    NEGA_RETURN(ctc_get_image_hash((u8 *)hdr, cert->img_size, hash));
    NEGA_RETURN(ctc_get_keyindex(cert, &keyindex));
    NEGA_RETURN(ctc_verify_sign(cert, hash, keyindex));
    
    err = ctc5236_spl_parse_ih(spl_image, hdr);
    NEGA_GOTO_END(err, end_stream);

    //getdcrc = ctc5236_image_get_dcrc(hdr);
    ctc_memcpy_data((void *)spl_image->load_addr, data, spl_image->size);
    //res = ctc5236_image_check_dcrc(getdcrc, spl_image);
    //ZERO_GOTO_END(res, end_stream);

    xyzModem_stream_terminate(true, &getcymodem);

    printf("Loaded 0x%x bytes total 0x%x\n", size, spl_image->size);

    return 0;
    
end_stream:
    xyzModem_stream_terminate(true, &getcymodem);

    printf("Loaded 0x%x bytes total 0x%x\n", size, spl_image->size);
    
    return -1;
}
#else
static int ymodem_normal_boot_flow(struct spl_image_info *spl_image)
{
    int err, res, size = 0;
    connection_info_t info;
    ulong addr = 0, getdcrc;
    ulong uboot_offset;
    char *buf = (char *)CONFIG_SYS_TEXT_BASE;
    ctc5236_ih_t *hdr = (ctc5236_ih_t *)(CONFIG_SYS_TEXT_BASE);

    printf("Please download image through UART with ymodem protocol.\n");
    printf("The UART parameter is :\n");
    printf("BaudRate : 115200\n");
    printf("DataBit  : 8\n");
    printf("Parity   : none\n");
    printf("StopBit  : 1\n");
    printf("FlowCtrl : none\n");
    
    info.mode = xyzModem_ymodem;
    res = xyzModem_stream_open(&info, &err);
    NONZ_GOTO_END(res, end_stream);
    
    /* skip spl image */
    uboot_offset = CONFIG_SPL_PAD_TO;
    while(uboot_offset>0)
    {
        res = xyzModem_stream_read(buf, BUF_SIZE, &err);
        NOGT_GOTO_END(res, 0, end_stream);
        uboot_offset -= BUF_SIZE;
    }
    
    /* get 64 bytes uboot image header and (BUF_SIZE-64) bytes data */
    res = xyzModem_stream_read(buf, BUF_SIZE, &err);
    NOGT_GOTO_END(res, 0, end_stream);

    err = ctc5236_spl_parse_ih(spl_image, hdr);
    NEGA_GOTO_END(err, end_stream);
    
    getdcrc = ctc5236_image_get_dcrc(hdr);

    /* copy (BUF_SIZE-64) bytes data to load_addr */
    addr = spl_image->load_addr;
    memcpy((void *)addr, (buf+CTC5236_IH_SIZE), res-CTC5236_IH_SIZE);
    size += res-CTC5236_IH_SIZE;
    addr += res-CTC5236_IH_SIZE;

    while (((res = xyzModem_stream_read((void *)addr, BUF_SIZE, &err)) > 0)
        && (size<spl_image->size)) {
        size += res;
        addr += res;
    }

    res = ctc5236_image_check_dcrc(getdcrc, spl_image);
    ZERO_GOTO_END(res, end_stream);

    xyzModem_stream_terminate(true, &getcymodem);

    printf("Loaded 0x%x bytes total 0x%x\n", size, spl_image->size);

    return 0;
    
end_stream:
    xyzModem_stream_terminate(true, &getcymodem);

    printf("Loaded 0x%x bytes total 0x%x\n", size, spl_image->size);
    
    return -1;
}
#endif

static int spl_ymodem_load_image(struct spl_image_info *spl_image,
            struct spl_boot_device *bootdev)
{
#ifdef CONFIG_CTC_SEC_BOOT
    return ymodem_secure_boot_flow(spl_image);
#else
    return ymodem_normal_boot_flow(spl_image);
#endif
}

SPL_LOAD_IMAGE_METHOD("UART", 0, BOOT_DEVICE_UART, spl_ymodem_load_image);
