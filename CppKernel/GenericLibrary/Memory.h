#pragma once

void* operator new(size_t size, POOL_TYPE type, ULONG tag = 0);

// placement new
void* operator new(size_t size, void* p);

void operator delete(void* p, size_t);

template<typename T = void, typename Traits = PoolTraits>
struct unique_ptr {
	unique_ptr(T* object = nullptr) : _object(object) {}
	~unique_ptr() {
		Release();
	}

	unique_ptr(const unique_ptr& other) = delete;
	unique_ptr& operator=(const unique_ptr& other) = delete;

	operator T*() {
		return _object;
	}

	operator bool() const {
		return _object != nullptr;
	}

	void Attach(T* p) {
		NT_ASSERT(_object == nullptr);
		Release();
		_object = p;
	}

	void Release() {
		if (_object) {
			Traits::Free(_object);
			_object = nullptr;
		}
	}

private:
	T* _object;
};

struct PoolTraits {
	static void Free(void* p) {
		ExFreePool(p);
	}
};

//template<typename T, typename... TArgs>
//unique_ptr make_unique(TArgs... args) {
//	return unique_ptr<T>(std::forward(args));
//}
