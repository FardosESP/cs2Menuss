// DLL Injector - Task 3
// Manual mapping injection to avoid detection

#include "dll_injector.h"
#include <fstream>
#include <iostream>
#include <vector>

bool DLLInjector::InjectDLL(HANDLE hProcess, const char* dllPath) {
    std::cout << "[*] Reading DLL file: " << dllPath << std::endl;

    // Read DLL file
    std::ifstream file(dllPath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "[!] Failed to open DLL file" << std::endl;
        return false;
    }

    size_t fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<BYTE> dllData(fileSize);
    if (!file.read(reinterpret_cast<char*>(dllData.data()), fileSize)) {
        std::cerr << "[!] Failed to read DLL file" << std::endl;
        return false;
    }
    file.close();

    std::cout << "[+] DLL file read successfully (" << fileSize << " bytes)" << std::endl;

    // Try simple LoadLibrary injection first for testing
    std::cout << "[*] Attempting simple LoadLibrary injection for testing..." << std::endl;
    if (InjectDLLSimple(hProcess, dllPath)) {
        std::cout << "[+] Simple injection succeeded!" << std::endl;
        return true;
    }
    
    std::cout << "[!] Simple injection failed, trying manual mapping..." << std::endl;

    // Perform manual mapping
    return ManualMap(hProcess, dllData.data(), fileSize);
}

bool DLLInjector::InjectDLLSimple(HANDLE hProcess, const char* dllPath) {
    // Get full path
    char fullPath[MAX_PATH];
    GetFullPathNameA(dllPath, MAX_PATH, fullPath, nullptr);
    
    std::cout << "[*] Full DLL path: " << fullPath << std::endl;
    
    // Allocate memory for DLL path in target process
    size_t pathLen = strlen(fullPath) + 1;
    LPVOID pRemotePath = VirtualAllocEx(hProcess, nullptr, pathLen, 
                                        MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!pRemotePath) {
        std::cerr << "[!] Failed to allocate memory for DLL path (Error: " << GetLastError() << ")" << std::endl;
        return false;
    }
    
    std::cout << "[*] Allocated memory at 0x" << std::hex << (uintptr_t)pRemotePath << std::dec << std::endl;
    
    // Write DLL path to target process
    if (!WriteProcessMemory(hProcess, pRemotePath, fullPath, pathLen, nullptr)) {
        std::cerr << "[!] Failed to write DLL path (Error: " << GetLastError() << ")" << std::endl;
        VirtualFreeEx(hProcess, pRemotePath, 0, MEM_RELEASE);
        return false;
    }
    
    std::cout << "[*] DLL path written to remote process" << std::endl;
    
    // Get LoadLibraryA address
    HMODULE hKernel32 = GetModuleHandleA("kernel32.dll");
    FARPROC pLoadLibrary = GetProcAddress(hKernel32, "LoadLibraryA");
    
    if (!pLoadLibrary) {
        std::cerr << "[!] Failed to get LoadLibraryA address" << std::endl;
        VirtualFreeEx(hProcess, pRemotePath, 0, MEM_RELEASE);
        return false;
    }
    
    std::cout << "[*] LoadLibraryA address: 0x" << std::hex << (uintptr_t)pLoadLibrary << std::dec << std::endl;
    
    // Create remote thread to call LoadLibraryA
    DWORD threadId = 0;
    HANDLE hThread = CreateRemoteThread(hProcess, nullptr, 0,
                                       reinterpret_cast<LPTHREAD_START_ROUTINE>(pLoadLibrary),
                                       pRemotePath, 0, &threadId);
    if (!hThread) {
        DWORD error = GetLastError();
        std::cerr << "[!] Failed to create remote thread (Error: " << error << ")" << std::endl;
        VirtualFreeEx(hProcess, pRemotePath, 0, MEM_RELEASE);
        return false;
    }
    
    std::cout << "[*] Remote thread created (TID: " << threadId << ")" << std::endl;
    std::cout << "[*] Waiting for LoadLibrary to complete..." << std::endl;
    
    DWORD waitResult = WaitForSingleObject(hThread, 10000); // 10 second timeout
    
    if (waitResult == WAIT_TIMEOUT) {
        std::cerr << "[!] LoadLibrary timed out" << std::endl;
        CloseHandle(hThread);
        VirtualFreeEx(hProcess, pRemotePath, 0, MEM_RELEASE);
        return false;
    }
    
    DWORD exitCode = 0;
    if (!GetExitCodeThread(hThread, &exitCode)) {
        std::cerr << "[!] Failed to get thread exit code (Error: " << GetLastError() << ")" << std::endl;
        CloseHandle(hThread);
        VirtualFreeEx(hProcess, pRemotePath, 0, MEM_RELEASE);
        return false;
    }
    
    std::cout << "[*] Thread exit code: 0x" << std::hex << exitCode << std::dec << std::endl;
    
    CloseHandle(hThread);
    VirtualFreeEx(hProcess, pRemotePath, 0, MEM_RELEASE);
    
    if (exitCode == 0) {
        std::cerr << "[!] LoadLibrary returned NULL - DLL failed to load" << std::endl;
        std::cerr << "[!] Possible reasons:" << std::endl;
        std::cerr << "    - Missing dependencies (VCRUNTIME, MSVCP)" << std::endl;
        std::cerr << "    - DllMain crashed or returned FALSE" << std::endl;
        std::cerr << "    - Anti-cheat blocked the injection" << std::endl;
        return false;
    }
    
    std::cout << "[+] DLL loaded at 0x" << std::hex << exitCode << std::dec << std::endl;
    return true;
}

