#include "wnd.h"
#include "rxmsg.h"
//#include <future>

extern rxcpp::observable<rx_msg> obs;

int APIENTRY WinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPTSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    MyRegisterClass(hInstance);

    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    MSG msg;


    obs.subscribe([](rx_msg m) {
        static int i = 0;
        static int j = 0;
        if (m.message == WM_KEYDOWN) {
            ++j;
        }
        i++;
    });

    obs.filter([](rx_msg m) { return m.message == WM_KEYUP; }).subscribe([](rx_msg m) {
        static int i = 0;
        i++;
    });

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}
