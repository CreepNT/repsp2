#ifndef _SCE_KERNEL_DIPSW_H
#define _SCE_KERNEL_DIPSW_H

/**
 * @defgroup kernel_dipsw
 * @ingroup kernel
 * @{
 */

#include <scetypes.h>

#define SCE_DIPSW_USER_MIN			0x00
#define SCE_DIPSW_USER_MAX			0x3F

/**
 * @defgroup kernel_dipsw_sdk_sce
 * @brief SDK(SCE) DIP switches
 * @{
 */
#define SCx_DIPSW_memory_size_switch		0x80
#define SCx_DIPSW_release_check_mode_console	0x81
#define SCx_DIPSW__82				0x82
#define SCx_DIPSW__83				0x83
#define SCx_DIPSW__84				0x84
#define SCx_DIPSW__85				0x85
#define SCx_DIPSW__86				0x86
#define SCx_DIPSW__87				0x87
#define SCx_DIPSW__88				0x88
#define SCx_DIPSW__89				0x89
#define SCx_DIPSW__8A				0x8A
#define SCx_DIPSW__8B				0x8B
#define SCx_DIPSW__8C				0x8C
#define SCx_DIPSW__8D				0x8D
#define SCx_DIPSW__8E				0x8E
#define SCx_DIPSW__8F				0x8F
#define SCx_DIPSW__90				0x90
#define SCx_DIPSW__91				0x91
#define SCx_DIPSW__92				0x92
#define SCx_DIPSW__93				0x93
#define SCx_DIPSW__94				0x94
#define SCx_DIPSW__95				0x95
#define SCx_DIPSW__96				0x96
#define SCx_DIPSW__97				0x97
#define SCx_DIPSW_platform_emulation_dolce	0x98
#define SCx_DIPSW__99				0x99
#define SCx_DIPSW__9A				0x9A
#define SCx_DIPSW__9B				0x9B
#define SCx_DIPSW__9C				0x9C
#define SCx_DIPSW__9D				0x9D
#define SCx_DIPSW__9E				0x9E
#define SCx_DIPSW_development_mode		0x9F
/** @} kernel_dipsw_sdk_sce */

/**
 * @defgroup kernel_dipsw_shell
 * @brief Shell DIP switches
 * @{
 */
#define SCx_DIPSW__A0				0xA0
#define SCx_DIPSW__A1				0xA1
#define SCx_DIPSW__A2				0xA2
#define SCx_DIPSW__A3				0xA3
#define SCx_DIPSW__A4				0xA4
#define SCx_DIPSW__A5				0xA5
#define SCx_DIPSW__A6				0xA6
#define SCx_DIPSW__A7				0xA7
#define SCx_DIPSW__A8				0xA8
#define SCx_DIPSW__A9				0xA9
#define SCx_DIPSW__AA				0xAA
#define SCx_DIPSW__AB				0xAB
#define SCx_DIPSW__AC				0xAC
#define SCx_DIPSW__AD				0xAD
#define SCx_DIPSW__AE				0xAE
#define SCx_DIPSW__AF				0xAF
#define SCx_DIPSW__B0				0xB0
#define SCx_DIPSW__B1				0xB1
#define SCx_DIPSW__B2				0xB2
#define SCx_DIPSW__B3				0xB3
#define SCx_DIPSW__B4				0xB4
#define SCx_DIPSW__B5				0xB5
#define SCx_DIPSW__B6				0xB6
#define SCx_DIPSW__B7				0xB7
#define SCx_DIPSW__B8				0xB8
#define SCx_DIPSW__B9				0xB9
#define SCx_DIPSW__BA				0xBA
#define SCx_DIPSW__BB				0xBB
#define SCx_DIPSW__BC				0xBC
#define SCx_DIPSW__BD				0xBD
#define SCx_DIPSW__BE				0xBE
#define SCx_DIPSW__BF				0xBF
/** @} kernel_dipsw_shell */

/**
 * @defgroup kernel_dipsw_debug_control
 * @brief Debug control DIP switches
 * @{
 */
