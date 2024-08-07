#include <Windows.h>
#include <strsafe.h>

int main() {
	DWORD errcode = 0;
	TCHAR output[1024] = { 0 };
	SIZE_T size = 10;
	PVOID p = NULL;

	Sleep(1444);
	while (1) {
		PVOID pnow = VirtualAlloc(
			NULL,
			size,
			MEM_COMMIT,
			PAGE_READWRITE
		);
		if (pnow != NULL) {
			if (p != NULL) {
				VirtualFree(p, 0, MEM_RELEASE);
				size *= 10;
			}
			p = pnow;
			for (SIZE_T i = 0; i < size; ++i) {
				*(char*)p = 1;
			}
		}
		StringCchPrintf(output, 1024, L"pointer: %llx size: %llu \r\n", (SIZE_T)p, size);
		OutputDebugString(output);
		Sleep(4);
	}
}