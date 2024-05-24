#include <Windows.h>
#include <detours.h>
#include <thread>
#include <chrono>
#include <d3d9.h>
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_dx9.h>
#include "util.h"
#include "hijack.h"
#include "d3d9vt.h"

extern "C" FnPtrPresent HookD3d9Device(LPDIRECT3DDEVICE9 device);
FnPtrPresent g_persentPtr;

typedef HWND (*FnPtrCreateWindowExA)(
    DWORD dwExStyle,
    LPCSTR lpClassName,
    LPCSTR lpWindowName,
    DWORD dwStyle,
    int X,
    int Y,
    int nWidth,
    int nHeight,
    HWND hWndParent,
    HMENU hMenu,
    HINSTANCE hInstance,
    LPVOID lpParam);
typedef ATOM (*FnPtrRegisterClassA)(
    const WNDCLASSA* lpWndClass
);
typedef ATOM(*FnPtrRegisterClassExA)(
    const WNDCLASSEXA* unnamedParam1
);
typedef BOOL (*FnPtrPeekMessageA)(
    LPMSG lpMsg,
    HWND  hWnd,
    UINT  wMsgFilterMin,
     UINT  wMsgFilterMax,
    UINT  wRemoveMsg
);
typedef BOOL(*FnPtrPeekMessageW)(
    LPMSG lpMsg,
    HWND  hWnd,
    UINT  wMsgFilterMin,
     UINT  wMsgFilterMax,
    UINT  wRemoveMsg
);

typedef IDirect3D9* (*FnPtrDirect3DCreate9)(
    UINT SDKVersion
);
typedef HRESULT (*FnPtrDirect3DCreate9Ex)(
    UINT         SDKVersion,
    IDirect3D9Ex** unnamedParam2
);


static LPDIRECT3D9              g_pD3D = nullptr;
static LPDIRECT3DDEVICE9        g_pd3dDevice = nullptr;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};
static HWND g_wnd = nullptr;
static long long g_render_at = 0;

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
VOID renderD3d9(HWND wnd);

FnPtrCreateWindowExA createWindowExAPtr;
FnPtrRegisterClassA registerClassAPtr;
FnPtrRegisterClassExA registerClassExAPtr;
FnPtrPeekMessageA peekMessageAPtr;
FnPtrPeekMessageW peekMessageWPtr;
FnPtrDirect3DCreate9 direct3DCreate9Ptr;
FnPtrDirect3DCreate9Ex direct3DCreate9ExPtr;

// TODO LoadLibrary

WNDPROC wndProcPtr;

