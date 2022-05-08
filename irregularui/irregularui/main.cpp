#include <Windows.h>

TCHAR windowClass[] = TEXT("Irregularui Class");
TCHAR title[] = TEXT("不规则窗口");
HBITMAP skin_bmp = nullptr;
HBITMAP mask_bmp = nullptr;
DWORD transparent_color = RGB(0, 0, 0);
int bmp_width = 0;
int bmp_height = 0;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_SIZE:
		break;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			HDC shdc = CreateCompatibleDC(NULL);
			SelectObject(shdc, skin_bmp);
			BitBlt(hdc, 0, 0, bmp_width, bmp_height, shdc, 0, 0, SRCCOPY);
			DeleteDC(shdc);
			EndPaint(hWnd, &ps);
		}
		break;
	case WM_LBUTTONDOWN:
		//SendMessage(hWnd, WM_NCHITTEST, wParam, lParam);
		SendMessage(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, lParam);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}

int Dispose(int r) {
	if (skin_bmp) {
		DeleteObject(skin_bmp);
	}
	if (mask_bmp) {
		DeleteObject(mask_bmp);
	}
	return r;
}



int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow
) {
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEXW);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = windowClass;
	wc.hIconSm = LoadIcon(wc.hInstance, IDI_APPLICATION);

	if (!RegisterClassExW(&wc))
	{
		MessageBoxA(NULL, "Call to RegisterClassEx failed!", "Windows Desktop Guided Tour", NULL);

		return Dispose(1);
	}
	
	mask_bmp = (HBITMAP)LoadImage(0, TEXT("mask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	skin_bmp = (HBITMAP)LoadImage(0, TEXT("skin.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	BITMAP bmp;
	HRGN line_region = nullptr;
	HRGN full_region = nullptr;
	BOOL is_first = TRUE;

	GetObject(mask_bmp, sizeof(BITMAP), &bmp);
	HDC hdc = CreateCompatibleDC(NULL);
	SelectObject(hdc, mask_bmp);
	bmp_width = bmp.bmWidth;
	bmp_height = bmp.bmHeight;

	for (int y = 0; y < bmp_height; ++y) {
		for (int x = 0; x < bmp_width; ++x) {
			if (GetPixel(hdc, x, y) != transparent_color) {
				if (is_first) {
					is_first = FALSE;
					full_region = CreateRectRgn(x, y, x + 1, y + 1);
				}
				else {
					line_region = CreateRectRgn(x, y, x + 1, y + 1);
					CombineRgn(full_region, full_region, line_region, RGN_OR);
					DeleteObject(line_region);
				}
			}
		}
	}
	DeleteDC(hdc);

	HWND hWnd = CreateWindow(
		windowClass,
		title,
		//WS_OVERLAPPEDWINDOW,
		WS_POPUPWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		bmp_width, bmp_height,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (!hWnd)
	{
		MessageBoxA(NULL, "Call to CreateWindow failed!", "Windows Desktop Guided Tour", NULL);
		return Dispose(1);
	}

	
	SetWindowRgn(hWnd, full_region, TRUE);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return Dispose((int)msg.wParam);
}