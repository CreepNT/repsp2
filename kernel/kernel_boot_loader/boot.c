
#include <__everything.h>

#include <sdk_version.h>
#include <scetypes.h>
#include <sceerror.h>

#include <arm.h>
#include <hw_timer.h>
#include <kernel/debug.h>
#include <kernel/dipsw.h>
#include <kernel/error.h>

///////////////////////////////////////////////////////
//TEMP IMPORTS
///////////////////////////////////////////////////////
#define KBL_REVISION_STR	"r96726"			//TODO: autogenerate
#define KBL_BUILD_DATE_STR	"2017-02-23 19:47:57+0900"	//TODO: autogenerate

typedef struct {
	SceUInt32 spin; //SceKernelSpinlock spin;
	SceUInt16 unk4;
	SceUInt16 owner;
} SceKernelCorelock;

typedef struct {
	SceUIntPAddr addr;
	SceSize size;
} SceKernelPARange;

typedef struct {
	SceUInt16 company_code;
	SceUInt16 product_code;
	SceUInt16 product_subcode;
	SceUInt16 factory_code;
} ScePSCode;

typedef struct {
	SceUInt16 version;
	SceUInt16 size;
	SceUInt32 current_fw;
	SceUInt32 minimal_fw;
	SceUInt32 unused_C;
	SceUInt32 unk_10;
	SceUInt32 unused_14[3];
	SceUInt8 qaf[16];
	SceUInt8 boot_flags[16];

	SceUInt32 bootsw[8]; /**!< aka DIP switches */
#define BOOTSW_CP_INFO		1
#define BOOTSW_ASLR_SEED	3

	SceUIntPAddr dram_base;
	SceSize dram_size;
	SceUInt32 unused_68;
	SceUInt32 BootTypeIndicator1;
	SceUInt8 openpsid[16];
	SceKernelPARange cached_secure_modules[4];
	ScePSCode pscode;
	SceUInt32 stack_canary; 		//= RNG from Bigmac
	SceUInt32 random_AC;    		//= RNG from Bigmac
	SceUInt8 session_id[16];
	SceUInt32 sleepFactor;
	SceUInt32 wakeupFactor; 		/**!< Official name */
	SceUInt32 connInfo; //???
	SceUInt32 bootControlsInfo;
	SceUIntPAddr suspendinfo_adr; 	/**!< Official name */
	SceUInt32 hardwareInfo;		  	/**!< Official name */
	SceUInt32 powerInfo;
	SceUInt32 unk_DC;
	SceUInt32 kbl_load_time;		/**!< Time taken by SBL to load KBL */
	SceUInt8 hwInfo2[16];
	SceUInt32 sbl_revision;			/**!< SBL revision */
	SceUInt32 magic;
} SceKBLParam;
#define KBL_PARAM_MAGIC 0xCBAC03AA

typedef struct { //TODO: there are official names among these
	const char *name;
	SceUIntPAddr pbase;
	SceUIntVAddr vbase;
	SceSize psize;
	SceSize vsize;
	SceSize extraHigh;
} kbp_boot_block;

typedef struct {
	SceUInt32 TTBR1;
	SceUInt32 DACR;
	SceUInt32 contextid;					/**!< (OFFICIAL NAME) */
} SceKernelMMUContext;

typedef struct {
	SceSize size; 							/**!< Size of this structure */
	SceBool secure;							/**!< SCE_TRUE in Secure state, SCE_FALSE in Non-secure */
	SceSize num_memory;						/**!< number of entries in "memory" */
	SceKernelPARange memory[4]; 			/**!< seems official name? */
	SceKBLParam *pKblParam;					/**!< official name? */
	SceUInt32 unused_30[5]; //???
	SceUInt32 revision0;					/**!< official name? */
	SceUInt32 revision1;
	SceUInt32 KermitRevision;				/**!< = low bits of revision0 */
	SceUInt32 unk_50[2];					/**!< depends on pKblParam->hardwareInfo */
	kbp_boot_block ttbr0;					/**!< (OFFICIAL NAME) */
	SceUIntVAddr ttbr0_max_addr;
	SceSize sizeTTBR0Address;				/**!< (OFFICIAL NAME) */
	kbp_boot_block ttbr1;					/**!< (OFFICIAL NAME) */
	SceSize sizeTTBR1Address;				/**!< (OFFICIAL NAME) */
	SceUIntVAddr l2pt000_map_base;			/**!< first vaddr managed by SceKernelL2PageTable000 */
	kbp_boot_block reset;
	kbp_boot_block excpEntry;
	kbp_boot_block l2pt000;
	kbp_boot_block l2v;						/**!< (OFFICIAL NAME) (= L2 Vector) - TODO: where is name from? */
	kbp_boot_block sysroot;
	kbp_boot_block fh32b;
	kbp_boot_block fh48b;
	kbp_boot_block fh64b;
	kbp_boot_block fhUIDEntry;
	kbp_boot_block fhL2Object;
	kbp_boot_block unk188;					/**!< unused? */
	kbp_boot_block phypage;
	kbp_boot_block phypageHigh;				/** (OFFICIAL NAME) */
	kbp_boot_block bootkernimg;
	kbp_boot_block hwreg;
	SceKernelCorelock *pCorelock;

	struct {
		SceUInt32 unk_00[2];
		SceKernelMMUContext mmuContext;
		SceSize size;
		SceUID blkId;
		void *stackBottom;
	} bootCpu[SCx_KERNEL_NUM_CPUS];

	void *pSysroot; //TODO: retype
	SceUInt32 l2pt000_map_base__again;
	SceUInt32 *pL2PageTable000; // vaddr of L2PT000
	void *resetVector; // vaddr of reset vector

	void *phyMemPartKD;				//TODO: retype (SceKernelPhyMemPart)
	void *phyMemPartTool;			//TODO: retype (SceKernelPhyMemPart)

	void *pPageKernelReset;				//TODO: retype (PhyPage)
	void *pPageL2PageTable000;			//TODO: retype (PhyPage)
	void *pPageSysroot;					//TODO: retype (PhyPage)
	void *pPageTTBR0;					//TODO: retype (PhyPage)
	void *pPageTTBR1;					//TODO: retype (PhyPage)
	void *pPageL2Vector;				//TODO: retype (PhyPage)
	void *pPagePhypage;					//TODO: retype (PhyPage)
	void *pPagePhypageHigh;				/**!< (OFFICIAL NAME) TODO: retype (PhyPage) */
	void *pPageBootKernelImage;			//TODO: retype (PhyPage)
	void *pPageFixedHeap32B;			//TODO: retype (PhyPage)
	void *pPageFixedHeap48B;			//TODO: retype (PhyPage)
	void *pPageFixedHeap64B;			//TODO: retype (PhyPage)
	void *pPageFixedHeapForL2Object;	//TODO: retype (PhyPage)

	void *pFixedHeap32B;				//TODO: retype (SceUIDFixedHeap)
	void *pFixedHeap48B;				//TODO: retype (SceUIDFixedHeap)
	void *pFixedHeap64B;				//TODO: retype (SceUIDFixedHeap)
	void *pFixedHeapForL2Object;		//TODO: retype (SceUIDFixedHeap)

	void *pPageUIDHeap;					//TODO: retype (PhyPage)
	void *pGUIDEntryHeap;				//TODO: retype (SceUIDEntryHeap)

	void *pL2PageTable_for_SceKernelL2PageTable000;	//TODO: retype (L2PageTable)
	void *pL2PageTable_for_PhyPage;					//TODO: retype (L2PageTable)

	void *pPartitionKernel;				/**!< SceKernelRoot partition (TODO: retype SceUIDPartition) */
	SceUID uidPartitionKernel;			/**!< (OFFICIAL NAME) */

	void *pKernelAddressSpace;			//TODO: retype (SceUIDAddressSpace)

	SceUInt32 unk_2fc;
	void *unk_300;

	void *putchar_handler;
	SceUInt32 minimum_log_level;

	SceUInt32 magic;
} SceKernelBootParam;
#define KERNEL_BOOT_PARAM_MAGIC 0x7F407C30


