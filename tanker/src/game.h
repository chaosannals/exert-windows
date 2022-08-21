#pragma once

#include <Windows.h>
#include <gdiplus.h>
#include "point.h"

namespace wnt {
	class game_t {
	public:
		void on_update();
		void on_render();
		void on_mouse_move(UINT flag, point_t point);
		void on_left_button_up(UINT flag, point_t point);
	};
}