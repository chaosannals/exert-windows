#pragma once

#include <Windows.h>

namespace wnt {
	class window_t {
		static const TCHAR* CLASSNAME;
		HWND handle;
	public:
		window_t():handle(nullptr) {}
		void attach(HINSTANCE);

	};
}