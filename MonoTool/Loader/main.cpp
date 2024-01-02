#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#include <Windows.h>
#include "detours.h"
#define EXE_NAME L"ONE.exe"
#define DLL_NAME "MonoEnableDebugger.dll"

int main(int, char**) {
	STARTUPINFOW si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	if (DetourCreateProcessWithDllW(
		EXE_NAME, NULL, NULL, NULL, FALSE, CREATE_SUSPENDED,
		NULL, NULL, &si, &pi, DLL_NAME, NULL)) {
		ResumeThread(pi.hThread);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}
	return 0;
}