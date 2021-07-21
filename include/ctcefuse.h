/*
 * (C) Copyright 2004-2017 Centec Networks (suzhou) Co., LTD.
 * Liuht <liuht@centecnetworks.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef __CTCEFUSE_H
#define __CTCEFUSE_H

#define CONFIG_SYS_HASH_BASE            0x00100000
#define CONFIG_SYS_EFUSE_BASE           0x00100020

/*eFuse layout*/
#define BIT_BYTE(nr)		   ((nr) / 8)
#define EFUSE_NV1_BIT          96
#define EFUSE_NV1_BIT_WORD     BIT_BYTE(96)
#define EFUSE_NV2_BIT          100
#define EFUSE_NV2_BIT_WORD     BIT_BYTE(100)
#define EFUSE_SBKH_BIT         128
#define EFUSE_SBKH_BIT_WORD    BIT_BYTE(128)
#define EFUSE_SBHK_BIT         384
#define EFUSE_SBHK_BIT_WORD    BIT_BYTE(384)
#define EFUSE_SBE_BIT          104
#define EFUSE_SBE_BIT_WORD     BIT_BYTE(104)

void efuse_get_nv2(u8 *nv2);
void efuse_get_sbhk(u8 **sbhk);

#endif

