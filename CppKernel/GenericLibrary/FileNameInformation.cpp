#include "pch.h"
#include "FileNameInformation.h"

FileNameInformation::FileNameInformation(PFLT_CALLBACK_DATA data, FileNameOptions options) {
	auto status = FltGetFileNameInformation(data, (FLT_FILE_NAME_OPTIONS)options, &_info);
	if (!NT_SUCCESS(status))
		_info = nullptr;
}

FileNameInformation::~FileNameInformation() {
	if (_info)
		FltReleaseFileNameInformation(_info);
}

NTSTATUS FileNameInformation::Parse() {
	return FltParseFileNameInformation(_info);
}
