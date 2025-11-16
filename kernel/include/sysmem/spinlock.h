
#ifndef _SYSMEM_SPINLOCK_H_
#define _SYSMEM_SPINLOCK_H_

#include <scetypes.h>

/** Single-owner spinlock */
typedef SceUInt32 SceKernelSpinlock;

/**
 * @brief Read-write spinlock
 *
 * Can be owned by one thread in Write mode
 * or multiple threads in Read mode.
 */
typedef SceUInt32 SceKernelRWSpinlock;

#endif /* _SYSMEM_SPINLOCK_H_ */
