#include <iostream>
#include <windows.h>
#include "monotool.h"

namespace mono {
	
	_mono_func p_mono_set_defaults mono_set_defaults = nullptr;
	_mono_func p_mono_jit_parse_options mono_jit_parse_options = nullptr;
	_mono_func p_mono_debug_init mono_debug_init = nullptr;
	HMODULE MONO_MODULE = NULL;

	template<typename type>
	static inline type getp(const char* name) {
		return (type)GetProcAddress(MONO_MODULE, name);
	}

	void initialize(const void* monomod) {
		if (monomod == nullptr) return;
		MONO_MODULE = (HMODULE)monomod;
		mono_set_defaults	   = getp<p_mono_set_defaults>("mono_set_defaults");
		mono_jit_parse_options = getp<p_mono_jit_parse_options>("mono_jit_parse_options");
		mono_debug_init        = getp<p_mono_debug_init>("mono_debug_init");
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