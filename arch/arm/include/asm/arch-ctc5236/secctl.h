/*
 * (C) Copyright 2004-2017 Centec Networks (suzhou) Co., LTD.
 * Jay Cao <caoj@centecnetworks.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#ifndef __CTC5236_SECCTL_H__
#define __CTC5236_SECCTL_H__

#ifndef __ASSEMBLY__

/* define SECCTL_MEM_BASE   0x00000100 */
/* defing SECCTL_REG_BASE   0x00000000 */

struct SecCtl_regs {
   u32 SecEFuseCtl;                     /* 0x00000000 */
   u32 SecCoreCfg;                      /* 0x00000004 */
   u32 SecGicCfg;                       /* 0x00000008 */
   u32 SecAxiProtCfg;                   /* 0x0000000c */
   u32 SecAuthDbgCfg;                   /* 0x00000010 */
   u32 SecDbgRomCtl;                    /* 0x00000014 */
   u32 SecResetCtl;                     /* 0x00000018 */
   u32 SecTzcFpCfg;                     /* 0x0000001c */
   u32 SecTzcCpuCfg[6];                 /* 0x00000020 */
   u32 rsv14;
   u32 rsv15;
   u32 SecTzcBusCfg[8];                 /* 0x00000040 */
   u32 SecTzcMacCfg[4];                 /* 0x00000060 */
   u32 SecTzcDmaCfg[4];                 /* 0x00000070 */
   u32 SysDmaBootCfg;                   /* 0x00000080 */
   u32 SecDmaBootAddr;                  /* 0x00000084 */
   u32 SecInitCtl;                      /* 0x00000088 */
   u32 SecSysCntCfg;                    /* 0x0000008c */
   u32 SecSysCntValue[2];               /* 0x00000090 */
   u32 SecSysCntReload[2];              /* 0x00000098 */
   u32 SecOcmCfg[2];                    /* 0x000000a0 */
   u32 SecOcmDmaCfg[2];                 /* 0x000000a8 */
   u32 SecOcmPcieCfg[2];                /* 0x000000b0 */
   u32 SecOcmMshCfg[2];                 /* 0x000000b8 */
   u32 SecOcmUsbCfg[2];                 /* 0x000000c0 */
   u32 SecNumCtl;                       /* 0x000000c8 */
   u32 rsv51;
   u32 SecEFuseEn[2];                   /* 0x000000d0 */
};

/* ################################################################################
 * # SecEFuseCtl Definition
 */
#define SEC_E_FUSE_CTL_W0_E_FUSE_PROG_BUSY                           BIT(8)
#define SEC_E_FUSE_CTL_W0_E_FUSE_DATA_READY                          BIT(4)
#define SEC_E_FUSE_CTL_W0_E_FUSE_WR_COMPLETE                         BIT(0)

#define SEC_E_FUSE_CTL_W0_E_FUSE_PROG_BUSY_MASK                      0x00000100
#define SEC_E_FUSE_CTL_W0_E_FUSE_DATA_READY_MASK                     0x00000010
#define SEC_E_FUSE_CTL_W0_E_FUSE_WR_COMPLETE_MASK                    0x00000001

/* ################################################################################
 * # SecCoreCfg Definition
 */
#define SEC_CORE_CFG_W0_CFG_AINACTS                                  BIT(1)
#define SEC_CORE_CFG_W0_CFG_END                                      BIT(2)
#define SEC_CORE_CFG_W0_CFG_L1_RST_DISABLE                           BIT(9)
#define SEC_CORE_CFG_W0_CFG_L2_RST_DISABLE                           BIT(8)
#define SEC_CORE_CFG_W0_CFG_L2_FLUSH_REQ                             BIT(0)
#define SEC_CORE_CFG_W0_CFG_AA64NAA32                                BIT(4)
#define SEC_CORE_CFG_W0_CFG_V_INIT_HI                                BIT(14)
#define SEC_CORE_CFG_W0_CFG_TE                                       BIT(12)
#define SEC_CORE_CFG_W0_CFG_CP15S_DISABLE                            BIT(6)

#define SEC_CORE_CFG_W0_CFG_AINACTS_MASK                             0x00000002
#define SEC_CORE_CFG_W0_CFG_END_MASK                                 0x0000000c
#define SEC_CORE_CFG_W0_CFG_L1_RST_DISABLE_MASK                      0x00000200
#define SEC_CORE_CFG_W0_CFG_L2_RST_DISABLE_MASK                      0x00000100
#define SEC_CORE_CFG_W0_CFG_L2_FLUSH_REQ_MASK                        0x00000001
#define SEC_CORE_CFG_W0_CFG_AA64NAA32_MASK                           0x00000030
#define SEC_CORE_CFG_W0_CFG_V_INIT_HI_MASK                           0x0000c000
#define SEC_CORE_CFG_W0_CFG_TE_MASK                                  0x00003000
#define SEC_CORE_CFG_W0_CFG_CP15S_DISABLE_MASK                       0x000000c0

/* ################################################################################
 * # SecGicCfg Definition
 */
#define SEC_GIC_CFG_W0_CFG_GIC_SEC_DISABLE                           BIT(0)

#define SEC_GIC_CFG_W0_CFG_GIC_SEC_DISABLE_MASK                      0x00000001

/* ################################################################################
 * # SecAxiProtCfg Definition
 */
#define SEC_AXI_PROT_CFG_W0_SEC_USB_INTF_PROT                        BIT(4)
#define SEC_AXI_PROT_CFG_W0_SEC_MSH_INTF_PROT_EN                     BIT(12)
#define SEC_AXI_PROT_CFG_W0_SEC_USB_INTF_PROT_EN                     BIT(13)
#define SEC_AXI_PROT_CFG_W0_SEC_PCIE_INTF_PROT_EN                    BIT(14)
#define SEC_AXI_PROT_CFG_W0_SEC_MSH_INTF_PROT                        BIT(0)
#define SEC_AXI_PROT_CFG_W0_SEC_PCIE_INTF_PROT                       BIT(8)

#define SEC_AXI_PROT_CFG_W0_SEC_USB_INTF_PROT_MASK                   0x000000f0
#define SEC_AXI_PROT_CFG_W0_SEC_MSH_INTF_PROT_EN_MASK                0x00001000
#define SEC_AXI_PROT_CFG_W0_SEC_USB_INTF_PROT_EN_MASK                0x00002000
#define SEC_AXI_PROT_CFG_W0_SEC_PCIE_INTF_PROT_EN_MASK               0x00004000
#define SEC_AXI_PROT_CFG_W0_SEC_MSH_INTF_PROT_MASK                   0x00000007
#define SEC_AXI_PROT_CFG_W0_SEC_PCIE_INTF_PROT_MASK                  0x00000700

