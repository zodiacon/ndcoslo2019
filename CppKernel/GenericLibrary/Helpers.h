#pragma once

#include "Handles.h"
#include "ObjectAttributes.h"

struct Helpers abstract final {
	KernelHandle OpenProcess(ACCESS_MASK accessMask, HANDLE pid, ObjectAttributesFlags flags = ObjectAttributesFlags::KernelHandle);
};

