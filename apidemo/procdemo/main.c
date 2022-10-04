#include <Windows.h>
#include <strsafe.h>
#include <stdarg.h>

#define EchoM(format, ...) do { WCHAR output[1024]; StringCchPrintfW(output, 1024, format,##__VA_ARGS__); OutputDebugStringW(output); } while(0);

void Echo(STRSAFE_LPWSTR format, ...) {
	//WCHAR output[1024];
	size_t bsz = 4098;
	PWSTR output = (PWSTR)malloc(bsz * sizeof(WCHAR));
	if (output != NULL) {
		va_list args;
		va_start(args, format);
		StringCchVPrintfW(output, bsz, format, args);
		va_end(args);
		OutputDebugStringW(output);
		free(output);
	}
}

int wmain() {
	HMODULE m = GetModuleHandleW(NULL);

	HMODULE m2;
	GetModuleHandleExW(
		GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
		NULL,
		&m2
	);
	Echo(L"module handle: %p  %p \n", m, m2);


	// args
	INT argc = 0;
	INT i = 0;
	PWSTR *args = CommandLineToArgvW(GetCommandLineW(), &argc);
	for (i = 0; i < argc; ++i) {
		Echo(L"arg %i / %i: [ %s ]\n", i, argc, args[i]);
	}
	HeapFree(GetProcessHeap(), 0, args);


	// env
	/* 进程环境块类似下面。=开头的都不是有效环境变量
	=::=::\
	vname=vvalue\0
	vname2=vvalue2\0
	\0
	*/
	PWSTR envs = GetEnvironmentStringsW();
	WCHAR name[MAX_PATH];
	WCHAR value[MAX_PATH];
	PWSTR p = envs;
	PCWSTR pc = NULL;
	HRESULT hr = S_OK;
	int current = 0;

	while (p != NULL) {
		if (*p != L'=') {
			pc = wcschr(p, L'=');
			pc++;
			size_t nl = (size_t)pc - (size_t)p - sizeof(WCHAR);
			hr = StringCbCopyNW(name, MAX_PATH, p, nl);
			if (FAILED(hr)) {
				break;
			}
			hr = StringCchCopyNW(value, MAX_PATH, pc, wcslen(pc) + 1);
			if (SUCCEEDED(hr)) {
				Echo(L"%u %p %s=%s \n", current, p, name, value);
			}
			else if (hr == STRSAFE_E_INSUFFICIENT_BUFFER) {
				Echo(L"%u %p %s=%s...\n", current, p, name, value);
			}
			else {
				Echo(L"%u %p %s=???\n", current, p, name);
			}
		}
		else {
			Echo(L"%u %s ???\n", current, p);
		}
		current++;

		while (*p != L'\0') {
			p++;
		}
		p++;
		if (*p == L'\0') {
			break;
		}
	}
	FreeEnvironmentStringsW(envs);

	PCWSTR pname = L"Path";
	PWSTR pvalue = NULL;
	DWORD r = GetEnvironmentVariableW(pname, pvalue, 0);
	if (r != 0) {
		DWORD sz = r * sizeof(WCHAR);
		pvalue = (PWSTR)malloc(sz);
		GetEnvironmentVariableW(pname, pvalue, sz);
		Echo(L"Path: %s ||||| \n", pvalue);
		free(pvalue);
	}
	else {
		Echo(L"Path unknown \n");
	}

	PCWSTR cht = L"PATH=%PATH%";
	DWORD chv = ExpandEnvironmentStringsW(cht, NULL, 0);
	PWSTR pbuf = (PWSTR)malloc(chv * sizeof(WCHAR));
	chv = ExpandEnvironmentStringsW(cht, pbuf, chv);
	Echo(L"expand: %s \n", pbuf);
	free(pbuf);



	// 错误模式
	SetErrorMode(SEM_FAILCRITICALERRORS);
}