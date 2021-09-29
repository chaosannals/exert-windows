#pragma once

#include <Windows.h>

class pefile_t {
	DWORD length;
	char* buffer;
public:
	pefile_t(char *p, DWORD l):buffer(p), length(l) {}
	~pefile_t() {
		if (buffer) delete[] buffer;
	}
};

typedef struct NEW_IID {
	DWORD FirstThunk;
	DWORD Name;
	DWORD OriginalFirstThunk;
};

int iid_count(IMAGE_IMPORT_DESCRIPTOR* p) {
	int r = 0;
	while (p->Name != 0) {
		++p;
		++r;
	}
	return r;
}

void restore_iid(NEW_IID* np, IMAGE_IMPORT_DESCRIPTOR* op) {
	while (op->Name != NULL) {
		// 存到新地方
		np->FirstThunk = op->FirstThunk;
		np->Name = op->Name;
		np->OriginalFirstThunk = op->OriginalFirstThunk;

		// 擦除
		op->FirstThunk = 0;
		op->OriginalFirstThunk = 0;
		op->Name = 0;
		op->ForwarderChain = 0;
		op->TimeDateStamp = 0;

		// 指向下一个
		++np;
		++op;
	}
}

typedef struct NEW_IBR {

};

void restore_ibr(IMAGE_BASE_RELOCATION* np, IMAGE_BASE_RELOCATION* op) {
	while (op->SizeOfBlock != 0) {
		op = reinterpret_cast<IMAGE_BASE_RELOCATION*>(op->VirtualAddress + op->SizeOfBlock);
	}
}