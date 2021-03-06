#include<Windows.h>
#include "util.h"

int make_file() {
	closer_t<HANDLE> handle = CreateFileW(
		L"idtd.log",
		GENERIC_WRITE,
		0,
		0,
		CREATE_NEW,
		FILE_ATTRIBUTE_NORMAL,
		0
	);
	if (INVALID_HANDLE_VALUE == handle) {
		return 1;
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE self, HINSTANCE prev, LPSTR argv, int argc) {
	make_file();
	return 0;
}