// CS2 Premium Menu - Cheat DLL

#include <Windows.h>
#include <thread>
#include <iostream>
#include <chrono>
#include <fstream>

#include "memory/memory.h"
#include "memory/offset_manager.h"
#include "entity/entity_manager.h"
#include "hooks/hook_manager.h"
#include "esp/esp_renderer.h"
#include "ui/menu_manager.h"
#include "config/config_manager.h"

// Global instances
Memory* g_Memory = nullptr;
OffsetManager* g_OffsetManager = nullptr;
EntityManager* g_EntityManager = nullptr;
HookManager* g_HookManager = nullptr;
ESPRenderer* g_ESPRenderer = nullptr;
MenuManager* g_MenuManager = nullptr;
ConfigManager* g_ConfigManager = nullptr;

// Module handle
HMODULE g_hModule = nullptr;

void InitializeCheat() {
    // Allocate console for debugging
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    freopen_s(&f, "CONOUT$", "w", stderr);
    
    // Also create log file
    std::ofstream logFile("C:\\fardos_log.txt", std::ios::app);
    logFile << "\n=== Fardos.cc Initialization ===" << std::endl;
    logFile.close();
    
    std::cout << R"(
    ███████╗ █████╗ ██████╗ ██████╗  ██████╗ ███████╗   ██████╗ ██████╗ 
    ██╔════╝██╔══██╗██╔══██╗██╔══██╗██╔═══██╗██╔════╝  ██╔════╝██╔════╝ 
    █████╗  ███████║██████╔╝██║  ██║██║   ██║███████╗  ██║     ██║      
    ██╔══╝  ██╔══██║██╔══██╗██║  ██║██║   ██║╚════██║  ██║     ██║      
    ██║     ██║  ██║██║  ██║██████╔╝╚██████╔╝███████║██╗╚██████╗╚██████╗
    ╚═╝     ╚═╝  ╚═╝╚═╝  ╚═╝╚═════╝  ╚═════╝ ╚══════╝╚═╝ ╚═════╝ ╚═════╝
    
    CS2 Premium Menu v1.0
    )" << std::endl;
    
    std::cout << "\n[*] Starting initialization sequence..." << std::endl;
    
    try {
        // Step 1: Initialize Memory Engine
        std::cout << "[*] Initializing Memory Engine..." << std::endl;
        g_Memory = new Memory();
        std::cout << "[+] Memory Engine initialized" << std::endl;
        
        // Step 2: Initialize Offset Manager and scan signatures
        std::cout << "[*] Initializing Offset Manager..." << std::endl;
        g_OffsetManager = new OffsetManager();
        if (!g_OffsetManager->Initialize()) {
            throw std::runtime_error("Failed to initialize Offset Manager");
        }
        std::cout << "[+] Offset Manager initialized" << std::endl;
        
        // Step 3: Initialize Entity Manager
        std::cout << "[*] Initializing Entity Manager..." << std::endl;
        g_EntityManager = new EntityManager();
        std::cout << "[+] Entity Manager initialized" << std::endl;
        
        // Step 4: Initialize ESP Renderer
        std::cout << "[*] Initializing ESP Renderer..." << std::endl;
        g_ESPRenderer = new ESPRenderer();
        std::cout << "[+] ESP Renderer initialized" << std::endl;
        
        // Step 5: Initialize Menu Manager
        std::cout << "[*] Initializing Menu Manager..." << std::endl;
        g_MenuManager = new MenuManager();
        g_MenuManager->ApplyNeverloseStyle();
        std::cout << "[+] Menu Manager initialized" << std::endl;
        
        // Step 6: Initialize Config Manager
        std::cout << "[*] Initializing Config Manager..." << std::endl;
        g_ConfigManager = new ConfigManager();
        std::cout << "[+] Config Manager initialized" << std::endl;
        std::cout << "[*] Config path: " << g_ConfigManager->GetConfigPath() << std::endl;
        
        // Step 7: Install hooks
        std::cout << "[*] Installing hooks..." << std::endl;
        g_HookManager = new HookManager();
        if (!g_HookManager->Initialize()) {
            throw std::runtime_error("Failed to initialize Hook Manager");
        }
        std::cout << "[+] Hooks installed successfully" << std::endl;
        
        // Step 8: Load default configuration
        std::cout << "[*] Loading default configuration..." << std::endl;
        auto configs = g_ConfigManager->GetConfigList();
        if (!configs.empty()) {
            g_ConfigManager->LoadConfig(configs[0]);
        } else {
            std::cout << "[*] No saved configs found, using defaults" << std::endl;
        }
        
        std::cout << "\n[+] ===== INITIALIZATION COMPLETE =====" << std::endl;
        std::cout << "[+] Press INSERT to open menu" << std::endl;
        std::cout << "[+] Press END to unload cheat" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "\n[!] INITIALIZATION FAILED: " << e.what() << std::endl;
        MessageBoxA(nullptr, 
            ("Initialization failed:\n" + std::string(e.what())).c_str(),
            "Fardos.cc - Error", 
            MB_OK | MB_ICONERROR);
        FreeLibraryAndExitThread(g_hModule, 0);
    }
}

