#include "pch.h"
#include "Memory.h"

void* operator new(size_t size, POOL_TYPE type, ULONG tag) {
	auto p = tag == 0 ? ExAllocatePool(type, size) : ExAllocatePoolWithTag(type, size, tag);
	if (p == nullptr) {
		KdPrint(("Failed to allocate %d bytes\n", size));
	}
	return p;
}

void* operator new(size_t, void* p) {
	return p;
}

void operator delete(void* p, size_t) {
	ExFreePool(p);
}
