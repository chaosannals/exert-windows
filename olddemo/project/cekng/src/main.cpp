#include <Windows.h>

static LPCTSTR CLASS_NAME = L"exert windows class";
static LPCTSTR TITLE_NAME = L"cekn gui";

HBRUSH hRedBrush;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		//程序结束
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_PAINT: {
			PAINTSTRUCT ps;
			HBRUSH hOldBrush;

			//开始绘图
			BeginPaint(hWnd, &ps);

			//选用红色刷为当前画刷
			hOldBrush = HBRUSH(SelectObject(ps.hdc, hRedBrush));

			//绘制一个矩形，用当前的红色画刷填充背景
			Rectangle(ps.hdc, 100, 100, 300, 300);

			//恢复以前的画刷状态
			SelectObject(ps.hdc, hOldBrush);

			//结束绘制
			EndPaint(hWnd, &ps);
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return(0L);
}

INT APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	hRedBrush = CreateSolidBrush(RGB(255, 0, 0));
	HBRUSH hBlueBrush = CreateSolidBrush(RGB(0, 0, 255));
	WNDCLASS       wc;//窗口类型结构
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_HAND);

	//使用蓝色画刷填充窗口背景
	wc.hbrBackground = hBlueBrush;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = CLASS_NAME;

	// 注册窗口类失败的话退出
	if (RegisterClass(&wc) == 0) {
		return -1;
	}

	//创建应用程序主窗口
	HWND hWnd = CreateWindow(
		CLASS_NAME,
		TITLE_NAME,
		WS_OVERLAPPEDWINDOW,
		100, 100,
		550, 550,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	//如果窗口创建失败，退出程序
	if (hWnd == NULL) {
		return false;
	}

	//显示窗口
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);


	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	DeleteObject(hBlueBrush);
	DeleteObject(hRedBrush);

	return msg.wParam;
}