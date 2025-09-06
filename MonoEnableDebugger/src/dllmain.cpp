#include <iostream>
#include <windows.h>
#include <mono_utils.hpp>
#include <patch.hpp>

extern "C" 
{
	namespace Hooks
	{
		static auto WINAPI LoadLibraryExW(LPCWSTR lpLibFileName, HANDLE hFile, DWORD dwFlags) -> HMODULE
		{
			auto hMod{ Patch::Hooker::Call<Hooks::LoadLibraryExW>(lpLibFileName, hFile, dwFlags) };
			if (hMod != nullptr)
			{
				std::wstring_view name{ lpLibFileName };
				if (name.find(L"mono") != std::wstring_view::npos)
				{
					mono_utils::init(reinterpret_cast<void*>(hMod));
					mono_utils::enable_debugger();
				}
			}
			return { hMod };
		};

		static auto WINAPI LoadLibraryA(LPCSTR lpLibFileName) -> HMODULE
		{
			auto hMod{ Patch::Hooker::Call<Hooks::LoadLibraryA>(lpLibFileName) };
			if (hMod != nullptr)
			{
				std::string_view name{ lpLibFileName };
				if (name.find("mono") != std::wstring_view::npos)
				{
					mono_utils::init(reinterpret_cast<void*>(hMod));
					mono_utils::enable_debugger();
				}
			}
			return { hMod };
		}


		static auto WINAPI LoadLibraryW(LPCWSTR lpLibFileName) -> HMODULE
		{
			auto hMod{ Patch::Hooker::Call<Hooks::LoadLibraryW>(lpLibFileName) };
			if (hMod != nullptr)
			{
				std::wstring_view name{ lpLibFileName };
				if (name.find(L"mono") != std::wstring_view::npos)
				{
					mono_utils::init(reinterpret_cast<void*>(hMod));
					mono_utils::enable_debugger();
				}
			}
			return { hMod };
		}
	}

	__declspec(dllexport) auto _patch_by_iTsukezigen_(void) -> void
	{
	}

	auto APIENTRY DllMain(HMODULE, DWORD  ul_reason_for_call, LPVOID) -> BOOL
	{
		if (ul_reason_for_call == DLL_PROCESS_ATTACH)
		{
			Patch::Hooker::Begin();
			Patch::Hooker::Add<Hooks::LoadLibraryExW>(::LoadLibraryExW);
			Patch::Hooker::Add<Hooks::LoadLibraryA>(::LoadLibraryA);
			Patch::Hooker::Add<Hooks::LoadLibraryW>(::LoadLibraryW);
			Patch::Hooker::Commit();
		}
		return TRUE;
	}
}