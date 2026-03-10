#pragma once

#include <Windows.h>
#include <cstdint>

// Process Manager - Task 2
// Detects and manages CS2 process

class ProcessManager {
public:
    ProcessManager() = default;
    ~ProcessManager();

    // Find CS2 process and open handle
    bool FindCS2Process();
    
    // Get process handle with PROCESS_ALL_ACCESS
    HANDLE GetProcessHandle();
    
    // Get base address of a module (e.g., "client.dll")
    uintptr_t GetModuleBase(const char* moduleName);
    
    // Validate process is still running and is x64
    bool IsProcessValid();
    
    // Get process ID
    DWORD GetProcessId();

private:
    DWORD processId = 0;
    HANDLE processHandle = nullptr;
    uintptr_t clientDllBase = 0;
};
