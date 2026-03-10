#pragma once

#include <vector>
#include <cstdint>
#include "common/types.h"
#include "memory/memory.h"
#include "memory/offset_manager.h"

// Entity Manager - Task 10
// TODO: Implement in Task 10

struct EntityData {
    uintptr_t address;
    int index;
    int health;
    int team;
    Vector3 origin;
    Vector3 headPosition;
    bool isDormant;
    bool isAlive;
    char name[128];
    int weaponId;
};

class EntityManager {
public:
    EntityManager() = default;
    ~EntityManager() = default;

    // TODO: Implement these methods in Task 10
    void Update();
    std::vector<EntityData> GetPlayers();
    EntityData GetLocalPlayer();
    bool IsEntityValid(uintptr_t entity);
    Vector3 GetBonePosition(uintptr_t entity, int boneId);

private:
    std::vector<EntityData> cachedEntities;
    EntityData localPlayer{};
    Memory* memory = nullptr;
    OffsetManager* offsets = nullptr;

    bool IsPlayerAlive(uintptr_t entity);
    bool IsDormant(uintptr_t entity);
};
