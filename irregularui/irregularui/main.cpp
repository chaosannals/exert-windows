#include <Windows.h>
#include "system_tray.h"
#include "irregular_window.h"


int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow
) {
	system_tray_t tray(TEXT("tray"));
	irregular_window_t window(TEXT("skin.bmp"), TEXT("mask.bmp"));

	tray.create(hInstance);
	window.create(hInstance, TEXT("不规则窗口"));

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	window.remove();
	tray.remove();

	return (int)msg.wParam;
}