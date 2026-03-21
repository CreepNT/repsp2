
#define KBL_PARAM_ADDRESS	0x00000100
#define __offsetof_SceKblParam_BootTypeIndicator1	0x6C
#define __offsetof_SceKblParam_wakeupFactor		0xC4

#define PERIPHBASE		0x1A000000


#define SCU_BASE		(PERIPHBASE + 0x0000) //!< Snoop Control Unit
#define SCU_OFF_CTRL		0x00
#define SCU_OFF_INVALL		0x0C
#define SCU_OFF_DIAGCTRL	0x30
#define SCU_OFF_SAC		0x50
#define SCU_OFF_SNSAC		0x54

#define SCU_CTRL_ENABLE		(1 << 0)
#define SCU_CTRL_ADDR_FLTR_EN	(1 << 1)
#define SCU_CTRL_STDBY_EN	(1 << 5)
#define SCU_CTRL_IC_STDBY_EN	(1 << 6)

#define GICC_BASE	(PERIPHBASE + 0x0100) //!< GIC per-CPU Interface



#define GT_BASE		(PERIPHBASE + 0x0200) //!< Global Timer



/* 0x300~0x5FF are reserved/unused */



#define PTWDG_BASE	(PERIPHBASE + 0x0600) //!< Private Timers & Watchdogs



#define GICD_BASE	(PERIPHBASE + 0x1000) //!< GIC Distributor



#define L2C_BASE	(PERIPHBASE + 0x2000) //!< L2 Cache Controller (PL310)
#define L2C_OFF_REG1_CTRL	0x100

///// ARM CPU SPECIFIC DEFINITIONS
#define SCTLR_TE		(1 << 30)	/* Exceptions taken in Thumb state */
#define SCTLR_AFE		(1 << 29)	/* Access flag enable */
#define SCTLR_TRE		(1 << 28)	/* TEX remap enable */
#define	SCTLR_EE		(1 << 25)	/* Exception Endianness (0=LE,1=BE) */
#define SCTLR_RR		(1 << 14)	/* Round Robin cache replacement */
#define SCTLR_V			(1 << 13)	/* Vectors remap bit (1=Hivecs) */
#define SCTLR_I			(1 << 12)	/* Instruction cache enable */
#define SCTLR_Z			(1 << 11)	/* Branch prediction enable */
#define SCTLR_SW		(1 << 10)	/* SWP/SWPB enable */
#define SCTLR_C			(1 << 2)	/* Data & unified cache enable */
#define SCTLR_A			(1 << 1)	/* Alignment fault checking */
#define SCTLR_M			(1 << 0)	/* MMU enable */



#define SCR_FIQ			(1 << 2) /* take FIQ in Monitor mode */



#define CPACR_CP10_FULL		(0x3 << 20)
#define CPACR_CP11_FULL		(0x3 << 22)



#define NSACR_CP10		(1 << 10)
#define NSACR_CP11		(1 << 11)

/**
 * Enable NS access to the Preload Engine.
 * This bit is IMPLEMENTATION DEFINED at
 * arch level and thus Cortex-A9 specific.
 */
#define NSACR_PLE		(1 << 16)


#define PLEUAR_U		(1 << 0)



// CPU special registers
#define CPSR_A			(1 << 8) /* Asynchronous abort mask */



// FPU special registers
#define FPEXC_EN		(1 << 30)



/*
 * Note: Kermit does not implement the
 * Virtualization Extensions so these
 * definitions do not include Hyp mode.
 */
#define MODE_USR	0x10	/*!< 0b10000 = d16 */
#define MODE_FIQ	0x11	/*!< 0b10001 = d17 */
#define MODE_IRQ	0x12	/*!< 0b10010 = d18 */
#define MODE_SVC	0x13	/*!< 0b10011 = d19 */
#define MODE_MON	0x16	/*!< 0b10110 = d22 */
#define MODE_ABT	0x17	/*!< 0b10111 = d23 */
#define MODE_UND	0x1B	/*!< 0b11011 = d27 */
#define MODE_SYS	0x1F	/*!< 0b11111 = d31 */
///// ENDOF ARM CPU SPECIFIC DEFINITIONS

