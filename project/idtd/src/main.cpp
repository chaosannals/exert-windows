#include<Windows.h>
#include<cstddef>
#include<exert/base.h>
#include<exert/exploit/runtime.h>
#include<exert/exploit/filesystem.h>

int WINAPI WinMain(HINSTANCE self, HINSTANCE prev, LPSTR argv, int argc) {
	ext::init_heap();
	ext::file_put(L"idtd.log", L"Hello");
	// int *a = new int(123);
	// delete a;
	return 0;
}