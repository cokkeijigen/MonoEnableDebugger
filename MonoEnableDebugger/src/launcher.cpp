#include <Windows.h>
#include <detours.h>
#include <iostream>
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"wmainCRTStartup\"")

auto wmain(int argc, const wchar_t* argv[]) -> int
{
	if (argc > 1) 
	{
		STARTUPINFOW si{};
		PROCESS_INFORMATION pi{};
		auto created_successfully
		{
			::DetourCreateProcessWithDllW
			(
				{ argv[1] },
				{ NULL  },
				{ NULL  },
				{ NULL  },
				{ FALSE },
				{ CREATE_SUSPENDED },
				{ NULL },
				{ NULL },
				{ &si  },
				{ &pi  },
				{ PROJECT_NAME".dll"},
				{ NULL }
			)
		};
		if (created_successfully)
		{
			::ResumeThread(pi.hThread);
			::CloseHandle(pi.hThread);
			::CloseHandle(pi.hProcess);
		}
	}
	return {};
}