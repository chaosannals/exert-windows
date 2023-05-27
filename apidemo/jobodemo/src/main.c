#include <Windows.h>
#include <strsafe.h>

int main() {
	DWORD errcode = 0;
	TCHAR output[1024];
	DWORD code = 0;

	// visual studio 的 bug. 截获 OutputDebugString 的输出打印时，如果没有换行就没有显示。。
	OutputDebugString(TEXT("START \r\n"));

	HANDLE job = CreateJobObject(NULL, NULL);
	if (job == NULL) {
		code = -1;
		goto defer;
	}
	// TODO 限制内存申请
	JOBOBJECT_EXTENDED_LIMIT_INFORMATION limitInfo = { 0 };
	limitInfo.BasicLimitInformation.LimitFlags = 
		JOB_OBJECT_LIMIT_BREAKAWAY_OK |
		JOB_OBJECT_LIMIT_SILENT_BREAKAWAY_OK |
		JOB_OBJECT_LIMIT_DIE_ON_UNHANDLED_EXCEPTION |
		JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
	limitInfo.JobMemoryLimit = 100; // 无效。
	BOOL ret = SetInformationJobObject(job, JobObjectExtendedLimitInformation, &limitInfo, sizeof(limitInfo));

	if (!ret) {
		code = -2;
		goto defer;
	}


	STARTUPINFO startupInfo = { 0 };
	startupInfo.cb = sizeof(startupInfo);
	PROCESS_INFORMATION processInfo = { 0 };
	ZeroMemory(&processInfo, sizeof(processInfo));
	if (!CreateProcess(TEXT("job1demo.exe"), NULL, NULL, NULL, FALSE, 0, NULL, NULL, &startupInfo, &processInfo)) {
		code = -3;
		goto defer;
	}

	if (!AssignProcessToJobObject(job, processInfo.hProcess)) {
		code = -4;
		goto defer;
	}

	WaitForSingleObject(processInfo.hProcess, 10000);

	goto end;

defer:
	errcode = GetLastError();
	StringCchPrintf(output, 1024, L"error: %d \r\n", errcode);
	OutputDebugString(output);
end: 
	if (job != NULL) {
		CloseHandle(job);
	}
	if (processInfo.hProcess != NULL) {
		CloseHandle(processInfo.hProcess);
	}
	if (processInfo.hThread != NULL) {
		CloseHandle(processInfo.hThread);
	}
	return code;
}