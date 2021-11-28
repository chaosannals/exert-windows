#pragma once

#include <vector>
#include <memory>
#include <Windows.h>

struct logic_drive_t {
	WCHAR name[MAX_PATH];
};

DWORD wchar_len(WCHAR* p, DWORD c) {
	for (DWORD i = 0; i < c; ++i) {
		if (p[i] == '\0') {
			return i + 1;
		}
	}
	return c;
}

std::vector<logic_drive_t> list_disc() {
	std::vector<logic_drive_t> result;
	DWORD c = GetLogicalDriveStringsW(0, NULL);
	std::unique_ptr<WCHAR[]> buff(new WCHAR[c]);
	WCHAR* p = buff.get();
	if (GetLogicalDriveStringsW(c, p)) {
		for (DWORD i = 0; i < c;) {
			WCHAR *pi = p + i;
			DWORD nl = wchar_len(pi, c);
			if (nl > 1) {
				logic_drive_t one;
				std::copy(pi, pi + nl, one.name);
				result.push_back(one);
			}
			i += nl;
		}
	}
	return result;
}