#include "gdiwndcls.h"

TCHAR szWindowClass[] = TEXT("GDI DEMO STAR WND CLASS NAME");
TCHAR szTitle[] = TEXT("GDI Demo Simple");

VOID WndPaint(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);
    TextOut(hdc, 0, 0, "Hello, Windows!", 15);

    RECT rc;
    GetClientRect(hWnd, &rc);
    SetMapMode(hdc, MM_ANISOTROPIC);
    SetWindowExtEx(hdc, 100, 100, NULL);
    SetViewportExtEx(hdc, rc.right, rc.bottom, NULL);
    POINT aptStar[6] = { 50,2, 2,98, 98,33, 2,33, 98,98, 50,2 };
    Polyline(hdc, aptStar, 6);

    POINT aptTriangle[4] = { 50,2, 98,86,  2,86, 50,2 },
        aptRectangle[5] = { 2,2, 98,2,  98,98,  2,98, 2,2 },
        aptPentagon[6] = { 50,2, 98,35, 79,90, 21,90, 2,35, 50,2 },
        aptHexagon[7] = { 50,2, 93,25, 93,75, 50,98, 7,75, 7,25, 50,2 };

    SetRect(&rc, 0, 0, 100, 100);

    if (RectVisible(hdc, &rc)) {
        Polyline(hdc, aptTriangle, 4);
    }

    SetViewportOrgEx(hdc, 100, 0, NULL);
    if (RectVisible(hdc, &rc)) {
        Polyline(hdc, aptRectangle, 5);
    }

    SetViewportOrgEx(hdc, 0, 100, NULL);
    if (RectVisible(hdc, &rc)) {
        Polyline(hdc, aptPentagon, 6);
    }

    SetViewportOrgEx(hdc, 100, 100, NULL);
    if (RectVisible(hdc, &rc)) {
        Polyline(hdc, aptHexagon, 7);
    }

    EndPaint(hWnd, &ps);
}

HBRUSH hbrWhite, hbrGray;

//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        hbrWhite = (HBRUSH)GetStockObject(WHITE_BRUSH);
        hbrGray = (HBRUSH)GetStockObject(GRAY_BRUSH);
        break;
    case WM_PAINT:
        if (IsIconic(hWnd)) {
            // MSDN 文档说如果没有图标 wcex.hIcon = NULL; 会调用这个方法来绘制图标，但是 WIN10 实测无效。
            PAINTSTRUCT ps;
            RECT rc;
            HDC hdc = BeginPaint(hWnd, &ps);
            GetClientRect(hWnd, &rc);
            SetMapMode(hdc, MM_ANISOTROPIC);
            SetWindowExtEx(hdc, 100, 100, NULL);
            SetViewportExtEx(hdc, rc.right, rc.bottom, NULL);
            POINT aptStar[6] = { 50,2, 2,98, 98,33, 2,33, 98,98, 50,2 };
            Polyline(hdc, aptStar, 6);
            EndPaint(hWnd, &ps);
        }
        else {
            WndPaint(hWnd, message, wParam, lParam);
        }
        break;
    case WM_ERASEBKGND: {
            // 背景层，这个先于 WM_PAINT ，类似 Dx 的 ClearScreen
        RECT rc;
        HDC hdc = (HDC)wParam;
        GetClientRect(hWnd, &rc);
        SetMapMode(hdc, MM_ANISOTROPIC);
        SetWindowExtEx(hdc, 100, 100, NULL);
        SetViewportExtEx(hdc, rc.right, rc.bottom, NULL);
        FillRect(hdc, &rc, hbrWhite);

        for (int i = 0; i < 13; i++)
        {
            int x = (i * 40) % 100;
            int y = ((i * 40) / 100) * 20;
            SetRect(&rc, x, y, x + 20, y + 20);
            FillRect(hdc, &rc, hbrGray);
        }
        return 1L;
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
