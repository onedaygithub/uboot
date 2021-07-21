/*
 * (C) Copyright 2004-2017 Centec Networks (suzhou) Co., LTD.
 * Jay Cao <caoj@centecnetworks.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <asm/armv8/mmu.h>
#include <asm/io.h>
#include <ctc5236_ddr.h>

#include <asm/arch/cpu.h>
#include <asm/arch/sysctl.h>

DECLARE_GLOBAL_DATA_PTR;

int arch_cpu_init(void)
{
	/* TODO: Jay Cao */
	return 0;
}

#if defined(CONFIG_ARCH_EARLY_INIT_R)
int arch_early_init_r(void)
{
	/* TODO: Jay Cao */
	return 0;
}
#endif /* CONFIG_ARCH_EARLY_INIT_R */

static struct mm_region ctc5236_mem_map[] = {
{
		.virt = CONFIG_SYS_CTC5236_CPUROM_BASE,
		.phys = CONFIG_SYS_CTC5236_CPUROM_BASE,
		.size = 0x200000,
		.attrs = PTE_BLOCK_MEMTYPE(MT_DEVICE_NGNRNE) |
			PTE_BLOCK_NON_SHARE
},
{
		.virt = CONFIG_SYS_CTC5236_SPI0_BASE,
		.phys = CONFIG_SYS_CTC5236_SPI0_BASE,
		.size = CONFIG_SYS_CTC5236_SPI0_SIZE,
		.attrs = PTE_BLOCK_MEMTYPE(MT_DEVICE_NGNRNE) |
		PTE_BLOCK_NON_SHARE |
		PTE_BLOCK_PXN | PTE_BLOCK_UXN
},
{
		.virt = CONFIG_SYS_CTC5236_PCIECFG_BASE,
		.phys = CONFIG_SYS_CTC5236_PCIECFG_BASE,
		.size = 0x20000000,
		.attrs = PTE_BLOCK_MEMTYPE(MT_DEVICE_NGNRNE) |
		PTE_BLOCK_NON_SHARE |
		PTE_BLOCK_PXN | PTE_BLOCK_UXN
},
{
		.virt = CONFIG_SYS_CTC5236_PCIEMEM_BASE,
		.phys = CONFIG_SYS_CTC5236_PCIEMEM_BASE,
		.size = CONFIG_SYS_CTC5236_PCIEMEM_SIZE,
		.attrs = PTE_BLOCK_MEMTYPE(MT_DEVICE_NGNRNE) |
		PTE_BLOCK_NON_SHARE |
		PTE_BLOCK_PXN | PTE_BLOCK_UXN
},
{
		.virt = CONFIG_SYS_CTC5236_DRAM0_BASE,
		.phys = CONFIG_SYS_CTC5236_DRAM0_BASE,
		.size = CONFIG_SYS_CTC5236_DRAM0_SIZE,
		.attrs = PTE_BLOCK_MEMTYPE(MT_NORMAL) |
		PTE_BLOCK_OUTER_SHARE
},
{
		.virt = CONFIG_SYS_CTC5236_DRAM1_BASE,
		.phys = CONFIG_SYS_CTC5236_DRAM1_BASE,
		.size = CONFIG_SYS_CTC5236_DRAM1_SIZE,
		.attrs = PTE_BLOCK_MEMTYPE(MT_NORMAL) |
		PTE_BLOCK_OUTER_SHARE
},
{
		/* List terminator */
		0,
	}
};
struct mm_region *mem_map = ctc5236_mem_map;

u64 get_page_table_size(void)
{
	/* TODO: Jay Cao */
	return 0x10000;
}

