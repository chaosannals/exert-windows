#include <Windows.h>
#include <wininet.h>
#include <strsafe.h>
#include <string>
#include <format>
#include <detours.h>

#define HIJ_API extern "C" __declspec(dllexport)
#ifdef UNICODE
typedef std::wstring TSTRING;
#else
typedef std::string TSTRING;
#endif

typedef HINTERNET (*FnPtrInternetConnectA)(
    HINTERNET hInternet,
    LPCSTR lpszServerName,
    INTERNET_PORT nServerPort,
    LPCSTR lpszUserName,
    LPCSTR lpszPassword,
    DWORD dwService,
    DWORD dwFlags,
    DWORD_PTR dwContext);
typedef HINTERNET (*FnPtrInternetOpenW)(
    LPCWSTR lpszAgent,
    DWORD dwAccessType,
    LPCWSTR lpszProxy,
    LPCWSTR lpszProxyBypass,
    DWORD dwFlags);
typedef BOOL (*FnPtrHttpEndRequestW)(
    HINTERNET hRequest,
    LPINTERNET_BUFFERSW lpBuffersOut,
    DWORD dwFlags,
    DWORD_PTR dwContext);
typedef BOOL (*FnPtrInternetCloseHandle)(
    HINTERNET hInternet);
typedef BOOL (*FnPtrHttpSendRequestExW)(
    HINTERNET hRequest,
    LPINTERNET_BUFFERSW lpBuffersIn,
    LPINTERNET_BUFFERSW lpBuffersOut,
    DWORD dwFlags,
    DWORD_PTR dwContext);

typedef HINTERNET (*FnPtrInternetConnectW)(
    HINTERNET hInternet,
    LPCWSTR lpszServerName,
    INTERNET_PORT nServerPort,
    LPCWSTR lpszUserName,
    LPCWSTR lpszPassword,
    DWORD dwService,
    DWORD dwFlags,
    DWORD_PTR dwContext);
typedef BOOL (*FnPtrHttpQueryInfoW)(
    HINTERNET hRequest,
    DWORD     dwInfoLevel,
    LPVOID    lpBuffer,
    LPDWORD   lpdwBufferLength,
    LPDWORD   lpdwIndex
);
typedef BOOL (*FnPtrHttpAddRequestHeadersW)(
    HINTERNET hRequest,
    LPCWSTR   lpszHeaders,
    DWORD     dwHeadersLength,
    DWORD     dwModifiers
);
typedef BOOL(*FnPtrHttpSendRequestW)(
     HINTERNET hRequest,
     LPCWSTR   lpszHeaders,
     DWORD     dwHeadersLength,
     LPVOID    lpOptional,
     DWORD     dwOptionalLength
);
typedef BOOL(*FnPtrInternetSetOptionW)(
     HINTERNET hInternet,
     DWORD     dwOption,
     LPVOID    lpBuffer,
    DWORD     dwBufferLength
);
typedef BOOL(*FnPtrHttpQueryInfoA)(
         HINTERNET hRequest,
          DWORD     dwInfoLevel,
     LPVOID    lpBuffer,
     LPDWORD   lpdwBufferLength,
    LPDWORD   lpdwIndex
);
typedef HINTERNET(*FnPtrHttpOpenRequestW)(
    HINTERNET hConnect,
     LPCWSTR   lpszVerb,
     LPCWSTR   lpszObjectName,
     LPCWSTR   lpszVersion,
     LPCWSTR   lpszReferrer,
     LPCWSTR* lplpszAcceptTypes,
     DWORD     dwFlags,
     DWORD_PTR dwContext
);
typedef BOOL(*FnPtrInternetWriteFile)(
      HINTERNET hFile,
      LPCVOID   lpBuffer,
      DWORD     dwNumberOfBytesToWrite,
     LPDWORD   lpdwNumberOfBytesWritten
);
typedef BOOL(*FnPtrInternetQueryDataAvailable)(
      HINTERNET hFile,
     LPDWORD   lpdwNumberOfBytesAvailable,
      DWORD     dwFlags,
      DWORD_PTR dwContext
);
typedef HINTERNET(*FnPtrInternetOpenUrlW)(
    HINTERNET hInternet,
    LPCWSTR   lpszUrl,
    LPCWSTR   lpszHeaders,
    DWORD     dwHeadersLength,
    DWORD     dwFlags,
    DWORD_PTR dwContext
);
typedef BOOL(*FnPtrInternetReadFile)(
    HINTERNET hFile,
    LPVOID    lpBuffer,
    DWORD     dwNumberOfBytesToRead,
    LPDWORD   lpdwNumberOfBytesRead
);