#define ALIGN_UP(ptr, align) ((ScePVoid)((((SceUIntPtr)(ptr)) + (align) - 1u) & ~((align) - 1u)))


extern char VECTOR_TABLE[];
const SceSize VECTOR_TABLE_SIZE = 0x100; //TODO: get this programatically? (END - START)?


void sceKernelCorelockLock(SceKernelCorelock *corelock, SceUInt32 firstcpu);
SceUInt32 sceKernelCpuId(void);
void sceKernelCorelockUnlock(SceKernelCorelock *corelock);
void *memcpy(void *, const void *, SceSize);
void __set_dipsw_source(const void *pDipsw); // FUN_5101ef0c
void sceKernelPrintfLevel(SceInt32 lvl, const char *fmt, ...);
SceInt32 sceKernelGetAssertLevel(void); // FUN_5101bc64
void sceKernelAssertLevel(SceInt32 lvl, _Bool cond, const void *kmc); // FUN_5101bfb0 | FUN_4003e538

void FUN_5101c56c(SceUInt32 flags, void *kmc, char *fmt, ...);
void FUN_5101c5dc(SceInt32 lvl, SceUInt32 flags, void *kmc, char *fmt, ...);

// FUN_400417d4 / FUN_5101ec88
SceInt32 sceKernelVAtoPA(const void *va, SceUIntPAddr *pPA);

// FUN_4004086c / FUN_5101df2c
SceUInt32 l1pt_to_ttbr(SceUIntPAddr l1pt_paddr) {
	// TODO: document
	return l1pt_paddr | 0x4A;
}

void sceKblMain(SceKernelBootParam *pBoot);

int smc_0x103(void); // "FreeSharedMemory_S"

/* BSS segment boundaries */
extern void _fbss;
extern void _end;

static inline SceUInt32 __perv_get_revision0(void) { return *((volatile SceUInt32 *)0xE3100000); }
static inline SceUInt32 __perv_get_revision1(void) { return *((volatile SceUInt32 *)0xE3100004); }

//TODO: kmc auto-generation
	// FUN_4003E9DC for SKBL
#define DBG_KMC_PRINT(flags, fmt, ...) FUN_5101c56c(flags, SCE_NULL, fmt, ##__VA_ARGS__)

	// FUN_???? for SKBL
#define DBG_KMC_PRINT_LVL(lvl, flags, fmt, ...) FUN_5101c5dc(lvl, flags, SCE_NULL, fmt, ##__VA_ARGS__)

