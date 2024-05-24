#include "hijack.h"
#include "util.h"
#include <strsafe.h>

#define HIJ_API extern "C" __declspec(dllexport)

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

HIJ_API DWORD myTimeGetTime()
{
    return timeGetTimePtr();
}

HIJ_API MMRESULT myTimeEndPeriod(UINT period)
{
    return timeEndPeriodPtr(period);
}

HIJ_API MMRESULT myTimeBeginPeriod(UINT period)
{
    return timeBeginPeriodPtr(period);
}

HIJ_API LONG myMmioRead(HMMIO hmmio, HPSTR pch, LONG cch)
{
    return mmioReadPtr(hmmio, pch, cch);
}

HIJ_API MMRESULT myMmioClose(HMMIO hmmio, UINT fuClose)
{
    return mmioClosePtr(hmmio, fuClose);
}

HIJ_API LONG myMmioSeek(HMMIO hmmio, LONG lOffset, int iOrigin)
{
    return mmioSeekPtr(hmmio, lOffset, iOrigin);
}

HIJ_API MMRESULT myMmioDescend(
    HMMIO hmmio,
    LPMMCKINFO pmmcki,
    const MMCKINFO* pmmckiParent,
    UINT fuDescend)
{
    return mmioDescendPtr(hmmio, pmmcki, pmmckiParent, fuDescend);
}

HIJ_API MMRESULT myMmioAscend(HMMIO hmmio, LPMMCKINFO pmmcki, UINT fuAscend)
{
    return mmioAscendPtr(hmmio, pmmcki, fuAscend);
}

HIJ_API HMMIO myMmioOpenA(
    LPSTR pszFileName,
    LPMMIOINFO pmmioinfo,
    DWORD fdwOpen)
{
    Log("[winmm] mmio open: {}", pszFileName);
    return mmioOpenAPtr(pszFileName, pmmioinfo, fdwOpen);
}

VOID Hijack_Winmm() {
    TCHAR systemDir[MAX_PATH] = { 0 };
    UINT systemDirLen = GetSystemDirectory(systemDir, MAX_PATH);
    TCHAR winmmPath[MAX_PATH] = { 0 };
    size_t nameLen = 0;
    if (StringCchLength(dllName, MAX_PATH, &nameLen) != S_OK)
    {
        OutputDebugString("[winmm] winmm.dll name length unknown.");
        return;
    }
    size_t systemDirEnd = systemDirLen + 1;
    if (StringCchCopy(winmmPath, systemDirEnd, systemDir) != S_OK)
    {
        OutputDebugString("[winmm] system dir unknown.");
        return;
    }
    winmmPath[systemDirLen] = TEXT('\\');
    if (StringCchCopy(winmmPath + systemDirEnd, systemDirEnd + nameLen, dllName) != S_OK)
    {
        OutputDebugString("[winmm] winmm.dll path unknown.");
        return;
    }
    // OutputDebugString(winmmPath);
    Log("[winmm] path: {}", winmmPath);
    HMODULE winmmMod = LoadLibrary(winmmPath);
    if (NULL == winmmMod)
    {
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
