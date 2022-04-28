#include <Windows.h>
#include <iostream>
#include <exert/exploit/filesystem.h>

TCHAR ServiceName[] = TEXT("exert windows service");
SERVICE_STATUS ServiceStatus;
SERVICE_STATUS_HANDLE ServiceStatusHandle;

// 控制函数
void WINAPI service_control(DWORD command) {
	ext::file_putln(L"svc.log", L"service control start");
	switch (command) {
	case SERVICE_CONTROL_STOP:
	case SERVICE_CONTROL_SHUTDOWN:
		ServiceStatus.dwCurrentState = SERVICE_STOPPED;
		if (!SetServiceStatus(ServiceStatusHandle, &ServiceStatus)) {
			ext::file_putln(L"svc.log", L"close service failed");
		}
		break;
	default:
		break;
	}
}

// 服务处理
void WINAPI service_process(DWORD argc, LPWSTR* argv) {
	ext::file_putln(L"svc.log", L"service process start");
	ServiceStatus.dwServiceType = SERVICE_WIN32;
	ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_PAUSE_CONTINUE | SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
	ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
	ServiceStatus.dwWin32ExitCode = 0;
	ServiceStatus.dwCheckPoint = 0;
	ServiceStatus.dwWaitHint = 0;
	ServiceStatus.dwServiceSpecificExitCode = 0;
	ServiceStatusHandle = RegisterServiceCtrlHandler(ServiceName, &service_control);
	if (ServiceStatusHandle == 0) {
		DWORD error = GetLastError();
		ext::file_putln(L"svc.log", L"register control handler failed.");
		return;
	}
	ServiceStatus.dwCurrentState = SERVICE_RUNNING;
	if (!SetServiceStatus(ServiceStatusHandle, &ServiceStatus)) {
		ext::file_putln(L"svc.log", L"set running failed.");
	}
	while (TRUE) {
		ext::file_putln(L"svc.log", L"service loop");
		Sleep(1000);
	}
}

// 程序主入口。
int main(int argc, char* args[]) {
	ext::init_heap();
	ext::file_putln(L"svc.log", L"start main");
	SERVICE_TABLE_ENTRY ste[2];
	ste[0].lpServiceName = ServiceName;
	ste[0].lpServiceProc = service_process;
	ste[1].lpServiceName = nullptr;
	ste[1].lpServiceProc = nullptr;
	StartServiceCtrlDispatcher(ste);
	return 0;
}