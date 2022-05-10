#pragma once
#include <Windows.h>
#include <functional>

struct system_tray_state_t;

class system_tray_t {
	system_tray_state_t* state;
public:
	system_tray_t(const TCHAR *title);
	virtual ~system_tray_t() { remove(); }
	void create(HINSTANCE instance);
	void remove();
	static LRESULT CALLBACK process_message(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	void listen_visible_toggle(const std::function<void()>& f);
};