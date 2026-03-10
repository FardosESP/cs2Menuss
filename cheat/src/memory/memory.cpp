// Memory Engine - Task 6
// Safe memory read/write operations

#include "memory/memory.h"
#include <Psapi.h>

// Template implementations must be in header or explicitly instantiated
// Explicit instantiations for common types
template int Memory::Read<int>(uintptr_t address);
template float Memory::Read<float>(uintptr_t address);
template uintptr_t Memory::Read<uintptr_t>(uintptr_t address);
template bool Memory::Read<bool>(uintptr_t address);

template bool Memory::Write<int>(uintptr_t address, int value);
template bool Memory::Write<float>(uintptr_t address, float value);
template bool Memory::Write<uintptr_t>(uintptr_t address, uintptr_t value);
template bool Memory::Write<bool>(uintptr_t address, bool value);

bool Memory::IsValidAddress(uintptr_t address) {
    if (address == 0) return false;
    
    MEMORY_BASIC_INFORMATION mbi;
    if (VirtualQuery(reinterpret_cast<LPCVOID>(address), &mbi, sizeof(mbi)) == 0) {
        return false;
    }
    
    // Check if memory is committed and not guard page
    if (mbi.State != MEM_COMMIT) return false;
    if (mbi.Protect & PAGE_GUARD) return false;
    if (mbi.Protect & PAGE_NOACCESS) return false;
    
    return true;
}

uintptr_t Memory::GetModuleBase(const char* moduleName) {
    HMODULE hModule = GetModuleHandleA(moduleName);
    return reinterpret_cast<uintptr_t>(hModule);
}

bool Memory::IsReadable(uintptr_t address) {
    if (!IsValidAddress(address)) return false;
    
    MEMORY_BASIC_INFORMATION mbi;
    VirtualQuery(reinterpret_cast<LPCVOID>(address), &mbi, sizeof(mbi));
    
    // Check if memory has read permissions
    return (mbi.Protect & PAGE_READONLY) ||
           (mbi.Protect & PAGE_READWRITE) ||
           (mbi.Protect & PAGE_WRITECOPY) ||
           (mbi.Protect & PAGE_EXECUTE_READ) ||
           (mbi.Protect & PAGE_EXECUTE_READWRITE) ||
           (mbi.Protect & PAGE_EXECUTE_WRITECOPY);
}

bool Memory::IsWritable(uintptr_t address) {
    if (!IsValidAddress(address)) return false;
    
    MEMORY_BASIC_INFORMATION mbi;
    VirtualQuery(reinterpret_cast<LPCVOID>(address), &mbi, sizeof(mbi));
    
    // Check if memory has write permissions
    return (mbi.Protect & PAGE_READWRITE) ||
           (mbi.Protect & PAGE_WRITECOPY) ||
           (mbi.Protect & PAGE_EXECUTE_READWRITE) ||
           (mbi.Protect & PAGE_EXECUTE_WRITECOPY);
}