/* ################################################################################
 * # SecAuthDbgCfg Definition
 */
#define SEC_AUTH_DBG_CFG_W0_CST_AUTH_NID_EN                          BIT(1)
#define SEC_AUTH_DBG_CFG_W0_CPU_CORE_AUTH_SPNID_EN                   BIT(14)
#define SEC_AUTH_DBG_CFG_W0_CPU_CORE_AUTH_DBG_EN                     BIT(8)
#define SEC_AUTH_DBG_CFG_W0_CPU_CORE_AUTH_SPID_EN                    BIT(12)
#define SEC_AUTH_DBG_CFG_W0_CST_AUTH_DBG_EN                          BIT(0)
#define SEC_AUTH_DBG_CFG_W0_CPU_CORE_AUTH_NID_EN                     BIT(10)
#define SEC_AUTH_DBG_CFG_W0_CST_AUTH_SPID_EN                         BIT(2)
#define SEC_AUTH_DBG_CFG_W0_CST_AUTH_SPNID_EN                        BIT(3)

#define SEC_AUTH_DBG_CFG_W0_CST_AUTH_NID_EN_MASK                     0x00000002
#define SEC_AUTH_DBG_CFG_W0_CPU_CORE_AUTH_SPNID_EN_MASK              0x0000c000
#define SEC_AUTH_DBG_CFG_W0_CPU_CORE_AUTH_DBG_EN_MASK                0x00000300
#define SEC_AUTH_DBG_CFG_W0_CPU_CORE_AUTH_SPID_EN_MASK               0x00003000
#define SEC_AUTH_DBG_CFG_W0_CST_AUTH_DBG_EN_MASK                     0x00000001
#define SEC_AUTH_DBG_CFG_W0_CPU_CORE_AUTH_NID_EN_MASK                0x00000c00
#define SEC_AUTH_DBG_CFG_W0_CST_AUTH_SPID_EN_MASK                    0x00000004
#define SEC_AUTH_DBG_CFG_W0_CST_AUTH_SPNID_EN_MASK                   0x00000008

/* ################################################################################
 * # SecDbgRomCtl Definition
 */
#define SEC_DBG_ROM_CTL_W0_CFG_DBG_ROM_ADDR_V                        BIT(31)
#define SEC_DBG_ROM_CTL_W0_CFG_DBG_ROM_ADDR                          BIT(0)

#define SEC_DBG_ROM_CTL_W0_CFG_DBG_ROM_ADDR_V_MASK                   0x80000000
#define SEC_DBG_ROM_CTL_W0_CFG_DBG_ROM_ADDR_MASK                     0x0fffffff

/* ################################################################################
 * # SecResetCtl Definition
 */
#define SEC_RESET_CTL_W0_LOG_TZC_RESET                               BIT(4)
#define SEC_RESET_CTL_W0_CFG_TZC_RESET                               BIT(0)

#define SEC_RESET_CTL_W0_LOG_TZC_RESET_MASK                          0x00000010
#define SEC_RESET_CTL_W0_CFG_TZC_RESET_MASK                          0x00000001

/* ################################################################################
 * # SecTzcFpCfg Definition
 */
#define SEC_TZC_FP_CFG_W0_CFG_CPU_DDR_FP_EN                          BIT(0)
#define SEC_TZC_FP_CFG_W0_CFG_CPU_MAC_DDR_FP_EN                      BIT(2)
#define SEC_TZC_FP_CFG_W0_CFG_SYS_BUS_DDR_FP_EN                      BIT(1)
#define SEC_TZC_FP_CFG_W0_CFG_TZC_CPU_QVN_EN                         BIT(8)
#define SEC_TZC_FP_CFG_W0_CFG_SWITCH_DMA_DDR_FP_EN                   BIT(3)
#define SEC_TZC_FP_CFG_W0_CFG_TZC_BUS_QVN_EN                         BIT(4)

#define SEC_TZC_FP_CFG_W0_CFG_CPU_DDR_FP_EN_MASK                     0x00000001
#define SEC_TZC_FP_CFG_W0_CFG_CPU_MAC_DDR_FP_EN_MASK                 0x00000004
#define SEC_TZC_FP_CFG_W0_CFG_SYS_BUS_DDR_FP_EN_MASK                 0x00000002
#define SEC_TZC_FP_CFG_W0_CFG_TZC_CPU_QVN_EN_MASK                    0x00000100
#define SEC_TZC_FP_CFG_W0_CFG_SWITCH_DMA_DDR_FP_EN_MASK              0x00000008
#define SEC_TZC_FP_CFG_W0_CFG_TZC_BUS_QVN_EN_MASK                    0x000000f0

/* ################################################################################
 * # SecTzcCpuCfg Definition
 */
