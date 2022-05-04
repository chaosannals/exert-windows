// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include "../imject/ijshare.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    static HANDLE h = nullptr;
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        ijshare_t data;
        data.ijdll = hModule;
        h = ijshare_new("ijdll", &data);
        MessageBoxA(NULL, "aaaaa", "sdf", MB_OK);
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        if (h) {
            CloseHandle(h);
        }
        MessageBoxA(NULL, "bbbbb", "sdf", MB_OK);
        break;
    }
    return TRUE;
}

