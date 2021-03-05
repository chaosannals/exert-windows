#include <iostream>
#include <Windows.h>
#include <Psapi.h>
#include <TlHelp32.h>

void bypsapi() {
    DWORD size;
    DWORD ids[1000];
    TCHAR filename[MAX_PATH];
    if (EnumProcesses(ids, sizeof(ids), &size)) {
        std::cout << "size:\n" << size << std::endl;
        for (DWORD i = 0; i != size; ++i) {
            HANDLE h = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, ids[i]);
            if (h == nullptr) {
                std::cout << "IHV: " << GetLastError() << std::endl;
            }
            if (GetProcessImageFileName(h, filename, MAX_PATH)) {
                std::wcout << filename << std::endl;
            }
            else {
                std::cout << "error: " << GetLastError() << std::endl;
            }
            CloseHandle(h);
        }
    }
    else {
        std::cout << "no" << std::endl;
    }
}