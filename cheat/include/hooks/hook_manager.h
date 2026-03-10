#pragma once

#include <Windows.h>

// Hook Manager - Task 14
// TODO: Implement in Task 14

class HookManager {
public:
    HookManager() = default;
    ~HookManager() = default;

    // TODO: Implement these methods in Task 14
    bool Initialize();
    bool HookPresent();
    bool HookWndProc();
    bool UnhookAll();

    void* GetOriginalPresent();
    void* GetOriginalWndProc();

private:
    void* originalPresent = nullptr;
    void* originalWndProc = nullptr;
    bool hooksInstalled = false;

    bool InstallHook(void* target, void* detour, void** original);
    bool RemoveHook(void* target);
};
