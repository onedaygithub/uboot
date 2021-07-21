/*
 * (C) Copyright 2004-2017 Centec Networks (suzhou) Co., LTD.
 * Jay Cao <caoj@centecnetworks.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <wait_bit.h>
#include <asm/io.h>

#include <asm/arch/cpu.h>
#include <ctc5236_ddr.h>
#include <asm/arch-ctc5236/sysctl.h>

#include "ddr_ctl_reg.h"
#include "ddr_phy_reg.h"

DECLARE_GLOBAL_DATA_PTR;

/*ddr clock config*/
#define PREDIV_2400 	0x0
#define PREDIV_2133 	0x2
#define PREDIV_1866 	0x0
#define PREDIV_1600 	0x0
#define PREDIV_1333 	0x2
#define PREDIV_1066 	0x2
#define PREDIV_800  		0x0

#define MULTINT_2400 	0x2f
#define MULTINT_2133 	0x7f
#define MULTINT_1866 	0x37
#define MULTINT_1600 	0x2f
#define MULTINT_1333 	0x9f
#define MULTINT_1066 	0x9f
#define MULTINT_800  	0x37

#define POSTDIV_2400 	0x3
#define POSTDIV_2133 	0x3
#define POSTDIV_1866 	0x5
#define POSTDIV_1600 	0x5
#define POSTDIV_1333 	0x7
#define POSTDIV_1066 	0x9
#define POSTDIV_800  	0xd

#define INTFBK_2400 		0x1
#define INTFBK_2133 		0x1
#define INTFBK_1866 		0x1
#define INTFBK_1600 		0x1
#define INTFBK_1333 		0x1
#define INTFBK_1066 		0x1
#define INTFBK_800 		0x1

#define SIC_2400 		0xf
#define SIC_2133 		0x19
#define SIC_1866 		0x13
#define SIC_1600 		0xf
#define SIC_1333 		0x19
#define SIC_1066 		0x19
#define SIC_800  			0x13


#define SIP_2400 		0x9
#define SIP_2133 		0x13
#define SIP_1866 		0xd
#define SIP_1600 		0x9
#define SIP_1333 		0x13
#define SIP_1066 		0x13
#define SIP_800 			0xd

#define SGAIN_2400 		0x4
#define SGAIN_2133 		0x4
#define SGAIN_1866 		0x4
#define SGAIN_1600 		0x4
#define SGAIN_1333 		0x4
#define SGAIN_1066 		0x4
#define SGAIN_800  		0x4

#define SLOCK_2400 		0x1
#define SLOCK_2133 		0x1
#define SLOCK_1866 		0x1
#define SLOCK_1600 		0x1
#define SLOCK_1333 		0x1
#define SLOCK_1066 		0x1
#define SLOCK_800  		0x1

/* ################################################################################
 * # SysPllDdrCfg0 Definition
 */
#define SYS_PLL_DDR_CFG0_DCO_BYPASS                      (1)
#define SYS_PLL_DDR_CFG0_PRE_DIV                          (4)
#define SYS_PLL_DDR_CFG0_RESET                           (0)
#define SYS_PLL_DDR_CFG0_INT_FBK                          (2)
#define SYS_PLL_DDR_CFG0_PLL_PWD                         (3)
#define SYS_PLL_DDR_CFG0_MULT_INT                         (20)
#define SYS_PLL_DDR_CFG0_POST_DIV                        (12)

#define SYS_PLL_DDR_CFG0_DCO_BYPASS_MASK                  0x00000002
#define SYS_PLL_DDR_CFG0_PRE_DIV_MASK                     0x000001f0
#define SYS_PLL_DDR_CFG0_RESET_MASK                       0x00000001
#define SYS_PLL_DDR_CFG0_INT_FBK_MASK                     0x00000004
#define SYS_PLL_DDR_CFG0_PLL_PWD_MASK                     0x00000008
#define SYS_PLL_DDR_CFG0_MULT_INT_MASK                    0x0ff00000
#define SYS_PLL_DDR_CFG0_POST_DIV_MASK                    0x0003f000

/* ################################################################################
 * # SysPllDdrCfg1 Definition
 */
#define SYS_PLL_DDR_CFG1_SLOCK                            (16)
#define SYS_PLL_DDR_CFG1_BYPASS                        (24)
#define SYS_PLL_DDR_CFG1_MON_LOCK                   (28)
#define SYS_PLL_DDR_CFG1_SIC                            (8)
#define SYS_PLL_DDR_CFG1_SIP                              (0)
#define SYS_PLL_DDR_CFG1_SGAIN                          (20)

