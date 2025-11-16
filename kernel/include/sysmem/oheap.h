
#ifndef _SYSMEM_OHEAP_H_
#define _SYSMEM_OHEAP_H_

#include <scetypes.h>
#include <sysmem/spinlock.h>
#include <sysmem/uid_heap.h>

/** Holder for underlying heap object */
struct HeapDescriptor {
	SceUInt32 itemSize;
	SceUIDHeap *pHeap;
	SceUID heapUid;
};

/** Number of distinct resource types supported */
#define OHEAP_RESOURCE_TYPES_NUM 6

/**
 * @addtogroup oheap
 * @{
 */

/** Object Heap structure */
typedef struct {
	SceKernelSpinlock lock;
	/* SceKernelPhyMemPart */void *pPhyMemPart;
	SceUInt32 num_heaps;
	struct HeapDescriptor *heaps;
	SceKernelHeapHook *pHeapHook;
	SceUIDHeap *unk_14;
	SceUIDHeap *unk_18; //probably "SceUIDFixedHeap*"
	struct {
		/** @sfnok */
		SceInt32 nMaxLimit;
		SceUInt32 max_freelist_size;
		SceInt32 num_alloc;
		SceInt32 num_alloc_highwater;
		SceUInt32 free_list_size;
		void **free_list_head;
		void **free_list_tail;
	} per_type_info[OHEAP_RESOURCE_TYPES_NUM];
} ObjectHeap;


/** @todo documentation + maybe this is private? */
void ObjectHeap_init(
	ObjectHeap *pObjectHeap,
	SceKernelPhyMemPart *pPhyMemPart,
	SceUInt32 num_heaps,
	struct HeapDescriptor *heaps,
	SceKernelHeapHook *pHeapHook);

/** @todo documentation + maybe this is private? */
void ObjectHeap_set_heap_for_size(ObjectHeap *pObjectHeap, SceSize itemSize, SceUIDHeap *pHeap, int heapGuid);

//FUN_81000CF0

/**
 * @brief Get appropriate heap for allocations of given size.
 *
 * @param pObjectHeap Target object heap
 * @param size Size to allocate
 * @retval Non-NULL Heap from which memory can be allocated
 * @retval NULL No appropriate heap found
 */
SceUIDHeap *ObjectHeapGetHeap(ObjectHeap *pObjectHeap, SceSize size);

/** @todo documentation */
SceInt32 ObjectHeapSetResourceLimit(ObjectHeap *pObjectHeap, SceUInt32 resourceType, SceInt32 nMaxLimit, SceSize max_cache_size);

/** @todo documentation */
void *ObjectHeap_get_unk18(ObjectHeap *pObjectHeap);

/**
 * @brief Allocate resource from object heap with limit checking.
 *
 * @param pObjectHeap Target object heap
 * @param type Resource type
 * @param[out] ppObject Pointer receiving pointer to allocated memory
 * @retval SCE_TRUE Resource allocation successful
 * @retval SCE_FALSE Resource allocation failed (object limit overflow)
 *
 * @note When SCE_TRUE is returned, `*ppObject` may be NULL; in such cases,
 * the caller must perform the allocation using @ref UIDHeapCallAlloc.
 */
SceBool ObjectHeapAllocLimit(ObjectHeap *pObjectHeap, SceUInt32 type, ScePVoid *ppObject);

/**
 * @brief Attempt freeing memory into object heap.
 *
 * @param pObjectHeap Target object heap
 * @param type Resource type
 * @param pObject Pointer to memory
 * @retval SCE_TRUE Resource has been free'ed (added to object heap freelist)
 * @retval SCE_FALSE Resource has been ignored
 * When this value is returned, the caller should call @ref UIDHeapCallFree on
 * the appropriate heap to release the resource.
 */
SceBool ObjectHeapFree(ObjectHeap *pObjectHeap, SceUInt32 type, ScePVoid pObject);





/*
 *====================================================
 *           Exported functions & variables
 *====================================================
 */





