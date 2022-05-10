#include "input_text.h"

struct input_text_state_t {

};

void input_text_t::create(HWND parent, LPCREATESTRUCT create_struct) {
	HWND hwndTxt = CreateWindow(
		TEXT("edit"),
		TEXT(""),
		WS_CHILD | WS_VISIBLE | WS_BORDER | BS_TEXT,
		250, 100, 300, 22,
		parent,
		(HMENU)1,
		create_struct->hInstance,
		NULL
	);
}

void input_text_t::remove() {

}