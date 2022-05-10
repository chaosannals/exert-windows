#pragma once
#include <Windows.h>
#include <map>

struct input_text_state_t;

class input_text_t {
	input_text_state_t* state;
public:
	virtual void create(HWND parent, LPCREATESTRUCT create_struct);
	virtual void remove();
};