HRESULT MyPresent(LPDIRECT3DDEVICE9 device, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion) {
    Log("[winmm] MyPresent");
    return g_persentPtr(device, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

struct MyDirect3D9 : public IDirect3D9 {
    IDirect3D9* d3d9;

    MyDirect3D9(IDirect3D9* p) :d3d9(p) {}

    virtual ~MyDirect3D9() {
        if (d3d9) {
            d3d9->~IDirect3D9();
        }
    }

    /*** IUnknown methods ***/
    virtual HRESULT QueryInterface(THIS_ REFIID riid, void** ppvObj) {
        Log("[winmm] d3d9 QueryInterface");
        return d3d9->QueryInterface(riid, ppvObj);
    }
    virtual ULONG AddRef() {
        Log("[winmm] d3d9 AddRef");
        return d3d9->AddRef();
    }
    virtual ULONG Release() {
        Log("[winmm] d3d9 Release");
        return d3d9->Release();
    }

    /*** IDirect3D9 methods ***/
    virtual HRESULT RegisterSoftwareDevice(THIS_ void* pInitializeFunction) {
        return d3d9->RegisterSoftwareDevice(pInitializeFunction);
    }
    virtual UINT GetAdapterCount() {
        return d3d9->GetAdapterCount();
    }
    virtual HRESULT GetAdapterIdentifier(THIS_ UINT Adapter, DWORD Flags, D3DADAPTER_IDENTIFIER9* pIdentifier) {
        return d3d9->GetAdapterIdentifier(Adapter, Flags, pIdentifier);
    }
    virtual UINT GetAdapterModeCount(THIS_ UINT Adapter, D3DFORMAT Format) {
        return d3d9->GetAdapterModeCount(Adapter, Format);
    }
    virtual HRESULT EnumAdapterModes(THIS_ UINT Adapter, D3DFORMAT Format, UINT Mode, D3DDISPLAYMODE* pMode) {
        return d3d9->EnumAdapterModes(Adapter, Format, Mode, pMode);
    }
    virtual HRESULT GetAdapterDisplayMode(THIS_ UINT Adapter, D3DDISPLAYMODE* pMode) {
        return d3d9->GetAdapterDisplayMode(Adapter, pMode);
    }
    virtual HRESULT CheckDeviceType(THIS_ UINT Adapter, D3DDEVTYPE DevType, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, BOOL bWindowed) {
        return d3d9->CheckDeviceType(Adapter, DevType, AdapterFormat, BackBufferFormat, bWindowed);
    }
    virtual HRESULT CheckDeviceFormat(THIS_ UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, DWORD Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat) {
        return d3d9->CheckDeviceFormat(Adapter, DeviceType, AdapterFormat, Usage, RType, CheckFormat);
    }
    virtual HRESULT CheckDeviceMultiSampleType(THIS_ UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SurfaceFormat, BOOL Windowed, D3DMULTISAMPLE_TYPE MultiSampleType, DWORD* pQualityLevels) {
        return d3d9->CheckDeviceMultiSampleType(Adapter, DeviceType, SurfaceFormat, Windowed, MultiSampleType, pQualityLevels);
    }
    virtual HRESULT CheckDepthStencilMatch(THIS_ UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat) {
        return d3d9->CheckDepthStencilMatch(Adapter, DeviceType, AdapterFormat, RenderTargetFormat, DepthStencilFormat);
    }
    virtual HRESULT CheckDeviceFormatConversion(THIS_ UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SourceFormat, D3DFORMAT TargetFormat) {
        return d3d9->CheckDeviceFormatConversion(Adapter, DeviceType, SourceFormat, TargetFormat);
    }
    virtual HRESULT GetDeviceCaps(THIS_ UINT Adapter, D3DDEVTYPE DeviceType, D3DCAPS9* pCaps) {
        return d3d9->GetDeviceCaps(Adapter, DeviceType, pCaps);
    }
    virtual HMONITOR GetAdapterMonitor(THIS_ UINT Adapter) {
        return d3d9->GetAdapterMonitor(Adapter);
    }
    virtual HRESULT(CreateDevice)(THIS_ UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DDevice9** ppReturnedDeviceInterface) {
        Log("[winmm] d3d9 CreateDevice");
        HRESULT r = d3d9->CreateDevice(Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, ppReturnedDeviceInterface);
        Log("[winmm] d3d9 CreateDevice p; {}", reinterpret_cast<std::size_t>(*ppReturnedDeviceInterface));
        g_pd3dDevice = *ppReturnedDeviceInterface;
        g_persentPtr = HookD3d9Device(g_pd3dDevice);
        Log("[winmm] d3d9 persent p; {}", reinterpret_cast<std::size_t>(g_persentPtr));

        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());

        // 不报错，但是没有效果，不知道是不是这里太迟了。
        DetourAttach(&g_persentPtr, MyPresent);

        LONG dr = DetourTransactionCommit();
        if (dr != NO_ERROR)
        {
            Log(
                "[winmm] ERROR_INVALID_BLOCK: {} | ERROR_INVALID_HANDLE: {} | ERROR_INVALID_OPERATION: {} | ERROR_NOT_ENOUGH_MEMORY: {}",
                ERROR_INVALID_BLOCK,
                ERROR_INVALID_HANDLE,
                ERROR_INVALID_OPERATION,
                ERROR_NOT_ENOUGH_MEMORY);
            Log("[winmm] detours attach error: {}", dr);
        }

        return r;
    }

};

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


VOID MyTimer(HWND hwnd, UINT msg, UINT_PTR timerId, DWORD dwTime) {
    Log("[winmm] timer {} tick {}", timerId, dwTime);
    renderD3d9(hwnd);
}

//bool CreateDeviceD3D(HWND hWnd)
//{
//    if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == nullptr) {
//        Log("[winmm] create d3d9 failed.");
//        return false;
//    }
//
//    // Create the D3DDevice
//    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
//    g_d3dpp.Windowed = TRUE;
//    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
//    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN; // Need to use an explicit format with alpha if needing per-pixel alpha composition.
//    g_d3dpp.EnableAutoDepthStencil = TRUE;
//    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
//    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           // Present with vsync
//    //g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
//    if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0) {
//        Log("[winmm] create d3d9 device failed.");
//        return false;
//    }
//    Log("[winmm] create d3d9 device.");
//    return true;
//}

//void CleanupDeviceD3D()
//{
//    Log("[winmm] CleanupDeviceD3D.");
//    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
//    //if (g_pD3D) { g_pD3D->Release(); g_pD3D = nullptr; }
//}

//void ResetDevice()
//{
//    Log("[winmm] ResetDevice.");
//    ImGui_ImplDX9_InvalidateDeviceObjects();
//    HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
//    if (hr == D3DERR_INVALIDCALL)
//        IM_ASSERT(0);
//    ImGui_ImplDX9_CreateDeviceObjects();
//}

LRESULT WINAPI MyWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            break;
            //return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
        g_ResizeHeight = (UINT)HIWORD(lParam);
        Log("[winmm] window size. {}, {}", g_ResizeWidth, g_ResizeHeight);
        break;
    case WM_SHOWWINDOW:
        if (g_pD3D == nullptr) {
            Log("[winmm] window show.");

            //if (CreateDeviceD3D(hWnd)) {
            //    Log("[winmm] d3d9 render start.");
            //    g_wnd = hWnd;
            //    SetTimer(hWnd, 4444, 100, MyTimer);
                // TODO
                //std::thread d3dthread(renderD3d9, hWnd);
            //}

           /* g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

            if (g_pD3D == nullptr) {
                Log("[winmm] d3d9 null.");
                break;
            }

            Log("[winmm] d3d9 create.");*/
            

            // std::thread d3dthread(renderD3d9, hWnd);
        }
        break;
        //return 0;
    //case WM_SYSCOMMAND:
    //    if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
    //        return 0;
    //    break;
    //case WM_DESTROY:
    //    ::PostQuitMessage(0);
    //    return 0;
    }

    return wndProcPtr(hWnd, msg, wParam, lParam);
}

