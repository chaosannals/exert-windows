#pragma once
#include <Windows.h>
#include <memory>

struct ijshare_t {
	HMODULE ijdll;
};

HANDLE ijshare_new(const char *name, const ijshare_t *data) {
    HANDLE h = CreateFileMappingA((HANDLE)INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(ijshare_t), name);
    if (h == nullptr) {
        return nullptr;
    }
    LPVOID addr = MapViewOfFile(h, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    if (addr) {
        memcpy(addr, data, sizeof(ijshare_t));
        FlushViewOfFile(addr, sizeof(ijshare_t));
        UnmapViewOfFile(addr);
    }
    return h;
}

ijshare_t* ijshare_open(const char* name) {
    HANDLE h = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, name);
    if (h) {
        ijshare_t* r = (ijshare_t*)MapViewOfFile(h, FILE_MAP_ALL_ACCESS, 0, 0, 0);
        CloseHandle(h);
        if (r) {
            ijshare_t *p = new ijshare_t;
            memcpy(p, r, sizeof(ijshare_t));
            UnmapViewOfFile(r);
            return p;
        }
    } 
    return nullptr;
}