#define SCx_DIPSW__C0				0xC0
#define SCx_DIPSW__C1				0xC1
#define SCx_DIPSW__C2				0xC2
#define SCx_DIPSW__C3				0xC3
#define SCx_DIPSW__C4				0xC4
#define SCx_DIPSW__C5				0xC5
#define SCx_DIPSW__C6				0xC6
#define SCx_DIPSW__C7				0xC7
#define SCx_DIPSW__C8				0xC8
#define SCx_DIPSW__C9				0xC9
#define SCx_DIPSW__CA				0xCA
#define SCx_DIPSW__CB				0xCB
#define SCx_DIPSW__CC				0xCC
#define SCx_DIPSW__CD				0xCD
#define SCx_DIPSW__CE				0xCE
#define SCx_DIPSW__CF				0xCF
#define SCx_DIPSW__D0				0xD0
#define SCx_DIPSW__D1				0xD1

/**
 * @brief Enable TOOL physical memory partition.
 *
 * Requires the SoC to be equipped with additional LPDDR2.
 */
#define SCE_DIPSW_ENABLE_TOOL_PHYMEMPART	0xD2

#define SCx_DIPSW__D3				0xD3
#define SCx_DIPSW__D4				0xD4

/**
 * PA (Performance Analysis) RAM selection
 * When 1: 0x0800'0000 bytes at PA 0x7800'0000
 * When 0: 0x2000'0000 bytes at PA 0x8000'0000
 */
#define SCx_DIPSW__D5				0xD5
#define SCx_DIPSW__D6				0xD6
#define SCx_DIPSW__D7				0xD7
#define SCx_DIPSW__D8				0xD8
#define SCx_DIPSW__D9				0xD9
#define SCx_DIPSW__DA				0xDA
#define SCx_DIPSW__DB				0xDB
#define SCx_DIPSW__DC				0xDC
#define SCx_DIPSW__DD				0xDD
#define SCx_DIPSW__DE				0xDE
#define SCx_DIPSW__DF				0xDF
/** @} kernel_dipsw_debug_control */

/**
 * @defgroup kernel_dipsw_system_control
 * @brief System control DIP switches
 * @{
 */

/**
 * @brief Enable psp2config loading from SD.
 *
 * When set, `SceSysStageMgr` allows using `sd0:psp2-config.txt`
 * as plaintext boot configuration.
 *
 * Since firmware ?.??, this DIP switch is ignored.
 */
#define SCE_DIPSW_PSP2_CONFIG_SD		0xE0

#define SCx_DIPSW__E1				0xE1
#define SCx_DIPSW__E2				0xE2
#define SCx_DIPSW__E3				0xE3
#define SCx_DIPSW__E4				0xE4
#define SCx_DIPSW__E5				0xE5
#define SCx_DIPSW__E6				0xE6
#define SCx_DIPSW__E7				0xE7
#define SCx_DIPSW__E8				0xE8
#define SCx_DIPSW__E9				0xE9
#define SCx_DIPSW__EA				0xEA
#define SCx_DIPSW__EB				0xEB
#define SCx_DIPSW__EC				0xEC
#define SCx_DIPSW__ED				0xED
#define SCx_DIPSW__EE				0xEE
#define SCx_DIPSW__EF				0xEF
#define SCx_DIPSW__F0				0xF0
#define SCx_DIPSW__F1				0xF1
#define SCx_DIPSW__F2				0xF2
#define SCx_DIPSW__F3				0xF3
#define SCx_DIPSW__F4				0xF4
#define SCx_DIPSW__F5				0xF5
#define SCx_DIPSW__F6				0xF6
#define SCx_DIPSW__F7				0xF7
#define SCx_DIPSW__F8				0xF8
#define SCx_DIPSW__F9				0xF9
#define SCx_DIPSW__FA				0xFA
#define SCx_DIPSW__FB				0xFB
#define SCx_DIPSW__FC				0xFC
#define SCx_DIPSW__FD				0xFD
#define SCx_DIPSW__FE				0xFE
#define SCx_DIPSW__FF				0xFF
/** @} kernel_dipsw_system_control */

/**
 * @brief Check state of a DIP switch
 *
 * @param no DIP switch number
 * @return DIP switch state (0 or 1)
 */
SceBool sceKernelCheckDipsw(SceUInt32 no);

/**
 * @brief Set DIP switch
 *
 * @param no DIP switch number
 */
void sceKernelSetDipsw(SceUInt32 no);

/**
 * @brief Clear DIP switch
 *
 * @param no DIP switch number
 */
void sceKernelClearDipsw(SceUInt32 no);
/** @} kernel_dipsw */


/** */

#endif /* _SCE_KERNEL_DIPSW_H */
