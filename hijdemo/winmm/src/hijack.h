#ifndef HIJACK_H
#define HIJACK_H

#include <Windows.h>

typedef DWORD(*FnPtrTimeGetTime)();
typedef MMRESULT(*FnPtrTimeEndPeriod)(UINT);
typedef MMRESULT(*FnPtrTimeBeginPeriod)(UINT);
typedef LONG(*FnPtrMmioRead)(HMMIO, HPSTR, LONG);
typedef MMRESULT(*FnPtrMmioClose)(HMMIO, UINT);
typedef LONG(*FnPtrMmioSeek)(HMMIO, LONG, int);
typedef MMRESULT(*FnPtrMmioDescend)(HMMIO, LPMMCKINFO, const MMCKINFO*, UINT);
typedef MMRESULT(*FnPtrMmioAscend)(HMMIO, LPMMCKINFO, UINT);
typedef HMMIO(*FnPtrMmioOpenA)(
    LPSTR pszFileName,
    LPMMIOINFO pmmioinfo,
    DWORD fdwOpen);


VOID Hijack_Winmm();

#endif
