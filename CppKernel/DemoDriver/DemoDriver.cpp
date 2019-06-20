#include <ntddk.h>
#include "..\GenericLibrary\Memory.h"
#include "..\GenericLibrary\ComponentBase.h"
#include "..\GenericLibrary\kvector.h"
#include "..\GenericLibrary\kstring.h"
#include "..\GenericLibrary\LinkedList.h"

#define DRIVER_TAG 'omed'

struct IFilter abstract : IComponent {
	enum { IID = 10 };
	virtual void FilterRequest(PIRP Irp) = 0;
};

struct IProcessNotify abstract : IComponent {
	enum { IID = 11 };
	virtual void ProcessCreated(HANDLE ProcessId, PEPROCESS Process) = 0;
};


struct MyFilter : ComponentBase<IFilter, IProcessNotify> {
	template<POOL_TYPE type = PagedPool, ULONG tag = 0>
	static IComponent* Create() {
		return static_cast<IFilter*>(new (type, tag) MyFilter);
	}

	void FilterRequest(PIRP Irp) override {
		UNREFERENCED_PARAMETER(Irp);
		// do something
	}

	void ProcessCreated(HANDLE ProcessId, PEPROCESS Process) override {
		UNREFERENCED_PARAMETER(ProcessId);
		UNREFERENCED_PARAMETER(Process);
	}

private:
	MyFilter() {}
};

IFilter* g_Filter;

void DemoUnload(PDRIVER_OBJECT) {
	g_Filter->Release();
}

void WorkWithComponent(IComponent* c) {
	UNREFERENCED_PARAMETER(c);
}

template<typename T>
using vector = kvector<T, DRIVER_TAG>;

struct MyData {
	int Data;
	LIST_ENTRY Entry;
	int MoreData;
};

struct MyData2 {
	int Count;
	FastMutex Lock;
	vector<PDEVICE_OBJECT> Devices;
};

LinkedList<MyData> MyList;

extern "C" NTSTATUS
DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath) {
	auto data = new (NonPagedPool, DRIVER_TAG) MyData2;
	data->Count = 0;

	auto data2 = static_cast<MyData*>(ExAllocatePoolWithTag(NonPagedPool, sizeof(MyData), DRIVER_TAG));
	new (data2) MyData;

	data2->~MyData();
	ExFreePool(data2);

	MyList.Init();
	auto item = new (PagedPool, DRIVER_TAG) MyData;
	item->Data = 8;
	item->MoreData = 19;

	MyList.PushBack(item);

	DriverObject->DriverUnload = DemoUnload;

	kstring<DRIVER_TAG> regPath(RegistryPath);
	regPath.ToLower();
	regPath += L"hello";

	vector<int> v;
	v.Add(5);
	v.Add(12);

	auto c = MyFilter::Create<NonPagedPool>();
	if (c == nullptr)
		return STATUS_INSUFFICIENT_RESOURCES;

	c->QueryInterface(IFilter::IID, reinterpret_cast<void**>(&g_Filter));
	if (g_Filter) {
		g_Filter->FilterRequest(nullptr);
		g_Filter->Release();
	}

	IProcessNotify* notify;
	auto status = g_Filter->QueryInterface(IProcessNotify::IID, reinterpret_cast<void**>(&notify));
	if (!NT_SUCCESS(status)) {
		KdPrint(("No IProcessNotify interface\n"));
	}
	else {
		// do more work
		notify->Release();
	}

	return STATUS_SUCCESS;
}
