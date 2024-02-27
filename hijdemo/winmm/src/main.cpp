#include <Windows.h>
#include <strsafe.h>
#include <detours.h>
#include <format>

#define HIJ_API extern "C" __declspec(dllexport)
#ifdef UNICODE
typedef std::wstring TSTRING;
#else
typedef std::string TSTRING;
#endif

typedef DWORD (*FnPtrTimeGetTime)();
typedef MMRESULT (*FnPtrTimeEndPeriod)(UINT);
typedef MMRESULT (*FnPtrTimeBeginPeriod)(UINT);
typedef LONG (*FnPtrMmioRead)(HMMIO, HPSTR, LONG);
typedef MMRESULT (*FnPtrMmioClose)(HMMIO, UINT);
typedef LONG (*FnPtrMmioSeek)(HMMIO, LONG, int);
typedef MMRESULT (*FnPtrMmioDescend)(HMMIO, LPMMCKINFO, const MMCKINFO *, UINT);
typedef MMRESULT (*FnPtrMmioAscend)(HMMIO, LPMMCKINFO, UINT);
typedef HMMIO (*FnPtrMmioOpenA)(
    LPSTR pszFileName,
    LPMMIOINFO pmmioinfo,
    DWORD fdwOpen);

typedef HWND (*FnPtrCreateWindowExA)(
    DWORD dwExStyle,
    LPCSTR lpClassName,
    LPCSTR lpWindowName,
    DWORD dwStyle,
    int X,
    int Y,
    int nWidth,
    int nHeight,
    HWND hWndParent,
    HMENU hMenu,
    HINSTANCE hInstance,
    LPVOID lpParam);
typedef ATOM (*FnPtrRegisterClassA)(
    const WNDCLASSA* lpWndClass
);
typedef ATOM(*FnPtrRegisterClassExA)(
    const WNDCLASSEXA* unnamedParam1
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

FnPtrCreateWindowExA createWindowExAPtr;
FnPtrRegisterClassA registerClassAPtr;
FnPtrRegisterClassExA registerClassExAPtr;
WNDPROC wndProcPtr;

template <typename... Args>
VOID Log(const TSTRING &format, const Args &...args)
{
    auto content = std::vformat(format, std::make_format_args(args...));
    OutputDebugString(content.c_str());
}

LRESULT WINAPI MyWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    return wndProcPtr(hWnd, msg, wParam, lParam);
}

ATOM MyRegisterClassA(
    const WNDCLASSA* lpWndClass
) {
#ifdef HIJ_TARGET_WND_CLASS
    std::string targetClass = HIJ_TARGET_WND_CLASS;
    if (targetClass.compare(lpWndClass->lpszClassName) == 0)
    {
        wndProcPtr = lpWndClass->lpfnWndProc;
        ((WNDCLASSA*)lpWndClass)->lpfnWndProc = MyWndProc;
        Log("[winmm] {} wndproc: {} => {}", lpWndClass->lpszClassName, reinterpret_cast<std::size_t>(wndProcPtr), reinterpret_cast<std::size_t>(&MyWndProc));
    }
#endif
    return registerClassAPtr(lpWndClass);
}

ATOM MyRegisterClassExA(
    const WNDCLASSEXA* lpWndClass
) {
#ifdef HIJ_TARGET_WND_CLASS
    std::string targetClass = HIJ_TARGET_WND_CLASS;
    if (targetClass.compare(lpWndClass->lpszClassName) == 0)
    {
        wndProcPtr = lpWndClass->lpfnWndProc;
        ((WNDCLASSA*)lpWndClass)->lpfnWndProc = MyWndProc;
        Log("[winmm] {} wndproc ex: {} => {}", lpWndClass->lpszClassName, reinterpret_cast<std::size_t>(wndProcPtr), reinterpret_cast<std::size_t>(&MyWndProc));
    }
#endif
    return registerClassExAPtr(lpWndClass);
}

