#pragma once

#include <Windows.h>
#include <cstdint>

// DLL Injector - Task 3
// Manual mapping injection to avoid detection

class DLLInjector {
public:
    DLLInjector() = default;
    ~DLLInjector() = default;

    // Inject DLL from file path using manual mapping
    bool InjectDLL(HANDLE hProcess, const char* dllPath);
    
    // Simple LoadLibrary injection (more reliable for testing)
    bool InjectDLLSimple(HANDLE hProcess, const char* dllPath);
    
    // Manual map DLL data into target process
    bool ManualMap(HANDLE hProcess, BYTE* dllData, size_t dllSize);

private:
    // Resolve import address table
    bool ResolveImports(BYTE* baseAddress);
    
    // Apply base relocations
    bool RelocateSections(BYTE* baseAddress, uintptr_t delta);
    
    // Call DLL entry point (DllMain)
    bool CallEntryPoint(HANDLE hProcess, uintptr_t baseAddress, uintptr_t entryPoint);
};
