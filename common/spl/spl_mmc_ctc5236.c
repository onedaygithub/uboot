/*
 * (C) Copyright 2004-2017 Centec Networks (suzhou) Co., LTD.
 * Jay Cao <caoj@centecnetworks.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <dm.h>
#include <mmc.h>
#include <spl.h>
#include <ctc5236image.h>
#include <spl_ctc5236.h>
#include <image-verify.h>
#include <ctcefuse.h>
/*****************************************************************************/

#define MMC_BLK_SZ  512

static int ctc5236_spl_mmc_find_device(struct mmc **mmcp, u32 boot_device)
{
    int ret;

    ret = mmc_initialize(NULL);
    NONZ_RETURN(ret);

    *mmcp = find_mmc_device(0);
    ret = *mmcp ? 0 : -ENODEV;
    NONZ_RETURN(ret);

    return 0;
}

#ifdef CONFIG_CTC_SEC_BOOT
static int emmc_secure_boot_flow(struct spl_image_info *spl_image,
            struct mmc *mmc)
{
    u8 nv;
    u8 keyindex;
    u32 data;
    u8 hash[CER_HASH_LEN], sochash[CER_HASH_LEN];
    int ret;
    ulong uboot_offset;
    ulong count = 0, bootstart;
    //ulong getdcrc;
    lbaint_t ccount, cstart, icount, istart;
    struct blk_desc *block_dev=NULL;
    ctc5236_ih_t *hdr;
    struct ctc_cert *cert;

    cert = (struct ctc_cert *)(CONFIG_SYS_TEXT_BASE);
    hdr  = (ctc5236_ih_t *)(CONFIG_SYS_TEXT_BASE+CTC5236_CERT_SIZE);
    data = (CONFIG_SYS_TEXT_BASE+CTC5236_CERT_SIZE+CTC5236_IH_SIZE);
        
    block_dev = mmc_get_blk_desc(mmc);
    NULL_RETURN(block_dev);

    DIFF_RETURN(block_dev->blksz, MMC_BLK_SZ);

    if (!IS_SD(mmc))
    {
        bootstart = CTC5236_MMC_BOOT_START;
    }
    else
    {
        bootstart = CTC5236_SD_BOOT_START;
    }
    
    uboot_offset = CONFIG_SPL_PAD_TO + CTC5236_CERT_SIZE*2;
    cstart = bootstart + uboot_offset/block_dev->blksz;
    ccount = CTC5236_CERT_SIZE/block_dev->blksz;
    count = blk_dread(block_dev, cstart, ccount, (void *)cert);
    
    ZERO_RETURN(count);

    efuse_get_nv2(&nv);        
    
    GRTR_RETURN(nv, cert->nv);

    NEGA_RETURN(ctc_get_pubkey_hash(cert, hash));
    ctc_load_pkey_hash(sochash);
    NONZ_RETURN(memcmp(sochash, hash, CER_HASH_LEN));

    istart = cstart + ccount;
    if(cert->img_size%block_dev->blksz){
        icount = cert->img_size/block_dev->blksz + 1;
    }else{
        icount = cert->img_size/block_dev->blksz;
    }
    count = blk_dread(block_dev, istart, icount, (void *)hdr);
    ZERO_RETURN(count);
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
    //getdcrc = ctc5236_image_get_dcrc(hdr);
    ctc_memcpy_data((void *)spl_image->load_addr, data, spl_image->size);
    //ret = ctc5236_image_check_dcrc(getdcrc, spl_image);
    //ZERO_RETURN(ret);
    
    return 0;
}
#else
static int emmc_normal_boot_flow(struct spl_image_info *spl_image,
            struct mmc *mmc)
{
    int ret;
    ulong uboot_offset;
    ulong count = 0, bootstart;
    ulong valid_data, remain_data, getdcrc;
    lbaint_t hstart, hcount, dstart, dcount;
    struct blk_desc *block_dev=NULL;
    char *buf = (char *)CONFIG_SYS_TEXT_BASE;
    ctc5236_ih_t *hdr = (ctc5236_ih_t *)CONFIG_SYS_TEXT_BASE;
    
    block_dev = mmc_get_blk_desc(mmc);
    NULL_RETURN(block_dev);

    DIFF_RETURN(block_dev->blksz, MMC_BLK_SZ);

    if (!IS_SD(mmc))
    {
        bootstart = CTC5236_MMC_BOOT_START;
    }
    else
    {
        bootstart = CTC5236_SD_BOOT_START;
    }
    
    hcount = 1;
    uboot_offset = CONFIG_SPL_PAD_TO;
    hstart = bootstart + uboot_offset/block_dev->blksz;
    count = blk_dread(block_dev, hstart, hcount, (void *)buf);
    ZERO_RETURN(count);

    ret = ctc5236_spl_parse_ih(spl_image, hdr);
    NEGA_RETURN(ret);

    getdcrc = ctc5236_image_get_dcrc(hdr);
    
    valid_data = block_dev->blksz - CTC5236_IH_SIZE;
    memcpy((void *)spl_image->load_addr, (void *)buf + CTC5236_IH_SIZE, valid_data);
    
    dstart = hstart+1;
    remain_data = spl_image->size - valid_data;
    if(remain_data%block_dev->blksz)
    {
        dcount = remain_data/block_dev->blksz + 1;
    }
    else
    {
        dcount = remain_data/block_dev->blksz;
    }
    
    count = blk_dread(block_dev, dstart, dcount, (void *)(spl_image->load_addr+valid_data));
    ZERO_RETURN(count);
 
    ret = ctc5236_image_check_dcrc(getdcrc, spl_image);
    ZERO_RETURN(ret);
    
    return 0;
}
#endif

static int ctc5236_spl_mmc_load_image(struct spl_image_info *spl_image,
                struct spl_boot_device *bootdev)
{
    int ret = 0;
    struct mmc *mmc = NULL;

    ret = ctc5236_spl_mmc_find_device(&mmc, bootdev->boot_device);
    NONZ_RETURN(ret);

    ret = mmc_init(mmc);
    NONZ_RETURN(ret);

    if (!IS_SD(mmc))
    {
        ret = blk_dselect_hwpart(mmc_get_blk_desc(mmc), 1);
        NONZ_RETURN(ret);
    }
#ifdef VELOCE_PLATFORM
    /* For speeding up bootup speed on mentor veloce platform,
     * here we don't move the rest qspi data to DDR just parse 
     * the image hader (0x40 bytes), and then directly jump to 
     * DDR, so need use backdoor to write image to DDR
     */
    return 0;
#endif
    
#ifdef CONFIG_CTC_SEC_BOOT
    NONZ_RETURN(emmc_secure_boot_flow(spl_image, mmc));
#else
    NONZ_RETURN(emmc_normal_boot_flow(spl_image, mmc));
#endif
    
    return 0;
}

SPL_LOAD_IMAGE_METHOD("MMC1", 0, BOOT_DEVICE_MMC1, ctc5236_spl_mmc_load_image);