#define SEC_TZC_CPU_CFG_W0_CFG_CPU_DDR_RA_DOMAIN                     BIT(8)
#define SEC_TZC_CPU_CFG_W0_CFG_CPU_DDR_WA_DOMAIN_SEL                 BIT(4)
#define SEC_TZC_CPU_CFG_W0_CFG_CPU_DDR_RA_DOMAIN_SEL                 BIT(12)
#define SEC_TZC_CPU_CFG_W0_CFG_CPU_DDR_WA_DOMAIN                     BIT(0)
#define SEC_TZC_CPU_CFG_W1_CFG_CPU0_NSA_RA_ID                        BIT(16)
#define SEC_TZC_CPU_CFG_W1_CFG_CPU0_NSA_WA_MASK                      BIT(8)
#define SEC_TZC_CPU_CFG_W1_CFG_CPU0_NSA_WA_ID                        BIT(0)
#define SEC_TZC_CPU_CFG_W1_CFG_CPU0_NSA_RA_MASK                      BIT(24)
#define SEC_TZC_CPU_CFG_W2_CFG_CPU1_NSA_RA_ID                        BIT(16)
#define SEC_TZC_CPU_CFG_W2_CFG_CPU1_NSA_WA_MASK                      BIT(8)
#define SEC_TZC_CPU_CFG_W2_CFG_CPU1_NSA_RA_MASK                      BIT(24)
#define SEC_TZC_CPU_CFG_W2_CFG_CPU1_NSA_WA_ID                        BIT(0)
#define SEC_TZC_CPU_CFG_W3_CFG_CPU2_NSA_RA_MASK                      BIT(24)
#define SEC_TZC_CPU_CFG_W3_CFG_CPU2_NSA_WA_MASK                      BIT(8)
#define SEC_TZC_CPU_CFG_W3_CFG_CPU2_NSA_RA_ID                        BIT(16)
#define SEC_TZC_CPU_CFG_W3_CFG_CPU2_NSA_WA_ID                        BIT(0)
#define SEC_TZC_CPU_CFG_W4_CFG_CPU3_NSA_RA_ID                        BIT(16)
#define SEC_TZC_CPU_CFG_W4_CFG_CPU3_NSA_RA_MASK                      BIT(24)
#define SEC_TZC_CPU_CFG_W4_CFG_CPU3_NSA_WA_MASK                      BIT(8)
#define SEC_TZC_CPU_CFG_W4_CFG_CPU3_NSA_WA_ID                        BIT(0)
#define SEC_TZC_CPU_CFG_W5_CFG_CPU4_NSA_RA_MASK                      BIT(24)
#define SEC_TZC_CPU_CFG_W5_CFG_CPU4_NSA_WA_MASK                      BIT(8)
#define SEC_TZC_CPU_CFG_W5_CFG_CPU4_NSA_RA_ID                        BIT(16)
#define SEC_TZC_CPU_CFG_W5_CFG_CPU4_NSA_WA_ID                        BIT(0)

#define SEC_TZC_CPU_CFG_W0_CFG_CPU_DDR_RA_DOMAIN_MASK                0x00000300
#define SEC_TZC_CPU_CFG_W0_CFG_CPU_DDR_WA_DOMAIN_SEL_MASK            0x00000030
#define SEC_TZC_CPU_CFG_W0_CFG_CPU_DDR_RA_DOMAIN_SEL_MASK            0x00003000
#define SEC_TZC_CPU_CFG_W0_CFG_CPU_DDR_WA_DOMAIN_MASK                0x00000003
#define SEC_TZC_CPU_CFG_W1_CFG_CPU0_NSA_RA_ID_MASK                   0x003f0000
#define SEC_TZC_CPU_CFG_W1_CFG_CPU0_NSA_WA_MASK_MASK                 0x00003f00
#define SEC_TZC_CPU_CFG_W1_CFG_CPU0_NSA_WA_ID_MASK                   0x0000003f
#define SEC_TZC_CPU_CFG_W1_CFG_CPU0_NSA_RA_MASK_MASK                 0x3f000000
#define SEC_TZC_CPU_CFG_W2_CFG_CPU1_NSA_RA_ID_MASK                   0x003f0000
#define SEC_TZC_CPU_CFG_W2_CFG_CPU1_NSA_WA_MASK_MASK                 0x00003f00
#define SEC_TZC_CPU_CFG_W2_CFG_CPU1_NSA_RA_MASK_MASK                 0x3f000000
#define SEC_TZC_CPU_CFG_W2_CFG_CPU1_NSA_WA_ID_MASK                   0x0000003f
#define SEC_TZC_CPU_CFG_W3_CFG_CPU2_NSA_RA_MASK_MASK                 0x3f000000
#define SEC_TZC_CPU_CFG_W3_CFG_CPU2_NSA_WA_MASK_MASK                 0x00003f00
#define SEC_TZC_CPU_CFG_W3_CFG_CPU2_NSA_RA_ID_MASK                   0x003f0000
#define SEC_TZC_CPU_CFG_W3_CFG_CPU2_NSA_WA_ID_MASK                   0x0000003f
#define SEC_TZC_CPU_CFG_W4_CFG_CPU3_NSA_RA_ID_MASK                   0x003f0000
#define SEC_TZC_CPU_CFG_W4_CFG_CPU3_NSA_RA_MASK_MASK                 0x3f000000
#define SEC_TZC_CPU_CFG_W4_CFG_CPU3_NSA_WA_MASK_MASK                 0x00003f00
#define SEC_TZC_CPU_CFG_W4_CFG_CPU3_NSA_WA_ID_MASK                   0x0000003f
#define SEC_TZC_CPU_CFG_W5_CFG_CPU4_NSA_RA_MASK_MASK                 0x3f000000
#define SEC_TZC_CPU_CFG_W5_CFG_CPU4_NSA_WA_MASK_MASK                 0x00003f00
#define SEC_TZC_CPU_CFG_W5_CFG_CPU4_NSA_RA_ID_MASK                   0x003f0000
#define SEC_TZC_CPU_CFG_W5_CFG_CPU4_NSA_WA_ID_MASK                   0x0000003f

/* ################################################################################
 * # SecTzcBusCfg Definition
 */