ATOM MyRegisterClassA(
    const WNDCLASSA* lpWndClass
) {
#ifdef HIJ_TARGET_WND_CLASS
    std::string targetClass = HIJ_TARGET_WND_CLASS;
    if (targetClass.compare(lpWndClass->lpszClassName) == 0)
    {
        wndProcPtr = lpWndClass->lpfnWndProc;
        ((WNDCLASSA*)lpWndClass)->lpfnWndProc = MyWndProc;
        Log("[winmm] {} wndproc: {} => {}", lpWndClass->lpszClassName, reinterpret_cast<std::size_t>(wndProcPtr), reinterpret_cast<std::size_t>(&MyWndProc));
    }
#endif
    return registerClassAPtr(lpWndClass);
}

ATOM MyRegisterClassExA(
    const WNDCLASSEXA* lpWndClass
) {
#ifdef HIJ_TARGET_WND_CLASS
    std::string targetClass = HIJ_TARGET_WND_CLASS;
    if (targetClass.compare(lpWndClass->lpszClassName) == 0)
    {
        wndProcPtr = lpWndClass->lpfnWndProc;
        ((WNDCLASSA*)lpWndClass)->lpfnWndProc = MyWndProc;
        Log("[winmm] {} wndproc ex: {} => {}", lpWndClass->lpszClassName, reinterpret_cast<std::size_t>(wndProcPtr), reinterpret_cast<std::size_t>(&MyWndProc));
    }
#endif
    return registerClassExAPtr(lpWndClass);
}


// TODO 在目标窗口上开子窗口，不然会和原程序渲染互相刷掉，闪烁。
VOID renderD3d9(HWND wnd) {
    Log("[winmm] d3d9 thread start.");

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(wnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    //bool done = false;
    //while (!done) {
        // Handle window resize (we don't resize directly in the WM_SIZE handler)
        if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        {
            //g_d3dpp.BackBufferWidth = g_ResizeWidth;
            //g_d3dpp.BackBufferHeight = g_ResizeHeight;
            //g_ResizeWidth = g_ResizeHeight = 0;
            //ResetDevice();
        }

        // Start the Dear ImGui frame
        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

        // Rendering
        ImGui::EndFrame();
        g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
        D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x * clear_color.w * 255.0f), (int)(clear_color.y * clear_color.w * 255.0f), (int)(clear_color.z * clear_color.w * 255.0f), (int)(clear_color.w * 255.0f));
        g_pd3dDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
        if (g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
        }
        HRESULT result = g_pd3dDevice->Present(nullptr, nullptr, nullptr, nullptr);

        // Handle loss of D3D9 device
        /*if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
            ResetDevice();*/
    //}
}

