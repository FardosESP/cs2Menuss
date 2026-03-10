// ESP Renderer - Task 12
// Real ESP rendering with ImGui

#include "esp/esp_renderer.h"
#include "imgui.h"
#include <sstream>
#include <iomanip>

void ESPRenderer::Render() {
    if (!config.enabled || !entityManager) return;
    
    drawList = ImGui::GetBackgroundDrawList();
    if (!drawList) return;
    
    // Get all players
    std::vector<EntityData> players = entityManager->GetPlayers();
    EntityData localPlayer = entityManager->GetLocalPlayer();
    
    for (const auto& player : players) {
        // Skip if not alive
        if (!player.isAlive) continue;
        
        // Team check
        if (config.teamCheck && player.team == localPlayer.team) {
            continue;
        }
        
        // Determine color
        Color color = (player.team == localPlayer.team) ? config.teamColor : config.enemyColor;
        
        // Render ESP elements
        if (config.box) {
            RenderBox(player, color);
        }
        
        Vector2 screenPos;
        if (WorldToScreen(player.origin, screenPos)) {
            if (config.name) {
                RenderName(player, screenPos);
            }
            
            if (config.health) {
                RenderHealth(player, screenPos);
            }
            
            if (config.distance) {
                float dist = localPlayer.origin.Distance(player.origin) / 100.0f; // Convert to meters
                std::stringstream ss;
                ss << std::fixed << std::setprecision(1) << dist << "m";
                
                ImVec2 textSize = ImGui::CalcTextSize(ss.str().c_str());
                ImVec2 textPos(screenPos.x - textSize.x / 2, screenPos.y + 30);
                
                drawList->AddText(textPos, ImColor(255, 255, 255, 255), ss.str().c_str());
            }
        }
        
        if (config.skeleton) {
            RenderSkeleton(player);
        }
        
        if (config.snaplines) {
            RECT rect;
            GetClientRect(GetForegroundWindow(), &rect);
            float centerX = (rect.right - rect.left) / 2.0f;
            float bottomY = static_cast<float>(rect.bottom - rect.top);
            
            Vector2 screenPos;
            if (WorldToScreen(player.origin, screenPos)) {
                drawList->AddLine(
                    ImVec2(centerX, bottomY),
                    ImVec2(screenPos.x, screenPos.y),
                    color.ToImU32(),
                    1.0f
                );
            }
        }
    }
}

void ESPRenderer::RenderBox(const EntityData& entity, const Color& color) {
    // Get head and feet positions
    Vector2 headScreen, feetScreen;
    
    if (!WorldToScreen(entity.headPosition, headScreen)) return;
    if (!WorldToScreen(entity.origin, feetScreen)) return;
    
    // Calculate box dimensions
    float height = feetScreen.y - headScreen.y;
    float width = height * 0.4f;  // Box width is 40% of height
    
    Vector2 topLeft(headScreen.x - width / 2, headScreen.y);
    Vector2 bottomRight(headScreen.x + width / 2, feetScreen.y);
    
    DrawBox2D(topLeft, bottomRight, color, config.boxThickness);
}

void ESPRenderer::RenderName(const EntityData& entity, const Vector2& screenPos) {
    if (entity.name[0] == '\0') return;
    
    ImVec2 textSize = ImGui::CalcTextSize(entity.name);
    ImVec2 textPos(screenPos.x - textSize.x / 2, screenPos.y - 40);
    
    // Draw text with outline for visibility
    drawList->AddText(ImVec2(textPos.x + 1, textPos.y + 1), ImColor(0, 0, 0, 255), entity.name);
    drawList->AddText(textPos, ImColor(255, 255, 255, 255), entity.name);
}

