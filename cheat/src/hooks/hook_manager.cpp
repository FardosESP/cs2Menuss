// Hook Manager - Task 14
// Real DirectX 11 hooking with MinHook

#include "hooks/hook_manager.h"
#include "MinHook.h"
#include <d3d11.h>
#include <iostream>
#include <dxgi.h>

// Global pointers for hooks
typedef HRESULT(__stdcall* Present)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);

Present oPresent = nullptr;
WNDPROC oWndProc = nullptr;

// Forward declarations of hook functions
HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
LRESULT CALLBACK hkWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

bool HookManager::Initialize() {
    std::cout << "[*] Initializing Hook Manager..." << std::endl;
    
    __try {
        // Initialize MinHook
        if (MH_Initialize() != MH_OK) {
            std::cerr << "[!] Failed to initialize MinHook" << std::endl;
            return false;
        }
        
        std::cout << "[+] MinHook initialized successfully" << std::endl;
        
        // Hook Present and WndProc
        if (!HookPresent()) {
            std::cerr << "[!] Failed to hook Present" << std::endl;
            MH_Uninitialize();
            return false;
        }
        
        if (!HookWndProc()) {
            std::cerr << "[!] Failed to hook WndProc" << std::endl;
            MH_Uninitialize();
            return false;
        }
        
        hooksInstalled = true;
        std::cout << "[+] All hooks installed successfully" << std::endl;
        
        return true;
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        std::cerr << "[!] Exception during hook initialization: 0x" << std::hex << GetExceptionCode() << std::endl;
        return false;
    }
}

bool HookManager::HookPresent() {
    std::cout << "[*] Hooking Present..." << std::endl;
    
    // Get D3D11 device and swap chain
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, DefWindowProc, 0L, 0L, 
                      GetModuleHandle(NULL), NULL, NULL, NULL, NULL, L"DX", NULL };
    RegisterClassEx(&wc);
    HWND hWnd = CreateWindow(wc.lpszClassName, NULL, WS_OVERLAPPEDWINDOW, 
                             100, 100, 300, 300, NULL, NULL, wc.hInstance, NULL);
    
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };
    
    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = hWnd;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.Windowed = TRUE;
    swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    
    IDXGISwapChain* pSwapChain = nullptr;
    ID3D11Device* pDevice = nullptr;
    ID3D11DeviceContext* pContext = nullptr;
    
    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, featureLevels, 2,
        D3D11_SDK_VERSION, &swapChainDesc, &pSwapChain, &pDevice, &featureLevel, &pContext
    );
    
    if (FAILED(hr)) {
        std::cerr << "[!] Failed to create D3D11 device" << std::endl;
        DestroyWindow(hWnd);
        UnregisterClass(wc.lpszClassName, wc.hInstance);
        return false;
    }
    
    // Get Present function address from vtable
    void** pVTable = *reinterpret_cast<void***>(pSwapChain);
    void* pPresentAddr = pVTable[8]; // Present is at index 8 in IDXGISwapChain vtable
    
    // Create hook
    if (MH_CreateHook(pPresentAddr, &hkPresent, reinterpret_cast<LPVOID*>(&oPresent)) != MH_OK) {
        std::cerr << "[!] Failed to create Present hook" << std::endl;
        pSwapChain->Release();
        pDevice->Release();
        pContext->Release();
        DestroyWindow(hWnd);
        UnregisterClass(wc.lpszClassName, wc.hInstance);
        return false;
    }
    
    // Enable hook
    if (MH_EnableHook(pPresentAddr) != MH_OK) {
        std::cerr << "[!] Failed to enable Present hook" << std::endl;
        pSwapChain->Release();
        pDevice->Release();
        pContext->Release();
        DestroyWindow(hWnd);
        UnregisterClass(wc.lpszClassName, wc.hInstance);
        return false;
    }
    
    originalPresent = oPresent;
    
    // Cleanup
    pSwapChain->Release();
    pDevice->Release();
    pContext->Release();
    DestroyWindow(hWnd);
    UnregisterClass(wc.lpszClassName, wc.hInstance);
    
    std::cout << "[+] Present hooked successfully at 0x" << std::hex << pPresentAddr << std::dec << std::endl;
    
    return true;
}

