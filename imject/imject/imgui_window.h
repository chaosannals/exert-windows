#pragma once
#include <Windows.h>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx10.h"
#include <d3d10_1.h>
#include <d3d10.h>
#include <tchar.h>

struct imgui_window_state_t;

class imgui_window_t {
	imgui_window_state_t* state;
public:
	imgui_window_t();
	virtual ~imgui_window_t() { remove(); }
	void create();
	void remove();
	void render();

	bool create_d3ddevice(HWND hWnd);
	void cleanup_d3ddevice();
	void create_render_target();
	void cleanup_render_target();
	static LRESULT WINAPI process_message(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};