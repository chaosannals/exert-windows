#pragma once

#include <Windows.h>
#include <rxcpp/rx.hpp>

struct rx_msg {
    HWND hWnd;
    UINT message;
    WPARAM wParam;
    LPARAM lParam;
};
