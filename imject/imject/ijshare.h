#pragma once
#include <Windows.h>
#include <memory>

struct ijshare_t {
	HMODULE ijdll;
};

HANDLE ijshare_new(const char* name, const ijshare_t* data);
ijshare_t* ijshare_open(const char* name);
