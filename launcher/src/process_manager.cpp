// Process Manager - Task 2
// Detects and manages CS2 process

#include "process_manager.h"
#include <TlHelp32.h>
#include <Psapi.h>
#include <iostream>

ProcessManager::~ProcessManager() {
    if (processHandle && processHandle != INVALID_HANDLE_VALUE) {
        CloseHandle(processHandle);
    }
}

bool ProcessManager::FindCS2Process() {
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) {
        std::cerr << "[!] Failed to create process snapshot" << std::endl;
        return false;
    }

    PROCESSENTRY32W processEntry = {};
    processEntry.dwSize = sizeof(PROCESSENTRY32W);

    if (Process32FirstW(snapshot, &processEntry)) {
        do {
            // Check for cs2.exe
            if (wcscmp(processEntry.szExeFile, L"cs2.exe") == 0) {
                processId = processEntry.th32ProcessID;
                CloseHandle(snapshot);
                
                std::cout << "[+] Found CS2 process (PID: " << processId << ")" << std::endl;
                
                // Open process handle
                processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
                if (!processHandle || processHandle == INVALID_HANDLE_VALUE) {
                    std::cerr << "[!] Failed to open process handle" << std::endl;
                    return false;
                }
                
                return true;
            }
        } while (Process32NextW(snapshot, &processEntry));
    }

    CloseHandle(snapshot);
    return false;
}

HANDLE ProcessManager::GetProcessHandle() {
    return processHandle;
}

uintptr_t ProcessManager::GetModuleBase(const char* moduleName) {
    if (!processHandle || processHandle == INVALID_HANDLE_VALUE) {
        return 0;
    }

    // Wait for the game to be fully loaded by checking window state
    // Professional approach: wait for main window to be ready
    HWND gameWindow = nullptr;
    const int maxWindowWait = 30; // 30 seconds max wait
    
    std::cout << "[*] Waiting for game window to be ready..." << std::endl;
    
    for (int i = 0; i < maxWindowWait; i++) {
        // Find CS2 window
        gameWindow = FindWindowW(nullptr, L"Counter-Strike 2");
        if (!gameWindow) {
            gameWindow = FindWindowW(L"SDL_app", nullptr); // SDL window class
        }
        
        if (gameWindow) {
            // Check if window is visible and not minimized
            if (IsWindowVisible(gameWindow) && !IsIconic(gameWindow)) {
                std::cout << "[+] Game window is ready" << std::endl;
                break;
            }
        }
        
        Sleep(1000);
    }
    
    if (!gameWindow) {
        std::cout << "[!] Warning: Could not find game window, proceeding anyway..." << std::endl;
    }
    
    // Now enumerate modules
    HMODULE modules[1024];
    DWORD bytesNeeded;

    if (!EnumProcessModules(processHandle, modules, sizeof(modules), &bytesNeeded)) {
        std::cerr << "[!] Failed to enumerate process modules" << std::endl;
        return 0;
    }

    DWORD moduleCount = bytesNeeded / sizeof(HMODULE);

    for (DWORD i = 0; i < moduleCount; i++) {
        char modName[MAX_PATH];
        if (GetModuleBaseNameA(processHandle, modules[i], modName, sizeof(modName))) {
            if (_stricmp(modName, moduleName) == 0) {
                std::cout << "[+] Found module: " << moduleName << " at 0x" 
                         << std::hex << (uintptr_t)modules[i] << std::dec << std::endl;
                return (uintptr_t)modules[i];
            }
        }
    }

    std::cerr << "[!] Module not found: " << moduleName << std::endl;
    std::cerr << "[*] Loaded modules:" << std::endl;
    
    // List all loaded modules for debugging
    for (DWORD i = 0; i < moduleCount && i < 20; i++) {
        char modName[MAX_PATH];
        if (GetModuleBaseNameA(processHandle, modules[i], modName, sizeof(modName))) {
            std::cerr << "    - " << modName << std::endl;
        }
    }
    
    return 0;
}

bool ProcessManager::IsProcessValid() {
    if (!processHandle || processHandle == INVALID_HANDLE_VALUE) {
        return false;
    }

    // Check if process is still running
    DWORD exitCode;
    if (!GetExitCodeProcess(processHandle, &exitCode)) {
        return false;
    }

    if (exitCode != STILL_ACTIVE) {
        return false;
    }

    // Validate x64 architecture
    BOOL isWow64 = FALSE;
    if (IsWow64Process(processHandle, &isWow64)) {
        if (isWow64) {
            std::cerr << "[!] CS2 process is not x64 (WOW64 detected)" << std::endl;
            return false;
        }
    }

    return true;
}

DWORD ProcessManager::GetProcessId() {
    return processId;
}
