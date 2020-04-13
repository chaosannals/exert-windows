#pragma once

#ifdef WNT_DLL_EXPORT
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#endif

#ifdef WNT_DLL_IMPORT
#define DLL_IMPORT __declspec(dllimport)
#else
#define DLL_IMPORT
#endif

#define WNT_API DLL_EXPORT DLL_IMPORT
