﻿// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include <Windows.h>

BOOL APIENTRY DllMain(
    HMODULE module,
    DWORD  reason,
    LPVOID reserved
) {
    switch (reason) {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}