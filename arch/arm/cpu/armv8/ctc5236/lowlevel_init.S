/*
 * (C) Copyright 2004-2017 Centec Networks (suzhou) Co., LTD.
 * Jay Cao <caoj@centecnetworks.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <asm-offsets.h>
#include <config.h>
#include <linux/linkage.h>
#include <asm/macro.h>
#include <asm/arch/cpu.h>

ENTRY(lowlevel_init)
	mov	x29, lr			/* Save LR */
	/*
	 * disable qspi secure feature
	 */
	ldr     x0, =0x3100000c
	mov     w1, #0x1
	str     w1, [x0]
	
	bl	ctc5236_sram_init

#if defined(CONFIG_GICV2) || defined(CONFIG_GICV3)
	branch_if_slave x0, 1f
	ldr	x0, =GICD_BASE
	bl	gic_init_secure
1:
#if defined(CONFIG_GICV3)
	ldr	x0, =GICR_BASE
	bl	gic_init_secure_percpu
#elif defined(CONFIG_GICV2)
	ldr	x0, =GICD_BASE
	ldr	x1, =GICC_BASE
	bl	gic_init_secure_percpu
#endif
#endif

#ifdef CONFIG_ARMV8_MULTIENTRY
	branch_if_master x0, x1, 2f

	/*
	 * Slave should wait for master clearing spin table.
	 * This sync prevent salves observing incorrect
	 * value of spin table and jumping to wrong place.
	 */
#if defined(CONFIG_GICV2) || defined(CONFIG_GICV3)
#ifdef CONFIG_GICV2
	ldr	x0, =GICC_BASE
#endif
	/* bl	gic_wait_for_interrupt */
#endif

	/*
	 * All slaves will enter EL2 and optionally EL1.
	 */
	adr	x4, lowlevel_in_el2
	ldr	x5, =ES_TO_AARCH64
	bl	armv8_switch_to_el2

lowlevel_in_el2:
#ifdef CONFIG_ARMV8_SWITCH_TO_EL1
	adr	x4, lowlevel_in_el1
	ldr	x5, =ES_TO_AARCH64
	bl	armv8_switch_to_el1

lowlevel_in_el1:
#endif

#endif /* CONFIG_ARMV8_MULTIENTRY */

2:

	mov	lr, x29			/* Restore LR */
	ret
ENDPROC(lowlevel_init)

ENTRY(ctc5236_sram_init)
	mov	x28, lr			/* Save LR */
	bl	ctc5236_clear_sram
	mov	lr, x28			/* Restore LR */
	ret
ENDPROC(ctc5236_sram_init)

/* TODO: Jay Cao */
ENTRY(ctc5236_clear_sram)
	ldr	x0, =0x1010fff0 /* =CONFIG_SYS_CTC5236_CPUSRAM_BASE */
	ldr	x1, =0x10110000 /* =(CONFIG_SYS_CTC5236_CPUSRAM_BASE + CONFIG_SYS_CTC5236_CPUSRAM_SIZE) */
	mov	x2, #0
clear_loop:
	str	x2, [x0]
	add	x0, x0, #8
	cmp	x0, x1
	b.lo	clear_loop
	ret
ENDPROC(ctc5236_clear_sram)