typedef HWND (*FnPtrCreateWindowA)(
    LPCTSTR lpClassName,
    LPCTSTR lpWindowName,
    DWORD dwStyle,
    int x,
    int y,
    int nWidth,
    int nHeight,
    HWND hWndParent,
    HMENU hMenu,
    HINSTANCE hInstance,
    LPVOID lpParam);

FnPtrInternetConnectA internetConnectAPtr;
FnPtrInternetOpenW internetOpenWPtr;
FnPtrHttpEndRequestW httpEndRequestWPtr;
FnPtrInternetCloseHandle internetCloseHandlePtr;
FnPtrHttpSendRequestExW httpSendRequestExWPtr;
FnPtrInternetConnectW internetConnectWPtr;
FnPtrHttpQueryInfoW httpQueryInfoWPtr;
FnPtrHttpAddRequestHeadersW httpAddRequestHeadersWPtr;
FnPtrHttpSendRequestW httpSendRequestWPtr;
FnPtrInternetSetOptionW internetSetOptionWPtr;
FnPtrHttpQueryInfoA httpQueryInfoAPtr;
FnPtrHttpOpenRequestW httpOpenRequestWPtr;
FnPtrInternetWriteFile internetWriteFilePtr;
FnPtrInternetQueryDataAvailable internetQueryDataAvailablePtr;
FnPtrInternetOpenUrlW internetOpenUrlWPtr;
FnPtrInternetReadFile internetReadFilePtr;

FnPtrCreateWindowA createWindowAPtr;

const TCHAR dllName[] = TEXT("wininet.dll");

template <typename... Args>
VOID Log(const TSTRING &format, const Args &...args)
{
    auto content = std::vformat(format, std::make_format_args(args...));
    OutputDebugString(content.c_str());
}

HWND MyCreateWindowA(
    LPCTSTR lpClassName,
    LPCTSTR lpWindowName,
    DWORD dwStyle,
    int x,
    int y,
    int nWidth,
    int nHeight,
    HWND hWndParent,
    HMENU hMenu,
    HINSTANCE hInstance,
    LPVOID lpParam)
{
    Log("[wininet] create window a: {} by {}", lpWindowName, lpClassName);
    return createWindowAPtr(
        lpClassName,
        lpWindowName,
        dwStyle,
        x, y, nWidth, nHeight,
        hWndParent,
        hMenu,
        hInstance,
        lpParam);
}