HWND MyCreateWindowExA(
    DWORD dwExStyle,
    LPCSTR lpClassName,
    LPCSTR lpWindowName,
    DWORD dwStyle,
    int X,
    int Y,
    int nWidth,
    int nHeight,
    HWND hWndParent,
    HMENU hMenu,
    HINSTANCE hInstance,
    LPVOID lpParam)
{
    Log("[winmm] create window a: {} by {}", lpWindowName, lpClassName);

    HWND wnd = createWindowExAPtr(
        dwExStyle,
        lpClassName,
        lpWindowName,
        dwStyle,
        X,
        Y,
        nWidth,
        nHeight,
        hWndParent,
        hMenu,
        hInstance,
        lpParam);

#ifdef HIJ_TARGET_WND_CLASS
    std::string targetClass = HIJ_TARGET_WND_CLASS;
    if (targetClass.compare(lpClassName) == 0)
    {
        Log("[winmm] is target window: {} by {} {}", lpWindowName, lpClassName, reinterpret_cast<std::size_t>(wnd));
        auto wpp = (WNDPROC)GetWindowLongPtr(wnd, GWLP_WNDPROC);
        Log("[winmm] is target window proc: {} ", reinterpret_cast<std::size_t>(wpp));
    }
#endif

    return wnd;
}

VOID AttachHij()
{
    TCHAR systemDir[MAX_PATH] = {0};
    UINT systemDirLen = GetSystemDirectory(systemDir, MAX_PATH);
    TCHAR winmmPath[MAX_PATH] = {0};
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

    LONG error = DetourTransactionBegin();
    if (error != NO_ERROR)
    {
        Log("[winmm] ERROR_INVALID_OPERATION: {}, {}", ERROR_INVALID_OPERATION, error);
        return;
    }
    error = DetourUpdateThread(GetCurrentThread());
    if (error != NO_ERROR)
    {
        Log("[winmm] ERROR_NOT_ENOUGH_MEMORY: {}, {}", ERROR_NOT_ENOUGH_MEMORY, error);
        return;
    }
    auto user32Path = std::format(TEXT("{}\\User32.dll"), systemDir);
    Log("[winmm] user32: {}", user32Path);
    createWindowExAPtr = (FnPtrCreateWindowExA)DetourFindFunction("User32.dll", "CreateWindowExA");
    registerClassAPtr = (FnPtrRegisterClassA)DetourFindFunction("User32.dll", "RegisterClassA");
    registerClassExAPtr = (FnPtrRegisterClassExA)DetourFindFunction("User32.dll", "RegisterClassExA");
    if (createWindowExAPtr == nullptr || registerClassAPtr == nullptr || registerClassExAPtr==nullptr)
    {
        Log(
            "[winmm] CreateWindowExA({}) or registerClassAPtr({}) or registerClassExAPtr({}) DetourFindFunction failed",
            reinterpret_cast<std::size_t>(createWindowExAPtr),
            reinterpret_cast<std::size_t>(registerClassAPtr),
            reinterpret_cast<std::size_t>(registerClassExAPtr)
        );
        return;
    }
    DetourAttach(&createWindowExAPtr, MyCreateWindowExA);
    DetourAttach(&registerClassAPtr, MyRegisterClassA);
    DetourAttach(&registerClassExAPtr, MyRegisterClassExA);
    LONG r = DetourTransactionCommit();
    if (r != NO_ERROR)
    {
        Log(
            "[winmm] ERROR_INVALID_BLOCK: {} | ERROR_INVALID_HANDLE: {} | ERROR_INVALID_OPERATION: {} | ERROR_NOT_ENOUGH_MEMORY: {}",
            ERROR_INVALID_BLOCK,
            ERROR_INVALID_HANDLE,
            ERROR_INVALID_OPERATION,
            ERROR_NOT_ENOUGH_MEMORY);
        Log("[winmm] detours attach error: {}", r);
    }
}

VOID DetachHij()
{
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&createWindowExAPtr, MyCreateWindowExA);
    DetourDetach(&registerClassAPtr, MyRegisterClassA);
    DetourDetach(&registerClassExAPtr, MyRegisterClassExA);
    LONG r = DetourTransactionCommit();
    if (r != NO_ERROR)
    {
        Log("[winmm] detours detach error: {}", r);
    }
}

BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved)
{
    switch (reason)
    {
    case DLL_PROCESS_ATTACH:
        // MessageBox(NULL, TEXT("I'm winmm.dll, attach."), "Info", MB_OK);
        OutputDebugString("[winmm] attach.");
        AttachHij();
        break;
    case DLL_PROCESS_DETACH:
        // MessageBox(NULL, TEXT("I'm winmm.dll, detach."), "Info", MB_OK);
        OutputDebugString("[winmm] detach.");
        DetachHij();
        break;
    }

    return TRUE;
}

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
    const MMCKINFO *pmmckiParent,
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
