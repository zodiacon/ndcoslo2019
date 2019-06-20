#pragma once

enum class ObjectAttributesFlags {
	None = 0,
	KernelHandle = OBJ_KERNEL_HANDLE,
	Caseinsensitive = OBJ_CASE_INSENSITIVE,
	OpenIf = OBJ_OPENIF,
	ForceAccessCheck = OBJ_FORCE_ACCESS_CHECK,
	Permanent = OBJ_PERMANENT,
	Exclusive = OBJ_EXCLUSIVE
};
DEFINE_ENUM_FLAG_OPERATORS(ObjectAttributesFlags);

struct ObjectAttributes : OBJECT_ATTRIBUTES {
	ObjectAttributes(PUNICODE_STRING name, ObjectAttributesFlags flags = ObjectAttributesFlags::None,
		HANDLE rootDirectory = nullptr, PSECURITY_DESCRIPTOR sd = nullptr);
};