void ShutdownCheat() {
    std::cout << "\n[*] Starting shutdown sequence..." << std::endl;
    
    // Step 1: Unhook all functions
    if (g_HookManager) {
        std::cout << "[*] Removing hooks..." << std::endl;
        g_HookManager->UnhookAll();
        delete g_HookManager;
        g_HookManager = nullptr;
        std::cout << "[+] Hooks removed" << std::endl;
    }
    
    // Step 2: Save configuration
    if (g_ConfigManager) {
        std::cout << "[*] Saving configuration..." << std::endl;
        g_ConfigManager->SaveConfig("autosave");
        delete g_ConfigManager;
        g_ConfigManager = nullptr;
        std::cout << "[+] Configuration saved" << std::endl;
    }
    
    // Step 3: Clean up resources
    if (g_MenuManager) {
        delete g_MenuManager;
        g_MenuManager = nullptr;
    }
    
    if (g_ESPRenderer) {
        delete g_ESPRenderer;
        g_ESPRenderer = nullptr;
    }
    
    if (g_EntityManager) {
        delete g_EntityManager;
        g_EntityManager = nullptr;
    }
    
    if (g_OffsetManager) {
        delete g_OffsetManager;
        g_OffsetManager = nullptr;
    }
    
    if (g_Memory) {
        delete g_Memory;
        g_Memory = nullptr;
    }
    
    std::cout << "[+] Cleanup complete" << std::endl;
    std::cout << "[+] Fardos.cc unloaded successfully" << std::endl;
    
    // Give time to read console
    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    // Free console
    fclose(stdout);
    fclose(stderr);
    FreeConsole();
}

// Thread function for initialization
DWORD WINAPI InitThread(LPVOID lpParam) {
    // Create debug log using C-style I/O (compatible with SEH)
    HANDLE hFile = CreateFileA("C:\\fardos_init.txt", GENERIC_WRITE, FILE_SHARE_READ,
                               nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (hFile != INVALID_HANDLE_VALUE) {
        const char* msg = "InitThread started\n";
        DWORD written;
        WriteFile(hFile, msg, strlen(msg), &written, nullptr);
        CloseHandle(hFile);
    }
    
    InitializeCheat();
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved) {
    if (dwReason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
        g_hModule = hModule;
        
        // Create log file IMMEDIATELY - simplest possible way
        HANDLE hFile = CreateFileA("C:\\fardos_attached.txt", GENERIC_WRITE, 0,
                                   nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
        if (hFile != INVALID_HANDLE_VALUE) {
            const char* msg = "ATTACHED\n";
            DWORD written;
            WriteFile(hFile, msg, 9, &written, nullptr);
            FlushFileBuffers(hFile);
            CloseHandle(hFile);
        }
        
        // Try to create thread - but don't fail if it doesn't work
        CreateThread(nullptr, 0, InitThread, nullptr, 0, nullptr);
        
        // Always return TRUE
        return TRUE;
    }
    else if (dwReason == DLL_PROCESS_DETACH) {
        ShutdownCheat();
    }
    
    return TRUE;
}
