#pragma once

#ifdef _WIN64
#define monocall __fastcall
#define MONOCALL __fastcall
#elif _WIN32
#define monocall __cdecl
#define MONOCALL __cdecl
#endif

#if not OLD_VERSION_MONO
namespace mono_utils {
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

namespace mono_utils {

	struct mono_jit_parse_options
	{
		static auto monocall func(int argc, const char* argv[]) -> size_t;
		static inline constexpr char name[]{ "mono_jit_parse_options" };
		static inline decltype(func)* call{ nullptr };
	};

	struct mono_debug_init
	{
		static auto monocall func(int format) -> void;
		static inline constexpr char name[]{ "mono_debug_init" };
		static inline decltype(func)* call{ nullptr };
	};

	extern auto init(const void* monomod) -> void;
	extern auto enable_debugger() -> bool;
}