void ESPRenderer::RenderHealth(const EntityData& entity, const Vector2& screenPos) {
    // Health bar
    Vector2 headScreen;
    if (!WorldToScreen(entity.headPosition, headScreen)) return;
    
    float barWidth = 50.0f;
    float barHeight = 4.0f;
    float healthPercent = entity.health / 100.0f;
    
    // Background
    drawList->AddRectFilled(
        ImVec2(headScreen.x - barWidth / 2, headScreen.y - 15),
        ImVec2(headScreen.x + barWidth / 2, headScreen.y - 15 + barHeight),
        ImColor(0, 0, 0, 180)
    );
    
    // Health bar (green to red gradient based on health)
    Color healthColor;
    if (healthPercent > 0.5f) {
        healthColor = Color(1.0f - (healthPercent - 0.5f) * 2.0f, 1.0f, 0.0f);
    } else {
        healthColor = Color(1.0f, healthPercent * 2.0f, 0.0f);
    }
    
    drawList->AddRectFilled(
        ImVec2(headScreen.x - barWidth / 2, headScreen.y - 15),
        ImVec2(headScreen.x - barWidth / 2 + barWidth * healthPercent, headScreen.y - 15 + barHeight),
        healthColor.ToImU32()
    );
    
    // Health text
    std::stringstream ss;
    ss << entity.health << " HP";
    ImVec2 textSize = ImGui::CalcTextSize(ss.str().c_str());
    ImVec2 textPos(headScreen.x - textSize.x / 2, headScreen.y - 25);
    
    drawList->AddText(ImVec2(textPos.x + 1, textPos.y + 1), ImColor(0, 0, 0, 255), ss.str().c_str());
    drawList->AddText(textPos, ImColor(255, 255, 255, 255), ss.str().c_str());
}

void ESPRenderer::RenderSkeleton(const EntityData& entity) {
    // Skeleton bone connections
    const int boneConnections[][2] = {
        {static_cast<int>(BoneId::Head), static_cast<int>(BoneId::Neck)},
        {static_cast<int>(BoneId::Neck), static_cast<int>(BoneId::UpperChest)},
        {static_cast<int>(BoneId::UpperChest), static_cast<int>(BoneId::Chest)},
        {static_cast<int>(BoneId::Chest), static_cast<int>(BoneId::LowerChest)},
        {static_cast<int>(BoneId::LowerChest), static_cast<int>(BoneId::Pelvis)},
        
        // Left arm
        {static_cast<int>(BoneId::UpperChest), static_cast<int>(BoneId::LeftShoulder)},
        {static_cast<int>(BoneId::LeftShoulder), static_cast<int>(BoneId::LeftElbow)},
        {static_cast<int>(BoneId::LeftElbow), static_cast<int>(BoneId::LeftHand)},
        
        // Right arm
        {static_cast<int>(BoneId::UpperChest), static_cast<int>(BoneId::RightShoulder)},
        {static_cast<int>(BoneId::RightShoulder), static_cast<int>(BoneId::RightElbow)},
        {static_cast<int>(BoneId::RightElbow), static_cast<int>(BoneId::RightHand)},
        
        // Left leg
        {static_cast<int>(BoneId::Pelvis), static_cast<int>(BoneId::LeftHip)},
        {static_cast<int>(BoneId::LeftHip), static_cast<int>(BoneId::LeftKnee)},
        {static_cast<int>(BoneId::LeftKnee), static_cast<int>(BoneId::LeftFoot)},
        
        // Right leg
        {static_cast<int>(BoneId::Pelvis), static_cast<int>(BoneId::RightHip)},
        {static_cast<int>(BoneId::RightHip), static_cast<int>(BoneId::RightKnee)},
        {static_cast<int>(BoneId::RightKnee), static_cast<int>(BoneId::RightFoot)}
    };
    
    for (const auto& connection : boneConnections) {
        Vector3 bone1Pos = entityManager->GetBonePosition(entity.address, connection[0]);
        Vector3 bone2Pos = entityManager->GetBonePosition(entity.address, connection[1]);
        
        Vector2 bone1Screen, bone2Screen;
        if (WorldToScreen(bone1Pos, bone1Screen) && WorldToScreen(bone2Pos, bone2Screen)) {
            drawList->AddLine(
                ImVec2(bone1Screen.x, bone1Screen.y),
                ImVec2(bone2Screen.x, bone2Screen.y),
                config.boxColor.ToImU32(),
                2.0f
            );
        }
    }
}

void ESPRenderer::SetConfig(const ESPConfig& newConfig) {
    config = newConfig;
}

void ESPRenderer::DrawBox2D(const Vector2& topLeft, const Vector2& bottomRight,
                            const Color& color, float thickness) {
    drawList->AddRect(
        ImVec2(topLeft.x, topLeft.y),
        ImVec2(bottomRight.x, bottomRight.y),
        color.ToImU32(),
        0.0f,
        0,
        thickness
    );
}

void ESPRenderer::DrawBox3D(const Vector3 corners[8], const Color& color) {
    // 3D box rendering (for future use)
    // Not implemented yet
}