#define SYS_PLL_DDR_CFG1_SLOCK_MASK                       0x00010000
#define SYS_PLL_DDR_CFG1_BYPASS_MASK                      0x01000000
#define SYS_PLL_DDR_CFG1_MON_LOCK_MASK                    0x10000000
#define SYS_PLL_DDR_CFG1_SIC_MASK                         0x00001f00
#define SYS_PLL_DDR_CFG1_SIP_MASK                         0x0000001f
#define SYS_PLL_DDR_CFG1_SGAIN_MASK                       0x00700000



phys_size_t ctc_ddrclk_set(u32 ddr_rate)
{
    u32 PllDdrCfg0;
    u32 PllDdrCfg1;

    /* de-assert pllDdrPllPwd */
    clrbits_le32(&sysctl_base->SysPllDdrCfg0, SYS_PLL_DDR_CFG0_W0_PLL_DDR_PLL_PWD);

    switch(ddr_rate)
    {
    	case DDRINTF_RATE_2400:
		/*
		*PllDdrCfg0 = PreDiv|MultInt|PostDiv|IntFbk;
		*PllDdrCfg1 = Sic|Sip|Sgain|Slock;
		*/
		PllDdrCfg0 = (0x0
				|(PREDIV_2400 << SYS_PLL_DDR_CFG0_PRE_DIV)
				|(MULTINT_2400 << SYS_PLL_DDR_CFG0_MULT_INT)
				|(POSTDIV_2400 << SYS_PLL_DDR_CFG0_POST_DIV)
				|(INTFBK_2400 << SYS_PLL_DDR_CFG0_INT_FBK)
				);
		PllDdrCfg1 = (0x0
				|(SIC_2400<<SYS_PLL_DDR_CFG1_SIC)
				|(SIP_2400<<SYS_PLL_DDR_CFG1_SIP)
				|(SGAIN_2400<<SYS_PLL_DDR_CFG1_SGAIN)
				|(SLOCK_2400<<SYS_PLL_DDR_CFG1_SLOCK)
				);
		break;
	case DDRINTF_RATE_2133:
		/*
		*PllDdrCfg0 = PreDiv|MultInt|PostDiv|IntFbk;
		*PllDdrCfg1 = Sic|Sip|Sgain|Slock;
		*/
		PllDdrCfg0 = (0x0
				|(PREDIV_2133<<SYS_PLL_DDR_CFG0_PRE_DIV)
				|(MULTINT_2133<<SYS_PLL_DDR_CFG0_MULT_INT)
				|(POSTDIV_2133<<SYS_PLL_DDR_CFG0_POST_DIV)
				|(INTFBK_2133<<SYS_PLL_DDR_CFG0_INT_FBK)
				);
		PllDdrCfg1 = (0x0
				|(SIC_2133<<SYS_PLL_DDR_CFG1_SIC)
				|(SIP_2133<<SYS_PLL_DDR_CFG1_SIP)
				|(SGAIN_2133<<SYS_PLL_DDR_CFG1_SGAIN)
				|(SLOCK_2133<<SYS_PLL_DDR_CFG1_SLOCK)
				);
		break;
	case DDRINTF_RATE_1866:
		/*
		*PllDdrCfg0 = PreDiv|MultInt|PostDiv|IntFbk;
		*PllDdrCfg1 = Sic|Sip|Sgain|Slock;
		*/
		PllDdrCfg0 = (0x0
				|(PREDIV_1866<<SYS_PLL_DDR_CFG0_PRE_DIV)
				|(MULTINT_1866<<SYS_PLL_DDR_CFG0_MULT_INT)
				|(POSTDIV_1866<<SYS_PLL_DDR_CFG0_POST_DIV)
				|(INTFBK_1866<<SYS_PLL_DDR_CFG0_INT_FBK)
				);
		PllDdrCfg1 = (0x0
				|(SIC_1866<<SYS_PLL_DDR_CFG1_SIC)
				|(SIP_1866<<SYS_PLL_DDR_CFG1_SIP)
				|(SGAIN_1866<<SYS_PLL_DDR_CFG1_SGAIN)
				|(SLOCK_1866<<SYS_PLL_DDR_CFG1_SLOCK)
				);
		break;
	case DDRINTF_RATE_1600:
		/*
		*PllDdrCfg0 = PreDiv|MultInt|PostDiv|IntFbk;
		*PllDdrCfg1 = Sic|Sip|Sgain|Slock;
		*/
		PllDdrCfg0 = (0x0
				|(PREDIV_1600<<SYS_PLL_DDR_CFG0_PRE_DIV)
				|(MULTINT_1600<<SYS_PLL_DDR_CFG0_MULT_INT)
				|(POSTDIV_1600<<SYS_PLL_DDR_CFG0_POST_DIV)
				|(INTFBK_1600<<SYS_PLL_DDR_CFG0_INT_FBK)
				);
		PllDdrCfg1 = (0x0
				|(SIC_1600<<SYS_PLL_DDR_CFG1_SIC)
				|(SIP_1600<<SYS_PLL_DDR_CFG1_SIP)
				|(SGAIN_1600<<SYS_PLL_DDR_CFG1_SGAIN)
				|(SLOCK_1600<<SYS_PLL_DDR_CFG1_SLOCK)
				);
		break;
	case DDRINTF_RATE_1333:
		/*
		*PllDdrCfg0 = PreDiv|MultInt|PostDiv|IntFbk;
		*PllDdrCfg1 = Sic|Sip|Sgain|Slock;
		*/
		PllDdrCfg0 = (0x0
				|(PREDIV_1333<<SYS_PLL_DDR_CFG0_PRE_DIV)
				|(MULTINT_1333<<SYS_PLL_DDR_CFG0_MULT_INT)
				|(POSTDIV_1333<<SYS_PLL_DDR_CFG0_POST_DIV)
				|(INTFBK_1333<<SYS_PLL_DDR_CFG0_INT_FBK)
				);
		PllDdrCfg1 = (0x0
				|(SIC_1333<<SYS_PLL_DDR_CFG1_SIC)
				|(SIP_1333<<SYS_PLL_DDR_CFG1_SIP)
				|(SGAIN_1333<<SYS_PLL_DDR_CFG1_SGAIN)
				|(SLOCK_1333<<SYS_PLL_DDR_CFG1_SLOCK)
				);
		break;
	case DDRINTF_RATE_1066:
		/*
		*PllDdrCfg0 = PreDiv|MultInt|PostDiv|IntFbk;
		*PllDdrCfg1 = Sic|Sip|Sgain|Slock;
		*/
		PllDdrCfg0 = (0x0
				|(PREDIV_1066<<SYS_PLL_DDR_CFG0_PRE_DIV)
				|(MULTINT_1066<<SYS_PLL_DDR_CFG0_MULT_INT)
				|(POSTDIV_1066<<SYS_PLL_DDR_CFG0_POST_DIV)
				|(INTFBK_1066<<SYS_PLL_DDR_CFG0_INT_FBK)
				);
		PllDdrCfg1 = (0x0
				|(SIC_1066<<SYS_PLL_DDR_CFG1_SIC)
				|(SIP_1066<<SYS_PLL_DDR_CFG1_SIP)
				|(SGAIN_1066<<SYS_PLL_DDR_CFG1_SGAIN)
				|(SLOCK_1066<<SYS_PLL_DDR_CFG1_SLOCK)
				);
		break;
	case DDRINTF_RATE_800:
		/*
		*PllDdrCfg0 = PreDiv|MultInt|PostDiv|IntFbk;
		*PllDdrCfg1 = Sic|Sip|Sgain|Slock;
		*/
		PllDdrCfg0 = (0x0
				|(PREDIV_800<<SYS_PLL_DDR_CFG0_PRE_DIV)
				|(MULTINT_800<<SYS_PLL_DDR_CFG0_MULT_INT)
				|(POSTDIV_800<<SYS_PLL_DDR_CFG0_POST_DIV)
				|(INTFBK_800<<SYS_PLL_DDR_CFG0_INT_FBK)
				);
		PllDdrCfg1 = (0x0
				|(SIC_800<<SYS_PLL_DDR_CFG1_SIC)
				|(SIP_800<<SYS_PLL_DDR_CFG1_SIP)
				|(SGAIN_800<<SYS_PLL_DDR_CFG1_SGAIN)
				|(SLOCK_800<<SYS_PLL_DDR_CFG1_SLOCK)
				);
		break;
	default:
		return -1;
    }

    /*PLLDDR configer*/
   //debug("[write]CTC SYSCTL :  PllDdrCfg0 = 0x%08x\n", PllDdrCfg0);
   debug("PllDdrCfg0:0x%08x\n", PllDdrCfg0);
    writel(PllDdrCfg0, &sysctl_base->SysPllDdrCfg0);
    //debug("[write]CTC SYSCTL :  PllDdrCfg1 = 0x%08x\n", PllDdrCfg1);
    debug("PllDdrCfg1:0x%08x\n", PllDdrCfg1);
    writel(PllDdrCfg1, &sysctl_base->SysPllDdrCfg1);
	
    return 0;
}