#ifndef CONFIG_SYSRESET
void reset_cpu(ulong addr)
{
	u32 load;
	
	#define SP805_REG_WDOGLOAD         0x000
	#define SP805_REG_WDOGVAL          0x004
	#define SP805_REG_WDOGCTRL         0x008
	
	writel(0x0, &sysctl_base->SysWdt0ResetEnCtl);
#ifdef CONFIG_TARGET_EMU_CTC5236
	/* QSPI: speed up qspi boot clock div from 40 to 10 */
	writel(0x000a11f1, &sysctl_base->SysQspiBootCfg0);

	setbits_le32(&sysctl_base->SysWdt0ResetEnCtl, SYS_WDT0_RESET_EN_CTL_W0_WDT0_RST_EN_WDT0);
	setbits_le32(&sysctl_base->SysWdt0ResetEnCtl, SYS_WDT0_RESET_EN_CTL_W0_WDT0_RST_EN_WDT1);
	setbits_le32(&sysctl_base->SysWdt0ResetEnCtl, SYS_WDT0_RESET_EN_CTL_W0_WDT0_RST_EN_CPU0_CORE);
	setbits_le32(&sysctl_base->SysWdt0ResetEnCtl, SYS_WDT0_RESET_EN_CTL_W0_WDT0_RST_EN_CPU1_CORE);
	
	setbits_le32(&sysctl_base->SysWdt1ResetEnCtl, SYS_WDT1_RESET_EN_CTL_W0_WDT1_RST_EN_WDT0);
	setbits_le32(&sysctl_base->SysWdt1ResetEnCtl, SYS_WDT1_RESET_EN_CTL_W0_WDT1_RST_EN_WDT1);
	setbits_le32(&sysctl_base->SysWdt1ResetEnCtl, SYS_WDT1_RESET_EN_CTL_W0_WDT1_RST_EN_CPU0_CORE);
	setbits_le32(&sysctl_base->SysWdt1ResetEnCtl, SYS_WDT1_RESET_EN_CTL_W0_WDT1_RST_EN_CPU1_CORE);
	writel(1, &sysctl_base->SysWdt0Cnt);
	writel(1, &sysctl_base->SysWdt1Cnt);
#endif

	/* disable watchdog */
	writel(0, CONFIG_WDT_BASE+SP805_REG_WDOGCTRL);

	/* calculate the timeout value, and set */
	load = (CONFIG_WDT_CLOCK_KHZ*CONFIG_HW_WATCHDOG_TIMEOUT_MS)/2 - 1;
	writel(load, (CONFIG_WDT_BASE + SP805_REG_WDOGLOAD));

	/* enable watchdog */
	writel(3, CONFIG_WDT_BASE+SP805_REG_WDOGCTRL); 

	mdelay(2000);

}
#endif

#ifdef CONFIG_DISPLAY_CPUINFO
int print_cpuinfo(void)
{
	unsigned int cpu_freq_status;
	char buf[32];
	unsigned long freq_cpu,freq_ddr;
	unsigned int ddr_freq_status0,ddr_freq_status1;

	/* get cpu frequence */
	cpu_freq_status = readl(&sysctl_base->BootStrapPin);
	if(cpu_freq_status & BOOT_STRAP_PIN_W0_CPU_SPEED_LOG_MASK)
		freq_cpu = 800000000;
	else
		freq_cpu = 1200000000;

	/* get DDR frequence */
	ddr_freq_status0 = readl(&sysctl_base->SysPllDdrCfg0);
	ddr_freq_status1 = readl(&sysctl_base->SysPllDdrCfg1);
	
	if((ddr_freq_status0 == DDRINTF_MC_CLOCK_CONFIG0_200)
		&(ddr_freq_status1 == DDRINTF_MC_CLOCK_CONFIG1_2))
		freq_ddr = 800000000;

	if((ddr_freq_status0 == DDRINTF_MC_CLOCK_CONFIG0_266)
		&(ddr_freq_status1 == DDRINTF_MC_CLOCK_CONFIG1_1))
		freq_ddr = 1066000000;

	if((ddr_freq_status0 == DDRINTF_MC_CLOCK_CONFIG0_333)
		&(ddr_freq_status1 == DDRINTF_MC_CLOCK_CONFIG1_1))
		freq_ddr = 1333000000;

	if((ddr_freq_status0 == DDRINTF_MC_CLOCK_CONFIG0_400)
		&(ddr_freq_status1 == DDRINTF_MC_CLOCK_CONFIG1_0))
		freq_ddr = 1600000000;

	if((ddr_freq_status0 == DDRINTF_MC_CLOCK_CONFIG0_466)
		&(ddr_freq_status1 == DDRINTF_MC_CLOCK_CONFIG1_2))
		freq_ddr = 1866000000;
	
	if((ddr_freq_status0 == DDRINTF_MC_CLOCK_CONFIG0_533)
		&(ddr_freq_status1 == DDRINTF_MC_CLOCK_CONFIG1_1))
		freq_ddr = 2133000000;

	if((ddr_freq_status0 == DDRINTF_MC_CLOCK_CONFIG0_600)
		&(ddr_freq_status1 == DDRINTF_MC_CLOCK_CONFIG1_0))
		freq_ddr = 2400000000;
		
	
	puts("SoC:   Centec CTC5236 (TsingMa)\n");
	
	puts("Clock Configuration:");
	puts("\n       ");
	
	printf("CPU0(A53):%-4s MHz  ",strmhz(buf, freq_cpu));
	printf("CPU1(A53):%-4s MHz",strmhz(buf, freq_cpu));
	/* TsingMa  clockSup is 500MHz */
	printf("\n       Bus:      %-4s MHz  ",strmhz(buf, 500000000));

	printf("DDR:      %-4s Mbps", strmhz(buf, freq_ddr));
	puts("\n");
	return 0;
}
#endif

int timer_init(void)
{
	return 0;
}

int dram_init(void)
{
	ctc_init_dram();
	return 0;
}

int dram_init_banksize(void)
{
	gd->bd->bi_dram[0].start = CONFIG_SYS_SDRAM_BASE;
	gd->bd->bi_dram[0].size = CONFIG_SYS_SDRAM_SIZE;
	return 0;
}
