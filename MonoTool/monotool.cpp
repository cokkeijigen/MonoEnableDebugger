#include <iostream>
#include <windows.h>
#include "monotool.h"

namespace mono {
	
	p_mono_set_defaults mono_set_defaults = nullptr;
	p_mono_jit_parse_options mono_jit_parse_options = nullptr;
	p_mono_debug_init mono_debug_init = nullptr;

	void initialize(const void* monomod) {
		if (monomod == nullptr) return;
		mono_set_defaults = (p_mono_set_defaults)GetProcAddress((HMODULE)monomod, "mono_set_defaults");
		mono_jit_parse_options = (p_mono_jit_parse_options)GetProcAddress((HMODULE)monomod, "mono_jit_parse_options");
		mono_debug_init = (p_mono_debug_init)GetProcAddress((HMODULE)monomod, "mono_debug_init");
	}

	bool enable_debugger() {
		if (mono_jit_parse_options && mono_debug_init) {
			mono_jit_parse_options(0x01, options);
			mono_debug_init(0x01);
			return true;
		}
		return false;
	}
}