#define SEC_TZC_BUS_CFG_W0_SEC_APB_TZC_BUS_PROT                      BIT(16)
#define SEC_TZC_BUS_CFG_W0_CFG_BUS_DDR_WA_DOMAIN                     BIT(0)
#define SEC_TZC_BUS_CFG_W0_CFG_BUS_DDR_RA_DOMAIN_SEL                 BIT(12)
#define SEC_TZC_BUS_CFG_W0_CFG_BUS_DDR_RA_DOMAIN                     BIT(8)
#define SEC_TZC_BUS_CFG_W0_CFG_BUS_DDR_WA_DOMAIN_SEL                 BIT(4)
#define SEC_TZC_BUS_CFG_W1_CFG_BUS0_NSA_WA_ID                        BIT(0)
#define SEC_TZC_BUS_CFG_W1_CFG_BUS0_NSA_RA_ID                        BIT(16)
#define SEC_TZC_BUS_CFG_W1_CFG_BUS0_NSA_RA_MASK                      BIT(24)
#define SEC_TZC_BUS_CFG_W1_CFG_BUS0_NSA_WA_MASK                      BIT(8)
#define SEC_TZC_BUS_CFG_W2_CFG_BUS1_NSA_WA_MASK                      BIT(8)
#define SEC_TZC_BUS_CFG_W2_CFG_BUS1_NSA_WA_ID                        BIT(0)
#define SEC_TZC_BUS_CFG_W2_CFG_BUS1_NSA_RA_ID                        BIT(16)
#define SEC_TZC_BUS_CFG_W2_CFG_BUS1_NSA_RA_MASK                      BIT(24)
#define SEC_TZC_BUS_CFG_W3_CFG_BUS2_NSA_RA_ID                        BIT(16)
#define SEC_TZC_BUS_CFG_W3_CFG_BUS2_NSA_WA_ID                        BIT(0)
#define SEC_TZC_BUS_CFG_W3_CFG_BUS2_NSA_RA_MASK                      BIT(24)
#define SEC_TZC_BUS_CFG_W3_CFG_BUS2_NSA_WA_MASK                      BIT(8)
#define SEC_TZC_BUS_CFG_W4_CFG_BUS3_NSA_WA_ID                        BIT(0)
#define SEC_TZC_BUS_CFG_W4_CFG_BUS3_NSA_WA_MASK                      BIT(8)
#define SEC_TZC_BUS_CFG_W4_CFG_BUS3_NSA_RA_MASK                      BIT(24)
#define SEC_TZC_BUS_CFG_W4_CFG_BUS3_NSA_RA_ID                        BIT(16)
#define SEC_TZC_BUS_CFG_W5_CFG_BUS4_NSA_WA_ID                        BIT(0)
#define SEC_TZC_BUS_CFG_W5_CFG_BUS4_NSA_RA_ID                        BIT(16)
#define SEC_TZC_BUS_CFG_W5_CFG_BUS4_NSA_WA_MASK                      BIT(8)
#define SEC_TZC_BUS_CFG_W5_CFG_BUS4_NSA_RA_MASK                      BIT(24)
#define SEC_TZC_BUS_CFG_W6_CFG_BUS5_NSA_WA_MASK                      BIT(8)
#define SEC_TZC_BUS_CFG_W6_CFG_BUS5_NSA_RA_MASK                      BIT(24)
#define SEC_TZC_BUS_CFG_W6_CFG_BUS5_NSA_WA_ID                        BIT(0)
#define SEC_TZC_BUS_CFG_W6_CFG_BUS5_NSA_RA_ID                        BIT(16)
#define SEC_TZC_BUS_CFG_W7_CFG_BUS6_NSA_RA_MASK                      BIT(24)
#define SEC_TZC_BUS_CFG_W7_CFG_BUS6_NSA_WA_ID                        BIT(0)
#define SEC_TZC_BUS_CFG_W7_CFG_BUS6_NSA_WA_MASK                      BIT(8)
#define SEC_TZC_BUS_CFG_W7_CFG_BUS6_NSA_RA_ID                        BIT(16)

#define SEC_TZC_BUS_CFG_W0_SEC_APB_TZC_BUS_PROT_MASK                 0x00070000
#define SEC_TZC_BUS_CFG_W0_CFG_BUS_DDR_WA_DOMAIN_MASK                0x00000003
#define SEC_TZC_BUS_CFG_W0_CFG_BUS_DDR_RA_DOMAIN_SEL_MASK            0x00003000
#define SEC_TZC_BUS_CFG_W0_CFG_BUS_DDR_RA_DOMAIN_MASK                0x00000300
#define SEC_TZC_BUS_CFG_W0_CFG_BUS_DDR_WA_DOMAIN_SEL_MASK            0x00000030
#define SEC_TZC_BUS_CFG_W1_CFG_BUS0_NSA_WA_ID_MASK                   0x000000ff
#define SEC_TZC_BUS_CFG_W1_CFG_BUS0_NSA_RA_ID_MASK                   0x00ff0000
#define SEC_TZC_BUS_CFG_W1_CFG_BUS0_NSA_RA_MASK_MASK                 0xff000000
#define SEC_TZC_BUS_CFG_W1_CFG_BUS0_NSA_WA_MASK_MASK                 0x0000ff00
#define SEC_TZC_BUS_CFG_W2_CFG_BUS1_NSA_WA_MASK_MASK                 0x0000ff00
#define SEC_TZC_BUS_CFG_W2_CFG_BUS1_NSA_WA_ID_MASK                   0x000000ff
#define SEC_TZC_BUS_CFG_W2_CFG_BUS1_NSA_RA_ID_MASK                   0x00ff0000
#define SEC_TZC_BUS_CFG_W2_CFG_BUS1_NSA_RA_MASK_MASK                 0xff000000
#define SEC_TZC_BUS_CFG_W3_CFG_BUS2_NSA_RA_ID_MASK                   0x00ff0000
#define SEC_TZC_BUS_CFG_W3_CFG_BUS2_NSA_WA_ID_MASK                   0x000000ff
#define SEC_TZC_BUS_CFG_W3_CFG_BUS2_NSA_RA_MASK_MASK                 0xff000000
#define SEC_TZC_BUS_CFG_W3_CFG_BUS2_NSA_WA_MASK_MASK                 0x0000ff00
#define SEC_TZC_BUS_CFG_W4_CFG_BUS3_NSA_WA_ID_MASK                   0x000000ff
#define SEC_TZC_BUS_CFG_W4_CFG_BUS3_NSA_WA_MASK_MASK                 0x0000ff00
#define SEC_TZC_BUS_CFG_W4_CFG_BUS3_NSA_RA_MASK_MASK                 0xff000000
#define SEC_TZC_BUS_CFG_W4_CFG_BUS3_NSA_RA_ID_MASK                   0x00ff0000
#define SEC_TZC_BUS_CFG_W5_CFG_BUS4_NSA_WA_ID_MASK                   0x000000ff
#define SEC_TZC_BUS_CFG_W5_CFG_BUS4_NSA_RA_ID_MASK                   0x00ff0000
#define SEC_TZC_BUS_CFG_W5_CFG_BUS4_NSA_WA_MASK_MASK                 0x0000ff00
#define SEC_TZC_BUS_CFG_W5_CFG_BUS4_NSA_RA_MASK_MASK                 0xff000000
#define SEC_TZC_BUS_CFG_W6_CFG_BUS5_NSA_WA_MASK_MASK                 0x0000ff00
#define SEC_TZC_BUS_CFG_W6_CFG_BUS5_NSA_RA_MASK_MASK                 0xff000000
#define SEC_TZC_BUS_CFG_W6_CFG_BUS5_NSA_WA_ID_MASK                   0x000000ff
#define SEC_TZC_BUS_CFG_W6_CFG_BUS5_NSA_RA_ID_MASK                   0x00ff0000
#define SEC_TZC_BUS_CFG_W7_CFG_BUS6_NSA_RA_MASK_MASK                 0xff000000
#define SEC_TZC_BUS_CFG_W7_CFG_BUS6_NSA_WA_ID_MASK                   0x000000ff
#define SEC_TZC_BUS_CFG_W7_CFG_BUS6_NSA_WA_MASK_MASK                 0x0000ff00
#define SEC_TZC_BUS_CFG_W7_CFG_BUS6_NSA_RA_ID_MASK                   0x00ff0000

