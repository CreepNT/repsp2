#ifndef _SCE_ARM_CP15_H
#define _SCE_ARM_CP15_H

#ifndef _ASMLANGUAGE

static inline unsigned int sceArmGetDACR(void)
{
	return __builtin_arm_mrc(15, 0, 3, 0, 0);
}

static inline void sceArmSetDACR(unsigned int dacr)
{
	__builtin_arm_mcr(15, 0, dacr, 3, 0, 0);
}

#endif /* _ASMLANGUAGE */
#endif /* _SCE_ARM_CP15_H */
