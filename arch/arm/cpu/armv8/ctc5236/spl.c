/*
 * (C) Copyright 2004-2017 Centec Networks (suzhou) Co., LTD.
 * Jay Cao <caoj@centecnetworks.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <debug_uart.h>
#include <spl.h>
#include <spi_flash.h>

#include <asm/io.h>
#include <asm/spl.h>

#include <asm/arch/cpu.h>
#include <asm/arch/sysctl.h>

#include "spl.h"
#include <switch.h>

DECLARE_GLOBAL_DATA_PTR;

u32 spl_boot_device(void)
{
	/* TODO: Jay Cao */
	u32 reg = 0;
	u8 bootmode;

	reg = readl(&sysctl_base->BootStrapPin);

	bootmode = reg & BOOT_STRAP_PIN_W0_BOOT_STRAP_LOG_MASK;

	switch (bootmode) {
	case ROM_QSPI_MODE:
		return BOOT_DEVICE_SPI;
	case ROM_EMMC_MODE:
	case ROM_SD_MODE:
		return BOOT_DEVICE_MMC1;
	case ROM_UART_MODE:
		return BOOT_DEVICE_UART;
	case NON_ROM_MODE:
		return BOOT_DEVICE_SPI;
	case ROM_QSPI_DEBUG_MODE:
		return BOOT_DEVICE_SPI;
	default:
		printf("Invalid Boot Mode:0x%x\n", bootmode);
		break;
	}

	return 0;
}

void board_boot_order(u32 *spl_boot_list)
{
	spl_boot_list[0] = spl_boot_device();
	spl_boot_list[1] = BOOT_DEVICE_UART;
}

u32 spl_boot_mode(const u32 boot_device)
{
	/* TODO: Jay Cao */
	return 0;
}


#ifdef CONFIG_SPL_BUILD


