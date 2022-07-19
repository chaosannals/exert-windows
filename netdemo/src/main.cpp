#include <WinSock2.h>
#include <Windows.h>
#include <iostream>

const int BUFFER_SIZE = 1024;
const int OPT_READ = 1;
const int OPT_WRITE = 2;
const int OPT_ACCEPT = 3;

struct PER_HANDLE_DATA
{
	SOCKET socket;
	sockaddr_in addr;
};

struct PER_IO_DATA {
	OVERLAPPED ol; // 因为是第一个字段，所以取 ol 指针其实也是 PER_IO_DATA 的地址。
	char buf[BUFFER_SIZE];
	int optype;
};

DWORD WINAPI ServerThread(LPVOID p)
{
	HANDLE h = (HANDLE)p;
	DWORD dwTrans;
	PER_HANDLE_DATA* pphd;
	PER_IO_DATA* piod;
	while (true) {
		BOOL ok = ::GetQueuedCompletionStatus(
			h,
			&dwTrans,
			(PULONG_PTR)&pphd,
			(LPOVERLAPPED*)&piod,
			WSA_INFINITE
		);
		if (!ok) {
			::closesocket(pphd->socket);
			::GlobalFree(pphd);
			::GlobalFree(piod);
			continue;
		}
		if (dwTrans == 0 && (piod->optype == OPT_READ || piod->optype == OPT_WRITE))
		{
			::closesocket(pphd->socket);
			::GlobalFree(pphd);
			::GlobalFree(piod);
			continue;
		}
		switch (piod->optype) {
		case OPT_READ:
		{
			piod->buf[dwTrans] = '\0';
			std::cout << piod->buf << std::endl;
			WSABUF buf;
			buf.buf = piod->buf;
			buf.len = BUFFER_SIZE;
			piod->optype = OPT_READ;
			DWORD nFlags = 0;
			::WSARecv(pphd->socket, &buf, 1, &dwTrans, &nFlags, &piod->ol, NULL);
			break;
		}
		case OPT_WRITE:
		case OPT_ACCEPT:
			break;
		}
	}
	return 0;
}

int main() {
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	int port = 45678;
	HANDLE h = ::CreateIoCompletionPort(
		INVALID_HANDLE_VALUE,
		0, 0, 0
	);
	::CreateThread(NULL, 0, ServerThread,  h, 0, 0);
	SOCKET socket = ::socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN si;
	si.sin_family = AF_INET;
	si.sin_port = ::ntohs(port);
	si.sin_addr.S_un.S_addr = INADDR_ANY;
	::bind(socket, reinterpret_cast<sockaddr*>(&si), sizeof(si));
	::listen(socket, 5);

	while (true) {
		SOCKADDR_IN ci;
		int cs = sizeof(ci);
		SOCKET client = ::accept(socket, reinterpret_cast<sockaddr*>(&ci), &cs);
		PER_HANDLE_DATA* pphd = reinterpret_cast<PER_HANDLE_DATA*>(::GlobalAlloc(GPTR, sizeof(PER_HANDLE_DATA)));
		pphd->socket = client;
		memcpy(&pphd->addr, &ci, cs);
		::CreateIoCompletionPort((HANDLE)pphd->socket, h, (ULONG_PTR)pphd, 0);
		PER_IO_DATA *piod = (PER_IO_DATA*)(::GlobalAlloc(GPTR, sizeof(PER_IO_DATA)));
		piod->optype = OPT_READ;
		WSABUF buf;
		buf.buf = piod->buf;
		buf.len = BUFFER_SIZE;
		DWORD dwRecv;
		DWORD dwFlags = 0;
		::WSARecv(pphd->socket, &buf, 1, &dwRecv, &dwFlags, &piod->ol, NULL);
	}
	return 0;
}