/* ################################################################################
 * # SecTzcMacCfg Definition
 */
#define SEC_TZC_MAC_CFG_W0_CFG_CPU_MAC_WA_DOMAIN_SEL                 BIT(4)
#define SEC_TZC_MAC_CFG_W0_CFG_CPU_MAC_RA_DOMAIN_SEL                 BIT(12)
#define SEC_TZC_MAC_CFG_W0_CFG_CPU_MAC_WA_DOMAIN                     BIT(0)
#define SEC_TZC_MAC_CFG_W0_CFG_CPU_MAC_RA_DOMAIN                     BIT(8)
#define SEC_TZC_MAC_CFG_W1_CFG_CPU_MAC0_NSA_WA_ID                    BIT(0)
#define SEC_TZC_MAC_CFG_W1_CFG_CPU_MAC0_NSA_RA_MASK                  BIT(24)
#define SEC_TZC_MAC_CFG_W1_CFG_CPU_MAC0_NSA_WA_MASK                  BIT(8)
#define SEC_TZC_MAC_CFG_W1_CFG_CPU_MAC0_NSA_RA_ID                    BIT(16)
#define SEC_TZC_MAC_CFG_W2_CFG_CPU_MAC1_NSA_RA_ID                    BIT(16)
#define SEC_TZC_MAC_CFG_W2_CFG_CPU_MAC1_NSA_WA_MASK                  BIT(8)
#define SEC_TZC_MAC_CFG_W2_CFG_CPU_MAC1_NSA_RA_MASK                  BIT(24)
#define SEC_TZC_MAC_CFG_W2_CFG_CPU_MAC1_NSA_WA_ID                    BIT(0)
#define SEC_TZC_MAC_CFG_W3_CFG_CPU_MAC2_NSA_RA_ID                    BIT(16)
#define SEC_TZC_MAC_CFG_W3_CFG_CPU_MAC2_NSA_RA_MASK                  BIT(24)
#define SEC_TZC_MAC_CFG_W3_CFG_CPU_MAC2_NSA_WA_ID                    BIT(0)
#define SEC_TZC_MAC_CFG_W3_CFG_CPU_MAC2_NSA_WA_MASK                  BIT(8)

#define SEC_TZC_MAC_CFG_W0_CFG_CPU_MAC_WA_DOMAIN_SEL_MASK            0x00000030
#define SEC_TZC_MAC_CFG_W0_CFG_CPU_MAC_RA_DOMAIN_SEL_MASK            0x00003000
#define SEC_TZC_MAC_CFG_W0_CFG_CPU_MAC_WA_DOMAIN_MASK                0x00000003
#define SEC_TZC_MAC_CFG_W0_CFG_CPU_MAC_RA_DOMAIN_MASK                0x00000300
#define SEC_TZC_MAC_CFG_W1_CFG_CPU_MAC0_NSA_WA_ID_MASK               0x0000000f
#define SEC_TZC_MAC_CFG_W1_CFG_CPU_MAC0_NSA_RA_MASK_MASK             0x0f000000
#define SEC_TZC_MAC_CFG_W1_CFG_CPU_MAC0_NSA_WA_MASK_MASK             0x00000f00
#define SEC_TZC_MAC_CFG_W1_CFG_CPU_MAC0_NSA_RA_ID_MASK               0x000f0000
#define SEC_TZC_MAC_CFG_W2_CFG_CPU_MAC1_NSA_RA_ID_MASK               0x000f0000
#define SEC_TZC_MAC_CFG_W2_CFG_CPU_MAC1_NSA_WA_MASK_MASK             0x00000f00
#define SEC_TZC_MAC_CFG_W2_CFG_CPU_MAC1_NSA_RA_MASK_MASK             0x0f000000
#define SEC_TZC_MAC_CFG_W2_CFG_CPU_MAC1_NSA_WA_ID_MASK               0x0000000f
#define SEC_TZC_MAC_CFG_W3_CFG_CPU_MAC2_NSA_RA_ID_MASK               0x000f0000
#define SEC_TZC_MAC_CFG_W3_CFG_CPU_MAC2_NSA_RA_MASK_MASK             0x0f000000
#define SEC_TZC_MAC_CFG_W3_CFG_CPU_MAC2_NSA_WA_ID_MASK               0x0000000f
#define SEC_TZC_MAC_CFG_W3_CFG_CPU_MAC2_NSA_WA_MASK_MASK             0x00000f00

/* ################################################################################
 * # SecTzcDmaCfg Definition
 */
#define SEC_TZC_DMA_CFG_W0_CFG_SWITCH_DMA_RA_DOMAIN                  BIT(8)
#define SEC_TZC_DMA_CFG_W0_CFG_SWITCH_DMA_WA_DOMAIN_SEL              BIT(4)
#define SEC_TZC_DMA_CFG_W0_CFG_SWITCH_DMA_RA_DOMAIN_SEL              BIT(12)
#define SEC_TZC_DMA_CFG_W0_CFG_SWITCH_DMA_WA_DOMAIN                  BIT(0)
#define SEC_TZC_DMA_CFG_W1_CFG_SWITCH_DMA0_NSA_WA_ID                 BIT(0)
#define SEC_TZC_DMA_CFG_W1_CFG_SWITCH_DMA0_NSA_RA_MASK               BIT(24)
#define SEC_TZC_DMA_CFG_W1_CFG_SWITCH_DMA0_NSA_WA_MASK               BIT(8)
#define SEC_TZC_DMA_CFG_W1_CFG_SWITCH_DMA0_NSA_RA_ID                 BIT(16)
#define SEC_TZC_DMA_CFG_W2_CFG_SWITCH_DMA1_NSA_RA_MASK               BIT(24)
#define SEC_TZC_DMA_CFG_W2_CFG_SWITCH_DMA1_NSA_WA_ID                 BIT(0)
#define SEC_TZC_DMA_CFG_W2_CFG_SWITCH_DMA1_NSA_WA_MASK               BIT(8)
#define SEC_TZC_DMA_CFG_W2_CFG_SWITCH_DMA1_NSA_RA_ID                 BIT(16)
#define SEC_TZC_DMA_CFG_W3_CFG_SWITCH_DMA2_NSA_WA_ID                 BIT(0)
#define SEC_TZC_DMA_CFG_W3_CFG_SWITCH_DMA2_NSA_RA_MASK               BIT(24)
#define SEC_TZC_DMA_CFG_W3_CFG_SWITCH_DMA2_NSA_WA_MASK               BIT(8)
#define SEC_TZC_DMA_CFG_W3_CFG_SWITCH_DMA2_NSA_RA_ID                 BIT(16)

