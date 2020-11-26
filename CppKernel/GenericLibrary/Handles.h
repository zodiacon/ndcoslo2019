#pragma once

template<typename T, typename Traits>
struct GenericHandle {
	GenericHandle(T handle = Traits::InvalidHandle(), bool owner = true) : _handle(handle), _owner(owner) {
	}

	~GenericHandle() {
		Reset();
	}

	GenericHandle(GenericHandle&) = delete;
	GenericHandle& operator=(const GenericHandle&) = delete;

	GenericHandle(GenericHandle&& other) {
		_handle = other._handle;
		_owner = other._owner;
		other._handle = nullptr;
	}

	GenericHandle& operator=(GenericHandle&& other) {
		if (this != &other) {
			Reset();
			_handle = other._handle;
			_owner = other._owner;
			other._handle = nullptr;
		}
		return *this;
	}

	operator T() const {
		return Get();
	}

	T Get() const {
		return _handle;
	}

	operator bool const() {
		return Traits::IsValid(_handle);
	}

	T* GetAddressOf() {
		NT_ASSERT(!Traits::IsValid(_handle));
		return &_handle;
	}

	void Reset(T handle = nullptr, bool owner = true) {
		if (_owner && _handle)
			Traits::Close(_handle);
		_handle = handle;
		_owner = owner;
	}

	T Detach() {
		auto h = _handle;
		_handle = nullptr;
		return h;
	}

private:
	T _handle;
	bool _owner;
};

struct KernelHandleTraits {
	static void Close(HANDLE handle) {
		ZwClose(handle);
	}

	static bool IsValid(HANDLE handle) {
		return handle != nullptr;
	}

	static HANDLE InvalidHandle() {
		return nullptr;
	}
};

using KernelHandle = GenericHandle<HANDLE, KernelHandleTraits>;

struct FltHandleTraits {
	static void Close(HANDLE handle) {
		FltClose(handle);
	}

	static bool IsValid(HANDLE handle) {
		return handle != nullptr;
	}

	static HANDLE InvalidHandle() {
		return nullptr;
	}
};

using FltKernelHandle = GenericHandle<HANDLE, FltHandleTraits>;

template<typename T>
struct KernelObject {
	KernelObject(T* object = nullptr) : _object(object) {
	}

	~KernelObject() {
		Release();
	}

	KernelObject(const KernelObject& other) = delete;
	KernelObject& operator=(const KernelObject& other) = delete;

	KernelObject(KernelObject&& other) {
		_object = other.Detach();
	}

	KernelObject& operator=(KernelObject&& other) {
		if (&other != this) {
			Release();
			_object = other.Detach();
		}
		return *this;
	}

	operator T*() {
		return _object;
	}

	operator bool() const {
		return _object != nullptr;
	}

	T** GetAddressOf() {
		NT_ASSERT(_object == nullptr);
		return &_object;
	}

	void Release() {
		if (_object) {
			ObDereferenceObject(_object);
			_object = nullptr;
		}
	}

	void Attach(T* object) {
		Release();
		_object = object;
	}

	T* Detach() {
		auto temp = _object;
		_object = nullptr;
		return temp;
	}

private:
	T* _object;
};
