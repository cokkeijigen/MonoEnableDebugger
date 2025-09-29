#include <iostream>
#include <windows.h>
#include "mono_utils.hpp"

namespace mono 
{


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

	
	auto utils::init(const void* monomod) -> void
	{
		if (monomod == nullptr) 
		{
			return;
		}
		function_loader loader{ const_cast<void*>(monomod) };
		loader.load_from<mono_jit_parse_options>();
		loader.load_from<mono_debug_init>();
	}

	auto utils::enable_debugger() -> bool
	{
		mono::trycall<mono_jit_parse_options>(mono::options::size, mono::options::value);
		mono::trycall<mono_debug_init>(0x01);
		return true;
	}

}