#define SEC_TZC_DMA_CFG_W0_CFG_SWITCH_DMA_RA_DOMAIN_MASK             0x00000300
#define SEC_TZC_DMA_CFG_W0_CFG_SWITCH_DMA_WA_DOMAIN_SEL_MASK         0x00000030
#define SEC_TZC_DMA_CFG_W0_CFG_SWITCH_DMA_RA_DOMAIN_SEL_MASK         0x00003000
#define SEC_TZC_DMA_CFG_W0_CFG_SWITCH_DMA_WA_DOMAIN_MASK             0x00000003
#define SEC_TZC_DMA_CFG_W1_CFG_SWITCH_DMA0_NSA_WA_ID_MASK            0x0000001f
#define SEC_TZC_DMA_CFG_W1_CFG_SWITCH_DMA0_NSA_RA_MASK_MASK          0x1f000000
#define SEC_TZC_DMA_CFG_W1_CFG_SWITCH_DMA0_NSA_WA_MASK_MASK          0x00001f00
#define SEC_TZC_DMA_CFG_W1_CFG_SWITCH_DMA0_NSA_RA_ID_MASK            0x001f0000
#define SEC_TZC_DMA_CFG_W2_CFG_SWITCH_DMA1_NSA_RA_MASK_MASK          0x1f000000
#define SEC_TZC_DMA_CFG_W2_CFG_SWITCH_DMA1_NSA_WA_ID_MASK            0x0000001f
#define SEC_TZC_DMA_CFG_W2_CFG_SWITCH_DMA1_NSA_WA_MASK_MASK          0x00001f00
#define SEC_TZC_DMA_CFG_W2_CFG_SWITCH_DMA1_NSA_RA_ID_MASK            0x001f0000
#define SEC_TZC_DMA_CFG_W3_CFG_SWITCH_DMA2_NSA_WA_ID_MASK            0x0000001f
#define SEC_TZC_DMA_CFG_W3_CFG_SWITCH_DMA2_NSA_RA_MASK_MASK          0x1f000000
#define SEC_TZC_DMA_CFG_W3_CFG_SWITCH_DMA2_NSA_WA_MASK_MASK          0x00001f00
#define SEC_TZC_DMA_CFG_W3_CFG_SWITCH_DMA2_NSA_RA_ID_MASK            0x001f0000

/* ################################################################################
 * # SysDmaBootCfg Definition
 */
#define SYS_DMA_BOOT_CFG_W0_CFG_DMA_BOOT_IRQ_NS                      BIT(16)
#define SYS_DMA_BOOT_CFG_W0_CFG_DMA_BOOT_MGR_NS                      BIT(1)
#define SYS_DMA_BOOT_CFG_W0_CFG_DMA_BOOT_PERIPH_NS                   BIT(8)
#define SYS_DMA_BOOT_CFG_W0_CFG_DMA_BOOT_FROM_PC                     BIT(0)

#define SYS_DMA_BOOT_CFG_W0_CFG_DMA_BOOT_IRQ_NS_MASK                 0xffff0000
#define SYS_DMA_BOOT_CFG_W0_CFG_DMA_BOOT_MGR_NS_MASK                 0x00000002
#define SYS_DMA_BOOT_CFG_W0_CFG_DMA_BOOT_PERIPH_NS_MASK              0x0000ff00
#define SYS_DMA_BOOT_CFG_W0_CFG_DMA_BOOT_FROM_PC_MASK                0x00000001

/* ################################################################################
 * # SecDmaBootAddr Definition
 */
#define SEC_DMA_BOOT_ADDR_W0_CFG_DMA_BOOT_ADDR                       BIT(0)

#define SEC_DMA_BOOT_ADDR_W0_CFG_DMA_BOOT_ADDR_MASK                  0xffffffff

/* ################################################################################
 * # SecInitCtl Definition
 */
#define SEC_INIT_CTL_W0_MEM_INIT_SEC_EN                              BIT(0)

#define SEC_INIT_CTL_W0_MEM_INIT_SEC_EN_MASK                         0x00000001

/* ################################################################################
 * # SecSysCntCfg Definition
 */
#define SEC_SYS_CNT_CFG_W0_SEC_SYS_CNT_HALT                          BIT(8)
#define SEC_SYS_CNT_CFG_W0_CFG_SYS_CNT_RELOAD                        BIT(1)
#define SEC_SYS_CNT_CFG_W0_CFG_SYS_CNT_EN                            BIT(0)
#define SEC_SYS_CNT_CFG_W0_CFG_SYS_CNT_DIV                           BIT(16)
#define SEC_SYS_CNT_CFG_W0_CFG_SYS_CNT_INC_NUM                       BIT(4)
#define SEC_SYS_CNT_CFG_W0_SEC_SYS_CNT_OVER                          BIT(9)

#define SEC_SYS_CNT_CFG_W0_SEC_SYS_CNT_HALT_MASK                     0x00000100
#define SEC_SYS_CNT_CFG_W0_CFG_SYS_CNT_RELOAD_MASK                   0x00000002
#define SEC_SYS_CNT_CFG_W0_CFG_SYS_CNT_EN_MASK                       0x00000001
#define SEC_SYS_CNT_CFG_W0_CFG_SYS_CNT_DIV_MASK                      0x03ff0000
#define SEC_SYS_CNT_CFG_W0_CFG_SYS_CNT_INC_NUM_MASK                  0x000000f0
#define SEC_SYS_CNT_CFG_W0_SEC_SYS_CNT_OVER_MASK                     0x00000200

/* ################################################################################
 * # SecSysCntValue Definition
 */
#define SEC_SYS_CNT_VALUE_W0_SEC_SYS_CNT_VALUE_31_0                  BIT(0)
#define SEC_SYS_CNT_VALUE_W1_SEC_SYS_CNT_VALUE_63_32                 BIT(0)

