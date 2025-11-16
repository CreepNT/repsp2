
#ifndef _SYSMEM_UID_HEAP_H_
#define _SYSMEM_UID_HEAP_H_

#include <scetypes.h>
#include <sysmem/oheap.h>
#include <sysmem/uid_class.h>

/*
 * Forward declarations for circular dependencies
 */
typedef struct SceUIDHeapClass SceUIDHeapClass;

/**
 * @addtogroup uid_heap
 * @{
 */
typedef struct {
	ScePVoid userData;
	SceUIDHeapClass *pClass;
	ObjectHeap *pObjectHeap;
} SceUIDHeap;

struct SceUIDHeapClass {
	SceClass _super;
	SceInt32 (*allocObjectWithSizeFunc)(SceUIDHeap *pHeap, SceSize size, ScePVoid *ppObject);
	SceInt32 (*freeObjectFunc)(SceUIDHeap *pHeap, ScePVoid pObject);
};

typedef struct {
	ScePVoid handle;

	/** @sfnok */
	ScePVoid (*funcAlloc)(ScePVoid handle, SceSize size);
	SceInt32 (*funcFree)(ScePVoid handle, SceSize size, ScePVoid ptr);
} SceKernelHeapHook;

/**
 * @brief Allocate memory using heap hook.
 *
 * @param pHeapHook Heap hook
 * @param size Size of memory to allocate
 * @returns Non-NULL pointer on success, NULL on failure
 */
static ScePVoid sceKernelHeapHookAlloc(SceKernelHeapHook *pHeapHook, SceSize size) {
	return pHeapHook->funcAlloc(pHeapHook->handle, size);
}

/**
 * @brief Allocate object from heap.
 *
 * @param pHeap Heap object
 * @param size Size of object to allocate
 * @param ppObject Pointer to receive allocated object pointer
 * @returns_sceerr
 * @fnnok
 */
SceInt32 UIDHeapCallAlloc(SceUIDHeap *pHeap, SceUInt32 size, ScePVoid *ppObject);

/**
 * @brief Free object from heap.
 *
 * @param pHeap Heap object
 * @param pObject Pointer to object to free
 * @returns_sceerr
 */
SceInt32 UIDHeapCallFree(SceUIDHeap *pHeap, ScePVoid pObject);
/** @} */

#endif /* _SYSMEM_UID_HEAP_H_ */