#define DBG_PRINT_sensitive_lvl0(fmt, ...) \
	sceKernelPrintfLevel(__msg_loglevel(SCx_LOG_LVL_0), fmt, ##__VA_ARGS__)
#define DBG_PRINT_sensitive_lvl1(fmt, ...) \
	sceKernelPrintfLevel(__msg_loglevel(SCx_LOG_LVL_1), fmt, ##__VA_ARGS__)

#define __ASSERT_LVL(lvl, cond) do {	\
	if (sceKernelGetAssertLevel() >= (lvl)) { \
		sceKernelAssertLevel(lvl, !!(cond), SCE_NULL); \
	}} while (0) // TODO KMC generation

#define KERMIT_REVISION_MASK 0x1FFFF //Old: 0xFF

///////////////////////////////////////////////////////
//ENDOF TEMP IMPORTS
///////////////////////////////////////////////////////
#define __SCx_KBL_SECURE_BUILD 0  //0 for KBL, 1 for NSKBL



#if __SCx_KBL_SECURE_BUILD
#define KBL_PARAM_LOCATION ((const void *)0x00000100)
#else /* KBL (non-secure) */
#define KBL_PARAM_LOCATION ((const void *)0x40200100)
#endif





static SceKernelCorelock g_boot_corelock = { 1U, 4U, 4U };
static SceKernelCorelock kbp_corelock = { 1U, 4U, 4U }; // @ 0x51040008

static SceKBLParam gKblParam;
static SceKernelBootParam gKernelBootParam;
static SceKernelBootParam *gpKernelBootParam;

static inline int __msg_loglevel(SceInt32 lvl)
{
	//TODO: use SblQafMgr function
	return (gKblParam.qaf[0xD] & 0x1) ? lvl : SCx_LOG_LVL_2;
}


SceInt32 FUN_510013f8(void) { return 300; } //always this??











































































































































































































































































































































#define MEGA_ASLR_BITMAP_SIZE 16 //TODO: does it make sense to define it as 16MiB/1MiB?
static SceUInt8 mega_aslr_bitmap[MEGA_ASLR_BITMAP_SIZE];

SceInt32 mega_aslr_bitmap_alloc(SceUInt32 index)
{
	/* TODO */
}

SceUInt32 MapMegaASLR(SceSize size, SceUInt32 seed)
{
	/* TODO */

	DBG_KMC_PRINT_LVL(SCx_LOG_LVL_1, SCx_LOG_FLAG_4 | SCx_LOG_FLAG_2 | SCx_LOG_FLAG_1, "Failed\n"); // boot.c:617

	for (;;) {
		/* Halt boot process */
	}
}
























#define ASLR_BITMAP_SIZE (SCE_KERNEL_1MiB / SCE_KERNEL_4KiB) // 1 bit per page in L2PT000
static SceUInt8 aslr_bitmap[ASLR_BITMAP_SIZE];

SceInt32 aslr_bitmap_alloc(SceUInt32 offset, SceSize size)
{
	SceUInt32 startIndex = offset / SCx_PAGE_SIZE;
	SceUInt32 numBits = size / SCx_PAGE_SIZE;
	SceUInt32 endIndex;

	if (numBits == 0) {
		return SCE_OK;
	}

	//TODO: maybe this is compiler loop unrolling??
	if (aslr_bitmap[offset % ASLR_BITMAP_SIZE] != 1) {
		endIndex = startIndex + numBits;

		/* check that entire range is free in bitmap */
		for (SceUInt32 i = startIndex; i < endIndex; i++) {
			if (aslr_bitmap[i % ASLR_BITMAP_SIZE] == 1) {
				goto error;
			}
		}

		/* reserve the range in bitmap */
		for (SceUInt32 i = startIndex; i < endIndex; i++) {
			aslr_bitmap[i % ASLR_BITMAP_SIZE] = 1;
		}

		return 0;
	}

error:
    return SCE_KERNEL_ERROR_ERROR;
}

SceUInt32 MapASLR(SceSize size, SceUInt32 seed) {
	SceUInt32 maxIdx = ASLR_BITMAP_SIZE / (size / SCx_PAGE_SIZE);
	SceUInt32 startIdx = seed % maxIdx;
	maxIdx += startIdx;

	for (SceUInt32 i = startIdx; i < maxIdx; i++) {
		SceUInt32 offset = (i * size) % SCE_KERNEL_1MiB; // keep offset within 1MiB
		SceInt32 res = ASLR_bitheap_alloc(offset, size);

		if (res < 0) {
			continue;
		}

		return offset;
	}


	DBG_KMC_PRINT_LVL(SCx_LOG_LVL_1, SCx_LOG_FLAG_4 | SCx_LOG_FLAG_2 | SCx_LOG_FLAG_1, "Failed\n"); // boot.c:700

	for (;;) {
		/* Halt boot process */
	}
}



























































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































void boot(void) // 40020c8c in SKBL
{
	SceUInt32 aslrSeed, megaAslrSeed;

	sceKernelCorelockLock(&g_boot_corelock, 0);
	if (sceKernelCpuId() == 0) {
		/* CPU0-only initialization tasks */

		FUN_5101b63c(); //GPO initialization (0x4003E0E4 in SKBL)
		sceKernelSetGPO(SCx_GPO_CODE_KBL_01);

		if (!__SCx_KBL_SECURE_BUILD) {
			/* Unmap KBL in trusted kernel */
			smc_0x103();
		}

		/* Clear BSS segment */
		for (SceUInt32 *p = (SceUInt32 *)&_fbss; p < (SceUInt32 *)&_end; p++) {
			*p = 0;
		}

		/* Initialize kernel boot param */
		gKernelBootParam.revision0 = __perv_get_revision0();
		gKernelBootParam.revision1 = __perv_get_revision1();

		/* Retrieve KBL Param left for us by previous stage */
		memcpy(&gKblParam, KBL_PARAM_LOCATION, sizeof(SceKBLParam));
		gKernelBootParam.pKblParam = &gKblParam;

#if __SCx_KBL_SECURE_BUILD
		gKblParam.magic = KBL_PARAM_MAGIC;

		if ((gKblParam.pscode.product_code & 0xFEFFu) == 1
			&& gKblParam.bootsw[BOOTSW_CP_INFO] == 0) {
			/*
			 * If TEST or TOOL and "CP bid & version" DIP switch
			 * is zero, forcefully enable certain DIP switches.
			 */
			extern uint32_t g_NoCP_dipsw[8];
			for (int i = 0; i < 8; i++) {
				gKblParam.bootsw[i] |= g_NoCP_dipsw[i];
			}
		}
#endif /* __SCx_KBL_SECURE_BUILD */

		__set_dipsw_source(&gKblParam.bootsw);

#if __SCx_KBL_SECURE_BUILD
		if (gKblParam.boot_flags[1] == 0xFF) {
			sceKernelClearDipsw(SCx_DIPSW__C4);
			sceKernelClearDipsw(SCx_DIPSW__D3);
		} else {
			sceKernelSetDipsw(SCx_DIPSW__C4);
			sceKernelSetDipsw(SCx_DIPSW__D3);
		}

		if (!(gKblParam.boot_flags[4] & 0x1)) {
			sceKernelSetDipsw(SCx_DIPSW__BA);
		}

		if ((gKblParam.pscode.product_code & 0xFEFFu) == 1
			&& !sceKernelCheckDipsw(SCx_DIPSW__CB))
		{
			/*
			 * If TEST or TOOL and DIPsw 0xCB is not set,
			 * forcefully reconfigure certain DIP switches.
			 */
			if (!sceKernelCheckDipsw(SCx_DIPSW__9F)) {
				sceKernelClearDipsw(SCx_DIPSW__C0);
				sceKernelClearDipsw(SCx_DIPSW__C1);
				sceKernelClearDipsw(SCx_DIPSW__C2);
				sceKernelClearDipsw(SCx_DIPSW__C5);
				sceKernelClearDipsw(SCx_DIPSW__C6);
				sceKernelClearDipsw(SCx_DIPSW__C7);
				sceKernelClearDipsw(SCx_DIPSW__C8);
				sceKernelClearDipsw(SCx_DIPSW__C9);
				sceKernelClearDipsw(SCx_DIPSW__CC);
				sceKernelClearDipsw(SCx_DIPSW__CE);
				sceKernelClearDipsw(SCE_DIPSW_ENABLE_TOOL_PHYMEMPART);
				sceKernelClearDipsw(SCx_DIPSW__E4);
				sceKernelClearDipsw(SCx_DIPSW__D4);
				sceKernelClearDipsw(SCx_DIPSW__80);
				sceKernelClearDipsw(SCx_DIPSW__D5);

				if (!sceKernelCheckDipsw(SCx_DIPSW__81)) {
					sceKernelClearDipsw(SCx_DIPSW__C5);
					sceKernelClearDipsw(SCx_DIPSW__D3);
				} else {
					sceKernelSetDipsw(SCx_DIPSW__C5);
					sceKernelSetDipsw(SCx_DIPSW__D3);
				}
			} else {
				sceKernelSetDipsw(SCx_DIPSW__C0);
				sceKernelSetDipsw(SCx_DIPSW__C1);
				sceKernelSetDipsw(SCx_DIPSW__C2);
				sceKernelSetDipsw(SCx_DIPSW__C5);
				sceKernelSetDipsw(SCx_DIPSW__C6);
				sceKernelSetDipsw(SCx_DIPSW__C7);
				sceKernelSetDipsw(SCx_DIPSW__C8);
				sceKernelSetDipsw(SCx_DIPSW__C9);
				sceKernelSetDipsw(SCx_DIPSW__CC);
				sceKernelSetDipsw(SCx_DIPSW__CE);
				sceKernelSetDipsw(SCx_DIPSW__E4);
				sceKernelSetDipsw(SCx_DIPSW__D4);

				if ((gKernelBootParam.revision0 & 0x1FF00u) == 0u) {
					/*
					 * Kermit 1.0: extra TOOL LPDDR2 available
					 * when the top bit of revision0 is CLEAR
					 */
					if ((gKernelBootParam.revision0 & 0x80000000u) == 0u) {
						sceKernelSetDipsw(SCE_DIPSW_ENABLE_TOOL_PHYMEMPART);
					} else {
						sceKernelClearDipsw(SCE_DIPSW_ENABLE_TOOL_PHYMEMPART);
					}
				} else {
					/*
					 * Kermit 1.5: bits [31:30] of revision0
					 * indicate amount of LPDDR2 banks (???)
					 *
					 * 0b00/01/10 = 2/1/4 banks | 0b11 = illegal?
					 */
					if ((gKernelBootParam.revision0 & 0x30000000) == 0x20000000u) {
						/* 0b10: four banks => TOOL LPDDR2 available */
						sceKernelSetDipsw(SCE_DIPSW_ENABLE_TOOL_PHYMEMPART);
					} else {
						/* Not 0b10 (1/2 banks) => TOOL LPDDR2 not available */
						sceKernelClearDipsw(SCE_DIPSW_ENABLE_TOOL_PHYMEMPART);
					}
				}

				if (((gKernelBootParam.revision0 & 0x1FF00u) == 0x100u)
					&& ((gKernelBootParam.revision0 & 0x80000000u) != 0u)
					&& ((gKernelBootParam.revision0 & 0x30000000u) == 0x20000000u))
				{
					/*
					 * Kermit 1.5 with 4 LPDDR banks + revision0 top bit
					 * => enable alternate PA RAM mapping
					 */
					sceKernelSetDipsw(SCx_DIPSW__D5);
				} else {
					/* use normal PA RAM mapping */
					sceKernelClearDipsw(SCx_DIPSW__D5);
				}

				if (gKernelBootParam.pKblParam->powerInfo & 0x40)
				{	//TODO: what is 0x40?
					sceKernelClearDipsw(SCx_DIPSW__C8);
				}
			}

			sceKernelSetDipsw(SCx_DIPSW__FD);

			if (sceKernelCheckDipsw(SCx_DIPSW__C4)) {
				/*
				 * Disable Deci4pSTtyp (System TTY)
				 * if UART0 logging is enabled
				 */
				sceKernelClearDipsw(SCx_DIPSW__C6);
			}
		}
#endif /* __SCx_KBL_SECURE_BUILD */

		FUN_5101c6e8(); // set gAssertLevel - 4003EB58 in SKBL
		FUN_5101c750(); // memset(0) something - 4003EBC0 in SKBL

		if (sceKernelCheckDipsw(SCx_DIPSW__C5) ||
			sceKernelCheckDipsw(SCx_DIPSW__C4) ||
			sceKernelCheckDipsw(SCx_DIPSW__FD))
		{
			FUN_51020cb4(sceKernelCheckDipsw(SCx_DIPSW__C4)); // 40041A84 in SKBL
			sceKernelSetGPO(SCx_GPO_CODE_KBL_02);
			FUN_5101c43c(&FUN_51020c75, 0); // register putchar() callback - resp. 4003E8AC / 400419E9 in SKBL
		}

		DBG_KMC_PRINT(0, "===================================================\n");	// boot.c:1868
#if __SCx_KBL_SECURE_BUILD
		const SceUInt32 xxx = FUN_40020430();
		DBG_KMC_PRINT(0, "Starting PSP2 Kernel Boot Loader [0x%08X]: %d\n",	//boot.c:1871
			SCE_PSP2_SDK_VERSION, xxx);
		DBG_KMC_PRINT(0, "revision   : %s\nbuild date : %s\n",			//boot.c:1873
			KBL_REVISION_STR, KBL_BUILD_DATE_STR);

		const SceUInt32 cp_bid_ver = gKblParam.bootsw[BOOTSW_CP_INFO];

		if (cp_bid_ver != 0) {
			const SceUInt16 bid = (cp_bid_ver >> 16) & 0xFFFFu;
			const SceUInt16 ver = (cp_bid_ver >>  0) & 0xFFFFu;

			DBG_KMC_PRINT(0, "cp info.   : bid.%x ver.%04x\n", bid, ver);	//boot.c:1882
		} else {
			DBG_KMC_PRINT(0, "cp info.   : (N/A)\n");			//boot.c:1884
		}

		const bool is_product_mode = (gKblParam.BootTypeIndicator1 & 0x4) != 0;
		if (is_product_mode) {
			DBG_KMC_PRINT_LVL(1, 0, "\tProduct Mode : [ YES ]\n");		//boot.c:1889
		}

		if ((is_product_mode || !!(gKblParam.qaf[0xD] & 0x1)) && (gKblParam.BootTypeIndicator1 & 0x8) != 0) {
			FUN_400207F0(); // secure DRAM KblSimpleMemoryTest()
		}
#else
		DBG_PRINT_sensitive_lvl0("Starting PSP2 Kernel Boot Loader (Non-secure) [0x%08x]: %d\n", SCE_PSP2_SDK_VERSION, FUN_510013f8());
		DBG_KMC_PRINT_LVL(SCx_LOG_LVL_1, 0, "\n"); //boot.c:1897
#endif /* __SCx_KBL_SECURE_BUILD */

		gpKernelBootParam = &gKernelBootParam;

		const SceUInt32 kermitRevision = gKernelBootParam.revision0 & KERMIT_REVISION_MASK;
		gKernelBootParam.KermitRevision = kermitRevision;
		gKernelBootParam.size = sizeof(SceKernelBootParam);
		gKernelBootParam.secure = __SCx_KBL_SECURE_BUILD;
		gKernelBootParam.pCorelock = &kbp_corelock;
		gKernelBootParam.magic = KERNEL_BOOT_PARAM_MAGIC;
























		__ASSERT_LVL(SCx_ASSERT_LVL_1, gKernelBootParam.KermitRevision != 0); // boot.c:1932

#if __SCx_KBL_SECURE_BUILD
		const char *soc_name;
		if (gKblParam.qaf[0xD] & 0x1) {
			soc_name = ((kermitRevision & 0x1FF00u) == 0) ? "Kermit1.0" : "Kermit1.5";
		} else {
			soc_name = "SoC";
		}
		DBG_KMC_PRINT_LVL(1, 0, "\t%s Revision=%d.%d\n", soc_name,	// boot.c:1941
			(kermitRevision & 0xF0u) >> 4, kermitRevision & 0xFu);

		const SceUInt32 rev = gpKernelBootParam->kermitRevision;
		if ((rev == 0)
		    || (rev == 0x11)
		    || (rev == 0x20)
		    || (rev == 0x30)
		    || (rev == 0x32)) {
			__ASSERT_LVL(1, SCE_FALSE); // boot.c:1950
		} else if ((rev == 0x40)
			   || (rev == 0x42)
			   || (rev == 0x110)
			   || (rev == 0x111)
			   || (rev == 0x112)
			   || (rev == 0x113)
			   || (rev == 0x114)
			   || (rev == 0x115)) {
			const SceUInt32 revision1 = gpKernelBootParam->revision1;
			const bool bad_l2_cache = !!(revision1 & 0x2);

			if (bad_l2_cache) {
				DBG_PRINT_sensitive_lvl0("\tL2 Cache is defective\n");
			}
		} else {


			DBG_KMC_PRINT_LVL(1, 0, "\tUnknown chip revision [rev=%x]\n", rev);	//boot.c:1968
		}

		DBG_PRINT_sensitive_lvl1("\tASLR: 0x%08x\n", gKblParam.bootsw[BOOTSW_ASLR_SEED]);
#else /* __SCx_KBL_SECURE_BUILD */
		DBG_PRINT_sensitive_lvl1("\tBOOTSW\n");
		for (int i = 0; i < ARRAY_SIZE(gKblParam.bootsw); i++) {
			DBG_PRINT_sensitive_lvl1("\t\t%d: 0x%08x", i, gKblParam.bootsw[i]);
			switch (i) {
			case 0: DBG_PRINT_sensitive_lvl1(": CP time\n"); break;
			case 1: DBG_PRINT_sensitive_lvl1(": CP bid & version\n"); break;
			case 2: DBG_PRINT_sensitive_lvl1(": CP time\n"); break;
			case 3: DBG_PRINT_sensitive_lvl1(": ASLR\n"); break;
			case 4: DBG_PRINT_sensitive_lvl1(": SDK(SCE)\n"); break;
			case 5: DBG_PRINT_sensitive_lvl1(": SHELL\n"); break;
			case 6: DBG_PRINT_sensitive_lvl1(": debug control\n"); break;
			case 7: DBG_PRINT_sensitive_lvl1(": system control\n"); break;
			};
		}
#endif /* __SCx_KBL_SECURE_BUILD */

#if __SCx_KBL_SECURE_BUILD
		const SceUInt8 *pSessionId = gKblParam.session_id;


		DBG_KMC_PRINT(0, "session ID : %02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X\n",	// boot.c:1993
			pSessionId[0], pSessionId[1], pSessionId[2], pSessionId[3],
			pSessionId[4], pSessionId[5], pSessionId[6], pSessionId[7],
			pSessionId[8], pSessionId[9], pSessionId[10], pSessionId[11],
			pSessionId[12], pSessionId[13], pSessionId[14], pSessionId[15]);
#endif /* __SCx_KBL_SECURE_BUILD */


#if __SCx_KBL_SECURE_BUILD
		//TODO: take these values from #defines/...
		gpKernelBootParam->num_memory = 1;
		gpKernelBootParam->memory[0].addr = 0x40000000;
		gpKernelBootParam->memory[0].size = 0x02000000; // 2 MiB
#else /* __SCx_KBL_SECURE_BUILD */
		//TODO: take these values from #defines/...
		gpKernelBootParam->num_memory = 3;
		gpKernelBootParam->memory[0].addr = 0x40200000;
		if (sceKernelCheckDipsw(SCE_DIPSW_ENABLE_TOOL_PHYMEMPART)) {
			gpKernelBootParam->memory[0].size = 0x3fe00000; //1 GiB (-2 MiB for TZS)
		} else {
			gpKernelBootParam->memory[0].size = 0x1fe00000; //512 MiB (-2 MiB for TZS)
		}

		gpKernelBootParam->memory[1].addr = 0x20000000;
		gpKernelBootParam->memory[1].size = 0x08000000; //128 MiB
		gpKernelBootParam->memory[2].addr = 0x80000000;
		gpKernelBootParam->memory[2].size = 0x40000000; //1 GiB
#endif /* __SCx_KBL_SECURE_BUILD */

		for (int i = 0; i < gpKernelBootParam->num_memory; i++) {
			SceKernelPARange *mem = &gpKernelBootParam->memory[i];
			SceUIntPAddr endAddr = mem->addr + mem->size - 1;
			DBG_PRINT_sensitive_lvl1("\tmemory[%d] base: 0x%08x-0x%08x [0x%08x]\n",
				i, mem->addr, endAddr, mem->size);
		}

		DBG_PRINT_sensitive_lvl1("\twakeupFactor: 0x%08x\n", gKblParam.wakeupFactor);
		DBG_PRINT_sensitive_lvl1("\thardwareInfo: 0x%08x\n", gKblParam.hardwareInfo);

		//TODO: something sets gKernelBootParam.unk50[0/1] based on hardwareInfo
		//TODO: print "model: VITA" or "model: DOLCE"

		if ((gKernelBootParam.pKblParam->boot_flags[0] != 0xFF)
		    || sceKernelSysrootIsSafeMode()) { // SKBL: FUN_40020C0C | NSKBL:
			sceKernelClearDipsw(SCx_DIPSW_platform_emulation_dolce);
		}

		if (sceKernelCheckDipsw(SCx_DIPSW_platform_emulation_dolce)) {
			//TODO
		} else {
			//TODO
		}

		sceKernelSetGPO(SCx_GPO_CODE_KBL_03);

		FUN_5102bcb8(); //pervasive_init() - 40041bf4 in SKBL

#if __SCx_KBL_SECURE_BUILD
		FUN_40042124(); // pervClk_ena_dev0x170()
		FUN_40041ee4(); // pervRst_dis_dev0x170()

		// MMIO:LPDDR CH0 I/F stuff
		*(SceUInt32*)0xE6000090 = 0x7000001;
		*(SceUInt32*)0xE6000094 = 0xEC0004;
		__DMB();

		FUN_400421a0();
		FUN_400421d4();

		// MMIO:SceSonyRegbus stuff
		*(SceUInt32*)0xE8000010 = 0xA;
		__DMB();

		if ((gpKernelBootParam->KermitRevision & 0x1FFF0) == 0x30) {
			/* Workaround for Kermit1.0 ES3.0 - what's the problem? */
			// MMIO:PERVASIVE_MISC stuff
			(*(SceUInt32*)0xE3100228) |= 0x8;
		}

		FUN_40042100(); // pervClk_ena_dev0x158()
		FUN_40041eb8(); // pervRst_dis_dev0x158() - also clears PERV_MISC + 0x2F0!

		if (sceKernelCheckDipsw(SCx_DIPSW_development_mode)
		    || sceKernelCheckDipsw(SCx_DIPSW__C0)
		    || sceKernelCheckDipsw(SCx_DIPSW__C1)
		    || sceKernelCheckDipsw(SCx_DIPSW__C2)) {
			FUN_400420f0();	// pervClk_ena_dev0x154() - DebugBus
			FUN_40041ea8(); // pervRst_dis_dev0x154() - DebugBus
		}

		if (sceKernelCheckDipsw(SCx_DIPSW_development_mode)) {
			FUN_40042134(); // pervClk_ena_dev0x174()
			FUN_40041ef4(); // pervRst_dis_dev0x174()
			FUN_400420a4(); // pervClk_ena_dev0x048() - DebugPA
			FUN_40041e48(); // pervRst_dis_dev0x048() - DebugPA
		}

		if (sceKernelCheckDipsw(SCx_DIPSW__C0) || sceKernelCheckDipsw(SCx_DIPSW__C1)) {
			FUN_40042090(0); // pervClk_ena_devs0x40(0) - SDIO0
			FUN_40041E34(0); // pervRst_dis_devs0x40(0) - SDIO0
		}

		if (sceKernelCheckDipsw(SCx_DIPSW__E4)) {
			FUN_40042038(0xC0000);     // pervClk_ena_dev0_invertBit16_17(0xC0000) - enable ARM CPUs (?!?!)
			FUN_40042050(); // pervClk_ena_dev0x4() - ARM debug logic?
			FUN_40041de4(); // pervRst_dis_dev0x4()
		}

		if (sceKernelCheckDipsw(SCx_DIPSW_development_mode)) {
			if (gpKernelBootParam->revision0 & 0x80000000u) {
				DBG_PRINT_sensitive_lvl0("Disabled LPDDR2SUB\n");
			} else {
				// TODO: turn on lpddr2 sub
				// MMIO:ScePervasiveBaseClk
				*(SceUInt32*)0xE3103094 = 1;

				FUN_40041f34(1); // perv_misc_devs0x44_set_to_0(1)
				FUN_40042110(1); // perv_clk_ena_devs0x160(1)
				FUN_40041ed0(1); // perv_res_dis_devs0x160(1)

				// MMIO:DDRIF1
				SceUInt32 tmp = *(volatile SceUInt32*)0xe5880000u;
				*(SceUInt32*)0xe5880000 = 0x130b0011u;
				*(SceUInt32*)0xe58880c8 = 0x1fff;
				*(SceUInt32*)0xe5888030 = 0xa28;
				*(SceUInt32*)0xe5888038 = 5;
				*(SceUInt32*)0xe5888040 = 4;
				*(SceUInt32*)0xe5888048 = 0x2b;
				*(SceUInt32*)0xe5888050 = 5;
				*(SceUInt32*)0xe5888058 = 3;
				*(SceUInt32*)0xe5888070 = 7;
				*(SceUInt32*)0xe5888060 = 0x18;
				*(SceUInt32*)0xe5888068 = 3;
				*(SceUInt32*)0xe5888088 = 0xc;
				*(SceUInt32*)0xe5888090 = 0xc;
				*(SceUInt32*)0xe5888098 = 0x10;
				*(SceUInt32*)0xe58880b0 = 6;
				*(SceUInt32*)0xe58880e0 = 0x2e;
				*(SceUInt32*)0xe58880e8 = 0x2e;
				*(SceUInt32*)0xe58880f0 = 2;
				*(SceUInt32*)0xe5888140 = 4;
				*(SceUInt32*)0xe5888150 = 2;
				*(SceUInt32*)0xe5888158 = 2;
				*(SceUInt32*)0xe5888160 = 0xb;
				*(SceUInt32*)0xe5888168 = 5;
				*(SceUInt32*)0xe5888178 = 6;
				*(SceUInt32*)0xe58880a8 = 0x2475;
				*(SceUInt32*)0xe58881a8 = 3;
				*(SceUInt32*)0xe58881b8 = 3;
				*(SceUInt32*)0xe5888200 = 0x72;
				*(SceUInt32*)0xe5888228 = 0x60;
				*(SceUInt32*)0xe5888260 = 0x70;

				//TODO: use adefine for 0x80
				if (gKblParam.wakeupFactor & 0x80) { //0x80 = resume from suspend
					*(SceUInt32*)0xe5888000 = 5;
					*(SceUInt32*)0xe5888008 = 5;
					while ((*(volatile SceUInt32*)0xe5888020) != 0x44) {
						// ...
					}
				}

				*(SceUInt32*)0xe5888000 = 1;
				*(SceUInt32*)0xe5888008 = 1;
				while ((*(volatile SceUInt32*)0xe5888020) != 0x11) {
					// ...
				}

				*(SceUInt32*)0xe5888180 = 1;
				*(SceUInt32*)0xe5888100 = 0x302;
				*(SceUInt32*)0xe5888108 = 0x302;

				// test write to LPDDR2SUB (???)
				*(SceUInt32*)0x80000000 = 0xdeadcafe;
			}
		}

		if (sceKernelCheckDipsw(SCx_DIPSW_development_mode)) {
			// MMIO:SceGrab
			*(SceUInt32*)0xE8301210 = 0x1;
		}

		// MMIO: PervasiveMisc
		*(SceUInt32*)0xE31000B0 = 0x1F3DAu;
		*(SceUInt32*)0xE31000B4 = 0xFFu;

		// MMIO: Pervasive2
		*(SceUInt32*)0xE3110FC0 = 0x7FFFFu;

		// MMIO: SceLT5
		LongTimer *lt5 = (void *)0xE20B6000u;
		lt5->configuration = 0x2F345008u; //TODO: decompose
		lt5->current.u.lo = 0u;
		lt5->current.u.hi = 0u;
		lt5->compare.u.lo = 0xFFFFFFFFu;
		lt5->compare.u.hi = 0xFFFFFFFFu;
		lt5->configuration = 0x2F34500Du; //TODO: decompose
#endif /* __SCx_KBL_SECURE_BUILD */

		sceKernelSetGPO(SCx_GPO_CODE_KBL_04);

#if __SCx_KBL_SECURE_BUILD
		if (DAT_4007a800 == 0) {
			/*
			 * When coldbooting, copy KBL Param
			 * to non-secure DRAM for NSKBL.
			 *
			 * TODO: use a define for the dst address
			 */
			memcpy((void *)0x40200100, &gKblParam, sizeof(gKblParam));
		}
#endif /* __SCx_KBL_SECURE_BUILD */
	}

//LAB_5100044c: | LAB_40020cce:
	__set_CSSELR(0);	//p15,0x2,r3,cr0,cr0,0x0
	__set_TEECR(1);		//p14,0x6,r3,cr0,cr0,0x0
	__set_TEEHBR(8);	//p14,0x6,r3,cr1,cr0,0x0
	__set_JOSCR(1);		//p14,0x7,r3,cr1,cr0,0x0

	// Also this is saved on the stack...
	__set_VBAR(VECTOR_TABLE);	//p15,0x0,r7,cr12,cr0,0x0

	__set_TPIDRURW(0); //p15,0x0,r3,cr13,cr0,0x2
	__set_TPIDRURO(0); //p15,0x0,r3,cr13,cr0,0x3
	__set_TPIDRPRW(0); //p15,0x0,r3,cr13,cr0,0x4

	sceKernelCorelockUnlock(&g_boot_corelock);

	sceKernelSetGPO(SCx_GPO_CODE_KBL_05);

	//Note that "TTBR1" is used to actually hold a TTBCR value here!
	SceUInt32 cpuId = sceKernelCpuId();
	gpKernelBootParam->bootCpu[cpuId].mmuContext.TTBR1 = 2; //TODO: #define
	gpKernelBootParam->bootCpu[cpuId].mmuContext.DACR  = 0x55555555; //TODO: #define
	gpKernelBootParam->bootCpu[cpuId].mmuContext.contextid = 0xf7; //TODO: #define | for SKBL this is 0xF0
	sceArmSetDACR(0x55555555); //TODO: #define (for 0x55...)

	sceKernelCorelockLock(&g_boot_corelock, 0);
	if (cpuId == 0) {
		/* ... */

		/* Initialize ASLR bitmaps */
		for (int i = 0; i < ARRAY_SIZE(aslr_bitmap); i++) {
			aslr_bitmap[i] = 0;
		}
		for (int i = 0; i < ARRAY_SIZE(mega_aslr_bitmap); i++) {
			mega_aslr_bitmap[i] = 0;
		}

		/* Reserve bottom 16 KiB of L2PT000 for NULL guard page */
		aslr_bitmap_alloc(0, 4 * SCE_KERNEL_4KiB);

		/* Mark first 1MiB as used (for L2PT000) in MegaASLR bitmap */
		//OPTIMIZED TO
		//if (MegaASLR_bitmap[0] != 0x1)
		//   MegaASLR_bitmap[0] = 0x1;

		/* ...TODO... */
	}
	sceKernelCorelockUnlock(&g_boot_corelock);

	// Configure paging registers
	__set_TTBR0(l1pt_to_ttbr(gpKernelBootParam->ttbr0.pbase));
	__set_TTBR1(l1pt_to_ttbr(gpKernelBootParam->ttbr1.pbase));
	__set_TTBCR(gpKernelBootParam->bootCpu[cpuId].mmuContext.TTBR1);
	__set_DACR(0x55555555);
	__set_CONTEXTIDR(gpKernelBootParam->bootCpu[cpuId].mmuContext.contextid);

	sceKernelCorelockLock(&g_boot_corelock, 0);
	if (sceKernelCpuId() == 0) {
		/*
		 * Copy vector table to SceExceptionEntry (why?!)
		 * and to SceExceptionEntry+0x100 (used as VBAR)
		 */
		char *p = memcpy((void *)gpKernelBootParam->memory[0].addr, VECTOR_TABLE, VECTOR_TABLE_SIZE);
		memcpy(p + VECTOR_TABLE_SIZE, VECTOR_TABLE, VECTOR_TABLE_SIZE);
		gpKernelBootParam->resetVector = gpKernelBootParam->excpEntry.vbase + 0x100;

#if __SCx_KBL_SECURE_BUILD
		extern char MONITOR_TABLE[];
		const SceSize MONITOR_TABLE_SIZE = 0x34;

		/*
		 * Copy monitor table to SceExceptionEntry+0x03C
		 * (aligned up to 0x20 boundary as required for MVBAR)
		 * => In practice, MVBAR = SceExceptionEntry+0x40 (?)
		 *
		 * TODO: use a define for MVBAR alignement (0x20)
		 */
		memcpy(ALIGN_UP(gpKernelBootParam->memory[0].addr, 0x20), MONITOR_TABLE, MONITOR_TABLE_SIZE);

		//wtf?
		*(SceUInt32*)(gpKernelBootParam->memory[0].addr + 0xA0) = 0x9E3199B7;
#endif /* __SCx_KBL_SECURE_BUILD */
	}
	sceKernelCorelockUnlock(&g_boot_corelock);

#if __SCx_KBL_SECURE_BUILD
	if (sceKernelCpuId() == 0) {
		FUN_40040514(L2C_BASE); // disable L2C and apply proper configuration
	}
#endif /* __SCx_KBL_SECURE_BUILD */

	sceKernelCorelockLock(&g_boot_corelock, 0);
	/* TODO: old FW has something here? */
	sceKernelCorelockUnlock(&g_boot_corelock);

#if __SCx_KBL_SECURE_BUILD
	if (sceKernelCpuId() == 0) {
		FUN_40040514(L2C_BASE); //l2cache_init() - disable + configure

		if (sceKernelCheckDipsw(SCx_DIPSW_development_mode)) {
			FUN_4003fe3c(1); // l2cache_evt_monitor_bus_enable()
		}

		FUN_4003fee8(); // l2cache_inv_all();
	}

	FUN_4003f9fc(); // d$ inv all (PoU)
	FUN_4003f9d0(); // i$ inv all PoU

	if (sceKernelCpuId() == 0) {
		FUN_4003fe60();	// l2cache_enable();
	}
#endif /* __SCx_KBL_SECURE_BUILD */

	FUN_5101d6d4(); //set SCTLR / enable MMU - FUN_4003fad0 in SKBL

#if __SCx_KBL_SECURE_BUILD
	FUN_4003fb0c(); //set ACTLR stuff
#endif /* __SCx_KBL_SECURE_BUILD */

	sceKernelCorelockLock(&g_boot_corelock, 0);
	/* TODO: old FW has something here? */
	sceKernelCorelockUnlock(&g_boot_corelock);

	sceKernelCorelockLock(&kbp_corelock, 0);
	/* TODO: old FW has something here? */
	sceKernelCorelockUnlock(&kbp_corelock);

	__set_VBAR(gpKernelBootParam->resetVector);
#if __SCx_KBL_SECURE_BUILD
	__set_MVBAR(ROUND_UP((SceUIntPtr)gpKernelBootParam->resetVector + 0x5B, 0x20u));
#endif /* __SCx_KBL_SECURE_BUILD */

	FUN_5101ee30(); //set some global to 1 - FUN_400418d8 in skbl

	sceKernelCorelockLock(&kbp_corelock, 0);
	(void)sceKernelCpuId();
	/* TODO: old FW has something here? */
	sceKernelCorelockUnlock(&kbp_corelock);

	sceKernelSetGPO(SCx_GPO_CODE_KBL_06);

	/* TODO: old FW has something here? */

	sceKernelSetGPO(SCx_GPO_CODE_KBL_07);

	if (sceKernelCpuId() == 0) {
		SceUIntPAddr pa;
		sceKernelVAtoPA((void *)0x8000, &pa);
		sceKernelVAtoPA((void *)0x40100000, &pa);
		sceKernelVAtoPA((void *)0x47000000, &pa);
		sceKernelVAtoPA((void *)0xe0000000, &pa);
	}

	/* Jump to KBL main function */
	sceKblMain(gpKernelBootParam);

	sceKernelCorelockLock(&kbp_corelock, 0);
	(void)sceKernelCpuId();
	/* TODO: old FW has something here? */
	sceKernelCorelockUnlock(&kbp_corelock);

	for (;;) {
		/* Halt processor */
	}
}
