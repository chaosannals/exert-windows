#pragma once

#include <vector>
#include <memory>
#include <Windows.h>

struct logic_drive_t {
	WCHAR name[10];
	WCHAR path[10];
	WCHAR device[MAX_PATH];
};

DWORD wchar_len(const WCHAR* p) {
	DWORD i = 0;
	while (p[i++] != '\0');
	return i;
}

DWORD wchar_len(const WCHAR* p, DWORD c) {
	for (DWORD i = 0; i < c; ++i) {
		if (p[i] == '\0') {
			return i + 1;
		}
	}
	return c;
}

BOOL start_with(const WCHAR* p, const WCHAR *m) {
	DWORD i = 0;
	DWORD ml = wchar_len(m);
	while (i < ml) {
		if (p[i] == '\0') {
			return m[i] == '\0';
		}
		if (m[i] == '\0') {
			return TRUE;
		}
		if (p[i] != m[i]) {
			return FALSE;
		}
		++i;
	}
	return TRUE;
}

DWORD fill_path(const std::vector<logic_drive_t> &ds, const WCHAR* p, WCHAR *r) {
	for (auto d : ds) {
		if (start_with(p, d.device)) {
			DWORD dl = wchar_len(d.device) - 1;
			DWORD nl = wchar_len(d.name) - 1;
			DWORD pl = wchar_len(p);
			DWORD n = pl - dl + nl;
			std::copy(d.name, d.name + nl, r);
			std::copy(p + dl, p + pl, r + nl);
			r[n -1] = '\0';
			return n;
		}
	}
	r[0] = '\0';
	return 0;
}

/**
 *
 */
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
				one.name[nl - 2] = '\0';
				std::copy(pi, pi + nl, one.path);
				if (!QueryDosDeviceW(one.name, one.device, MAX_PATH)) {
					one.device[0] = '\0';
				}
				result.push_back(one);
			}
			i += nl;
		}
	}
	return result;
}

