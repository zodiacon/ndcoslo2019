#pragma once

struct IComponent abstract {
	enum { IID = 0 };

	virtual NTSTATUS QueryInterface(_In_ ULONG iid, _Outptr_ void** ppv) = 0;
	virtual LONG AddRef() = 0;
	virtual LONG Release() = 0;
};

