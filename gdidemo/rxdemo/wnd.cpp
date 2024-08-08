#include "wnd.h"
#include "rxmsg.h"

TCHAR szWindowClass[] = TEXT("RX DEMO STAR WND CLASS NAME");
TCHAR szTitle[] = TEXT("RX Demo Simple");

rxcpp::subjects::subject<rx_msg> subject;
rxcpp::subscriber<rx_msg> sub(subject.get_subscriber());
rxcpp::observable<rx_msg> obs(subject.get_observable());

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    sub.on_next(rx_msg{ hWnd, message, wParam, lParam });

    switch (message)
    {
    case WM_CREATE:
        break;
    case WM_PAINT:
        break;
    case WM_ERASEBKGND: {
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_LBUTTONDOWN:
        // TODO
        break;
    case WM_LBUTTONUP:
        // TODO
        break;
    case WM_MOUSEMOVE:
        // TODO
        break;
    case WM_KEYUP:
        // TODO
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    //wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_HelloStar));
    //wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wcex.hIcon = NULL;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    //wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_HelloStar);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    //wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    //wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);
    wcex.hIconSm = NULL;
    return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    HWND hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, 240, 320, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    return TRUE;
}
