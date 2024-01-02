#pragma once
#define MONO_CALL __cdecl
#define NEW_MONO_VER 1

namespace mono {
#if NEW_MONO_VER == 1
    static const char* options[1] = { "--debugger-agent=transport=dt_socket,server=y,address=127.0.0.1:55555,suspend=n" };
#else
    static const char* options[1] = { "--debugger-agent=transport=dt_socket,server=y,address=127.0.0.1:55555,defer=y" };
#endif
    typedef void(MONO_CALL* p_mono_set_defaults)(int verbose_level, unsigned int opts);
    typedef void(MONO_CALL* p_mono_jit_parse_options)(int argc, const char* argv[]);
    typedef void(MONO_CALL* p_mono_debug_init)(int format);
    
    extern p_mono_set_defaults      mono_set_defaults;
    extern p_mono_jit_parse_options mono_jit_parse_options;
    extern p_mono_debug_init        mono_debug_init;

    extern void initialize(const void* monomod);
    extern bool enable_debugger();
}