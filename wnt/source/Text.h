#pragma once

#include <Windows.h>
#include "Ally.h"
#include "Data.h"

class WNT_STRING;
class WNT_WSTRING;

/*****************************************************************************
 * 多字节字符串
 * 
 */
class WNT_API WNT_STRING {
	INT size;
	CHAR* data;
public:
	WNT_STRING(INT size) :
		size(size), data(NULL) {
		data = new CHAR[size];
	}
	WNT_STRING(CONST CHAR* source) :
		size(WntCountText(source)),
		data(NULL) {
		data = WntClone(source, size);
	}
	WNT_STRING(CONST WNT_STRING& other) :
		WNT_STRING(other.size) {
		data = WntClone(other.data, size);
	}
	WNT_STRING(WNT_STRING&& other) :
		size(other.size), data(other.data) {
		other.size = 0;
		other.data = NULL;
	}
	~WNT_STRING() {
		if (data != NULL) {
			delete[] data;
		}
	}
	operator CHAR* () CONST {
		return data;
	}
	CONST INT GetSize() CONST {
		return size;
	}
	WNT_WSTRING AsWString() CONST;
	WNT_STRING Format(CONST WNT_STRING& format);
};

/*****************************************************************************
 * 宽字符串
 * 
 */
class WNT_API WNT_WSTRING {
	INT size;
	WCHAR* data;
public:
	WNT_WSTRING(INT size) :
		size(size), data(NULL) {
		data = new WCHAR[size];
	}
	WNT_WSTRING(CONST WCHAR* source) :
		size(WntCountText(source)), data(NULL) {
		data = WntClone(source, size);
	}
	WNT_WSTRING(CONST WNT_WSTRING& other) :
		WNT_WSTRING(other.size) {
		data = WntClone(other.data, size);
	}
	WNT_WSTRING(WNT_WSTRING&& other) :
		size(other.size), data(other.data) {
		other.size = 0;
		other.data = NULL;
	}
	~WNT_WSTRING() {
		if (data != NULL) {
			delete[] data;
		}
	}
	operator WCHAR* () CONST {
		return data;
	}
	CONST INT GetSize() CONST {
		return size;
	}
	WNT_STRING AsString() CONST;
	WNT_WSTRING Format(CONST WNT_WSTRING& format);
};

#ifdef UNICODE
typedef WNT_WSTRING WNT_TEXT_T;
#else
typedef WNT_STRING WNT_TEXT_T;
#endif
