// BugExample.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>
#include <winnt.h>
#include "ntdll.h"

#pragma comment(lib,"ntdll_x64.lib")
//If you use ZwMapViewOfSection to load 2 copies of ntdll.dll (path is consistent) then the breakpoint on this module will be wrong. ZwOpenFile -> ZwCreateSection -> ZwMapViewOfSection//If you use ZwMapViewOfSection to load 2 copies of ntdll.dll (path is consistent) then the breakpoint on this module will be wrong. ZwOpenFile -> ZwCreateSection -> ZwMapViewOfSection
int main()
{
	UNICODE_STRING FileName;
	OBJECT_ATTRIBUTES ObjectAttributes = { 0 };
	HANDLE FileHandle = INVALID_HANDLE_VALUE;
	IO_STATUS_BLOCK IoStatusBlock = { 0 };
	HANDLE SectionHandle = INVALID_HANDLE_VALUE;

	RtlInitUnicodeString(&FileName, L"\\??\\C:\\Windows\\System32\\ntdll.dll");

	InitializeObjectAttributes(&ObjectAttributes, &FileName, OBJ_CASE_INSENSITIVE, NULL, NULL);

	if (!NT_SUCCESS(NtOpenFile(&FileHandle, GENERIC_READ, &ObjectAttributes, &IoStatusBlock, FILE_SHARE_READ, 0)))
	{
		printf("NtOpenFile failed\n");
		getchar();
		exit(0);
	}

	if (!NT_SUCCESS(NtCreateSection(&SectionHandle, SECTION_MAP_READ, NULL, NULL, PAGE_READONLY, SEC_IMAGE, FileHandle)))
	{
		printf("NtCreateSection failed\n");
		getchar();
		exit(0);
	}

	PVOID BaseAddress = NULL;
	SIZE_T ViewSize = 0;
	NTSTATUS ntStatus = NtMapViewOfSection(SectionHandle, GetCurrentProcess(), &BaseAddress, 0, 0, NULL, &ViewSize, ViewUnmap, 0, PAGE_READONLY);

	if (!NT_SUCCESS(ntStatus) && ntStatus != STATUS_IMAGE_NOT_AT_BASE)
	{
		printf("NtMapViewOfSection failed\n");
		getchar();
		exit(0);
	}

	printf("remap ntdll done  %p %08x\n", BaseAddress, ViewSize);

	getchar();

	return 0;
}