HWND MyCreateWindowExA(
    DWORD dwExStyle,
    LPCSTR lpClassName,
    LPCSTR lpWindowName,
    DWORD dwStyle,
    int X,
    int Y,
    int nWidth,
    int nHeight,
    HWND hWndParent,
    HMENU hMenu,
    HINSTANCE hInstance,
    LPVOID lpParam)
{
    Log("[winmm] create window a: {} by {}", lpWindowName, lpClassName);

    HWND wnd = createWindowExAPtr(
        dwExStyle,
        lpClassName,
        lpWindowName,
        dwStyle,
        X,
        Y,
        nWidth,
        nHeight,
        hWndParent,
        hMenu,
        hInstance,
        lpParam);

#ifdef HIJ_TARGET_WND_CLASS
    std::string targetClass = HIJ_TARGET_WND_CLASS;
    if (targetClass.compare(lpClassName) == 0)
    {
        Log("[winmm] is target window: {} by {} {}", lpWindowName, lpClassName, reinterpret_cast<std::size_t>(wnd));
        auto wpp = (WNDPROC)GetWindowLongPtr(wnd, GWLP_WNDPROC);
        Log("[winmm] is target window proc: {} ", reinterpret_cast<std::size_t>(wpp));
    }
#endif

    return wnd;
}

BOOL MyPeekMessageA(
    LPMSG lpMsg,
    HWND  hWnd,
    UINT  wMsgFilterMin,
    UINT  wMsgFilterMax,
    UINT  wRemoveMsg
) {
    /*
    Log(
        "[winmm] peek message a: {} the {} at {}",
        reinterpret_cast<std::size_t>(hWnd),
        lpMsg->message,
        lpMsg->time
    );
    */
    //if (g_wnd == hWnd) {
    if (g_wnd) {
        auto now = std::chrono::high_resolution_clock::now().time_since_epoch();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now).count();
        auto dms = ms - g_render_at;
        if (dms > 100) {
            g_render_at = ms;
            //renderD3d9(g_wnd);
        }
    }
    return peekMessageAPtr(
        lpMsg,
        hWnd,
        wMsgFilterMin,
        wMsgFilterMax,
        wRemoveMsg
    );
}

BOOL MyPeekMessageW(
    LPMSG lpMsg,
    HWND  hWnd,
    UINT  wMsgFilterMin,
    UINT  wMsgFilterMax,
    UINT  wRemoveMsg
) {
    /*
    Log(
        "[winmm] peek message w: {} the {} at {}",
        reinterpret_cast<std::size_t>(hWnd),
        lpMsg->message,
        lpMsg->time
    );
    */
    return peekMessageWPtr(
        lpMsg,
        hWnd,
        wMsgFilterMin,
        wMsgFilterMax,
        wRemoveMsg
    );
}

IDirect3D9* MyDirect3DCreate9(
    UINT SDKVersion
) {
    Log("[winmm] MyDirect3DCreate9: {}", SDKVersion);
    IDirect3D9 *p = direct3DCreate9Ptr(SDKVersion);
    Log("[winmm] MyDirect3DCreate9 p: {}", reinterpret_cast<std::size_t>(p));
    g_pD3D = p;
    return new MyDirect3D9(p);
}

HRESULT MyDirect3DCreate9Ex(
    UINT         SDKVersion,
    IDirect3D9Ex** unnamedParam2
) {
    Log("[winmm] MyDirect3DCreate9Ex: {}", SDKVersion);
    return direct3DCreate9ExPtr(SDKVersion, unnamedParam2);
}