bool DLLInjector::ManualMap(HANDLE hProcess, BYTE* dllData, size_t dllSize) {
    // Parse PE headers
    PIMAGE_DOS_HEADER dosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(dllData);
    if (dosHeader->e_magic != IMAGE_DOS_SIGNATURE) {
        std::cerr << "[!] Invalid DOS signature" << std::endl;
        return false;
    }

    PIMAGE_NT_HEADERS ntHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>(dllData + dosHeader->e_lfanew);
    if (ntHeaders->Signature != IMAGE_NT_SIGNATURE) {
        std::cerr << "[!] Invalid NT signature" << std::endl;
        return false;
    }

    std::cout << "[*] Allocating memory in target process..." << std::endl;

    // Allocate memory in target process
    size_t imageSize = ntHeaders->OptionalHeader.SizeOfImage;
    LPVOID remoteImage = VirtualAllocEx(hProcess, nullptr, imageSize,
                                        MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!remoteImage) {
        std::cerr << "[!] Failed to allocate memory in target process" << std::endl;
        return false;
    }

    std::cout << "[+] Allocated memory at 0x" << std::hex << (uintptr_t)remoteImage << std::dec << std::endl;

    // Create local copy for processing
    std::vector<BYTE> localImage(imageSize, 0);
    
    // Copy headers to local image
    memcpy(localImage.data(), dllData, ntHeaders->OptionalHeader.SizeOfHeaders);

    // Copy sections to local image
    PIMAGE_SECTION_HEADER sectionHeader = IMAGE_FIRST_SECTION(ntHeaders);
    for (WORD i = 0; i < ntHeaders->FileHeader.NumberOfSections; i++, sectionHeader++) {
        if (sectionHeader->SizeOfRawData == 0) continue;
        
        memcpy(localImage.data() + sectionHeader->VirtualAddress,
               dllData + sectionHeader->PointerToRawData,
               sectionHeader->SizeOfRawData);
    }

    std::cout << "[+] Sections copied to local buffer" << std::endl;

    // Resolve imports in local image
    if (!ResolveImports(localImage.data())) {
        std::cerr << "[!] Failed to resolve imports" << std::endl;
        VirtualFreeEx(hProcess, remoteImage, 0, MEM_RELEASE);
        return false;
    }

    // Apply relocations in local image
    uintptr_t delta = reinterpret_cast<uintptr_t>(remoteImage) - ntHeaders->OptionalHeader.ImageBase;
    if (delta != 0) {
        if (!RelocateSections(localImage.data(), delta)) {
            std::cerr << "[!] Failed to apply relocations" << std::endl;
            VirtualFreeEx(hProcess, remoteImage, 0, MEM_RELEASE);
            return false;
        }
    }

    // Write processed image to remote process
    std::cout << "[*] Writing processed image to remote process..." << std::endl;
    if (!WriteProcessMemory(hProcess, remoteImage, localImage.data(), imageSize, nullptr)) {
        std::cerr << "[!] Failed to write processed image" << std::endl;
        VirtualFreeEx(hProcess, remoteImage, 0, MEM_RELEASE);
        return false;
    }

    std::cout << "[+] Image written to remote process" << std::endl;

    // Call entry point
    uintptr_t entryPoint = reinterpret_cast<uintptr_t>(remoteImage) + ntHeaders->OptionalHeader.AddressOfEntryPoint;
    if (!CallEntryPoint(hProcess, reinterpret_cast<uintptr_t>(remoteImage), entryPoint)) {
        std::cerr << "[!] Failed to call entry point" << std::endl;
        VirtualFreeEx(hProcess, remoteImage, 0, MEM_RELEASE);
        return false;
    }

    std::cout << "[+] DLL injected successfully!" << std::endl;
    return true;
}