#define SEC_SYS_CNT_VALUE_W0_SEC_SYS_CNT_VALUE_31_0_MASK             0x00000001
#define SEC_SYS_CNT_VALUE_W1_SEC_SYS_CNT_VALUE_63_32_MASK            0x00000001

/* ################################################################################
 * # SecSysCntReload Definition
 */
#define SEC_SYS_CNT_RELOAD_W0_CFG_SYS_CNT_VALUE_31_0                 BIT(0)
#define SEC_SYS_CNT_RELOAD_W1_CFG_SYS_CNT_VALUE_63_32                BIT(0)

#define SEC_SYS_CNT_RELOAD_W0_CFG_SYS_CNT_VALUE_31_0_MASK            0x00000001
#define SEC_SYS_CNT_RELOAD_W1_CFG_SYS_CNT_VALUE_63_32_MASK           0x00000001

/* ################################################################################
 * # SecOcmCfg Definition
 */
#define SEC_OCM_CFG_W0_SEC_DBG_RAM_WR_EN                             BIT(4)
#define SEC_OCM_CFG_W0_SEC_CPU_CORE_ID                               BIT(16)
#define SEC_OCM_CFG_W0_SEC_MSH_RAM_RD_EN                             BIT(10)
#define SEC_OCM_CFG_W0_SEC_USB_RAM_WR_EN                             BIT(13)
#define SEC_OCM_CFG_W0_SEC_PCIE_ROM_EN                               BIT(2)
#define SEC_OCM_CFG_W0_SEC_PCIE_INTF_ID                              BIT(24)
#define SEC_OCM_CFG_W0_SEC_DMA_RAM_WR_EN                             BIT(9)
#define SEC_OCM_CFG_W0_SEC_DBG_ROM_EN                                BIT(1)
#define SEC_OCM_CFG_W0_SEC_PCIE_RAM_WR_EN                            BIT(7)
#define SEC_OCM_CFG_W0_SEC_BYPASS_EN                                 BIT(0)
#define SEC_OCM_CFG_W0_SEC_DBG_RAM_RD_EN                             BIT(5)
#define SEC_OCM_CFG_W0_SEC_EXT_RAM_EN                                BIT(3)
#define SEC_OCM_CFG_W0_SEC_PCIE_RAM_RD_EN                            BIT(6)
#define SEC_OCM_CFG_W0_SEC_USB_RAM_RD_EN                             BIT(12)
#define SEC_OCM_CFG_W0_SEC_MSH_RAM_WR_EN                             BIT(11)
#define SEC_OCM_CFG_W0_SEC_DMA_RAM_RD_EN                             BIT(8)
#define SEC_OCM_CFG_W1_SEC_DMA_INTF_ID                               BIT(8)
#define SEC_OCM_CFG_W1_SEC_USB_INTF_ID                               BIT(24)
#define SEC_OCM_CFG_W1_SEC_DBG_INTF_ID                               BIT(0)
#define SEC_OCM_CFG_W1_SEC_MSH_INTF_ID                               BIT(16)

#define SEC_OCM_CFG_W0_SEC_DBG_RAM_WR_EN_MASK                        0x00000010
#define SEC_OCM_CFG_W0_SEC_CPU_CORE_ID_MASK                          0x001f0000
#define SEC_OCM_CFG_W0_SEC_MSH_RAM_RD_EN_MASK                        0x00000400
#define SEC_OCM_CFG_W0_SEC_USB_RAM_WR_EN_MASK                        0x00002000
#define SEC_OCM_CFG_W0_SEC_PCIE_ROM_EN_MASK                          0x00000004
#define SEC_OCM_CFG_W0_SEC_PCIE_INTF_ID_MASK                         0x1f000000
#define SEC_OCM_CFG_W0_SEC_DMA_RAM_WR_EN_MASK                        0x00000200
#define SEC_OCM_CFG_W0_SEC_DBG_ROM_EN_MASK                           0x00000002
#define SEC_OCM_CFG_W0_SEC_PCIE_RAM_WR_EN_MASK                       0x00000080
#define SEC_OCM_CFG_W0_SEC_BYPASS_EN_MASK                            0x00000001
#define SEC_OCM_CFG_W0_SEC_DBG_RAM_RD_EN_MASK                        0x00000020
#define SEC_OCM_CFG_W0_SEC_EXT_RAM_EN_MASK                           0x00000008
#define SEC_OCM_CFG_W0_SEC_PCIE_RAM_RD_EN_MASK                       0x00000040
#define SEC_OCM_CFG_W0_SEC_USB_RAM_RD_EN_MASK                        0x00001000
#define SEC_OCM_CFG_W0_SEC_MSH_RAM_WR_EN_MASK                        0x00000800
#define SEC_OCM_CFG_W0_SEC_DMA_RAM_RD_EN_MASK                        0x00000100
#define SEC_OCM_CFG_W1_SEC_DMA_INTF_ID_MASK                          0x00001f00
#define SEC_OCM_CFG_W1_SEC_USB_INTF_ID_MASK                          0x1f000000
#define SEC_OCM_CFG_W1_SEC_DBG_INTF_ID_MASK                          0x0000001f
#define SEC_OCM_CFG_W1_SEC_MSH_INTF_ID_MASK                          0x001f0000

/* ################################################################################
 * # SecOcmDmaCfg Definition
 */
#define SEC_OCM_DMA_CFG_W0_CFG0_DMA_INTF_ADDR                        BIT(0)
#define SEC_OCM_DMA_CFG_W0_CFG0_DMA_INTF_MASK                        BIT(16)
#define SEC_OCM_DMA_CFG_W1_CFG1_DMA_INTF_MASK                        BIT(16)
#define SEC_OCM_DMA_CFG_W1_CFG1_DMA_INTF_ADDR                        BIT(0)

#define SEC_OCM_DMA_CFG_W0_CFG0_DMA_INTF_ADDR_MASK                   0x00003fff
#define SEC_OCM_DMA_CFG_W0_CFG0_DMA_INTF_MASK_MASK                   0x3fff0000
#define SEC_OCM_DMA_CFG_W1_CFG1_DMA_INTF_MASK_MASK                   0x3fff0000
#define SEC_OCM_DMA_CFG_W1_CFG1_DMA_INTF_ADDR_MASK                   0x00003fff

/* ################################################################################
 * # SecOcmPcieCfg Definition
 */
