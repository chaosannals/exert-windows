// Dear ImGui: standalone example application for DirectX 10
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx10.h"
#include <d3d10_1.h>
#include <d3d10.h>
#include <tchar.h>
#include "man.h"
#include "inject.h"
#include "imgui_window.h"

// Main code
int main(int, char**)
{
    imgui_window_t window;
    window.create();

    // Main loop
    bool done = false;
    while (!done)
    {
        MSG msg;
        int mc = 0;
        while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            ++mc;
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;
        if (mc == 0) {
            Sleep(1);
            // imgui ��С����ռ�� CPU
            // Sleep(0); // ��Ч
            // YieldProcessor(); // ��Ч 
        }

        window.render();
    }

    window.remove();

    return 0;
}

// ��֪����������İ������� WinMain ,����ʵ�� wWinMain �е�ͻأ��
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow) {
    return main(0, nullptr);
}
