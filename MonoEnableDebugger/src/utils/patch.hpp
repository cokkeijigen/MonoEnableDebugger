#pragma once
#include <detours.h>

namespace Patch {

	struct Hooker
	{
		template<auto Fun>
		inline static decltype(Fun) Call;

		inline static auto Begin() -> void
		{
			::DetourTransactionBegin();
		}

		template<auto Fun>
		inline static auto Add(decltype(Fun) target) -> void
		{
            ::DetourAttach({ &(Hooker::Call<Fun> = {target}) }, Fun);
		}

		template<auto Fun>
		inline static auto Add(const void* target) -> void
		{
			Hooker::Add<Fun>(reinterpret_cast<decltype(Fun)>(target));
		}

		inline static auto Commit() -> void
		{
			::DetourUpdateThread(::GetCurrentThread());
			::DetourTransactionCommit();
		}
	};
}

#ifdef _DEBUG
#define DEBUG_ONLY(...) __VA_ARGS__
#else
#define DEBUG_ONLY(...)
#endif