VOID AttachHij()
{
    TCHAR systemDir[MAX_PATH] = {0};
    UINT systemDirLen = GetSystemDirectory(systemDir, MAX_PATH);
    TCHAR wininetPath[MAX_PATH] = {0};
    size_t nameLen = 0;
    if (StringCchLength(dllName, MAX_PATH, &nameLen) != S_OK)
    {
        OutputDebugString("[wininet] wininet.dll name length unknown.");
        return;
    }
    size_t systemDirEnd = systemDirLen + 1;
    if (StringCchCopy(wininetPath, systemDirEnd, systemDir) != S_OK)
    {
        OutputDebugString("[wininet] system dir unknown.");
        return;
    }
    wininetPath[systemDirLen] = TEXT('\\');
    if (StringCchCopy(wininetPath + systemDirEnd, systemDirEnd + nameLen, dllName) != S_OK)
    {
        OutputDebugString("[wininet] wininet.dll path unknown.");
        return;
    }
    // OutputDebugString(wininetPath);
    Log("[wininet] path: {}", wininetPath);
    HMODULE wininetMod = LoadLibrary(wininetPath);
    if (NULL == wininetMod)
    {
        OutputDebugString("[wininet] wininet.dll load failed.");
        return;
    }
    internetConnectAPtr = (FnPtrInternetConnectA)GetProcAddress(wininetMod, "InternetConnectA");
    internetOpenWPtr = (FnPtrInternetOpenW)GetProcAddress(wininetMod, "InternetOpenW");
    httpEndRequestWPtr = (FnPtrHttpEndRequestW)GetProcAddress(wininetMod, "HttpEndRequestW");
    internetCloseHandlePtr = (FnPtrInternetCloseHandle)GetProcAddress(wininetMod, "InternetCloseHandle");
    httpSendRequestExWPtr = (FnPtrHttpSendRequestExW)GetProcAddress(wininetMod, "HttpSendRequestExW");
    internetConnectWPtr = (FnPtrInternetConnectW)GetProcAddress(wininetMod, "InternetConnectW");
    httpQueryInfoWPtr = (FnPtrHttpQueryInfoW)GetProcAddress(wininetMod, "HttpQueryInfoW");
    httpAddRequestHeadersWPtr = (FnPtrHttpAddRequestHeadersW)GetProcAddress(wininetMod, "HttpAddRequestHeadersW");
    httpSendRequestWPtr=(FnPtrHttpSendRequestW)GetProcAddress(wininetMod, "HttpSendRequestW");
    internetSetOptionWPtr=(FnPtrInternetSetOptionW)GetProcAddress(wininetMod, "InternetSetOptionW");
    httpQueryInfoAPtr = (FnPtrHttpQueryInfoA)GetProcAddress(wininetMod, "HttpQueryInfoA");
    httpOpenRequestWPtr = (FnPtrHttpOpenRequestW)GetProcAddress(wininetMod, "HttpOpenRequestW");
    internetWriteFilePtr = (FnPtrInternetWriteFile) GetProcAddress(wininetMod, "InternetWriteFile");
    internetQueryDataAvailablePtr =(FnPtrInternetQueryDataAvailable) GetProcAddress(wininetMod, "InternetQueryDataAvailable");
    internetOpenUrlWPtr = (FnPtrInternetOpenUrlW)GetProcAddress(wininetMod, "InternetOpenUrlW");
    internetReadFilePtr = (FnPtrInternetReadFile) GetProcAddress(wininetMod, "InternetReadFile");

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    auto user32Path = std::format(TEXT("{}\\User32.dll"), systemDir);
    Log("[wininet] user32: {}", user32Path);
    createWindowAPtr = (FnPtrCreateWindowA)DetourFindFunction(user32Path.c_str(), "createWindowA");
    if (createWindowAPtr == nullptr)
    {
        Log("[wininet] createWindowA DetourFindFunction failed");
        return;
    }
    DetourAttach(&createWindowAPtr, MyCreateWindowA);
    LONG r = DetourTransactionCommit();
    if (r != NO_ERROR)
    {
        Log(
            "[wininet] ERROR_INVALID_BLOCK: {} | ERROR_INVALID_HANDLE: {} | ERROR_INVALID_OPERATION: {} | ERROR_NOT_ENOUGH_MEMORY: {}",
            ERROR_INVALID_BLOCK,
            ERROR_INVALID_HANDLE,
            ERROR_INVALID_OPERATION,
            ERROR_NOT_ENOUGH_MEMORY);
        Log("[wininet] detours attach error: {}", r);
    }
}

VOID DetachHij()
{
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&createWindowAPtr, MyCreateWindowA);
    LONG r = DetourTransactionCommit();
    if (r != NO_ERROR)
    {
        Log("[wininet] detours detach error: {}", r);
    }
}

BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved)
{
    switch (reason)
    {
    case DLL_PROCESS_ATTACH:
        // MessageBox(NULL, TEXT("I'm wininet.dll, attach."), "Info", MB_OK);
        OutputDebugString("[wininet] attach.");
        AttachHij();
        break;
    case DLL_PROCESS_DETACH:
        // MessageBox(NULL, TEXT("I'm wininet.dll, detach."), "Info", MB_OK);
        OutputDebugString("[wininet] detach.");
        DetachHij();
        break;
    }

    return TRUE;
}

HIJ_API HINTERNET MyInternetConnectA(
    HINTERNET hInternet,
    LPCSTR lpszServerName,
    INTERNET_PORT nServerPort,
    LPCSTR lpszUserName,
    LPCSTR lpszPassword,
    DWORD dwService,
    DWORD dwFlags,
    DWORD_PTR dwContext)
{
    Log("[wininet] MyInternetConnectA: {} {}", lpszServerName, nServerPort);
    return internetConnectAPtr(
        hInternet,
        lpszServerName,
        nServerPort,
        lpszUserName,
        lpszPassword,
        dwService,
        dwFlags,
        dwContext);
}

HIJ_API HINTERNET MyInternetOpenW(
    LPCWSTR lpszAgent,
    DWORD dwAccessType,
    LPCWSTR lpszProxy,
    LPCWSTR lpszProxyBypass,
    DWORD dwFlags)
{
    Log("[wininet] MyInternetOpenW");
    return internetOpenWPtr(
        lpszAgent,
        dwAccessType,
        lpszProxy,
        lpszProxyBypass,
        dwFlags);
}

HIJ_API BOOL MyHttpEndRequestW(
    HINTERNET hRequest,
    LPINTERNET_BUFFERSW lpBuffersOut,
    DWORD dwFlags,
    DWORD_PTR dwContext)
{
    Log("[wininet] MyHttpEndRequestW");
    return httpEndRequestWPtr(
        hRequest,
        lpBuffersOut,
        dwFlags,
        dwContext);
}

BOOL MyInternetCloseHandle(HINTERNET hInternet)
{
    Log("[wininet] MyInternetCloseHandle");
    return internetCloseHandlePtr(hInternet);
}

BOOL MyHttpSendRequestExW(
    HINTERNET hRequest,
    LPINTERNET_BUFFERSW lpBuffersIn,
    LPINTERNET_BUFFERSW lpBuffersOut,
    DWORD dwFlags,
    DWORD_PTR dwContext)
{
    Log("[wininet] MyHttpSendRequestExW");
    return httpSendRequestExWPtr(
        hRequest,
        lpBuffersIn,
        lpBuffersOut,
        dwFlags,
        dwContext);
}

HINTERNET MyInternetConnectW(
    HINTERNET hInternet,
    LPCWSTR lpszServerName,
    INTERNET_PORT nServerPort,
    LPCWSTR lpszUserName,
    LPCWSTR lpszPassword,
    DWORD dwService,
    DWORD dwFlags,
    DWORD_PTR dwContext)
{
    Log("[wininet] MyInternetConnectW");
    return internetConnectWPtr(
        hInternet,
        lpszServerName,
        nServerPort,
        lpszUserName,
        lpszPassword,
        dwService,
        dwFlags,
        dwContext);
}

BOOL MyHttpQueryInfoW(
    HINTERNET hRequest,
    DWORD     dwInfoLevel,
    LPVOID    lpBuffer,
    LPDWORD   lpdwBufferLength,
    LPDWORD   lpdwIndex
) {
    Log("[wininet] MyHttpQueryInfoW");
    return httpQueryInfoWPtr(
        hRequest,
        dwInfoLevel,
        lpBuffer,
        lpdwBufferLength,
        lpdwIndex
    );
}

