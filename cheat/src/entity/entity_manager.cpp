// Entity Manager - Task 10
// Manages game entities and player data

#include "entity/entity_manager.h"

void EntityManager::Update() {
    if (!memory || !offsets) return;
    
    cachedEntities.clear();
    
    uintptr_t clientBase = memory->GetModuleBase("client.dll");
    if (clientBase == 0) return;
    
    Offsets& off = offsets->GetOffsets();
    
    // Get entity list and local player
    uintptr_t entityList = memory->Read<uintptr_t>(clientBase + off.dwEntityList);
    uintptr_t localPlayerController = memory->Read<uintptr_t>(clientBase + off.dwLocalPlayerController);
    
    if (entityList == 0 || localPlayerController == 0) return;
    
    // Update local player
    uintptr_t localPawn = memory->Read<uintptr_t>(localPlayerController + off.m_hPlayerPawn);
    if (localPawn != 0) {
        localPlayer.address = localPawn;
        localPlayer.origin = memory->Read<Vector3>(localPawn + off.m_vecOrigin);
        localPlayer.team = memory->Read<int>(localPawn + off.m_iTeamNum);
    }
    
    // Iterate through entities (max 64 players in CS2)
    for (int i = 1; i <= 64; i++) {
        uintptr_t entityController = memory->Read<uintptr_t>(entityList + (i * 0x78));
        
        if (entityController == 0 || entityController == localPlayerController) {
            continue;
        }
        
        uintptr_t playerPawn = memory->Read<uintptr_t>(entityController + off.m_hPlayerPawn);
        
        if (!IsEntityValid(playerPawn)) {
            continue;
        }
        
        EntityData entity{};
        entity.address = playerPawn;
        entity.index = i;
        entity.health = memory->Read<int>(playerPawn + off.m_iHealth);
        entity.team = memory->Read<int>(playerPawn + off.m_iTeamNum);
        entity.origin = memory->Read<Vector3>(playerPawn + off.m_vecOrigin);
        entity.isDormant = memory->Read<bool>(playerPawn + off.m_bDormant);
        entity.isAlive = (entity.health > 0);
        
        // Read player name
        uintptr_t namePtr = memory->Read<uintptr_t>(entityController + off.m_iszPlayerName);
        if (namePtr != 0) {
            for (int j = 0; j < 127; j++) {
                entity.name[j] = memory->Read<char>(namePtr + j);
                if (entity.name[j] == '\0') break;
            }
            entity.name[127] = '\0';
        }
        
        // Get head position
        uintptr_t gameSceneNode = memory->Read<uintptr_t>(playerPawn + off.m_pGameSceneNode);
        entity.headPosition = GetBonePosition(gameSceneNode, static_cast<int>(BoneId::Head));
        
        // Get weapon ID
        uintptr_t weapon = memory->Read<uintptr_t>(playerPawn + off.m_pClippingWeapon);
        if (weapon != 0) {
            entity.weaponId = memory->Read<int>(weapon + off.m_iItemDefinitionIndex);
        }
        
        cachedEntities.push_back(entity);
    }
}

std::vector<EntityData> EntityManager::GetPlayers() {
    return cachedEntities;
}

EntityData EntityManager::GetLocalPlayer() {
    return localPlayer;
}

bool EntityManager::IsEntityValid(uintptr_t entity) {
    if (entity == 0) return false;
    if (!memory) return false;
    
    return IsPlayerAlive(entity) && !IsDormant(entity);
}

Vector3 EntityManager::GetBonePosition(uintptr_t gameSceneNode, int boneId) {
    if (gameSceneNode == 0 || !memory || !offsets) {
        return Vector3(0, 0, 0);
    }
    
    Offsets& off = offsets->GetOffsets();
    
    uintptr_t modelState = memory->Read<uintptr_t>(gameSceneNode + off.m_modelState);
    if (modelState == 0) {
        return Vector3(0, 0, 0);
    }
    
    // Bone array offset (real offset from CS2 structure)
    uintptr_t boneArray = memory->Read<uintptr_t>(modelState + 0x80);
    if (boneArray == 0) {
        return Vector3(0, 0, 0);
    }
    
    // Read bone matrix (3x4 transformation matrix)
    // Each bone has a 32-byte structure (0x20)
    uintptr_t boneMatrix = boneArray + (boneId * 0x20);
    
    // Extract position from transformation matrix
    Vector3 position;
    position.x = memory->Read<float>(boneMatrix + 0x0C);  // Matrix[0][3]
    position.y = memory->Read<float>(boneMatrix + 0x1C);  // Matrix[1][3]
    position.z = memory->Read<float>(boneMatrix + 0x2C);  // Matrix[2][3]
    
    return position;
}

bool EntityManager::IsPlayerAlive(uintptr_t entity) {
    if (!memory || !offsets) return false;
    
    Offsets& off = offsets->GetOffsets();
    int health = memory->Read<int>(entity + off.m_iHealth);
    
    return health > 0;
}

bool EntityManager::IsDormant(uintptr_t entity) {
    if (!memory || !offsets) return false;
    
    Offsets& off = offsets->GetOffsets();
    return memory->Read<bool>(entity + off.m_bDormant);
}