bool DLLInjector::ResolveImports(BYTE* baseAddress) {
    PIMAGE_DOS_HEADER dosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(baseAddress);
    PIMAGE_NT_HEADERS ntHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>(baseAddress + dosHeader->e_lfanew);

    PIMAGE_DATA_DIRECTORY importDir = &ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
    if (importDir->Size == 0) {
        std::cout << "[*] No imports to resolve" << std::endl;
        return true;
    }

    PIMAGE_IMPORT_DESCRIPTOR importDesc = reinterpret_cast<PIMAGE_IMPORT_DESCRIPTOR>(baseAddress + importDir->VirtualAddress);

    while (importDesc->Name != 0) {
        char* moduleName = reinterpret_cast<char*>(baseAddress + importDesc->Name);
        HMODULE hModule = LoadLibraryA(moduleName);
        
        if (!hModule) {
            std::cerr << "[!] Failed to load module: " << moduleName << std::endl;
            return false;
        }

        PIMAGE_THUNK_DATA thunk = reinterpret_cast<PIMAGE_THUNK_DATA>(baseAddress + importDesc->FirstThunk);
        PIMAGE_THUNK_DATA origThunk = reinterpret_cast<PIMAGE_THUNK_DATA>(baseAddress + importDesc->OriginalFirstThunk);

        while (thunk->u1.AddressOfData != 0) {
            if (IMAGE_SNAP_BY_ORDINAL(origThunk->u1.Ordinal)) {
                FARPROC func = GetProcAddress(hModule, MAKEINTRESOURCEA(IMAGE_ORDINAL(origThunk->u1.Ordinal)));
                if (!func) {
                    std::cerr << "[!] Failed to resolve import by ordinal" << std::endl;
                    return false;
                }
                thunk->u1.Function = reinterpret_cast<uintptr_t>(func);
            } else {
                PIMAGE_IMPORT_BY_NAME importByName = reinterpret_cast<PIMAGE_IMPORT_BY_NAME>(baseAddress + origThunk->u1.AddressOfData);
                FARPROC func = GetProcAddress(hModule, importByName->Name);
                if (!func) {
                    std::cerr << "[!] Failed to resolve import: " << importByName->Name << std::endl;
                    return false;
                }
                thunk->u1.Function = reinterpret_cast<uintptr_t>(func);
            }

            thunk++;
            origThunk++;
        }

        importDesc++;
    }

    std::cout << "[+] Imports resolved successfully" << std::endl;
    return true;
}

bool DLLInjector::RelocateSections(BYTE* baseAddress, uintptr_t delta) {
    PIMAGE_DOS_HEADER dosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(baseAddress);
    PIMAGE_NT_HEADERS ntHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>(baseAddress + dosHeader->e_lfanew);

    PIMAGE_DATA_DIRECTORY relocDir = &ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC];
    if (relocDir->Size == 0) {
        std::cout << "[*] No relocations needed" << std::endl;
        return true;
    }

    PIMAGE_BASE_RELOCATION reloc = reinterpret_cast<PIMAGE_BASE_RELOCATION>(baseAddress + relocDir->VirtualAddress);

    while (reloc->VirtualAddress != 0) {
        DWORD numEntries = (reloc->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(WORD);
        PWORD relocData = reinterpret_cast<PWORD>(reinterpret_cast<uintptr_t>(reloc) + sizeof(IMAGE_BASE_RELOCATION));

        for (DWORD i = 0; i < numEntries; i++) {
            WORD type = relocData[i] >> 12;
            WORD offset = relocData[i] & 0xFFF;

            if (type == IMAGE_REL_BASED_DIR64) {
                uintptr_t* patchAddr = reinterpret_cast<uintptr_t*>(baseAddress + reloc->VirtualAddress + offset);
                *patchAddr += delta;
            }
        }

        reloc = reinterpret_cast<PIMAGE_BASE_RELOCATION>(reinterpret_cast<uintptr_t>(reloc) + reloc->SizeOfBlock);
    }

    std::cout << "[+] Relocations applied successfully" << std::endl;
    return true;
}

