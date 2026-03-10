#pragma once

#include <cstdint>
#include <mutex>
#include "memory/signature_scanner.h"

// Offset Manager - Task 8
// TODO: Implement in Task 8

struct Offsets {
    // Client.dll offsets
    uintptr_t dwEntityList;
    uintptr_t dwLocalPlayerController;
    uintptr_t dwViewMatrix;
    uintptr_t dwViewAngles;

    // Entity offsets
    uintptr_t m_iHealth;
    uintptr_t m_iTeamNum;
    uintptr_t m_vecOrigin;
    uintptr_t m_pGameSceneNode;
    uintptr_t m_modelState;
    uintptr_t m_bDormant;
    uintptr_t m_iszPlayerName;

    // Pawn offsets
    uintptr_t m_hPlayerPawn;
    uintptr_t m_vOldOrigin;
    uintptr_t m_angEyeAngles;
    uintptr_t m_vecViewOffset;

    // Weapon offsets
    uintptr_t m_pClippingWeapon;
    uintptr_t m_iClip1;
    uintptr_t m_iClip2;
    uintptr_t m_AttributeManager;
    uintptr_t m_Item;
    uintptr_t m_iItemDefinitionIndex;
};

class OffsetManager {
public:
    OffsetManager() = default;
    ~OffsetManager() = default;

    // TODO: Implement these methods in Task 8
    bool Initialize();
    bool UpdateOffsets();
    Offsets& GetOffsets();
    bool ValidateOffsets();

private:
    Offsets offsets{};
    std::mutex offsetMutex;
    SignatureScanner scanner;
};
