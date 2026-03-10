// WndProc Hook - Task 16
// Window procedure hook for input handling

#include "imgui.h"
#include "ui/menu_manager.h"
#include <Windows.h>

// External declarations
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Global instances (defined in dllmain.cpp)
extern class MenuManager* g_MenuManager;
extern HMODULE g_hModule;

// WndProc hook function
typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
extern WNDPROC oWndProc;

LRESULT CALLBACK hkWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    // Toggle menu with INSERT key
    if (uMsg == WM_KEYDOWN && wParam == VK_INSERT) {
        if (g_MenuManager) {
            g_MenuManager->SetVisible(!g_MenuManager->IsVisible());
        }
        return 0;
    }
    
    // Unload cheat with END key
    if (uMsg == WM_KEYDOWN && wParam == VK_END) {
        FreeLibraryAndExitThread(g_hModule, 0);
        return 0;
    }
    
    // Pass input to ImGui if menu is visible
    if (g_MenuManager && g_MenuManager->IsVisible()) {
        ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
        
        // Block game input when menu is open
        ImGuiIO& io = ImGui::GetIO();
        if (io.WantCaptureMouse || io.WantCaptureKeyboard) {
            return 1;
        }
    }
    
    // Call original WndProc
    return oWndProc(hWnd, uMsg, wParam, lParam);
}