/* for each CPU */
#define STACK_SIZE		0x400

#define	MON_STACK_BASE		0x40083900
#define IRQ_STACK_BASE		0x40082900
#define SVC_STACK_BASE		0x40081900
#define UND_STACK_BASE		0x40080900
#define ABT_STACK_BASE		0x40080900
#define SYS_STACK_BASE		0x4007f900

@@@@
@ ALL RE DONE HERE IS FOR kernel_boot_loader START
@ a.k.a. SKBL entrypoint
@
@ TODO: reverse NSKBL
@@@@

	.cpu cortex-a9
	.fpu neon-fp16
	.syntax unified

@ Imported functions
	.global	memcpy

@ Imported data
	.global	dat_4007a800


@@@@@@@@@@@@@@@@@@@@@@@@@
@ Start of KBL segment 1 head
@@@@@@@@@@@@@@@@@@@@@@@@@

	.arm
	.text
	.section	".text.head"
	.global	_start @ Address: 0x40020000
_start:
	@
	@ Kernel Boot Loader entrypoint
	@
	@
	@ Execution begins here in Supervisor mode.

	@ Disable interrupts
	cpsid if

	@ Clear exclusive monitor reservations
	clrex

	@ Disable many features by clearing SCTLR.
	@ This sequence is a bit silly since it clears
	@ bits from a value already initialized to 0...
	@ Maybe the side effect of using a macro?
	mov	r0, #0
	bic 	r0, #SCTLR_TE
	bic	r0, #SCTLR_TRE
	bic	r0, #SCTLR_AFE
	bic	r0, #SCTLR_EE
	bic	r0, #SCTLR_RR
	bic	r0, #SCTLR_V
	bic	r0, #SCTLR_I
	bic	r0, #SCTLR_Z
	bic	r0, #SCTLR_SW
	bic	r0, #SCTLR_A
	bic	r0, #SCTLR_C
	bic	r0, #SCTLR_M
	mcr	p15, #0, r0, c1, c0, #0

	@  - ICIALLUIS: invalidate I$ PoU IS
	@  - BPIALLIS:  invalidate branch predictor
	@  - ICIALLU:   invalidate I$ PoU
	mov	r0,#0x0
	mcr	p15, #0, r0, c7, c1, #0
	dsb	sy
	mcr	p15, #0, r0, c7, c1, #6
	isb	sy
	mcr	p15, #0, r0, c7, c5, #0
	dsb	sy

	@ Invalidate data cache by set/way
	@ (note how r0 is already 0x0 from above)
.Ldcache_inv_next_xxx:
	mov	r1, #0
