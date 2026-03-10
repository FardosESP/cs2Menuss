#pragma once

#include <cstdint>
#include <cmath>

// Vector structures
struct Vector2 {
    float x, y;

    Vector2() : x(0.0f), y(0.0f) {}
    Vector2(float x, float y) : x(x), y(y) {}

    Vector2 operator+(const Vector2& other) const {
        return Vector2(x + other.x, y + other.y);
    }

    Vector2 operator-(const Vector2& other) const {
        return Vector2(x - other.x, y - other.y);
    }

    Vector2 operator*(float scalar) const {
        return Vector2(x * scalar, y * scalar);
    }

    float Length() const {
        return std::sqrt(x * x + y * y);
    }
};

struct Vector3 {
    float x, y, z;

    Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    Vector3 operator+(const Vector3& other) const {
        return Vector3(x + other.x, y + other.y, z + other.z);
    }

    Vector3 operator-(const Vector3& other) const {
        return Vector3(x - other.x, y - other.y, z - other.z);
    }

    Vector3 operator*(float scalar) const {
        return Vector3(x * scalar, y * scalar, z * scalar);
    }

    float Distance(const Vector3& other) const {
        float dx = x - other.x;
        float dy = y - other.y;
        float dz = z - other.z;
        return std::sqrt(dx * dx + dy * dy + dz * dz);
    }

    float Length() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    Vector3 Normalize() const {
        float len = Length();
        if (len > 0.0f) {
            return Vector3(x / len, y / len, z / len);
        }
        return Vector3(0.0f, 0.0f, 0.0f);
    }
};

// View matrix for WorldToScreen
struct ViewMatrix {
    float matrix[4][4];
};

// Color structure (compatible with ImGui)
struct Color {
    float r, g, b, a;

    Color() : r(1.0f), g(1.0f), b(1.0f), a(1.0f) {}
    Color(float r, float g, float b, float a = 1.0f) : r(r), g(g), b(b), a(a) {}

    // Convert to ImU32 for ImGui
    unsigned int ToImU32() const {
        return ((unsigned int)(a * 255.0f) << 24) |
               ((unsigned int)(b * 255.0f) << 16) |
               ((unsigned int)(g * 255.0f) << 8) |
               ((unsigned int)(r * 255.0f));
    }
};

// Bone IDs for CS2
enum class BoneId : int {
    Head = 6,
    Neck = 5,
    UpperChest = 4,
    Chest = 3,
    LowerChest = 2,
    Pelvis = 0,
    LeftShoulder = 8,
    LeftElbow = 9,
    LeftHand = 10,
    RightShoulder = 13,
    RightElbow = 14,
    RightHand = 15,
    LeftHip = 22,
    LeftKnee = 23,
    LeftFoot = 24,
    RightHip = 25,
    RightKnee = 26,
    RightFoot = 27
};

// Team IDs
enum class Team : int {
    None = 0,
    Spectator = 1,
    Terrorist = 2,
    CounterTerrorist = 3
};

// Weapon IDs (common ones)
enum class WeaponId : int {
    None = 0,
    Deagle = 1,
    Elite = 2,
    Fiveseven = 3,
    Glock = 4,
    AK47 = 7,
    AUG = 8,
    AWP = 9,
    M4A1 = 16,
    M4A1S = 60,
    // Add more as needed
};
