#include "d3d9vt.h"

static FnPtrPresent persentPtr;

// 直接修改 C 暴露的虚表字段无效。
//HRESULT MyPresent(LPDIRECT3DDEVICE9 device, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion) {
//    OutputDebugString("[winmm] MyPresent");
//    return persentPtr(device, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
//}

FnPtrPresent HookD3d9Device(LPDIRECT3DDEVICE9 device) {
    // 可以读到真实的地址。但不能改。
    persentPtr = device->lpVtbl->Present;

    // 无效 不能直接替换，这个结构只是 COM 暴露给外部 C 调用的函数查找表而已，内部实现不依赖这个表。
    // device->lpVtbl->Present = MyPresent;
    OutputDebugString("[winmm] Hook Present");
    return persentPtr;
}