void board_init_f(ulong dummy)
{	
	unsigned int val, freq_cpu;
	unsigned int clk_peri_cfg0, clk_peri_cfg1;
	
	/* release Switchcore interface */
	writel(0x7, &sysctl_base->SysAxiSupResetCtl);
	writel(0x0, &sysctl_base->SysAxiSupResetCtl);

#ifndef CONFIG_TARGET_EMU_CTC5236
	ctc5236_switch_read(0x41128, 1, &val);
	if (0 == (val & 0x2)){
		/* bypass core pll */
		val |= 0x2;
		ctc5236_switch_write(0x41128, 1, &val);

		/* reset qspi */
		writel(0x1, &sysctl_base->SysQspiResetCtl);
		udelay(1);
		writel(0x0, &sysctl_base->SysQspiResetCtl);
		/* reset emmc */
		writel(0x7f, &sysctl_base->SysMshResetCtl);

		/* reset sysctl */
		writel(0x1ef3, &sysctl_base->SysResetCtl);
	}
#endif
	/* 0: 1.2GHz, 1: 800MHz */
	freq_cpu = (readl(&sysctl_base->BootStrapPin) & BOOT_STRAP_PIN_W0_CPU_SPEED_LOG_MASK);

	/* Do some interface clock DIV modify when SoC core freq is equal to 800MHz */
	if (freq_cpu) {
		clk_peri_cfg0 = readl(&sysctl_base->SysClkPeriCfg[0]);
		clk_peri_cfg1 = readl(&sysctl_base->SysClkPeriCfg[1]);

		/* eMMC Reference clock 100MHz */
		clk_peri_cfg0 &= (~ SYS_CLK_PERI_CFG_W0_CFG_DIV_MSH_REF_CNT_MASK);
		clk_peri_cfg0 |= 0x8 << 0;
		/* SSP clock 100MHz */
		clk_peri_cfg0 &= (~ SYS_CLK_PERI_CFG_W0_CFG_DIV_SSP_CNT_MASK);
		clk_peri_cfg0 |= 0x8 << 8;
		/* USB Phy clock 50MHz */
		clk_peri_cfg0 &= (~ SYS_CLK_PERI_CFG_W0_CFG_DIV_USB_PHY_CNT_MASK);
		clk_peri_cfg0 |= 0x10 << 16;
		/* eMMC TM clock 50MHz */
		clk_peri_cfg1 &= (~ SYS_CLK_PERI_CFG_W1_CFG_DIV_MSH_TM_CNT_MASK);
		clk_peri_cfg1 |= 0x10 << 0;
		/* MDIO Reference clock 2.5MHz */
		clk_peri_cfg1 &= (~ SYS_CLK_PERI_CFG_W1_CFG_DIV_MDIO_SOC_CNT_MASK);
		clk_peri_cfg1 |= 0x28 << 16;

		writel(clk_peri_cfg0, &sysctl_base->SysClkPeriCfg[0]);
		writel(clk_peri_cfg1, &sysctl_base->SysClkPeriCfg[1]);		
	}

	board_early_init_f();
	timer_init();

	/* Uart reset*/
	writel(0xffffffff, &sysctl_base->SysUartResetCtl);
	udelay(1);
	writel(0x0, &sysctl_base->SysUartResetCtl);
       
#ifdef CONFIG_DEBUG_UART
	/* Uart debug for sure */
	debug_uart_init();
	//puts("Debug uart enabled\n"); /* or printch() */
#endif

	/* DDR interface reset config */
	writel(0xffffffff, &sysctl_base->SysDdrResetCtl);
	udelay(1);
	writel(0x0, &sysctl_base->SysDdrResetCtl);

	/* EMMC interface reset config */
	writel(0x7f, &sysctl_base->SysMshResetCtl);
	udelay(1);
	clrbits_le32(&sysctl_base->SysMshCfg, SYS_MSH_CFG_W0_CFG_RESET_BAR_MSH_SEL);
	setbits_le32(&sysctl_base->SysMshCfg, SYS_MSH_CFG_W0_CFG_MSH_CARD_DETECT_IN_EN);
	setbits_le32(&sysctl_base->SysMshCfg, SYS_MSH_CFG_W0_CFG_MSH_CARD_WRITE_PROT_IN_EN);
	udelay(1);
	clrbits_le32(&sysctl_base->SysMshResetCtl, SYS_MSH_RESET_CTL_W0_CFG_MSH_CFG_RESET);
	udelay(1000);
	clrbits_le32(&sysctl_base->SysMshResetCtl, SYS_MSH_RESET_CTL_W0_CFG_MSH_AXI_RESET | SYS_MSH_RESET_CTL_W0_CFG_MSH_TM_RESET);
	udelay(1000);
	clrbits_le32(&sysctl_base->SysMshResetCtl, SYS_MSH_RESET_CTL_W0_CFG_MSH_REF_C_RESET);
    
	/* USB interface reset config */
	writel(0x7f, &sysctl_base->SysUsbResetCtl);
	udelay(1);
	clrbits_le32(&sysctl_base->SysUsbResetCtl, 
        SYS_USB_RESET_CTL_W0_CFG_USB_PHY_PWR_ON_RESET);
	udelay(1);
	clrbits_le32(&sysctl_base->SysUsbResetCtl, 
        SYS_USB_RESET_CTL_W0_CFG_USB_PHY_RESET
        |SYS_USB_RESET_CTL_W0_CFG_USB_PHY_PORT_RESET 
        | SYS_USB_RESET_CTL_W0_CFG_USB_UTMI_RESET);
	udelay(1);
	clrbits_le32(&sysctl_base->SysUsbResetCtl, 
        SYS_USB_RESET_CTL_W0_CFG_USB_INTF_RESET
        |SYS_USB_RESET_CTL_W0_CFG_USB_AUX_RESET);
	udelay(1);
	clrbits_le32(&sysctl_base->SysUsbResetCtl, 
		SYS_USB_RESET_CTL_W0_CFG_USB_PHY_ATE_RESET);
	
	/* release watchdog interface */    
	clrbits_le32(&sysctl_base->SysWdtResetCtl, SYS_WDT_RESET_CTL_W0_CFG_WDT0_RESET);
	clrbits_le32(&sysctl_base->SysWdtResetCtl, SYS_WDT_RESET_CTL_W0_CFG_WDT1_RESET);

	/* release cpumac interface */
	clrbits_le32(&sysctl_base->SysMacResetCtl, SYS_MAC_RESET_CTL_W0_CFG_CPU_MAC_RESET);

	/* release mdio interface */
	clrbits_le32(&sysctl_base->SysMdioSocResetCtl, SYS_MDIO_SOC_RESET_CTL_W0_CFG_MDIO_SOC_RESET);

	/* release spi interface */
	clrbits_le32(&sysctl_base->SysSpiResetCtl, SYS_SPI_RESET_CTL_W0_CFG_SPI_RESET);

	/* release i2c interface and disable i2c0 slave interface */
	clrbits_le32(&sysctl_base->SysI2CResetCtl, SYS_I2_C_RESET_CTL_W0_CFG_I2_C0_RESET);
	clrbits_le32(&sysctl_base->SysI2CResetCtl, SYS_I2_C_RESET_CTL_W0_CFG_I2_C1_RESET);
	clrbits_le32(&sysctl_base->SysI2CMultiCtl, SYS_I2_C_MULTI_CTL_W0_CFG_I2_C_SLAVE_EN);

	/* release PCIE interface */
	clrbits_le32(&sysctl_base->SysPcieResetCtl, SYS_PCIE_RESET_CTL_W0_CFG_PCIE_RESET);
	clrbits_le32(&sysctl_base->SysPcieResetCtl, SYS_PCIE_RESET_CTL_W0_CFG_PIPE_RESET);
	clrbits_le32(&sysctl_base->SysPcieResetCtl, SYS_PCIE_RESET_CTL_W0_CFG_PHY_REG_RESET);

	/* disable all reset modules by WDT */
	writel(0, &sysctl_base->SysWdt0ResetEnCtl);
	writel(0x1f4, &sysctl_base->SysWdt0Cnt);

	writel(0, &sysctl_base->SysWdt1ResetEnCtl);
	writel(0x1f4, &sysctl_base->SysWdt1Cnt);

	/* release timer interface */
	writel(0x0, &sysctl_base->SysTimerResetCtl);

}
#endif

#ifdef CONFIG_SPL_BOARD_INIT
void spl_board_init(void)
{
	int count = 10;
	struct spi_flash *flash;
	int verify;
#ifndef CONFIG_DIRECT_QSPI
	/* QSPI: speed up qspi boot clock div from 40 to 20  */
	writel(0x001411f1, &sysctl_base->SysQspiBootCfg0);
#endif
	preloader_console_init();

	dram_init();
	
	/* wait for getting input char*/
	while(count){
		udelay(10000);
		count--;
		if (tstc()) {
		/* 2 = ctrl +b in ASSIC, if get ctrl+b , erase top 100k of the flash*/
                	if(getc()==2){
				printf("\nDo you want to erase flash to destory spl image?(y or n)\n");
				while(1){
					verify = getc();
        				if(verify =='y'){
        					flash = ctc_spi_flash_probe(1, 0, 25000000, 3);
						ctc_spi_flash_cmd_erase_ops(flash, 0, CTC_REMOVE_FLASH_SIZE);
						printf("\nPlease turn off the power and boot from the uart!\n\n");
						break;
        				}
        				if(verify== 'n'){
        					printf("\nContinue booting uboot!\n\n");
        					break;
        				}
				}
                	}
		}
	}
	
	return;
}
#endif

