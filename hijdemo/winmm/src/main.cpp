#include <Windows.h>
#include <strsafe.h>
#include <detours.h>

#define HIJ_API extern "C" __declspec(dllexport)

typedef DWORD (*FnPtrTimeGetTime)();
typedef MMRESULT (*FnPtrTimeEndPeriod)(UINT);
typedef MMRESULT (*FnPtrTimeBeginPeriod)(UINT);
typedef LONG (*FnPtrMmioRead)(HMMIO, HPSTR, LONG);
typedef MMRESULT (*FnPtrMmioClose)(HMMIO, UINT);
typedef LONG (*FnPtrMmioSeek)(HMMIO, LONG,int);
typedef MMRESULT (*FnPtrMmioDescend)(HMMIO,LPMMCKINFO,const MMCKINFO*,UINT);
typedef MMRESULT (*FnPtrMmioAscend)(HMMIO, LPMMCKINFO, UINT);
typedef HMMIO (*FnPtrMmioOpenA)(
    LPSTR      pszFileName,
    LPMMIOINFO pmmioinfo,
    DWORD      fdwOpen
);

const TCHAR dllName[] = TEXT("winmm.dll");

FnPtrTimeGetTime timeGetTimePtr;
FnPtrTimeEndPeriod timeEndPeriodPtr;
FnPtrTimeBeginPeriod timeBeginPeriodPtr;
FnPtrMmioRead mmioReadPtr;
FnPtrMmioClose mmioClosePtr;
FnPtrMmioSeek mmioSeekPtr;
FnPtrMmioDescend mmioDescendPtr;
FnPtrMmioAscend mmioAscendPtr;
FnPtrMmioOpenA mmioOpenAPtr;

VOID AttachHij() {
    TCHAR systemDir[MAX_PATH] = { 0 };
    UINT systemDirLen = GetSystemDirectory(systemDir, MAX_PATH);
    TCHAR winmmPath[MAX_PATH] = { 0 };
    size_t nameLen = 0;
    if (StringCchLength(dllName, MAX_PATH, &nameLen) != S_OK) {
        OutputDebugString("[winmm] winmm.dll name length unknown.");
        return;
    }
    size_t systemDirEnd = systemDirLen + 1;
    if (StringCchCopy(winmmPath, systemDirEnd, systemDir) != S_OK) {
        OutputDebugString("[winmm] system dir unknown.");
        return;
    }
    winmmPath[systemDirLen] = TEXT('\\');
    if (StringCchCopy(winmmPath + systemDirEnd, systemDirEnd + nameLen, dllName) != S_OK) {
        OutputDebugString("[winmm] winmm.dll path unknown.");
        return;
    }
    OutputDebugString(winmmPath);
    HMODULE winmmMod = LoadLibrary(winmmPath);
    if (NULL == winmmMod) {
        OutputDebugString("[winmm] winmm.dll load failed.");
        return;
    }
    timeGetTimePtr = (FnPtrTimeGetTime)GetProcAddress(winmmMod, "timeGetTime");
    timeEndPeriodPtr = (FnPtrTimeEndPeriod)GetProcAddress(winmmMod, "timeEndPeriod");
    timeBeginPeriodPtr = (FnPtrTimeBeginPeriod)GetProcAddress(winmmMod, "timeBeginPeriod");
    mmioReadPtr = (FnPtrMmioRead)GetProcAddress(winmmMod, "mmioRead");
    mmioClosePtr = (FnPtrMmioClose)GetProcAddress(winmmMod, "mmioClose");
    mmioSeekPtr = (FnPtrMmioSeek)GetProcAddress(winmmMod, "mmioSeek");
    mmioDescendPtr = (FnPtrMmioDescend)GetProcAddress(winmmMod, "mmioDescend");
    mmioAscendPtr = (FnPtrMmioAscend)GetProcAddress(winmmMod, "mmioAscend");
    mmioOpenAPtr = (FnPtrMmioOpenA)GetProcAddress(winmmMod, "mmioOpenA");
}

VOID DetachHij() {

}

BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved) {
    switch (reason) {
    case DLL_PROCESS_ATTACH:
        // MessageBox(NULL, TEXT("I'm winmm.dll, attach."), "Info", MB_OK);
        OutputDebugString("[winmm] attach.");
        AttachHij();
        break;
    case DLL_PROCESS_DETACH:
        //MessageBox(NULL, TEXT("I'm winmm.dll, detach."), "Info", MB_OK);
        OutputDebugString("[winmm] detach.");
        DetachHij();
        break;
    }

    return TRUE;
}

HIJ_API DWORD timeGetTime() {
    return timeGetTimePtr();
}

HIJ_API MMRESULT timeEndPeriod(UINT period) {
    return timeEndPeriodPtr(period);
}

HIJ_API MMRESULT timeBeginPeriod(UINT period) {
    return timeBeginPeriodPtr(period);
}

HIJ_API LONG mmioRead(HMMIO hmmio, HPSTR pch, LONG  cch) {
    return mmioReadPtr(hmmio, pch, cch);
}

HIJ_API MMRESULT mmioClose(HMMIO hmmio, UINT  fuClose) {
    return mmioClosePtr(hmmio, fuClose);
}

HIJ_API LONG mmioSeek(HMMIO hmmio, LONG  lOffset, int iOrigin) {
    return mmioSeekPtr(hmmio, lOffset, iOrigin);
}

HIJ_API MMRESULT mmioDescend(
    HMMIO          hmmio,
    LPMMCKINFO     pmmcki,
    const MMCKINFO* pmmckiParent,
    UINT           fuDescend
) {
    return mmioDescendPtr(hmmio, pmmcki, pmmckiParent, fuDescend);
}

HIJ_API MMRESULT mmioAscend(HMMIO hmmio,LPMMCKINFO pmmcki,UINT fuAscend) {
    return mmioAscendPtr(hmmio, pmmcki, fuAscend);
}

HIJ_API HMMIO mmioOpenA(
    LPSTR      pszFileName,
    LPMMIOINFO pmmioinfo,
    DWORD      fdwOpen
) {
    return mmioOpenAPtr(pszFileName, pmmioinfo, fdwOpen);
}
