#include <Windows.h>
#include <strsafe.h>

int main() {
	// Global\kobjectname 全局
	// Local\kobjectname 显式局部
	// Session\<sid>\kobjectname 会话名字空间
	WCHAR samename[] = L"kobjectname";
	OutputDebugStringW(samename);
	HANDLE hm = CreateMutexW(NULL, FALSE, samename);
	DWORD hec = GetLastError();
	HANDLE hm2 = CreateMutexW(NULL, FALSE, samename);
	DWORD hec2 = GetLastError();
	if (hec2 == ERROR_ALREADY_EXISTS) {
		// 通过获取的错误提示，可以确认 hm2 就是同 hm 相同内核对象的不同句柄。
	}
	HANDLE hsem = CreateSemaphoreW(NULL, 1, 1, samename);
	DWORD ec = GetLastError();
	if (ec == ERROR_INVALID_HANDLE) {
		// 因为重名但不同类型而报该错误。
		// Open* 开头的函数打开同名但不同类型的时候也是报该错误。
	}
	WCHAR output[1024];
	// 内核对象不可重名，同类型是获得同个内核对象的不同句柄，不同类型直接失败， hsem 因为重名失败了。
	StringCchPrintfW(output, 1024, L"hm: %p ; hm2: %p ;  hs: %p ;  hec: %d ; hec2: %d ; ec: %d", hm, hm2, hsem, hec, hec2, ec);
	OutputDebugStringW(output);
	CloseHandle(hm);
	CloseHandle(hm2);

	DWORD processId = GetCurrentProcessId();
	DWORD sessionId;
	if (ProcessIdToSessionId(processId, &sessionId)) {
		StringCchPrintfW(output, 1024, L"pid: %d ; sid: %d", processId, sessionId);
	}
	else {
		StringCchPrintfW(output, 1024, L"pid: %d ; sid: unknown", processId);
	}
	OutputDebugStringW(output);


	//
	HANDLE fmap = CreateFileMappingW(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0, 10240,
		NULL
	);
	HANDLE fmapro;
	DuplicateHandle(
		GetCurrentProcess(),
		fmap,
		GetCurrentProcess(),
		&fmapro,
		FILE_MAP_READ,
		FALSE,
		0
	);

	// fmapro 只读。

	CloseHandle(fmapro);

	CloseHandle(fmap);
}