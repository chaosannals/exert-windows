#include "irregular_window.h"

static const TCHAR* const irregular_window_class_name = TEXT("irregular_window_class_name");
static BOOL is_register_irregular_window_class = FALSE;
static std::map<HWND, irregular_window_t*> allin;

struct irregular_window_state_t {
	HBITMAP skin_bmp;
	HBITMAP mask_bmp;
	int bmp_width;
	int bmp_height;
	int screen_width;
	int screen_height;
	DWORD transparent_color;
	HWND window_handle;

	irregular_window_state_t() :
		skin_bmp(nullptr),
		mask_bmp(nullptr),
		bmp_width(0),
		bmp_height(0),
		screen_width(0),
		screen_height(0),
		transparent_color(RGB(0, 0, 0)),
		window_handle(nullptr)
	{
		screen_width = GetSystemMetrics(SM_CXSCREEN);
		screen_height = GetSystemMetrics(SM_CYSCREEN);
	}
};

LRESULT CALLBACK irregular_window_t::process_message(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	auto iw = allin.find(hWnd);

	switch (message)
	{
	case WM_SIZE:
		break;
	case WM_MOVE:
		if (iw != allin.end()) {
			RECT rect;
			int x = (int)(short)LOWORD(lParam);   // horizontal position 
			int y = (int)(short)HIWORD(lParam);   // vertical position 
			if (GetWindowRect(hWnd, &rect)) {
				
			}
		}
		break;
	case WM_PAINT:
		if (iw != allin.end()) {
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			HDC shdc = CreateCompatibleDC(NULL);
			SelectObject(shdc, iw->second->state->skin_bmp);
			BitBlt(hdc, 0, 0, iw->second->state->bmp_width, iw->second->state->bmp_height, shdc, 0, 0, SRCCOPY);
			DeleteDC(shdc);
			EndPaint(hWnd, &ps);
		}
		break;
	case WM_LBUTTONDOWN:
		SendMessage(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, lParam);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

static BOOL register_irregular_window_class(HINSTANCE instance) {
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = irregular_window_t::process_message;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = instance;
	wc.hIcon = LoadIcon(instance, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = irregular_window_class_name;
	wc.hIconSm = LoadIcon(wc.hInstance, IDI_APPLICATION);

	return RegisterClassEx(&wc);
}

irregular_window_t::irregular_window_t(
	const TCHAR *skin_path,
	const TCHAR *mask_path)
	: state(nullptr) {
	state = new irregular_window_state_t();
	state->skin_bmp = (HBITMAP)LoadImage(0, skin_path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	state->mask_bmp = (HBITMAP)LoadImage(0, mask_path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	
}

void irregular_window_t::create(HINSTANCE instance, const TCHAR *title) {
	if (!is_register_irregular_window_class) {
		if (!register_irregular_window_class(instance)) {
			// TODO ±¨´í
			return;
		}
		is_register_irregular_window_class = TRUE;
	}

	HRGN full_region = calculate_range();
	HWND hWnd = CreateWindow(
		irregular_window_class_name,
		title,
		//WS_OVERLAPPEDWINDOW,
		WS_POPUPWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		state->bmp_width,
		state->bmp_height,
		NULL,
		NULL,
		instance,
		this
	);
	if (!hWnd)
	{
		//MessageBoxA(NULL, "Call to CreateWindow failed!", "Windows Desktop Guided Tour", NULL);
		return;
	}

	allin.emplace(hWnd, this);
	state->window_handle = hWnd;
	SetWindowRgn(hWnd, full_region, TRUE);
	DeleteObject(full_region);
	ShowWindow(hWnd, 1);
	UpdateWindow(hWnd);
}

void irregular_window_t::remove() {
	if (state) {
		if (state->skin_bmp) {
			DeleteObject(state->skin_bmp);
		}
		if (state->mask_bmp) {
			DeleteObject(state->mask_bmp);
		}
		if (state->window_handle) {
			allin.erase(state->window_handle);
		}
		delete state;
		state = nullptr;
	}
}

HRGN irregular_window_t::calculate_range() {
	BITMAP bmp;
	HRGN line_region = nullptr;
	HRGN full_region = nullptr;
	BOOL is_first = TRUE;

	GetObject(state->mask_bmp, sizeof(BITMAP), &bmp);
	HDC hdc = CreateCompatibleDC(NULL);
	SelectObject(hdc, state->mask_bmp);
	state->bmp_width = bmp.bmWidth;
	state->bmp_height = bmp.bmHeight;

	for (int y = 0; y < state->bmp_height; ++y) {
		for (int x = 0; x < state->bmp_width; ++x) {
			if (GetPixel(hdc, x, y) != state->transparent_color) {
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
	return full_region;
}

void irregular_window_t::toggle_visiable() {
	ShowWindow(state->window_handle, !IsWindowVisible(state->window_handle));
}