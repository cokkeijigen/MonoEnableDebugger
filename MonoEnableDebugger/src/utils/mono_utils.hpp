#pragma once

#ifdef _WIN64
#define monocall __fastcall
#define MONOCALL __fastcall
#elif _WIN32
#define monocall __cdecl
#define MONOCALL __cdecl
#endif

#include <type_traits>
#include <optional>

#if not OLD_VERSION_MONO
namespace mono {
	struct options
	{
		inline static const char* value[]{ "--debugger-agent=transport=dt_socket,server=y,address=127.0.0.1:56000,suspend=y" };
		inline static constexpr auto size{ sizeof(value) / sizeof(char*) };
	};
}
#else
namespace mono_utils {
	struct options
	{
		inline static const char* value[]{ "--debugger-agent=transport=dt_socket,server=y,address=127.0.0.1:56000,defer=y" };
		inline static constexpr auto size{ sizeof(value) / sizeof(char*) };
	};
}
#endif

namespace mono 
{
	struct mono_jit_parse_options
	{
		static inline constexpr char name[]{ "mono_jit_parse_options" };
		static auto monocall func(int argc, const char* argv[]) -> size_t {};
		static inline decltype(func)* call{ nullptr };
	};

	struct mono_debug_init
	{
		static inline constexpr char name[]{ "mono_debug_init" };
		static auto monocall func(int format) -> void {};
		static inline decltype(func)* call{ nullptr };
	};

	template<class T, class ...V, class R = std::invoke_result_t<decltype(T::call), V...>>
	inline auto trycall(V&&... args) -> std::conditional_t<std::is_void_v<R>, R, std::optional<R>>
	{

		if constexpr (std::is_void_v<R>)
		{
			if (T::call != nullptr)
			{
				T::call(args...);
			}
		}
		else
		{
			if (T::call != nullptr)
			{
				return T::call(args...);
			}
			return std::nullopt;
		}
	}

	namespace utils
	{
		extern auto init(const void* monomod) -> void;
		extern auto enable_debugger() -> bool;
	}
}