VOID AttachHij()
{
    Hijack_Winmm();

    LONG error = DetourTransactionBegin();
    if (error != NO_ERROR)
    {
        Log("[winmm] ERROR_INVALID_OPERATION: {}, {}", ERROR_INVALID_OPERATION, error);
        return;
    }
    error = DetourUpdateThread(GetCurrentThread());
    if (error != NO_ERROR)
    {
        Log("[winmm] ERROR_NOT_ENOUGH_MEMORY: {}, {}", ERROR_NOT_ENOUGH_MEMORY, error);
        return;
    }
    //auto user32Path = std::format(TEXT("{}\\User32.dll"), systemDir);
    //Log("[winmm] user32: {}", user32Path);
    createWindowExAPtr = (FnPtrCreateWindowExA)DetourFindFunction("User32.dll", "CreateWindowExA");
    registerClassAPtr = (FnPtrRegisterClassA)DetourFindFunction("User32.dll", "RegisterClassA");
    registerClassExAPtr = (FnPtrRegisterClassExA)DetourFindFunction("User32.dll", "RegisterClassExA");
    peekMessageAPtr= (FnPtrPeekMessageA)DetourFindFunction("User32.dll", "PeekMessageA");
    peekMessageWPtr= (FnPtrPeekMessageW)DetourFindFunction("User32.dll", "PeekMessageW");
    direct3DCreate9Ptr = (FnPtrDirect3DCreate9)DetourFindFunction("d3d9.dll", "Direct3DCreate9");
    direct3DCreate9ExPtr = (FnPtrDirect3DCreate9Ex)DetourFindFunction("d3d9.dll", "Direct3DCreate9Ex");
    if (
        createWindowExAPtr == nullptr ||
        registerClassAPtr == nullptr ||
        registerClassExAPtr==nullptr ||
        peekMessageAPtr == nullptr ||
        peekMessageWPtr == nullptr ||
        direct3DCreate9Ptr == nullptr ||
        direct3DCreate9ExPtr == nullptr
    ) {
        Log(
            "[winmm] CreateWindowExA({}) or registerClassAPtr({}) or registerClassExAPtr({}) or peekMessageAPtr({}) or peekMessageWPtr({}) or direct3DCreate9Ptr({}) or direct3DCreate9ExPtr({}) DetourFindFunction failed",
            reinterpret_cast<std::size_t>(createWindowExAPtr),
            reinterpret_cast<std::size_t>(registerClassAPtr),
            reinterpret_cast<std::size_t>(registerClassExAPtr),
            reinterpret_cast<std::size_t>(peekMessageAPtr),
            reinterpret_cast<std::size_t>(peekMessageWPtr),
            reinterpret_cast<std::size_t>(direct3DCreate9Ptr),
            reinterpret_cast<std::size_t>(direct3DCreate9ExPtr)
        );
        return;
    }
    DetourAttach(&createWindowExAPtr, MyCreateWindowExA);
    DetourAttach(&registerClassAPtr, MyRegisterClassA);
    DetourAttach(&registerClassExAPtr, MyRegisterClassExA);
    DetourAttach(&peekMessageAPtr, MyPeekMessageA);
    DetourAttach(&peekMessageWPtr, MyPeekMessageW);
    DetourAttach(&direct3DCreate9Ptr, MyDirect3DCreate9);
    DetourAttach(&direct3DCreate9ExPtr, MyDirect3DCreate9Ex);
    LONG r = DetourTransactionCommit();
    if (r != NO_ERROR)
    {
        Log(
            "[winmm] ERROR_INVALID_BLOCK: {} | ERROR_INVALID_HANDLE: {} | ERROR_INVALID_OPERATION: {} | ERROR_NOT_ENOUGH_MEMORY: {}",
            ERROR_INVALID_BLOCK,
            ERROR_INVALID_HANDLE,
            ERROR_INVALID_OPERATION,
            ERROR_NOT_ENOUGH_MEMORY);
        Log("[winmm] detours attach error: {}", r);
    }
}

VOID DetachHij()
{
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    //CleanupDeviceD3D();

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&createWindowExAPtr, MyCreateWindowExA);
    DetourDetach(&registerClassAPtr, MyRegisterClassA);
    DetourDetach(&registerClassExAPtr, MyRegisterClassExA);
    DetourDetach(&peekMessageAPtr, MyPeekMessageA);
    DetourDetach(&peekMessageWPtr, MyPeekMessageW);
    DetourDetach(&direct3DCreate9Ptr, MyDirect3DCreate9);
    DetourDetach(&direct3DCreate9ExPtr, MyDirect3DCreate9Ex);
    LONG r = DetourTransactionCommit();
    if (r != NO_ERROR)
    {
        Log("[winmm] detours detach error: {}", r);
    }
}

BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved)
{
    switch (reason)
    {
    case DLL_PROCESS_ATTACH:
        // MessageBox(NULL, TEXT("I'm winmm.dll, attach."), "Info", MB_OK);
        OutputDebugString("[winmm] attach.");
        AttachHij();
        break;
    case DLL_PROCESS_DETACH:
        // MessageBox(NULL, TEXT("I'm winmm.dll, detach."), "Info", MB_OK);
        OutputDebugString("[winmm] detach.");
        DetachHij();
        break;
    }

    return TRUE;
}