#define SEC_OCM_PCIE_CFG_W0_CFG0_PCIE_INTF_ADDR                      BIT(0)
#define SEC_OCM_PCIE_CFG_W0_CFG0_PCIE_INTF_MASK                      BIT(16)
#define SEC_OCM_PCIE_CFG_W1_CFG1_PCIE_INTF_ADDR                      BIT(0)
#define SEC_OCM_PCIE_CFG_W1_CFG1_PCIE_INTF_MASK                      BIT(16)

#define SEC_OCM_PCIE_CFG_W0_CFG0_PCIE_INTF_ADDR_MASK                 0x00003fff
#define SEC_OCM_PCIE_CFG_W0_CFG0_PCIE_INTF_MASK_MASK                 0x3fff0000
#define SEC_OCM_PCIE_CFG_W1_CFG1_PCIE_INTF_ADDR_MASK                 0x00003fff
#define SEC_OCM_PCIE_CFG_W1_CFG1_PCIE_INTF_MASK_MASK                 0x3fff0000

/* ################################################################################
 * # SecOcmMshCfg Definition
 */
#define SEC_OCM_MSH_CFG_W0_CFG0_MSH_INTF_ADDR                        BIT(0)
#define SEC_OCM_MSH_CFG_W0_CFG0_MSH_INTF_MASK                        BIT(16)
#define SEC_OCM_MSH_CFG_W1_CFG1_MSH_INTF_ADDR                        BIT(0)
#define SEC_OCM_MSH_CFG_W1_CFG1_MSH_INTF_MASK                        BIT(16)

#define SEC_OCM_MSH_CFG_W0_CFG0_MSH_INTF_ADDR_MASK                   0x00003fff
#define SEC_OCM_MSH_CFG_W0_CFG0_MSH_INTF_MASK_MASK                   0x3fff0000
#define SEC_OCM_MSH_CFG_W1_CFG1_MSH_INTF_ADDR_MASK                   0x00003fff
#define SEC_OCM_MSH_CFG_W1_CFG1_MSH_INTF_MASK_MASK                   0x3fff0000

/* ################################################################################
 * # SecOcmUsbCfg Definition
 */
#define SEC_OCM_USB_CFG_W0_CFG0_USB_INTF_ADDR                        BIT(0)
#define SEC_OCM_USB_CFG_W0_CFG0_USB_INTF_MASK                        BIT(16)
#define SEC_OCM_USB_CFG_W1_CFG1_USB_INTF_MASK                        BIT(16)
#define SEC_OCM_USB_CFG_W1_CFG1_USB_INTF_ADDR                        BIT(0)

#define SEC_OCM_USB_CFG_W0_CFG0_USB_INTF_ADDR_MASK                   0x00003fff
#define SEC_OCM_USB_CFG_W0_CFG0_USB_INTF_MASK_MASK                   0x3fff0000
#define SEC_OCM_USB_CFG_W1_CFG1_USB_INTF_MASK_MASK                   0x3fff0000
#define SEC_OCM_USB_CFG_W1_CFG1_USB_INTF_ADDR_MASK                   0x00003fff

/* ################################################################################
 * # SecNumCtl Definition
 */
#define SEC_NUM_CTL_W0_SEC_NUM                                       BIT(0)

#define SEC_NUM_CTL_W0_SEC_NUM_MASK                                  0xffffffff

/* ################################################################################
 * # SecEFuseEn Definition
 */
#define SEC_E_FUSE_EN_W0_E_FUSE_WR_VEC                               BIT(0)
#define SEC_E_FUSE_EN_W1_E_FUSE_RD_VEC                               BIT(0)

#define SEC_E_FUSE_EN_W0_E_FUSE_WR_VEC_MASK                          0xffffffff
#define SEC_E_FUSE_EN_W1_E_FUSE_RD_VEC_MASK                          0xffffffff


struct SecCtl_mems {
   u32 SecEFuseMem0[1];                 /* 0x00000100 */
   u32 SecEFuseMem1[1];                 /* 0x00000104 */
   u32 SecEFuseMem2[1];                 /* 0x00000108 */
   u32 SecEFuseMem3[1];                 /* 0x0000010c */
   u32 SecEFuseMem4[1];                 /* 0x00000110 */
   u32 SecEFuseMem5[1];                 /* 0x00000114 */
   u32 SecEFuseMem6[1];                 /* 0x00000118 */
   u32 SecEFuseMem7[1];                 /* 0x0000011c */
   u32 SecEFuseMem8[1];                 /* 0x00000120 */
   u32 SecEFuseMem9[1];                 /* 0x00000124 */
   u32 SecEFuseMem10[1];                /* 0x00000128 */
   u32 SecEFuseMem11[1];                /* 0x0000012c */
   u32 SecEFuseMem12[1];                /* 0x00000130 */
   u32 SecEFuseMem13[1];                /* 0x00000134 */
   u32 SecEFuseMem14[1];                /* 0x00000138 */
   u32 SecEFuseMem15[1];                /* 0x0000013c */
   u32 SecEFuseMem16[1];                /* 0x00000140 */
   u32 SecEFuseMem17[1];                /* 0x00000144 */
   u32 SecEFuseMem18[1];                /* 0x00000148 */
   u32 SecEFuseMem19[1];                /* 0x0000014c */
   u32 SecEFuseMem20[1];                /* 0x00000150 */
   u32 SecEFuseMem21[1];                /* 0x00000154 */
   u32 SecEFuseMem22[1];                /* 0x00000158 */
   u32 SecEFuseMem23[1];                /* 0x0000015c */
   u32 SecEFuseMem24[1];                /* 0x00000160 */
   u32 SecEFuseMem25[1];                /* 0x00000164 */
   u32 SecEFuseMem26[1];                /* 0x00000168 */
   u32 SecEFuseMem27[1];                /* 0x0000016c */
   u32 SecEFuseMem28[1];                /* 0x00000170 */
   u32 SecEFuseMem29[1];                /* 0x00000174 */
   u32 SecEFuseMem30[1];                /* 0x00000178 */
   u32 SecEFuseMem31[1];                /* 0x0000017c */
};

/* ################################################################################
 * # SecEFuseMem Definition
 */
#define SEC_E_FUSE_MEM_W0_DATA                                       BIT(0)

#define SEC_E_FUSE_MEM_W0_DATA_MASK                                  0xffffffff

#define secctl_base ((struct SecCtl_regs *)CONFIG_SYS_CTC5236_SECCTL_BASE)
#endif
	
#endif /*__CTC5236_SECCTL_H__*/

