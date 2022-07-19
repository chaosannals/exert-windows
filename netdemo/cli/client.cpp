#include <iostream>
#include <WinSock2.h>
using namespace std;

void main()
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(6000);
    server.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

    SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    int flag;
    flag = connect(client, (sockaddr*)&server, sizeof(server));
    if (flag < 0) {
        cout << "error!" << endl;
        return;
    }
    while (1) {
        cout << "sent hello!!!!" << endl;
        char buffer[1024];
        strcpy(buffer, "hello");
        send(client, buffer, 1024, 0);

        memset(buffer, NULL, sizeof(buffer));

        cout << "recv: " << endl;
        int rev = recv(client, buffer, 1024, 0);
        if (rev == 0)
            cout << "recv nothing!" << endl;
        cout << buffer << endl;
        Sleep(10000);
    }

    closesocket(client);
    WSACleanup();
}