/**
 * @brief Obtain pointer to the kernel's object heap
 * @export{SceSysmemForKernel,?,857408DA}
 */
ObjectHeap *get_kernel_objectheap(void);

/**
 * @brief Create an object heap.
 *
 * @param processId Object heap owner PID
 * @param a2
 * @param pHeapHook Heap operations hook
 * @param ppObjectHeap Receives pointer to created object heap
 * @returns_sceerr
 * @export{SceSysmemForKernel,?,9D8F3BE8}
 * @fnnok
 * @fnaok{3}
 */
SceInt32 sceKernelCreateObjectHeap(
	ScePID processId,
	int a2,
	SceKernelHeapHook *pHeapHook,
	ObjectHeap **ppObjectHeap);

/**
 * @brief
 *
 * @param pObjectHeap
 * @param a2
 * @param a3
 * @returns_sceerr
 * @export{SceSysmemForKernel,?,660DC18A}
 * @fnnok
 *
 * @wip
 */
SceInt32 sceKernelObjectHeapStart(ObjectHeap *pObjectHeap, int a2, unsigned a3);

/**
 * @brief Delete an object heap.
 *
 * @param pObjectHeap Target object heap
 * @returns_sceerr
 * @export{SceSysmemForKernel,?,2F526AB8}
 */
SceInt32 sce_KernelDeleteObjectHeap(ObjectHeap *pObjectHeap);

/**
 * @brief Set size of resources of a given type in object heap.
 *
 * @param type
 * @param size
 * @returns_sceerr
 * @export{SceSysmemForKernel,?,B591E49F}
 *
 * @note this affects all object heaps globally - but how exactly?
 * @wip
 */
SceInt32 sceKernelObjectHeap_setResourceSize(SceUInt32 type, SceSize size);

/**
 * @brief Set limit for specific resource in object heap.
 *
 * @param pObjectHeap Target object heap
 * @param resourceType Target resource type
 * @param nMaxLimit Maximum number of allocations for resource type
 * @param max_cache_size Maximum cache size for the resource type
 * This is the number of free()'ed items kept allocated in a freelist
 * instead of releasing the memory back to the underlying heap.
 * @returns_sceerr
 * @export{SceSysmemForKernel,?,DFA1EB57}
 * @fnnok
 */
SceInt32 sceKernelObjectHeapSetResourceLimit(ObjectHeap *pObjectHeap, SceUInt32 resourceType, SceInt32 nMaxLimit, SceSize max_cache_size);

/**
 * @brief Set limit for "all" resource types in an object heap.
 *
 * @param pObjectHeap Target object heap
 * @param a2 Maximal resource type index on which limits are set
 * @param a3 Resource limits information
 * @returns_sceerr
 * @export{SceSysmemForKernel,?,067F2550}
 * @fnnok
 *
 * @wip
 */
SceInt32 sceKernelObjectHeapSetResourceLimitAll(ObjectHeap *pObjectHeap, unsigned a2, const void *a3);

/**
 * @brief Allocate memory from an object heap.
 *
 * @param pObjectHeap Object heap to allocate from
 * @param size Size of memory to allocate
 * @returns Non-NULL pointer on success, NULL on failure
 * @export{SceSysmemForKernel,?,EBFC8E10}
 * @fnnok
 */
ScePVoid sceKernelObjectHeapAlloc(ObjectHeap *pObjectHeap, SceSize size);

/**
 * @brief Free memory allocated from an object heap.
 *
 * @param pObjectHeap Object heap from which memory comes
 * @param size Size of allocated memory
 * @param ptr Pointer to allocated memory
 * @returns_sceerr
 * @export{SceSysmemForKernel,?,17D3D4BD}
 */
SceInt32 sceKernelObjectHeapFree(ObjectHeap *pObjectHeap, SceSize size, ScePVoid ptr);

/** @} */

#endif /* _SYSMEM_OHEAP_H_ */
