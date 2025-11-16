
#ifndef _SYSMEM_UID_CLASS_H_
#define _SYSMEM_UID_CLASS_H_

#include <scetypes.h>

/*
 * Forward declarations for circular dependencies
 */
typedef struct SceClass SceClass;

/**
 * @addtogroup uid_class
 * @{
 */
struct SceClass {
	SceClass *next;
	SceClass *pClass;
	SceClass *prev;
	SceName name;
	SceClass *parent;

	/** @sfnok */
	SceUInt32 attr;

	/** @sfnok */
	SceUInt16 itemSize;

	/** @sfnok */
	SceUInt16 itemAllocationSize;
	SceUInt32 objectType;

	SceInt32 (*constructor)(ScePVoid pObject);
	SceInt32 (*destructor)(ScePVoid pObject);

	SceUInt32 magic;
#	define SCE_CLASS_MAGIC (0xABCD9DA5)
};

/** @} */

#endif /* _SYSMEM_UID_CLASS_H_ */
