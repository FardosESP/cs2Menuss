#pragma once

#include <Windows.h>
#include <cstdint>

// Memory Engine - Task 6
// Safe memory read/write operations

class Memory {
public:
    Memory() = default;
    ~Memory() = default;

    // Read value from memory address
    template<typename T>
    T Read(uintptr_t address) {
        if (!IsReadable(address)) {
            return T{};  // Return default value if not readable
        }
        
        __try {
            return *reinterpret_cast<T*>(address);
        }
        __except (EXCEPTION_EXECUTE_HANDLER) {
            return T{};  // Return default value on exception
        }
    }

    // Write value to memory address
    template<typename T>
    bool Write(uintptr_t address, T value) {
        if (!IsWritable(address)) {
            return false;
        }
        
        __try {
            *reinterpret_cast<T*>(address) = value;
            return true;
        }
        __except (EXCEPTION_EXECUTE_HANDLER) {
            return false;
        }
    }

    // Check if address is valid
    bool IsValidAddress(uintptr_t address);
    
    // Get base address of a module
    uintptr_t GetModuleBase(const char* moduleName);

private:
    // Check if address is readable
    bool IsReadable(uintptr_t address);
    
    // Check if address is writable
    bool IsWritable(uintptr_t address);
};
