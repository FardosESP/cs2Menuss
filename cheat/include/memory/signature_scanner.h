#pragma once

#include <Windows.h>
#include <cstdint>
#include <unordered_map>
#include <string>

// Signature Scanner - Task 7
// TODO: Implement in Task 7

class SignatureScanner {
public:
    SignatureScanner() = default;
    ~SignatureScanner() = default;

    // TODO: Implement these methods in Task 7
    uintptr_t FindPattern(uintptr_t moduleBase, const char* pattern, const char* mask);
    uintptr_t FindPatternInSection(uintptr_t moduleBase, const char* sectionName,
                                    const char* pattern, const char* mask);
    bool CacheSignature(const char* name, uintptr_t address);
    uintptr_t GetCachedSignature(const char* name);

private:
    std::unordered_map<std::string, uintptr_t> signatureCache;
    bool ComparePattern(BYTE* data, const char* pattern, const char* mask);
};
