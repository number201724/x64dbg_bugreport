// BugExample.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>
#include <winnt.h>


void NTAPI tls_callback(PVOID DllHandle, DWORD dwReason, PVOID)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		MessageBox(0, L"Example", L"", 0);
		TerminateProcess(GetCurrentProcess(), 0);
	}
}

#ifdef _WIN64
#pragma comment (linker, "/INCLUDE:_tls_used") 
#pragma comment (linker, "/INCLUDE:tls_callback_func")
#else
#pragma comment (linker, "/INCLUDE:__tls_used") 
#pragma comment (linker, "/INCLUDE:_tls_callback_func")
#endif


#ifdef _WIN64
#pragma const_seg(".CRT$XLF")
EXTERN_C const
#else
#pragma data_seg(".CRT$XLF")
EXTERN_C
#endif
PIMAGE_TLS_CALLBACK tls_callback_func = tls_callback;
#ifdef _WIN64
#pragma const_seg()
#else
#pragma data_seg()
#endif //_WIN64

//edit your pe file
//EntryPoint to zero
int main()
{
	return 0;
}

