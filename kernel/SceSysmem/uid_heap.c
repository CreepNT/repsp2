
#include <scetypes.h>
#include <sysmem/uid_heap.h>

SceInt32 UIDHeapCallAlloc(SceUIDHeap *pHeap, SceUInt32 size, ScePVoid *pOutPtr) {
	return pHeap->pClass->allocObjectWithSizeFunc(pHeap, size, pOutPtr);
}

SceInt32 UIDHeapCallFree(SceUIDHeap *pHeap, ScePVoid pObject) {
	return pHeap->pClass->freeObjectFunc(pHeap, pObject);
}