BOOL MyHttpAddRequestHeadersW(
    HINTERNET hRequest,
    LPCWSTR   lpszHeaders,
    DWORD     dwHeadersLength,
    DWORD     dwModifiers
) {
    Log("[wininet] MyHttpAddRequestHeadersW");
    return httpAddRequestHeadersWPtr(
        hRequest,
        lpszHeaders,
        dwHeadersLength,
        dwModifiers
    );
}


BOOL MyHttpSendRequestW(
    HINTERNET hRequest,
    LPCWSTR   lpszHeaders,
    DWORD     dwHeadersLength,
    LPVOID    lpOptional,
    DWORD     dwOptionalLength
) {
    Log("[wininet] MyHttpSendRequestW");
    return httpSendRequestWPtr(
        hRequest,
        lpszHeaders,
        dwHeadersLength,
        lpOptional,
        dwOptionalLength
    );
}
BOOL MyInternetSetOptionW(
    HINTERNET hInternet,
    DWORD     dwOption,
    LPVOID    lpBuffer,
    DWORD     dwBufferLength
) {
    Log("[wininet] MyInternetSetOptionW");
    return internetSetOptionWPtr(
        hInternet,
        dwOption,
        lpBuffer,
        dwBufferLength
    );
}


BOOL MyHttpQueryInfoA(
    HINTERNET hRequest,
    DWORD     dwInfoLevel,
    LPVOID    lpBuffer,
    LPDWORD   lpdwBufferLength,
    LPDWORD   lpdwIndex
) {
    return httpQueryInfoAPtr(
        hRequest,
        dwInfoLevel,
        lpBuffer,
        lpdwBufferLength,
        lpdwIndex
    );
}
HINTERNET MyHttpOpenRequestW(
    HINTERNET hConnect,
    LPCWSTR   lpszVerb,
    LPCWSTR   lpszObjectName,
    LPCWSTR   lpszVersion,
    LPCWSTR   lpszReferrer,
    LPCWSTR* lplpszAcceptTypes,
    DWORD     dwFlags,
    DWORD_PTR dwContext
) {
    return httpOpenRequestWPtr(
        hConnect,
        lpszVerb,
        lpszObjectName,
        lpszVersion,
        lpszReferrer,
        lplpszAcceptTypes,
        dwFlags,
        dwContext
    );
}


BOOL MyInternetWriteFile(
    HINTERNET hFile,
    LPCVOID   lpBuffer,
    DWORD     dwNumberOfBytesToWrite,
    LPDWORD   lpdwNumberOfBytesWritten
) {
    return internetWriteFilePtr(
        hFile,
        lpBuffer,
        dwNumberOfBytesToWrite,
        lpdwNumberOfBytesWritten
    );
}
BOOL MyInternetQueryDataAvailable(
    HINTERNET hFile,
    LPDWORD   lpdwNumberOfBytesAvailable,
    DWORD     dwFlags,
    DWORD_PTR dwContext
) {
    return internetQueryDataAvailablePtr(
        hFile,
        lpdwNumberOfBytesAvailable,
        dwFlags,
        dwContext
    );
}

HINTERNET MyInternetOpenUrlW(
    HINTERNET hInternet,
    LPCWSTR   lpszUrl,
    LPCWSTR   lpszHeaders,
    DWORD     dwHeadersLength,
    DWORD     dwFlags,
    DWORD_PTR dwContext
) {
    return internetOpenUrlWPtr(
        hInternet,
        lpszUrl,
        lpszHeaders,
        dwHeadersLength,
        dwFlags,
        dwContext
    );
}

BOOL MyInternetReadFile(
    HINTERNET hFile,
    LPVOID    lpBuffer,
    DWORD     dwNumberOfBytesToRead,
    LPDWORD   lpdwNumberOfBytesRead
) {
    return internetReadFilePtr(
        hFile,
        lpBuffer,
        dwNumberOfBytesToRead,
        lpdwNumberOfBytesRead
    );
}
