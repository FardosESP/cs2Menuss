#pragma once

#include "common/types.h"
#include "entity/entity_manager.h"

// Forward declaration
struct ImDrawList;

// ESP Renderer - Task 12

struct ESPConfig {
    bool enabled;
    bool box;
    bool name;
    bool health;
    bool distance;
    bool weapon;
    bool skeleton;
    bool snaplines;
    bool teamCheck;

    Color enemyColor;
    Color teamColor;
    Color boxColor;
    float boxThickness;
};

class ESPRenderer {
public:
    ESPRenderer() = default;
    ~ESPRenderer() = default;

    void Render();
    void RenderBox(const EntityData& entity, const Color& color);
    void RenderName(const EntityData& entity, const Vector2& screenPos);
    void RenderHealth(const EntityData& entity, const Vector2& screenPos);
    void RenderSkeleton(const EntityData& entity);
    void SetConfig(const ESPConfig& config);

private:
    ESPConfig config{};
    EntityManager* entityManager = nullptr;
    ImDrawList* drawList = nullptr;

    bool WorldToScreen(const Vector3& world, Vector2& screen);
    void DrawBox2D(const Vector2& topLeft, const Vector2& bottomRight,
                   const Color& color, float thickness);
    void DrawBox3D(const Vector3 corners[8], const Color& color);
};
