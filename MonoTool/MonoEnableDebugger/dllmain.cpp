#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <filesystem>
#include <windows.h>
#include "detours.h"
#include "../monotool.h"

namespace Hook {

    namespace ApiTy {
        typedef HMODULE(WINAPI* LoadLibraryExW)(LPCWSTR lpLibFileName, HANDLE hFile, DWORD dwFlags);
        typedef HMODULE(WINAPI* LoadLibraryW)(LPCWSTR lpLibFileName);
        typedef HMODULE(WINAPI* LoadLibraryA)( LPCSTR lpLibFileName);
    }

    static void TryEnableDebugger(const std::filesystem::path dll, const void* hMod) {
        std::wstring name = dll.filename().wstring();
        if (name.find(L"mono") != std::wstring::npos) {
            mono::initialize(hMod);
            mono::enable_debugger();
        }
    }

    ApiTy::LoadLibraryExW OldLoadLibraryExW;
    static HMODULE WINAPI NewLoadLibraryExW(LPCWSTR lpLibFileName, HANDLE hFile, DWORD dwFlags) {
        HMODULE hMod = OldLoadLibraryExW(lpLibFileName, hFile, dwFlags);
        TryEnableDebugger(lpLibFileName, hMod);
        return hMod;
    };

    ApiTy::LoadLibraryA OldLoadLibraryA;
    static HMODULE WINAPI NewLoadLibraryA(LPCSTR lpLibFileName) {
        HMODULE hMod = OldLoadLibraryA(lpLibFileName);
        TryEnableDebugger(lpLibFileName, hMod);
        return hMod;
    }

    ApiTy::LoadLibraryW OldLoadLibraryW;
    static HMODULE WINAPI NewLoadLibraryW(LPCWSTR lpLibFileName) {
        HMODULE hMod = OldLoadLibraryW(lpLibFileName);
        TryEnableDebugger(lpLibFileName, hMod);
        return hMod;
    }

    static void Init() {
        if (HMODULE KERNEL32_DLL = GetModuleHandleW(L"kernel32.dll")) {
            OldLoadLibraryExW = (ApiTy::LoadLibraryExW)GetProcAddress(KERNEL32_DLL, "LoadLibraryExW");
            OldLoadLibraryA = (ApiTy::LoadLibraryA)GetProcAddress(KERNEL32_DLL, "LoadLibraryA");
            OldLoadLibraryW = (ApiTy::LoadLibraryW)GetProcAddress(KERNEL32_DLL, "LoadLibraryW");
        }
    }

    static void Start() {
        DetourTransactionBegin();
        if (OldLoadLibraryExW != NULL) {
            DetourAttach((void**)&OldLoadLibraryExW, NewLoadLibraryExW);
        }
        if (OldLoadLibraryA != NULL) {
            DetourAttach((void**)&OldLoadLibraryA, NewLoadLibraryA);
        }
        if (OldLoadLibraryW != NULL) {
            DetourAttach((void**)&OldLoadLibraryW, NewLoadLibraryW);
        }
        DetourUpdateThread(GetCurrentThread());
        DetourTransactionCommit();
    }
}

extern "C" __declspec(dllexport) void hook(void) { }
BOOL APIENTRY DllMain(HMODULE, DWORD  ul_reason_for_call, LPVOID) {
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    #ifdef _DEBUG
        AllocConsole();
        (void)freopen("CONOUT$", "w", stdout);
        (void)freopen("CONIN$", "r", stdin);
    #endif 
        Hook::Init();
        Hook::Start();
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}