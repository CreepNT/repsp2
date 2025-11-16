
#include <scetypes.h>
#include <sysmem/oheap.h>

//ObjectHeap_init#

//ObjectHeap_set_heap_for_size#

//FUN_81000CF0

static SceUIDHeap *ObjectHeapGetHeap(ObjectHeap *pObjectHeap, SceSize itemSize) {
	if (pObjectHeap == SCE_NULL) {
		goto return_null;
	}

	if (pObjectHeap->num_heaps == 0) {
		goto return_null;
	}

	for (SceUInt32 i = 0; i < pObjectHeap->num_heaps; i++) {
		struct HeapDescriptor *pHeapDesc = &pObjectHeap->heaps[i];
		if (itemSize <= pHeapDesc->itemSize && pHeapDesc->pHeap != SCE_NULL) {
			return pHeapDesc->pHeap;
		}
	}

return_null:
	/* SceDebugForDriver_D9703808("Cannot found for 0x%08x\n", itemSize); */
	return SCE_NULL;
}

//ObjectHeapSetResourceLimit

//ObjectHeap_get_unk18_heap

//ObjectHeapAllocLimit

//ObjectHeapFree

//get_kernel_objectheap

//sceKernelCreateObjectHeap

//sceKernelObjectHeapStart

//sceKernelObjectHeap__delete

//objectheap_set_resource_size

//sceKernelObjectHeapSetResourceLimit

//sceKernelObjectHeapSetResourceLimitAll

//sceKernelObjectHeapAlloc

SceInt32 sceKernelObjectHeapFree(ObjectHeap *pObjectHeap, SceSize size, ScePVoid ptr) {
	SceUIDHeap *pHeap = ObjectHeapGetHeap(pObjectHeap, size);
	return UIDHeapCallFree(pHeap, ptr);
}
