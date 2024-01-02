#pragma once
#include "../detours.h"
#ifdef _WIN32
#pragma comment(lib, "..\\detours_x86.lib")
#else
#pragma comment(lib, "..\\detours_x64.lib")
#endif