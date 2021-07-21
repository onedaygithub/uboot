/*
 * (C) Copyright 2004-2017 Centec Networks (suzhou) Co., LTD.
 * Liuht <liuht@centecnetworks.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <ctcefuse.h>

void efuse_get_nv2(u8 *nv2)
{
    u8 *p = (u8 *)(CONFIG_SYS_EFUSE_BASE+EFUSE_NV2_BIT_WORD);
    
    *nv2 = (*p >> 4) & 0xf;
}

void efuse_get_sbhk(u8 **sbhk)
{
    *sbhk = (u8 *)(CONFIG_SYS_EFUSE_BASE+EFUSE_SBHK_BIT_WORD);
}

