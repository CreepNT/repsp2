
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Monitor mode support code
@

@
@ Imports from C code
@
	.global	monitor_call_handler
	.global	monitor_prefetch_abort_handler
	.global	monitor_data_abort_handler
	.global	monitor_irq_handler
	.global	monitor_fiq_handler

//TODO: get these from a shared header
#define SCR_NS		(1 << 0)
#define MODE_MON	0x16
#define MODE_SVC	0x13
#define CPSR_I		(1 << 7)	/* Mask IRQ interrupts */

#define	MON_STACK_BASE		0x40083900
#define MON_STACK_SIZE		0x400

	.cpu cortex-a9
	.fpu neon-fp16
	.syntax unified

	.arm
	.text
	.section	".text.monitor"

	.global	MONITOR_VECTOR_TABLE
MONITOR_VECTOR_TABLE: @ Address: 0x40020a34
	.word	0x0	@ <Not used>
	.word	0x0	@ <Not used>
	ldr	pc, 1f	@ Secure Monitor Call
	ldr	pc, 2f	@ Prefetch Abort
	ldr	pc, 3f	@ Data Abort
	.word	0x0	@ <Not used>
	ldr	pc, 4f	@ IRQ interrupt
	ldr	pc, 5f	@ FIQ interrupt

1:
	.word .Lmon_smc_handler
2:
	.word .Lmon_prefetch_abort_handler
3:
	.word .Lmon_data_abort_handler
4:
	.word .Lmon_irq_handler
5:
	.word .Lmon_fiq_handler

	.global	fun_40020a68
fun_40020a68:	@ Address: 0x40020a68
	@ Read CPU ID
	mrc	p15, #0, r3, c0, c0, #5		@ MPIDR
	and	r3, r3, #0xF

	@ Compute stack base for this CPU
	mov	r0, #STACK_SIZE
	mul	r0, r3, r0

	@ Initialize sp_mon, lr_mon and spsr_mon
	cpsid	if, #MODE_MON
	ldr	sp, =MON_STACK_BASE	@ constant pool at 0x40020B1C
	sub	sp, sp, r0
	mov	r1, #0
	cpy	lr, r1
	msr	spsr_cxsf, r1

	@ Go back into System mode and return
	cpsid	if, #MODE_SYS
	bx	lr

.Lmon_smc_handler: @ Address: 0x40020a98
	stmdb	sp!, {lr}
	cpy	r0, r12
	blx	monitor_call_handler
	clrex
	ldmia	sp!, {pc}^

.Lmon_prefetch_abort_handler: @ Address: 0x40020aac
	sub	lr, lr, #4
	stmdb	sp!, {r0-r3, r12, lr}
	blx	monitor_prefetch_abort_handler
	clrex
	ldmia	sp!, {r0-r3, r12, pc}^

.Lmon_data_abort_handler: @ Address: 0x40020ac0
	sub	lr, lr, #8
	stmdb	sp!, {r0-r3, r12, lr}
	blx	monitor_data_abort_handler
	clrex
	ldmia	sp!, {r0-r3, r12, pc}^

.Lmon_irq_handler: @ Address: 0x40020ad4
	sub	lr, lr, #4
	stmdb	sp!, {r0-r3, r12, lr}
	blx	monitor_irq_handler
	clrex
	ldmia	sp!, {r0-r3, r12, pc}^

.Lmon_fiq_handler: @ Address: 0x40020ae8
	sub	lr, lr, #4
	stmdb	sp!, {r0-r3, r12, lr}
	blx	monitor_fiq_handler
	clrex
	ldmia	sp!, {r0-r3, r12, pc}^

	.global fun_40020afc
fun_40020afc: @ Address: 0x40020afc
	@ r0 = target address

	@ Switch to Monitor mode which is always
	@ in Secure state regardless of SCR.NS bit
	cpsid	if, #MODE_MON

	@ Prepare SPSR for jump to Non-Secure state:
	@ => Supervisor mode with interrupts disabled
	movw	r1, #(CPSR_I | MODE_SVC)

	@ Prepare RFE stack frame: SPSR + PC
	str	r1, [sp, #-4]!
	str	r0, [sp, #-4]!

	@ Set SCR.NS=1 to enter Non-Secure state
	@ when we leave Monitor mode
	mrc	p15, #0, r1, c1, c1, #0
	orr	r1, r1, #SCR_NS
	mcr	p15, #0, r1, c1, c1, #0

	@ Enter Non-Secure state
	rfeia	sp!

@ Address: 0x44020B1C
@ = constant pool for fun_40020a68
