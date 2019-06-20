#include "pch.h"
#include "ObjectAttributes.h"

ObjectAttributes::ObjectAttributes(PUNICODE_STRING name, ObjectAttributesFlags flags, HANDLE rootDirectory, PSECURITY_DESCRIPTOR sd) {
	InitializeObjectAttributes(this, name, static_cast<ULONG>(flags), rootDirectory, sd);
}