bool HookManager::HookWndProc() {
    std::cout << "[*] Hooking WndProc..." << std::endl;
    
    // Try multiple window class names for CS2
    HWND hWnd = nullptr;
    const char* windowClasses[] = { "SDL_app", "Valve001", nullptr };
    const char* windowTitles[] = { "Counter-Strike 2", "CS2", nullptr };
    
    // Try by class name first
    for (int i = 0; windowClasses[i] != nullptr; i++) {
        hWnd = FindWindowA(windowClasses[i], nullptr);
        if (hWnd) {
            std::cout << "[+] Found CS2 window by class: " << windowClasses[i] << std::endl;
            break;
        }
    }
    
    // Try by window title if class name failed
    if (!hWnd) {
        for (int i = 0; windowTitles[i] != nullptr; i++) {
            hWnd = FindWindowA(nullptr, windowTitles[i]);
            if (hWnd) {
                std::cout << "[+] Found CS2 window by title: " << windowTitles[i] << std::endl;
                break;
            }
        }
    }
    
    if (!hWnd) {
        std::cerr << "[!] Failed to find CS2 window" << std::endl;
        std::cerr << "[!] WndProc hook will be skipped (menu toggle may not work)" << std::endl;
        // Don't fail completely, just skip WndProc hook
        return true;
    }
    
    // Get original WndProc
    LONG_PTR wndProc = GetWindowLongPtr(hWnd, GWLP_WNDPROC);
    if (!wndProc) {
        std::cerr << "[!] Failed to get WndProc" << std::endl;
        return true; // Don't fail completely
    }
    
    // Create hook
    if (MH_CreateHook(reinterpret_cast<LPVOID>(wndProc), &hkWndProc, 
                      reinterpret_cast<LPVOID*>(&oWndProc)) != MH_OK) {
        std::cerr << "[!] Failed to create WndProc hook" << std::endl;
        return true; // Don't fail completely
    }
    
    // Enable hook
    if (MH_EnableHook(reinterpret_cast<LPVOID>(wndProc)) != MH_OK) {
        std::cerr << "[!] Failed to enable WndProc hook" << std::endl;
        return true; // Don't fail completely
    }
    
    originalWndProc = oWndProc;
    
    std::cout << "[+] WndProc hooked successfully" << std::endl;
    
    return true;
}

bool HookManager::UnhookAll() {
    if (!hooksInstalled) return true;
    
    std::cout << "[*] Removing all hooks..." << std::endl;
    
    if (MH_DisableHook(MH_ALL_HOOKS) != MH_OK) {
        std::cerr << "[!] Failed to disable hooks" << std::endl;
        return false;
    }
    
    if (MH_Uninitialize() != MH_OK) {
        std::cerr << "[!] Failed to uninitialize MinHook" << std::endl;
        return false;
    }
    
    hooksInstalled = false;
    std::cout << "[+] All hooks removed successfully" << std::endl;
    
    return true;
}

void* HookManager::GetOriginalPresent() {
    return originalPresent;
}

void* HookManager::GetOriginalWndProc() {
    return originalWndProc;
}

bool HookManager::InstallHook(void* target, void* detour, void** original) {
    if (MH_CreateHook(target, detour, original) != MH_OK) {
        return false;
    }
    
    if (MH_EnableHook(target) != MH_OK) {
        return false;
    }
    
    return true;
}

bool HookManager::RemoveHook(void* target) {
    if (MH_DisableHook(target) != MH_OK) {
        return false;
    }
    
    return true;
}

// Hook function implementations will be in present_hook.cpp and wndproc_hook.cpp
