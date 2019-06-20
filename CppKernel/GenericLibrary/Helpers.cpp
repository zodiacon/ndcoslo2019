#include "pch.h"
#include "Helpers.h"

KernelHandle Helpers::OpenProcess(ACCESS_MASK accessMask, HANDLE pid, ObjectAttributesFlags flags) {
	KernelHandle hProcess;
	ObjectAttributes processAttributes(nullptr, flags);
	CLIENT_ID client;
	client.UniqueProcess = pid;
	client.UniqueThread = nullptr;
	ZwOpenProcess(hProcess.GetAddressOf(), accessMask, &processAttributes, &client);
	return hProcess;
}
