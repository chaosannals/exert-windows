#include "system_tray.h"
#include <map>

const int MID_EXIT = 11;
const int MID_SHOW = 12;

TCHAR system_tray_window_class_name[] = TEXT("system_tray_window_class_name");
static BOOL is_register_system_tray_window_class = FALSE;
static std::map<HWND, system_tray_t*> allin;

struct system_tray_state_t {
	TCHAR title[128];
	NOTIFYICONDATA icon;
	HMENU menu;
	HWND window_handle;
	std::function<void()> on_toggle_visible;
	system_tray_state_t():
		menu(nullptr),
		window_handle(nullptr)
	{
		ZeroMemory(&title, sizeof(title));
		ZeroMemory(&icon, sizeof(icon));
	}
};

LRESULT CALLBACK system_tray_t::process_message(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	auto st = allin.find(hwnd);
	UINT WM_TASKBARCREATED = RegisterWindowMessage(TEXT("TaskbarCreated"));

	switch (message) {
	case WM_CREATE:
		{
			system_tray_t* stp = (system_tray_t*)((LPCREATESTRUCT)lParam)->lpCreateParams;
			auto& icon = stp->state->icon;
			icon.cbSize = sizeof(NOTIFYICONDATA);
			icon.hWnd = hwnd;
			icon.uID = 0;
			icon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP | NIF_INFO;
			icon.uCallbackMessage = WM_USER;
			icon.hIcon = LoadIcon(NULL, IDI_APPLICATION);
			lstrcpy(icon.szTip, TEXT("tip tip tip"));
			lstrcpy(icon.szInfo, TEXT("info info info"));
			lstrcpy(icon.szInfoTitle, TEXT("info title"));
			icon.dwInfoFlags = NIIF_INFO;
			Shell_NotifyIcon(NIM_ADD, &icon);
			stp->state->menu = CreatePopupMenu();
			AppendMenu(stp->state->menu, MF_STRING, MID_SHOW, TEXT("ÏÔÊ¾"));
			AppendMenu(stp->state->menu, MF_SEPARATOR, 0, TEXT(""));
			AppendMenu(stp->state->menu, MF_STRING, MID_EXIT, TEXT("¹Ø±Õ"));
		}
		break;
	case WM_USER:
		if (st != allin.end()) {
			if (lParam == WM_RBUTTONDOWN) {
				POINT p;
				GetCursorPos(&p);
				SetForegroundWindow(hwnd);
				int mid = TrackPopupMenu(
					st->second->state->menu,
					TPM_RETURNCMD,
					p.x, p.y,
					NULL,
					hwnd,
					NULL
				);
				if (mid == MID_EXIT) {
					SendMessage(hwnd, WM_CLOSE, wParam, lParam);
				}
				else if(mid == MID_SHOW) {
					if (st->second->state->on_toggle_visible) {
						st->second->state->on_toggle_visible();
					}
				}
			}
		}
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		if (st != allin.end()) {
			Shell_NotifyIcon(NIM_DELETE, &st->second->state->icon);
		}
		PostQuitMessage(0);
		break;
	default:
		if (message == WM_TASKBARCREATED) {
			SendMessage(hwnd, WM_CREATE, wParam, lParam);
		}
		break;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

static BOOL register_system_tray_window_class(HINSTANCE instance) {
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = system_tray_t::process_message;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = instance;
	wc.hIcon = LoadIcon(instance, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = system_tray_window_class_name;
	wc.hIconSm = LoadIcon(wc.hInstance, IDI_APPLICATION);

	return RegisterClassEx(&wc);
}

system_tray_t::system_tray_t(const TCHAR *title): state(nullptr) {
	state = new system_tray_state_t;
	int tl = lstrlen(title);
	lstrcpy(state->title, title);
}


void system_tray_t::create(HINSTANCE instance) {
	if (!is_register_system_tray_window_class) {
		if (!register_system_tray_window_class(instance)) {
			// TODO error
			return;
		}
		is_register_system_tray_window_class = TRUE;
	}

	HWND hwnd = CreateWindowEx(
		WS_EX_TOOLWINDOW,
		system_tray_window_class_name,
		state->title,
		WS_POPUP,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL, NULL,
		instance,
		this
	);

	allin.emplace(hwnd, this);
	state->window_handle = hwnd;
	ShowWindow(hwnd, 1);
	UpdateWindow(hwnd);
}

void system_tray_t::remove() {
	if (state) {
		if (state->window_handle) {
			allin.erase(state->window_handle);
		}
		delete state;
		state = nullptr;
	}
}

void system_tray_t::listen_visible_toggle(const std::function<void()>& f) {
	state->on_toggle_visible = f;
}