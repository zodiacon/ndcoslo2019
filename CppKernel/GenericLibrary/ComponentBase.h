#pragma once

#include "IComponent.h"

template<typename... Interfaces>
struct ComponentBase : public Interfaces... {
	static_assert(sizeof...(Interfaces) > 0);

	LONG AddRef() override {
		return InterlockedIncrement(&_refCount);
	}

	LONG Release() override {
		auto newRef = InterlockedDecrement(&_refCount);
		if (newRef == 0)
			delete this;
		return newRef;
	}

	NTSTATUS QueryInterface(ULONG iid, void** ppv) override {
		if (ppv == nullptr)
			return STATUS_MEMORY_NOT_ALLOCATED;

		auto p = QueryInterface<Interfaces...>(iid);
		if (p) {
			AddRef();
			*ppv = p;
			return STATUS_SUCCESS;
		}
		*ppv = nullptr;
		return STATUS_NOT_FOUND;
	}


private:
	template<typename Interface, typename... Rest>
	void* QueryInterface(ULONG iid) {
		if (iid == Interface::IID)
			return static_cast<Interface*>(this);
		return QueryInterface<Rest...>(iid);
	}

	template<int = 0>
	void* QueryInterface(ULONG) {
		return nullptr;
	}

private:
	LONG _refCount{ 1 };
};
