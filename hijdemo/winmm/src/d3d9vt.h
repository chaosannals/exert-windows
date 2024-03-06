#ifndef D3D9VT_H
#define D3D9VT_H

#include <d3d9.h>
typedef HRESULT(*FnPtrPresent)(LPDIRECT3DDEVICE9 device, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion);

#endif
