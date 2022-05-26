#pragma once
#include <Windows.h>
#include <string>
#include "ijshare.h"
#include "imgui.h"
#include "rinject.h"

namespace InjectGui {

    class InjectBox {
        const DWORD P_P =
            PROCESS_VM_READ
            | PROCESS_VM_WRITE
            | PROCESS_CREATE_THREAD
            | PROCESS_QUERY_INFORMATION
            | PROCESS_SUSPEND_RESUME
            | PROCESS_VM_OPERATION
            ;

        int pid = 0;
        DWORD err = 0;
        int step = 0;
        DWORD64 scp = 0;
        DWORD64 scp2 = 0;
        DWORD64 scp3 = 0;

        void InjectTo() {
            err = 0;
            step = 0;
            HANDLE p = nullptr;
            HANDLE th = nullptr;
            __try {
                p = OpenProcess(P_P, false, pid);
                if (p == nullptr) {
                    step = 1;
                    __leave;
                }

                /*if (!OpenProcessToken(p, TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES, &th)) {
                    step = 2;
                    __leave;
                }
                TOKEN_PRIVILEGES tp;
                if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid)) {
                    step = 3;
                    __leave;
                }

                tp.PrivilegeCount = 1;
                tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
                if (!AdjustTokenPrivileges(th, FALSE, &tp, sizeof(tp), NULL, NULL)) {
                    step = 4;
                    __leave;
                }
                int e = GetLastError();
                if (e != ERROR_SUCCESS && e != ERROR_NOT_ALL_ASSIGNED) {
                    step = 5;
                    __leave;
                }*/

#if _DEBUG
                char tail[] = "\\..\\x64\\Debug\\ijdll.dll";
#else
                char tail[] = "\\ijdll.dll";
#endif // _DEBUG

                int s = GetCurrentDirectoryA(0, nullptr) + 1 + strlen(tail);
                auto buf = (LPSTR)malloc(s);
                int s1 = GetCurrentDirectoryA(s, buf);
                strcpy_s(buf + s1, s - s1, tail);

                LPVOID vp = VirtualAllocEx(p, NULL, s, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
                
                if (vp == nullptr) {
                    step = 6;
                    __leave;
                }

                WriteProcessMemory(p, (LPVOID)vp, buf, strlen(buf) + 1, NULL);

                // 获取自身进程中 LoadLibraryA 的内存地址.  (非文档化, 只是恰好 kernel32 总是被优先加载, 内存地址在每个进程中是一样的, 每次重启系统后会变 )


                auto LoadLibraryPtr = (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
                auto h = CreateRemoteThread(p, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibraryPtr, (LPVOID)vp, NULL, NULL);
                WaitForSingleObject(h, INFINITE);
                DWORD threadExitCode;
                GetExitCodeThread(h, &threadExitCode);
                CloseHandle(h);

                VirtualFreeEx(p, vp, 0, MEM_FREE);
            }
            __finally {
                if (step != 0) {
                    err = GetLastError();
                }
                if (th != nullptr) {
                    CloseHandle(th);
                }
                if (p != nullptr) {
                    CloseHandle(p);
                }
            }
        }

        void UnjectTo() {
            auto d = ijshare_open("ijdll");
            step = 0;
            err = 0;
            HANDLE p = nullptr;
            __try {
                if (d) {
                    p = OpenProcess(P_P, false, pid);
                    if (p == nullptr) {
                        step = 1;
                        __leave;
                    }
                    auto FreeLibraryPtr = (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "FreeLibrary");
                    auto h = CreateRemoteThread(p, NULL, NULL, (LPTHREAD_START_ROUTINE)FreeLibraryPtr, (LPVOID)d->ijdll, NULL, NULL);
                    WaitForSingleObject(h, INFINITE);
                    DWORD threadExitCode;
                    GetExitCodeThread(h, &threadExitCode);
                }
            }
            __finally {
                if (step != 0) {
                    err = GetLastError();
                }
                if (p != nullptr) {
                    CloseHandle(p);
                }
                if (d != nullptr) {
                    delete d;
                }
            }
        }

        void RInjectTo() {
#if _DEBUG
            TCHAR tail[] = TEXT("\\..\\x64\\Debug\\rijdll.dll");
#else
            TCHAR tail[] = TEXT("\\..\\x64\\Release\\rijdll.dll");
#endif // _DEBUG

            int s = GetCurrentDirectory(0, nullptr) + 1 + lstrlen(tail);
            auto buf = (LPTSTR)malloc(s);
            int s1 = GetCurrentDirectory(s, buf);
            lstrcpy(buf + s1, tail);
            CManualMapInject ri;
            ri.InjectorDLL(buf, pid);
        }

        void ROutShellCode() {
            void* p = *reinterpret_cast<void**>(&Shellcode);
            scp = (DWORD64) *Shellcode;
            scp2 = (DWORD64) &Shellcode;
            //scp3 = (DWORD64)*reinterpret_cast<void**>(&Shellcode);
            scp3 = *reinterpret_cast<char*>(Shellcode);

            FILE* f = NULL;
            fopen_s(&f, "sss.bin", "wb");
            if (f) {
                fwrite(p, 0x1000, 1, f);
                fclose(f);
            }

            HANDLE schf = CreateFile(
                TEXT("shellcode.bin"),
                GENERIC_WRITE,
                FILE_SHARE_READ,
                NULL,
                OPEN_ALWAYS,
                FILE_ATTRIBUTE_NORMAL,
                NULL
            );

            if (schf == INVALID_HANDLE_VALUE) {
                return;
            }
            DWORD scwcount = 0;
            if (WriteFile(schf, p, 0x1000, &scwcount, NULL)) {
                return;
            }
            FlushFileBuffers(schf);
            CloseHandle(schf);
        }
    public:
        void Render() {
            ImGui::Begin(u8"目标进程");

            ImGui::Text(u8"错误码：%d", err);
            ImGui::Text(u8"步骤：%d", step);
            ImGui::InputInt("pid", &pid);
            ImGui::Text(u8"Shell Code: %lld %lld %lld", scp, scp2, scp3);

            ImGui::SameLine();

            if (ImGui::Button(u8"注入")) {
                InjectTo();
            }
            if (ImGui::Button(u8"释放")) {
                UnjectTo();
            }
            if (ImGui::Button(u8"反射注入")) {
                RInjectTo();
            }
            if (ImGui::Button(u8"打出ShellCode")) {
                ROutShellCode();
            }
            ImGui::End();
        }
    };
}