bool DLLInjector::CallEntryPoint(HANDLE hProcess, uintptr_t baseAddress, uintptr_t entryPoint) {
    std::cout << "[*] Calling DLL entry point at 0x" << std::hex << entryPoint << std::dec << std::endl;
    std::cout << "[*] DLL base address: 0x" << std::hex << baseAddress << std::dec << std::endl;

    // Create shellcode to call DllMain
    // x64 calling convention: RCX, RDX, R8, R9
    // DllMain signature: BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
    unsigned char shellcode[] = {
        0x48, 0x83, 0xEC, 0x28,                                         // sub rsp, 0x28 (shadow space + alignment)
        0x48, 0xB9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // mov rcx, hModule (offset 6)
        0xBA, 0x01, 0x00, 0x00, 0x00,                                   // mov edx, 1 (DLL_PROCESS_ATTACH)
        0x4D, 0x31, 0xC0,                                               // xor r8, r8 (NULL)
        0x48, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    // mov rax, entryPoint (offset 25)
        0xFF, 0xD0,                                                     // call rax
        0x48, 0x83, 0xC4, 0x28,                                         // add rsp, 0x28
        0xC3                                                            // ret
    };
    
    // Patch shellcode with actual addresses
    *reinterpret_cast<uintptr_t*>(&shellcode[6]) = baseAddress;   // hModule parameter (RCX)
    *reinterpret_cast<uintptr_t*>(&shellcode[25]) = entryPoint;   // DllMain address to call
    
    std::cout << "[*] Shellcode patched - hModule: 0x" << std::hex << baseAddress 
              << ", EntryPoint: 0x" << entryPoint << std::dec << std::endl;
    
    // Allocate memory for shellcode
    LPVOID pShellcode = VirtualAllocEx(hProcess, nullptr, sizeof(shellcode),
                                       MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!pShellcode) {
        std::cerr << "[!] Failed to allocate memory for shellcode (Error: " << GetLastError() << ")" << std::endl;
        return false;
    }
    
    std::cout << "[*] Shellcode allocated at 0x" << std::hex << (uintptr_t)pShellcode << std::dec << std::endl;
    
    // Write shellcode
    if (!WriteProcessMemory(hProcess, pShellcode, shellcode, sizeof(shellcode), nullptr)) {
        std::cerr << "[!] Failed to write shellcode (Error: " << GetLastError() << ")" << std::endl;
        VirtualFreeEx(hProcess, pShellcode, 0, MEM_RELEASE);
        return false;
    }
    
    std::cout << "[*] Creating remote thread to execute DllMain..." << std::endl;
    
    // Create remote thread to execute shellcode
    HANDLE hThread = CreateRemoteThread(hProcess, nullptr, 0,
                                       reinterpret_cast<LPTHREAD_START_ROUTINE>(pShellcode),
                                       nullptr, 0, nullptr);
    if (!hThread) {
        std::cerr << "[!] Failed to create remote thread (Error: " << GetLastError() << ")" << std::endl;
        VirtualFreeEx(hProcess, pShellcode, 0, MEM_RELEASE);
        return false;
    }

    // Wait for thread to complete (with timeout)
    std::cout << "[*] Waiting for DllMain to complete..." << std::endl;
    DWORD waitResult = WaitForSingleObject(hThread, 10000); // 10 second timeout
    
    if (waitResult == WAIT_TIMEOUT) {
        std::cout << "[!] Warning: DllMain call timed out" << std::endl;
        std::cout << "[*] This may be normal if DllMain creates threads" << std::endl;
    } else if (waitResult == WAIT_OBJECT_0) {
        DWORD exitCode;
        if (GetExitCodeThread(hThread, &exitCode)) {
            std::cout << "[+] DllMain returned: " << exitCode << std::endl;
        }
    }
    
    CloseHandle(hThread);
    
    // Don't free shellcode immediately - let it stay for debugging
    // VirtualFreeEx(hProcess, pShellcode, 0, MEM_RELEASE);

    std::cout << "[+] Entry point called successfully" << std::endl;
    return true;
}