.Ldcache_inv_next_yyy:
	orr	r2, r1, r0
	mcr	p15, #0, r2, c7, c6, #2
	add	r1, r1, #0x40000000
	cmp	r1, #0
	bne 	.Ldcache_inv_next_yyy
	add 	r0, r0, #0x20
	cmp	r0, 0x2000
	bne 	.Ldcache_inv_next_xxx

	dsb	sy

	@ Invalidate branch predictor (IBPIALL)
	mov	r0, #0
	mcr	p15, #0, r0, c7, c5, #6
	isb	sy

	@ Invalidate TLB using TLBIALLIS
	@ (invalidate entire unified TLB - inner shareable)
	mcr	p15, #0, r0, c8, c3, #0

	@ Disable L2 cache (PL310)
	mov	r0, #0
	movw	r1, #:lower16:L2C_BASE
	movt	r1, #:upper16:L2C_BASE
	str	r0, [r1, #L2C_OFF_REG1_CTRL]

	@  ICIALLU: invalidate I$ PoU
	mov	r0, #0
	mcr	p15, #0, r0, c7, c5, #0

	@ TLBIALL: invalidate entire unified TLB
	mov	r0, #0
	mcr	p15, #0, r0, c8, c7, #0

	@ TTBCR=0
	mcr	p15, #0, r0, c2, c0, #2

	@ Invalidate branch predictor (IBPIALL)
	mov	r0, #0
	mcr	p15, #0, r0, c7, c5, #6
	isb	sy

	@ Configure ACTLR (c.f. Cortex-A9 TRM)
	mov	r0, #0
	orr	r0, r0, #0x00000040	@ ACTLR_SMP - take part in coherency
	orr	r0, r0, #0x00000001	@ ACTLR_FW - enable cache/TLB maintenance broadcast
	orr	r0, r0, #0x00000100	@ ACTLR_Alloc1Way - enable allocation in one cache way only
	orr	r0, r0, #0x00000008	@ ACTLR_FullLineOfZeros - enable full line of zeros mode
	mcr	p15, #0, r0, c1, c0, #1

	@
	@ Apply workarounds for Cortex-A9 errata
	@
	@ The following revisions of the Cortex-A9
	@ are explicitly documented as existing:
	@	- r1p0~r1p3
	@	- r2p0~r2p10
	@	- r3p0
	@	- r4p0
	@
	mrc	p15, #0, r0, c15, c0, #1 @ Diagnostic Control register (undocumented)
	mrc	p15, #0, r1, c0, c0, #0  @ MIDR: Main ID Register

	ands	r2, r1, #0x0000000F	@ MIDR.Revision
	ands	r1, r1, #0x00F00000	@ MIDR.Variant
	cmp	r1, #0x00200000		@ Compare Cortex-A9 major version to r2pX
	bgt	.Llab_40020128		@ >2: r3p0 or later
	blt	.Llab_4002010c		@ <2: r0pX or r1pX
	cmp	r2, #0x4		@ =2: check minor version
	bge	.Llab_40020108		@ skip next workaround if r2p4 or later

	@ Workaround for erratum 742231:
	@ "Incorrect hazard handling in the SCU might cause data corruption"
	@ => Set bits [12] and [22] "Disable Outstanding Accesses" and bit [22]
	@    "Disable Write Allocate Wait Mode" of the Diagnostic Control register.
	@
	@ This erratum affects r2p0/r2p1/r2p2 and is fixed in r2p5/r2p6.
	@
	@ Note: ARM's errata sheet indicates bit [22] should be set only for
	@ Cortex-A9 MPCore systems with one processor and the ACP, but Kermit
	@ has four processors... maybe older Errata Sheets did not indicate
	@ that bit [22] was unneeded for multi-core systems?
	orrs	r0, r0, #0x00400000	@ Bit [22]: "Disable Write Allocate Wait Mode"
	orrs	r0, r0, #0x00001000	@ Bit [12]: "Disable Outstanding Accesses"

.Llab_40020108:
	@ Workaround for erratum 743622:
	@ "Faulty logic in the Store Buffer might cause data corruption"
	@ => Set bit [6] of the Diagnostic Control register which disables
	@    the 'fast lookup' optimization in the Store Buffer
	@
	@ This erratum affects all r2 revisions and is fixed in r3p0.
	orrs	r0, r0, #0x00000040

.Llab_4002010c:
	@ Workaround for erratum 742230: "DMB operation might be faulty"
	@ => Set bit [4] of the Diagnostic Control register which causes
	@    the DMB instruction to be decoded and executed as a DSB
	@
	@ This erratum affects r1p0/r1p1/r1p2/r1p3/r2p0/r2p1/r2p2 and
	@ is fixed in all other revisions.
	orrs	r0, r0, #0x00000010

	cmp	r2, #0x8		@ check minor version again
	bge	.Llab_4002011c		@ skip next workaround if rXp8

	@ Workaround for erratum 743623: "Bad interaction between a minimum
	@ of seven PLDs and one Non-Cacheable LDM, can lead to deadlock"
	@ => Set bit [20] of the Diagnostic Control register which causes
	@    the PLD instruction to be decoded and executed as a NOP
	@
	@ This erratum affects all r0/r1/r2/r3 revisions and is fixed in r4p0.
	@
	@ Workaround for erratum 771221: "PLD instructions might allocate
	@ data in the Data Cache regardless of the Cache Enable bit value"
	@ => Set bit [20] of the Diagnostic Control register which causes
	@    the PLD instruction to be decoded and executed as a NOP
	@ (note that the workaround is identical as for the previous erratum)
	@
	@ This erratum affects all r0/r1/r2 revisions and is fixed in r3p0.
	orrs	r0, r0,	#0x00100000	@ TODO: what this is for

.Llab_4002011c:
	@ Workaround for erratum 751472: "An interrupted ICIALLUIS operation
	@ might prevent the completion of a following broadcast operation"
	@ => Set bit [11] of the Diagnostic Control register which prevents
	@    the interruption of CP15 maintenance operations
	@
	@ This erratum affects all r0/r1/r2 revisions and is fixed in r3p0.
	orrs	r0, r0, #0x00000800

	@ Workaround for erratum 761320: "Full cache line writes to the same
	@ memory region from at least two processors might deadlock the processor"
	@ => Set bit [21] of the Diagnostic Control register which disables
	@    the "direct eviction" optimization in the Bus Interface Unit
	@
	@ This erratum affects all r0/r1/r2/r3 revisions and is fixed in r4p0.
	orrs	r0, r0, #0x00200000

	@ Apply all workarounds by writing
	@ to the Diagnostic Control Register
	mcr	p15, #0, r0, c15, c0, #1

.Llab_40020128:
	@ Disable alignment fault checking
	mrc	p15, #0, r1, c1, c0, #0
	bic	r1, r1, #SCTLR_A
	mcr	p15, #0, r1, c1, c0, #0

	@ Read CPU ID in r3
	mrc	p15, #0, r3, c0, c0, #5
	and	r3, r3, #0xF

	@ Compute stack offset for current CPU
	mov	r0, #STACK_SIZE
	mul	r0, r3, r0

	@ Initialize a register shared
	@ by all processor modes to zero
	mov	r1, #0

@ For the following sequence:
@	r0 = stack offset for this CPU
@	r1 = zero
@	r3 = CPU ID
@
@ In each mode, initialize sp to a decent value
@ and set lr + spsr to zero.

	@ Initialize sp_svc, lr_svc and spsr_svc
	@ while we are still in Supervisor mode
	movw	sp, #:lower16:SVC_STACK_BASE
	movt	sp, #:upper16:SVC_STACK_BASE
	sub	sp, sp, r0
	cpy	lr, r1
	msr	spsr_cxsf, r1

	@ Initialize sp_und, lr_und and spsr_und
	cpsid	if, #MODE_UND
	movw	sp, #:lower16:UND_STACK_BASE
	movt	sp, #:upper16:UND_STACK_BASE
	sub	sp, sp, r0
	cpy	lr, r1
	msr	spsr_cxsf, r1

	@ Initialize sp_abt, lr_abt and spsr_abt
	cpsid	if, #MODE_ABT
	movw	sp, #:lower16:ABT_STACK_BASE
	movt	sp, #:upper16:ABT_STACK_BASE
	sub	sp, sp, r0
	cpy	lr, r1
	msr	spsr_cxsf, r1

	@ Initialize sp_irq, lr_irq and spsr_irq
	cpsid	if, #MODE_IRQ
	movw	sp, #:lower16:IRQ_STACK_BASE
	movt	sp, #:upper16:IRQ_STACK_BASE
	sub	sp, sp, r0
	cpy	lr, r1
	msr	spsr_cxsf, r1

	@ Poison FIQ-specific r8_fiq~r12_fiq then
	@ initialize sp_fiq, lr_fiq and spsr_fiq
	@ (note how we initialize sp_fiq=0 ?!)
	cpsid	if, #MODE_FIQ
	movw	r2, #0xBEEF
	movt	r2, #0xDEAD
	cpy	r8, r2
	cpy	r9, r2
	cpy	r10, r2
	cpy	r11, r2
	cpy	r12, r2
	cpy	sp, r1
	cpy	lr, r1
	msr	spsr_cxsf, r1

	@ Initialize sp_sys, lr_sys and spsr_sys...
	@ ...except there is no SPSR in System mode!
	@
	@ Note that System mode doesn't actually have
	@ dedicated sp/lr registers, so we really are
	@ initializing sp_usr and lr_usr here.
	cpsid	if, #MODE_SYS
	movw	sp, #:lower16:SYS_STACK_BASE
	movt	sp, #:upper16:SYS_STACK_BASE
	sub	sp, sp, r0, lsl #2		@ Use 4x larger stack (4 KiB) for System mode
	cpy	lr, r1				@ because that's the mode in which KBL executes
	msr	spsr_cxsf, r1	@ BUG!!!

	@ Initialize monitor mode
	bl	fun_40020a68

	@ Configure FIQ exceptions to be
	@ taken to Secure Monitor mode
	mrc	p15, #0, r0, c1, c1, #0
	orr	r0, r0, #SCR_FIQ
	mcr	p15, #0, r0, c1, c1, #0

	@ Enable dynamic clock gating
	@ => set bit [0] in the Cortex-A9
	@    CP15 Power Control register
	mrc	p15, #0, r0, c15, c0, #0
	orr	r0, r0, #1
	mcr	p15, #0, r0, c15, c0, #0

	@ Enable Secure State access to NEON/FPU
	mov	r0, #(CPACR_CP10_FULL | CPACR_CP11_FULL)
	mcr	p15, #0, r0, c1, c0, #2		@ CPACR

	@ Allow Non-Secure State access to NEON/FPU and PLE
	mov	r0, #(NSACR_PLE | NSACR_CP11 | NSACR_CP10)
	mcr	p15, #0, r0, c1, c1, #2		@ NSACR
	isb	sy

	@ Enable Advanced SIMD and Floating-point Extensions
	mov	r0, #FPEXC_EN
	vmsr	fpexc, r0
	isb	sy

	@ Clear NEON/FPU registers
	mov	r0, #0
	vmsr	fpscr, r0
	vdup.32	q0, r0
	vmov	q1, q0
	vmov	q2, q0
	vmov	q3, q0
	vmov	q4, q0
	vmov	q5, q0
	vmov	q6, q0
	vmov	q7, q0
	vmov	q8, q0
	vmov	q9, q0
	vmov	q10, q0
	vmov	q11, q0
	vmov	q12, q0
	vmov	q13, q0
	vmov	q14, q0
	vmov	q15, q0

	@ Check if NS PLE access is allowed
	@ and enable User mode access to PLE
	@ in Secure & Non-Secure states if so
	@ (note: we allowed NS PLE access earlier
	@  so User access will always be enabled)
	mrc	p15, #0, r0, c1, c1, #2		@ NSACR
	tst	r0, #NSACR_PLE
	mov	r0, #PLEUAR_U
	mcrne	p15, #0, r0, c11, c1, #0	@ PLEUAR

	@ The code that follows executes only on CPU 0.
	@ Other CPUs wait until CPU 0 is finished then
	@ jump over this code (they don't run it).
	cmp	r3, #0				@ r3 holds CPU ID
	wfene
	bne	.Llab_4002038c

	@ Disable the SCU and address filtering
	mov	r1, #SCU_BASE
	mov	r0, #0
	bic	r0, r0, #(SCU_CTRL_ENABLE | SCU_CTRL_ADDR_FLTR_EN)
	str	r0, [r1, #SCU_OFF_CTRL]

	@ Invalidate all SCU lines for all CPUs
	mvn	r0, #0				@ r0 = 0xFFFFFFFF
	str	r0, [r1, #SCU_OFF_INVALL]

	@ Enable access to the SCU for all CPUs
	ldr	r0, [r1, #SCU_OFF_SAC]
	orr	r0, #0xF
	str	r0, [r1, #SCU_OFF_SAC]

	@ Enable Non-Secure write access to SCU registers
	@ and read/write access to private timers and
	@ the global timer for all CPUs
	ldr	r0, [r1, #SCU_OFF_SNSAC]
	movw	r2, #0xFFF
	orr	r0, r0, r2
	str	r0, [r1, #SCU_OFF_SNSAC]

	@ Workaround for erratum 764369: "Data or unified cache line
	@ maintenance by MVA fails on Inner Shareable memory"
	@ => Set bit [0] in the undocumented SCU Diagnostic Control
	@    register located at offset 0x30 from the PERIPHBASE
	@    address which disables the "migratory bit" feature
	@
	@ This erratum affects all revisions of Cortex-A9.
	@
	@ Note: SCE performs a RMW operation but the register
	@ is seemingly write-only/RAZ. This is at least true
	@ for bit [0] which is the only documented bit...
	ldr	r0, [r1, #SCU_OFF_DIAGCTRL]
	orr	r0, r0, #1
	str	r0, [r1, #SCU_OFF_DIAGCTRL]

	@ Enable the SCU along with power-saving features
	ldr	r0, [r1, #SCU_OFF_CTRL]
	orr	r0, r0, #SCU_CTRL_IC_STDBY_EN
	orr	r0, r0, #SCU_CTRL_STDBY_EN
	orr	r0, r0, #SCU_CTRL_ENABLE
	str	r0, [r1, #SCU_OFF_CTRL]

	@ Check KBL Param for boot type information:
	@ - wakeupFactor has "resume from suspend" flag
	@ - wakeupFactor reason is not "BSOD reboot"
	@ - BootTypeIndicator1 has "resume" flag
	movw	r0, #:lower16:KBL_PARAM_ADDRESS
	movt	r0, #:upper16:KBL_PARAM_ADDRESS
	ldr	r1, [r0, #__offsetof_SceKblParam_wakeupFactor]

		@ Check "resume from suspend" flag
	and	r2, r1, #0x80
	cmp	r2, #0x80
	bne	.Llab_40020350	@ flag not present

		@ Check wakeupFactor reason
	and	r2, r1, #0x7F
	cmp	r2, #0x17	@ BSOD reboot
	beq	.Llab_40020350

		@ Check BootTypeIndicator1
	ldr	r1, [r0, #__offsetof_SceKblParam_BootTypeIndicator1]
	mov	r2, #0x20000
	and	r1, r1, r2
	cmp	r1, r2
	bne	.Llab_40020350

	@ TODO: how are these addresses obtained?
	@ seems to be a section start/end?
	movw	r0, #0xA800
	movt	r0, #0x4007
	movw	r1, #0xA870
	movt	r1, #0x4007
	sub	r2, r1, r0
	movw	r0, #0xA800
	movt	r0, #0x4007
	movw	r1, #0xA870
	movt	r1, #0x4007
	bl	memcpy

	movw	r0, #:lower16:dat_4007a800
	movt	r0, #:upper16:dat_4007a800
	mov	r1, #0x10			@ "resuming" flag
	str	r1, [r0]
	b	.Llab_40020388

.Llab_40020350:
	@ note: the addresses are identical here
	movw	r0, #0xA800
	movt	r0, #0x4007
	movw	r1, #0xA870
	movt	r1, #0x4007
	sub	r2, r1, r0
	@ ...but inverted for memcpy() src/dst!
	movw	r0, #0xA870
	movt	r0, #0x4007
	movw	r1, #0xA800
	movt	r1, #0x4007
	bl	memcpy

	movw	r0, #:lower16:dat_4007a800
	movt	r0, #:upper16:dat_4007a800
	mov	r1, #0x0			@ "coldboot" flag
	str	r1, [r0]

.Llab_40020388:
	@ CPU0-only sequence complete - wake up other CPUs
	sev

.Llab_4002038c:
	@ Unmask asynchronous aborts
	mrs	r0, cpsr
	bic	r0, r0, #CPSR_A
	msr	cpsr_x, r0

	@ Jump to next stage...
	ldr	pc, =reset_handler

@ Address 0x4002039C: constant pool of this function
@ containing one entry (address of reset_handler)

@
@ KBL Boot exception handlers
@
@ We can infer they are in a separate section because
@ the constant pool would otherwise be merged with that
@ of `_start`, but that is not what we can see in the
@ KBL binaries.
@
	@ Make C handlers visible
	.global	boot_und_handler
	.global	boot_svc_handler
	.global	boot_prefetch_abort_handler
	.global	boot_data_abort_handler
	.global	boot_irq_handler
	.global	boot_fiq_handler

	.text
	.section	".text.head2"
reset_handler: @ Address: 0x400203A0
	@ Jump to C KBL entrypoint
	blx	boot

.Lund_handler: @ Address: 0x400203A4
	sub	lr, lr, #4
	stmdb	sp!, {r0-r3, r12, lr}
	cpy	r0, lr
	blx	boot_und_handler
	clrex
	ldmia	sp!, {r0-r3, r12, pc}^

.Lsvc_handler: @ Address: 0x400203BC
	stmdb	sp!, {r0-r12, lr}
	cpy	r0, lr
	blx	boot_svc_handler
	clrex
	ldmia	sp!, {r0-r12, pc}^

.Lprefetch_abort_handler: @ Address: 0x400203D0
	sub	lr, lr, #4
	stmdb	sp!, {r0-r3, r12, lr}
	cpy	r0, lr
	blx	boot_prefetch_abort_handler
	clrex
	ldmia	sp!, {r0-r12, pc}^

.Ldata_abort_handler: @ Address: 0x400203E8
	sub	lr, lr, #8
	stmdb	sp!, {r0-r3, r12, lr}
	cpy	r0, lr
	blx	boot_data_abort_handler
	clrex
	ldmia	sp!, {r0-r3, r12, pc}^

.Lirq_handler: @ Address: 0x40020400
	sub	lr, lr, #4
	stmdb	sp!, {r0-r3, r12, lr}
	cpy	r0, lr
	blx	boot_irq_abort_handler
	clrex
	ldmia	sp!, {r0-r3, r12, pc}^

.Lfiq_handler: @ Address: 0x40020414
	sub	lr, lr, #4
	stmdb	sp!, {r0-r3, r12, lr}
	cpy	r0, lr
	blx	boot_fiq_abort_handler
	clrex
	ldmia	sp!, {r0-r3, r12, pc}^

@ NOP padding (ARM) until 0x40020430

@ These functions are extremely trivial so it is
@ surprising they weren't inlined. They must have
@ either been assembly or in a dedicated C file.
@
@ For proper binary ordering, put them in ASM here.

	.thumb
	.global	fun_40020430
fun_40020430:
	mov.w	r0, #0x12C
	bx	lr
	@ one Thumb NOP as padding

	.global	fun_40020438
fun_40020438:
	mov.w	r0, #0x11C
	bx	lr
	@ one Thumb NOP as padding

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	.arm
	.text
	.section	".text.long_after_head"
fun_40020a68:	@ Address: 0x40020a68 - may have been written in C?
	@ read CPU ID
	mrc	p15, #0, r3, c0, c0, #5		@ MPIDR
	and	r3, r3, #0xF

	@ compute stack base for this CPU
	mov	r0, #STACK_SIZE
	mul	r0, r3, r0

	@ Initialize sp_mon, lr_mon and spsr_mon
	cpsid	if, #MODE_MON
	ldr	sp, =MON_STACK_BASE	@ data is at 0x40020B1C (!!!)
	sub	sp, sp, r0
	mov	r1, #0
	cpy	lr, r1
	msr	spsr_cxsf, r1

	@ Go back into System mode
	cpsid	if, #MODE_SYS

	@ return
	bx	lr
