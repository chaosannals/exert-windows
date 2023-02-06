#include <strsafe.h>
#include "msys2.h"

HANDLE pipeIn = NULL;
HANDLE pipeOut = NULL;
PROCESS_INFORMATION process = { 0 };
WCHAR Msys2Command[1024];
WCHAR WorkDir[1024];

BOOL InitMsys2()
{
	return TRUE;
}

BOOL ClearMsys2()
{
	return TRUE;
}

BOOL StartMsys2()
{
    SECURITY_ATTRIBUTES sa = { 0 };
	sa.nLength = sizeof(sa);
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = NULL;

	if (!CreatePipe(&pipeIn, &pipeOut, &sa, 0))
	{
		return ClearMsys2();
	}
	OutputDebugStringW(L"msys2 create pipe.");

	STARTUPINFOW si = { 0 };
	si.cb = sizeof(si);
	si.hStdInput = pipeIn;
	si.hStdOutput = pipeOut;
	si.wShowWindow = SW_HIDE;
	si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;

	if (!CreateProcessW(NULL, Msys2Command, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, WorkDir, &si, &process))
	{
		WCHAR emsg[400];
		StringCchPrintfW(emsg, 40, L"create process error code: %d", GetLastError());
		OutputDebugStringW(emsg);
		return ClearMsys2();
	}

	OutputDebugStringW(L"msys2 create process.");
	return TRUE;
}