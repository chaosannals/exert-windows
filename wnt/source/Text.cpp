#include <utility>
#include "Text.h"


/*****************************************************************************
 * 多字节字符串
 *
 */
WNT_WSTRING WNT_API WNT_STRING::AsWString() CONST {
	INT size = MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, this->data, this->size, NULL, 0);
	WNT_WSTRING result(size);
	MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, this->data, this->size, result, size);
	return std::move(result);
}

WNT_STRING WNT_API WNT_STRING::Format(CONST WNT_STRING& format) {
	return format;
}


/*****************************************************************************
 * 宽字符串
 *
 */
WNT_STRING WNT_API WNT_WSTRING::AsString() CONST {
	INT size = WideCharToMultiByte(CP_UTF8, 0, this->data, this->size, NULL, 0, NULL, NULL);
	WNT_STRING result(size);
	WideCharToMultiByte(CP_UTF8, 0, this->data, this->size, result, size, NULL, NULL);
	return std::move(result);
}

WNT_WSTRING WNT_API WNT_WSTRING::Format(CONST WNT_WSTRING& format) {
	return format;
}