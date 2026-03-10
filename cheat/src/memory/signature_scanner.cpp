// Signature Scanner - Task 7
// Pattern scanning for dynamic offset resolution

#include "memory/signature_scanner.h"
#include <Psapi.h>

uintptr_t SignatureScanner::FindPattern(uintptr_t moduleBase, const char* pattern, const char* mask) {
    MODULEINFO modInfo;
    if (!GetModuleInformation(GetCurrentProcess(), reinterpret_cast<HMODULE>(moduleBase), &modInfo, sizeof(MODULEINFO))) {
        return 0;
    }

    size_t scanSize = modInfo.SizeOfImage;
    size_t patternLength = strlen(mask);
    BYTE* baseAddress = reinterpret_cast<BYTE*>(moduleBase);

    for (size_t i = 0; i < scanSize - patternLength; i++) {
        if (ComparePattern(baseAddress + i, pattern, mask)) {
            return moduleBase + i;
        }
    }

    return 0;
}

uintptr_t SignatureScanner::FindPatternInSection(uintptr_t moduleBase, const char* sectionName,
                                                  const char* pattern, const char* mask) {
    PIMAGE_DOS_HEADER dosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(moduleBase);
    if (dosHeader->e_magic != IMAGE_DOS_SIGNATURE) {
        return 0;
    }

    PIMAGE_NT_HEADERS ntHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>(moduleBase + dosHeader->e_lfanew);
    PIMAGE_SECTION_HEADER sectionHeader = IMAGE_FIRST_SECTION(ntHeaders);

    for (WORD i = 0; i < ntHeaders->FileHeader.NumberOfSections; i++, sectionHeader++) {
        if (strcmp(reinterpret_cast<const char*>(sectionHeader->Name), sectionName) == 0) {
            uintptr_t sectionStart = moduleBase + sectionHeader->VirtualAddress;
            size_t sectionSize = sectionHeader->Misc.VirtualSize;
            size_t patternLength = strlen(mask);

            for (size_t j = 0; j < sectionSize - patternLength; j++) {
                if (ComparePattern(reinterpret_cast<BYTE*>(sectionStart + j), pattern, mask)) {
                    return sectionStart + j;
                }
            }
        }
    }

    return 0;
}

bool SignatureScanner::CacheSignature(const char* name, uintptr_t address) {
    signatureCache[name] = address;
    return true;
}

uintptr_t SignatureScanner::GetCachedSignature(const char* name) {
    auto it = signatureCache.find(name);
    if (it != signatureCache.end()) {
        return it->second;
    }
    return 0;
}

bool SignatureScanner::ComparePattern(BYTE* data, const char* pattern, const char* mask) {
    size_t length = strlen(mask);
    for (size_t i = 0; i < length; i++) {
        if (mask[i] == 'x' && data[i] != static_cast<BYTE>(pattern[i])) {
            return false;
        }
    }
    return true;
}
