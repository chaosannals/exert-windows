#pragma once
#include <Windows.h>
#include <string>
#include <map>

struct irregular_window_state_t;

class irregular_window_t {
	irregular_window_state_t* state;
public:
	irregular_window_t(
		const TCHAR* skin_path,
		const TCHAR* mask_path);
	virtual ~irregular_window_t() { remove(); }
	void create(HINSTANCE instance, const TCHAR* title);
	void remove();
	HRGN calculate_range();
	static LRESULT CALLBACK process_message(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void toggle_visiable();
};