#include "imgui_window.h"
#include "inject.h"
#include "man.h"
#include <map>

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

struct imgui_window_state_t {
    HWND window_handle = NULL;
    ID3D10Device* pd3dDevice = NULL;
    IDXGISwapChain* pSwapChain = NULL;
    ID3D10RenderTargetView* mainRenderTargetView = NULL;
    ManGui::ManBox man_box;
    InjectGui::InjectBox inject_box;
};

TCHAR imgui_window_class_name[] = TEXT("ImGui Example");
static std::map<HWND, imgui_window_t*> allin;

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI imgui_window_t::process_message(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    auto iw = allin.find(hWnd);

    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (iw != allin.end() && iw->second->state != nullptr) {
            if (iw->second->state->pd3dDevice != NULL && wParam != SIZE_MINIMIZED) {
                iw->second->cleanup_render_target();
                if (iw->second->state->pSwapChain) {
                    iw->second->state->pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
                }
                iw->second->create_render_target();
            }
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}


imgui_window_t::imgui_window_t() :state(nullptr) {
    state = new imgui_window_state_t();
}

void imgui_window_t::create() {
    // Create application window
    //ImGui_ImplWin32_EnableDpiAwareness();
    WNDCLASSEX wc = {
        sizeof(WNDCLASSEX),
        CS_CLASSDC,
        process_message,
        0L, 0L,
        GetModuleHandle(NULL),
        NULL, NULL, NULL, NULL,
        imgui_window_class_name,
        NULL
    };

    ::RegisterClassEx(&wc);

    HWND hwnd = ::CreateWindow(
        wc.lpszClassName,
        _T("Dear ImGui DirectX10 Example"),
        WS_OVERLAPPEDWINDOW,
        100, 100, 1280, 800,
        NULL, NULL,
        wc.hInstance,
        NULL
    );

    if (state) {
        state->window_handle = hwnd;
        allin.emplace(hwnd, this);
    }

    // Initialize Direct3D
    if (!create_d3ddevice(hwnd))
    {
        cleanup_d3ddevice();
        UnregisterClass(wc.lpszClassName, wc.hInstance);
        return;
    }

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    //ImGui::StyleColorsDark();
    ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX10_Init(state->pd3dDevice);

    ImFont* font = io.Fonts->AddFontFromFileTTF("./SourceHanSerifCN-Light.ttf", 16.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());
    IM_ASSERT(font != NULL);

    // Our state
    ManGui::ManBox man_box;
    InjectGui::InjectBox inject_box;
}

void imgui_window_t::remove() {
    if (state) {
        ImGui_ImplDX10_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();

        cleanup_d3ddevice();
        if (state->window_handle) {
            DestroyWindow(state->window_handle);
            state->window_handle = nullptr;
        }
        UnregisterClass(
            imgui_window_class_name,
            GetModuleHandle(NULL)
        );

        delete state;
        state = nullptr;
    }
}

void imgui_window_t::render() {
    // Start the Dear ImGui frame
    ImGui_ImplDX10_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    if (state) {
        state->man_box.Render();
        state->inject_box.Render();

        // Rendering
        ImGui::Render();
        const ImVec4& clear_color = state->man_box.peek_clear_color();
        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        state->pd3dDevice->OMSetRenderTargets(1, &state->mainRenderTargetView, NULL);
        state->pd3dDevice->ClearRenderTargetView(state->mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX10_RenderDrawData(ImGui::GetDrawData());

        state->pSwapChain->Present(1, 0); // Present with vsync
        //g_pSwapChain->Present(0, 0); // Present without vsync
    }
}

bool imgui_window_t::create_d3ddevice(HWND hWnd)
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    //createDeviceFlags |= D3D10_CREATE_DEVICE_DEBUG;
    if (D3D10CreateDeviceAndSwapChain(
        NULL,
        D3D10_DRIVER_TYPE_HARDWARE,
        NULL,
        createDeviceFlags,
        D3D10_SDK_VERSION,
        &sd,
        &state->pSwapChain,
        &state->pd3dDevice) != S_OK) {
        return false;
    }

    create_render_target();
    return true;
}

void imgui_window_t::cleanup_d3ddevice()
{
    cleanup_render_target();
    if (state) {
        if (state->pSwapChain) {
            state->pSwapChain->Release();
            state->pSwapChain = NULL;
        }
        if (state->pd3dDevice) {
            state->pd3dDevice->Release();
            state->pd3dDevice = NULL;
        }
    }
}

void imgui_window_t::create_render_target()
{
    if (state) {
        ID3D10Texture2D* pBackBuffer = nullptr;
        if (state->pSwapChain) {
            state->pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
        }
        if (state->pd3dDevice && pBackBuffer != NULL) {
            state->pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &state->mainRenderTargetView);
        }
        if (pBackBuffer) {
            pBackBuffer->Release();
        }
    }
}

void imgui_window_t::cleanup_render_target()
{
    if (state) {
        if (state->mainRenderTargetView) {
            state->mainRenderTargetView->Release();
            state->mainRenderTargetView = NULL;
        }
    }
}
