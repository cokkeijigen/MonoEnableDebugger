#include <iostream>
#include <windows.h>
#include "mono_utils.hpp"

namespace mono_utils {


	struct function_loader
	{
		void* monomod;

		template<class T>
		inline auto load_from() -> void
		{
			auto mod{ reinterpret_cast<HMODULE>(monomod) };
			auto raw{ ::GetProcAddress(mod, T::name) };
			T::call = reinterpret_cast<decltype(T::func)*>(raw);
		}
	};

	auto monocall mono_jit_parse_options::func(int argc, const char* argv[]) -> size_t
	{
		if (mono_jit_parse_options::call != nullptr)
		{
			return mono_jit_parse_options::call(argc, argv);
		}
		return {};
	}

	auto monocall mono_debug_init::func(int format) -> void
	{
		if (mono_debug_init::call != nullptr)
		{
			return mono_debug_init::call(format);
		}
	}

	auto init(const void* monomod) -> void
	{
		if (monomod == nullptr) 
		{
			return;
		}
		function_loader loader{ const_cast<void*>(monomod) };
		loader.load_from<mono_jit_parse_options>();
		loader.load_from<mono_debug_init>();
	}

	auto enable_debugger() -> bool
	{
		mono_jit_parse_options::func(mono_utils::options::size, mono_utils::options::value);
		mono_debug_init::func(0x01);
		return false;
	}

}