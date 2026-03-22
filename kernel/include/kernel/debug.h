#ifndef _SCE_KERNEL_DEBUG_H
#define _SCE_KERNEL_DEBUG_H

#include <scetypes.h>

/**
 * @brief General-purpose Output LED codes
 *
 * 0x00~0x3F: not used (reserved for BROM?)
 * 0x40~0x7F: Secure Loader (a.k.a. second_loader)
 * 0x80~0x9F: Kernel Boot Loader
 * 0xA0~0xBF: Kernel Boot Loader (non-secure)
 * 0xC0~0xFF: not used?
 *
 * (not sure that 0xBF is end of KBL-NS but it makes most sense)
 */
#if __SCx_KBL_SECURE_BUILD
#define SCx_GPO_CODE_FACILITY_KBL 0x80
#else
#define SCx_GPO_CODE_FACILITY_KBL 0xA0
#endif /* __building_KBL_SECURE */

#define SCx_GPO_CODE_KBL_00								(SCx_GPO_CODE_FACILITY_KBL | 0x00) //GPO init OK?
#define SCx_GPO_CODE_KBL_01								(SCx_GPO_CODE_FACILITY_KBL | 0x01)
#define SCx_GPO_CODE_KBL_02		 						(SCx_GPO_CODE_FACILITY_KBL | 0x02)
#define SCx_GPO_CODE_KBL_03		 						(SCx_GPO_CODE_FACILITY_KBL | 0x03)
#define SCx_GPO_CODE_KBL_04						 		(SCx_GPO_CODE_FACILITY_KBL | 0x04) //Pervasive init OK?
#define SCx_GPO_CODE_KBL_05		 						(SCx_GPO_CODE_FACILITY_KBL | 0x05)
#define SCx_GPO_CODE_KBL_06		 						(SCx_GPO_CODE_FACILITY_KBL | 0x06)
#define SCx_GPO_CODE_KBL_07		 						(SCx_GPO_CODE_FACILITY_KBL | 0x07)
#define SCx_GPO_CODE_KBL_08		 						(SCx_GPO_CODE_FACILITY_KBL | 0x08)
#define SCx_GPO_CODE_KBL_09		 						(SCx_GPO_CODE_FACILITY_KBL | 0x09)
#define SCx_GPO_CODE_KBL_10		 						(SCx_GPO_CODE_FACILITY_KBL | 0x0A)
#define SCx_GPO_CODE_KBL_11		 						(SCx_GPO_CODE_FACILITY_KBL | 0x0B)
#define SCx_GPO_CODE_KBL_12		 						(SCx_GPO_CODE_FACILITY_KBL | 0x0C)
#define SCx_GPO_CODE_KBL_13		 						(SCx_GPO_CODE_FACILITY_KBL | 0x0D)
#define SCx_GPO_CODE_KBL_14		 						(SCx_GPO_CODE_FACILITY_KBL | 0x0E)
#define SCx_GPO_CODE_KBL_15		 						(SCx_GPO_CODE_FACILITY_KBL | 0x0F)
#define SCx_GPO_CODE_KBL_16		 						(SCx_GPO_CODE_FACILITY_KBL | 0x10)
#define SCx_GPO_CODE_KBL_17		 						(SCx_GPO_CODE_FACILITY_KBL | 0x11)
#define SCx_GPO_CODE_KBL_18		 						(SCx_GPO_CODE_FACILITY_KBL | 0x12)
#define SCx_GPO_CODE_KBL_19		 						(SCx_GPO_CODE_FACILITY_KBL | 0x13)
#define SCx_GPO_CODE_KBL_20		 						(SCx_GPO_CODE_FACILITY_KBL | 0x14)
#define SCx_GPO_CODE_KBL_21		 						(SCx_GPO_CODE_FACILITY_KBL | 0x15)
#define SCx_GPO_CODE_KBL_22		 						(SCx_GPO_CODE_FACILITY_KBL | 0x16)
#define SCx_GPO_CODE_KBL_23		 						(SCx_GPO_CODE_FACILITY_KBL | 0x17)
#define SCx_GPO_CODE_KBL_24		 						(SCx_GPO_CODE_FACILITY_KBL | 0x18)
#define SCx_GPO_CODE_KBL_25		 						(SCx_GPO_CODE_FACILITY_KBL | 0x19)
#define SCx_GPO_CODE_KBL_26		 						(SCx_GPO_CODE_FACILITY_KBL | 0x1A)
#define SCx_GPO_CODE_KBL_27		 						(SCx_GPO_CODE_FACILITY_KBL | 0x1B)
#define SCx_GPO_CODE_KBL_28		 						(SCx_GPO_CODE_FACILITY_KBL | 0x1C)
#define SCx_GPO_CODE_KBL_29		 						(SCx_GPO_CODE_FACILITY_KBL | 0x1D)
#define SCx_GPO_CODE_KBL_30		 						(SCx_GPO_CODE_FACILITY_KBL | 0x1E)
#define SCx_GPO_CODE_KBL_31		 						(SCx_GPO_CODE_FACILITY_KBL | 0x1F)

/**
 * @brief Set pattern to display on GPO LEDs
 *
 * @param uiBits Pattern to display (8 bits only)
 */
SceInt32 sceKernelSetGPO(SceUInt32 uiBits);

#endif /* _SCE_KERNEL_DEBUG_H */
