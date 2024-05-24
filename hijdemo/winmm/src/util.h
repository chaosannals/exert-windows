#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <format>

#ifdef UNICODE
typedef std::wstring TSTRING;
#else
typedef std::string TSTRING;
#endif

template <typename... Args>
VOID Log(const TSTRING& format, const Args &...args)
{
    auto content = std::vformat(format, std::make_format_args(args...));
    OutputDebugString(content.c_str());
}

#endif
