// CS2 Premium Menu - Launcher

#include <Windows.h>
#include <iostream>
#include <thread>
#include <chrono>
#include "process_manager.h"
#include "dll_injector.h"
#include "vac_bypass.h"

void PrintBanner() {
    std::cout << R"(
╔═══════════════════════════════════════════════════════════╗
║               CS2 Premium Menu - Launcher                 ║
╚═══════════════════════════════════════════════════════════╝

)" << std::endl;
}

int main() {
    PrintBanner();

    // Initialize components
    ProcessManager procManager;
    DLLInjector injector;
    VACBypass vacBypass;

    std::cout << "[*] Launcher initialized" << std::endl;
    std::cout << "[*] Waiting for CS2 process..." << std::endl;

    // Wait for CS2 process
    while (!procManager.FindCS2Process()) {
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    // Validate process
    if (!procManager.IsProcessValid()) {
        std::cerr << "[!] CS2 process is not valid (must be x64)" << std::endl;
        std::cout << "\nPress Enter to exit..." << std::endl;
        std::cin.get();
        return 1;
    }

    std::cout << "[+] CS2 process validated successfully" << std::endl;

    // Get client.dll base address
    uintptr_t clientBase = procManager.GetModuleBase("client.dll");
    if (clientBase == 0) {
        std::cerr << "[!] Failed to find client.dll" << std::endl;
        std::cout << "\nPress Enter to exit..." << std::endl;
        std::cin.get();
        return 1;
    }

    // Initialize VAC bypass
    std::cout << "\n[*] Initializing VAC bypass..." << std::endl;
    if (!vacBypass.Initialize()) {
        std::cerr << "[!] Failed to initialize VAC bypass" << std::endl;
        std::cout << "\nPress Enter to exit..." << std::endl;
        std::cin.get();
        return 1;
    }

    // Apply VAC bypass
    vacBypass.PatchVACModules();
    vacBypass.HideThreads();

    // Inject cheat DLL
    std::cout << "\n[*] Injecting cheat DLL..." << std::endl;
    const char* dllPath = "cheat.dll";  // Assumes DLL is in same directory
    
    if (!injector.InjectDLL(procManager.GetProcessHandle(), dllPath)) {
        std::cerr << "[!] Failed to inject DLL" << std::endl;
        std::cout << "\nPress Enter to exit..." << std::endl;
        std::cin.get();
        return 1;
    }

    std::cout << "\n[+] ========================================" << std::endl;
    std::cout << "[+] Injection completed successfully!" << std::endl;
    std::cout << "[+] ========================================" << std::endl;
    std::cout << "\n[*] The cheat should now be active in CS2" << std::endl;
    std::cout << "[*] Press INSERT in-game to open the menu" << std::endl;

    std::cout << "\nPress Enter to exit launcher..." << std::endl;
    std::cin.get();

    return 0;
}
