// Present Hook - Task 15
// DirectX 11 Present hook for rendering overlay

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "esp/esp_renderer.h"
#include "ui/menu_manager.h"
#include "entity/entity_manager.h"
#include <d3d11.h>
#include <iostream>

// External declarations
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Global instances (defined in dllmain.cpp)
extern class ESPRenderer* g_ESPRenderer;
extern class MenuManager* g_MenuManager;
extern class EntityManager* g_EntityManager;

// Global state
static ID3D11Device* g_pd3dDevice = nullptr;
static ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
static ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;
static bool g_ImGuiInitialized = false;

// Forward declarations
void InitImGui(IDXGISwapChain* pSwapChain);
void CleanupImGui();

// Present hook function
typedef HRESULT(__stdcall* Present)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
extern Present oPresent;

HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
    // Initialize ImGui on first call
    if (!g_ImGuiInitialized) {
        InitImGui(pSwapChain);
        g_ImGuiInitialized = true;
    }
    
    // Start ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    
    // Update entity manager
    if (g_EntityManager) {
        g_EntityManager->Update();
    }
    
    // Render ESP
    if (g_ESPRenderer) {
        g_ESPRenderer->Render();
    }
    
    // Render menu
    if (g_MenuManager && g_MenuManager->IsVisible()) {
        g_MenuManager->Render();
    }
    
    // Render ImGui
    ImGui::Render();
    
    // Set render target
    g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
    
    // Render ImGui draw data
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    
    // Call original Present
    return oPresent(pSwapChain, SyncInterval, Flags);
}

void InitImGui(IDXGISwapChain* pSwapChain) {
    std::cout << "[*] Initializing ImGui..." << std::endl;
    
    // Get device and context
    if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&g_pd3dDevice))) {
        g_pd3dDevice->GetImmediateContext(&g_pd3dDeviceContext);
        
        // Get back buffer
        ID3D11Texture2D* pBackBuffer = nullptr;
        pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
        
        if (pBackBuffer) {
            g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
            pBackBuffer->Release();
        }
    }
    
    // Get window handle
    DXGI_SWAP_CHAIN_DESC sd;
    pSwapChain->GetDesc(&sd);
    HWND hWnd = sd.OutputWindow;
    
    // Setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
    
    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hWnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);
    
    // Setup style (will be overridden by Neverlose style in Task 17)
    ImGui::StyleColorsDark();
    
    std::cout << "[+] ImGui initialized successfully" << std::endl;
}

void CleanupImGui() {
    if (g_ImGuiInitialized) {
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
        
        if (g_mainRenderTargetView) {
            g_mainRenderTargetView->Release();
            g_mainRenderTargetView = nullptr;
        }
        
        if (g_pd3dDeviceContext) {
            g_pd3dDeviceContext->Release();
            g_pd3dDeviceContext = nullptr;
        }
        
        if (g_pd3dDevice) {
            g_pd3dDevice->Release();
            g_pd3dDevice = nullptr;
        }
        
        g_ImGuiInitialized = false;
    }
}
