#include <Windows.h>
#include <detours.h>

#define HIJ_API extern "C" __declspec(dllexport)

BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved) {
    TCHAR systemDir[MAX_PATH] = { 0 };
    switch (reason) {
    case DLL_PROCESS_ATTACH:
        MessageBox(NULL, TEXT("I'm winmm.dll"), "Info", MB_OK);
        break;
    }

    return TRUE;
}

HIJ_API DWORD timeGetTime() {
    return 0;
}
