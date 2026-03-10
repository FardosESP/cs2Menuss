// WorldToScreen - Task 11
// Real 3D to 2D coordinate transformation using game's view matrix

#include "esp/esp_renderer.h"
#include "memory/memory.h"
#include "memory/offset_manager.h"

bool ESPRenderer::WorldToScreen(const Vector3& world, Vector2& screen) {
    if (!entityManager) return false;
    
    Memory mem;
    uintptr_t clientBase = mem.GetModuleBase("client.dll");
    if (clientBase == 0) return false;
    
    // Get offset manager to access view matrix offset
    static OffsetManager offsetMgr;
    static bool initialized = false;
    if (!initialized) {
        offsetMgr.Initialize();
        initialized = true;
    }
    
    Offsets& offsets = offsetMgr.GetOffsets();
    
    // Read view matrix from game (real 4x4 matrix used by Source 2 engine)
    ViewMatrix viewMatrix;
    uintptr_t viewMatrixAddr = clientBase + offsets.dwViewMatrix;
    
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            viewMatrix.matrix[i][j] = mem.Read<float>(viewMatrixAddr + (i * 16) + (j * 4));
        }
    }
    
    // Transform world position to clip space using matrix multiplication
    float clipX = world.x * viewMatrix.matrix[0][0] + 
                  world.y * viewMatrix.matrix[0][1] + 
                  world.z * viewMatrix.matrix[0][2] + 
                  viewMatrix.matrix[0][3];
                  
    float clipY = world.x * viewMatrix.matrix[1][0] + 
                  world.y * viewMatrix.matrix[1][1] + 
                  world.z * viewMatrix.matrix[1][2] + 
                  viewMatrix.matrix[1][3];
                  
    float clipW = world.x * viewMatrix.matrix[3][0] + 
                  world.y * viewMatrix.matrix[3][1] + 
                  world.z * viewMatrix.matrix[3][2] + 
                  viewMatrix.matrix[3][3];
    
    // Check if behind camera (W component must be positive)
    if (clipW < 0.1f) {
        return false;
    }
    
    // Normalize to NDC (Normalized Device Coordinates) [-1, 1]
    float ndcX = clipX / clipW;
    float ndcY = clipY / clipW;
    
    // Check if outside screen bounds
    if (ndcX < -1.0f || ndcX > 1.0f || ndcY < -1.0f || ndcY > 1.0f) {
        return false;
    }
    
    // Get screen dimensions
    RECT rect;
    GetClientRect(GetForegroundWindow(), &rect);
    float screenWidth = static_cast<float>(rect.right - rect.left);
    float screenHeight = static_cast<float>(rect.bottom - rect.top);
    
    // Convert NDC to screen pixel coordinates
    screen.x = (screenWidth / 2.0f) * (1.0f + ndcX);
    screen.y = (screenHeight / 2.0f) * (1.0f - ndcY);  // Flip Y axis
    
    return true;
}
