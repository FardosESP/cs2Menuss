// Config Manager - Task 19
// Real implementation with JSON serialization

#include "config/config_manager.h"
#include "esp/esp_renderer.h"
#include "ui/menu_manager.h"
#include <fstream>
#include <iostream>
#include <Windows.h>

// Temporary config structures (will be defined properly in menu_manager.h)
struct LegitConfig {
    bool enabled = false;
    bool aimbot = false;
    bool rcs = false;
    bool triggerbot = false;
    float fov = 5.0f;
    float smooth = 1.0f;
};

struct RageConfig {
    bool enabled = false;
    bool aimbot = false;
    bool autofire = false;
    bool multipoint = false;
    int hitbox = 0;
    float fov = 180.0f;
};

struct AntiAimConfig {
    bool enabled = false;
    int pitchMode = 0;
    int yawMode = 0;
    bool desync = false;
    bool fakelag = false;
};

struct VisualsConfig {
    bool glow = false;
    bool chams = false;
    bool noFlash = false;
    bool noSmoke = false;
    float fovChanger = 90.0f;
};

struct MiscConfig {
    bool bhop = false;
    bool autoStrafe = false;
    bool fastStop = false;
    bool doubleTap = false;
};

ConfigManager::ConfigManager() {
    // Get AppData path
    char* appDataPath = nullptr;
    size_t len = 0;
    _dupenv_s(&appDataPath, &len, "APPDATA");
    
    if (appDataPath) {
        configPath = std::string(appDataPath) + "\\Fardos.cc\\configs\\";
        free(appDataPath);
    } else {
        configPath = ".\\configs\\";
    }
    
    EnsureConfigDirectory();
}

void ConfigManager::EnsureConfigDirectory() {
    try {
        std::filesystem::create_directories(configPath);
    } catch (const std::exception& e) {
        std::cerr << "[!] Failed to create config directory: " << e.what() << std::endl;
    }
}

bool ConfigManager::SaveConfig(const std::string& name) {
    try {
        nlohmann::json config;
        config["version"] = CONFIG_VERSION;
        config["name"] = name;
        
        // TODO: Get actual config instances from global state
        // For now, create default configs
        ESPConfig espConfig{};
        espConfig.enabled = true;
        espConfig.box = true;
        espConfig.name = true;
        espConfig.health = true;
        espConfig.distance = false;
        espConfig.weapon = false;
        espConfig.skeleton = false;
        espConfig.snaplines = false;
        espConfig.teamCheck = true;
        espConfig.enemyColor = Color(1.0f, 0.0f, 0.0f, 1.0f);
        espConfig.teamColor = Color(0.0f, 1.0f, 0.0f, 1.0f);
        espConfig.boxColor = Color(1.0f, 1.0f, 1.0f, 1.0f);
        espConfig.boxThickness = 1.5f;
        
        config["esp"] = SerializeESPConfig(espConfig);
        
        // Save to file
        std::string filePath = configPath + name + ".json";
        std::ofstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "[!] Failed to open config file for writing: " << filePath << std::endl;
            return false;
        }
        
        file << config.dump(4);
        file.close();
        
        std::cout << "[+] Config saved: " << name << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "[!] Failed to save config: " << e.what() << std::endl;
        return false;
    }
}

bool ConfigManager::LoadConfig(const std::string& name) {
    try {
        std::string filePath = configPath + name + ".json";
        std::ifstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "[!] Failed to open config file for reading: " << filePath << std::endl;
            return false;
        }
        
        nlohmann::json config;
        file >> config;
        file.close();
        
        // Validate version
        if (!config.contains("version") || config["version"] != CONFIG_VERSION) {
            std::cerr << "[!] Config version mismatch" << std::endl;
            return false;
        }
        
        // TODO: Load into actual config instances
        // For now, just deserialize to verify format
        if (config.contains("esp")) {
            ESPConfig espConfig{};
            DeserializeESPConfig(config["esp"], espConfig);
        }
        
        std::cout << "[+] Config loaded: " << name << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "[!] Failed to load config: " << e.what() << std::endl;
        return false;
    }
}

std::vector<std::string> ConfigManager::GetConfigList() {
    std::vector<std::string> configs;
    
    try {
        if (!std::filesystem::exists(configPath)) {
            return configs;
        }
        
        for (const auto& entry : std::filesystem::directory_iterator(configPath)) {
            if (entry.is_regular_file() && entry.path().extension() == ".json") {
                configs.push_back(entry.path().stem().string());
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "[!] Failed to list configs: " << e.what() << std::endl;
    }
    
    return configs;
}

bool ConfigManager::DeleteConfig(const std::string& name) {
    try {
        std::string filePath = configPath + name + ".json";
        if (std::filesystem::remove(filePath)) {
            std::cout << "[+] Config deleted: " << name << std::endl;
            return true;
        }
        return false;
    } catch (const std::exception& e) {
        std::cerr << "[!] Failed to delete config: " << e.what() << std::endl;
        return false;
    }
}

// ESP Config Serialization
nlohmann::json ConfigManager::SerializeESPConfig(const ESPConfig& config) {
    nlohmann::json json;
    
    json["enabled"] = config.enabled;
    json["box"] = config.box;
    json["name"] = config.name;
    json["health"] = config.health;
    json["distance"] = config.distance;
    json["weapon"] = config.weapon;
    json["skeleton"] = config.skeleton;
    json["snaplines"] = config.snaplines;
    json["teamCheck"] = config.teamCheck;
    
    json["enemyColor"] = ColorToJson(config.enemyColor.r, config.enemyColor.g, 
                                     config.enemyColor.b, config.enemyColor.a);
    json["teamColor"] = ColorToJson(config.teamColor.r, config.teamColor.g, 
                                    config.teamColor.b, config.teamColor.a);
    json["boxColor"] = ColorToJson(config.boxColor.r, config.boxColor.g, 
                                   config.boxColor.b, config.boxColor.a);
    json["boxThickness"] = config.boxThickness;
    
    return json;
}

void ConfigManager::DeserializeESPConfig(const nlohmann::json& json, ESPConfig& config) {
    if (json.contains("enabled")) config.enabled = json["enabled"];
    if (json.contains("box")) config.box = json["box"];
    if (json.contains("name")) config.name = json["name"];
    if (json.contains("health")) config.health = json["health"];
    if (json.contains("distance")) config.distance = json["distance"];
    if (json.contains("weapon")) config.weapon = json["weapon"];
    if (json.contains("skeleton")) config.skeleton = json["skeleton"];
    if (json.contains("snaplines")) config.snaplines = json["snaplines"];
    if (json.contains("teamCheck")) config.teamCheck = json["teamCheck"];
    
    if (json.contains("enemyColor")) {
        JsonToColor(json["enemyColor"], config.enemyColor.r, config.enemyColor.g, 
                    config.enemyColor.b, config.enemyColor.a);
    }
    if (json.contains("teamColor")) {
        JsonToColor(json["teamColor"], config.teamColor.r, config.teamColor.g, 
                    config.teamColor.b, config.teamColor.a);
    }
    if (json.contains("boxColor")) {
        JsonToColor(json["boxColor"], config.boxColor.r, config.boxColor.g, 
                    config.boxColor.b, config.boxColor.a);
    }
    if (json.contains("boxThickness")) config.boxThickness = json["boxThickness"];
}

// Legit Config Serialization
nlohmann::json ConfigManager::SerializeLegitConfig(const LegitConfig& config) {
    nlohmann::json json;
    json["enabled"] = config.enabled;
    json["aimbot"] = config.aimbot;
    json["rcs"] = config.rcs;
    json["triggerbot"] = config.triggerbot;
    json["fov"] = config.fov;
    json["smooth"] = config.smooth;
    return json;
}

void ConfigManager::DeserializeLegitConfig(const nlohmann::json& json, LegitConfig& config) {
    if (json.contains("enabled")) config.enabled = json["enabled"];
    if (json.contains("aimbot")) config.aimbot = json["aimbot"];
    if (json.contains("rcs")) config.rcs = json["rcs"];
    if (json.contains("triggerbot")) config.triggerbot = json["triggerbot"];
    if (json.contains("fov")) config.fov = json["fov"];
    if (json.contains("smooth")) config.smooth = json["smooth"];
}

// Rage Config Serialization
nlohmann::json ConfigManager::SerializeRageConfig(const RageConfig& config) {
    nlohmann::json json;
    json["enabled"] = config.enabled;
    json["aimbot"] = config.aimbot;
    json["autofire"] = config.autofire;
    json["multipoint"] = config.multipoint;
    json["hitbox"] = config.hitbox;
    json["fov"] = config.fov;
    return json;
}

void ConfigManager::DeserializeRageConfig(const nlohmann::json& json, RageConfig& config) {
    if (json.contains("enabled")) config.enabled = json["enabled"];
    if (json.contains("aimbot")) config.aimbot = json["aimbot"];
    if (json.contains("autofire")) config.autofire = json["autofire"];
    if (json.contains("multipoint")) config.multipoint = json["multipoint"];
    if (json.contains("hitbox")) config.hitbox = json["hitbox"];
    if (json.contains("fov")) config.fov = json["fov"];
}

// Anti-Aim Config Serialization
nlohmann::json ConfigManager::SerializeAntiAimConfig(const AntiAimConfig& config) {
    nlohmann::json json;
    json["enabled"] = config.enabled;
    json["pitchMode"] = config.pitchMode;
    json["yawMode"] = config.yawMode;
    json["desync"] = config.desync;
    json["fakelag"] = config.fakelag;
    return json;
}

void ConfigManager::DeserializeAntiAimConfig(const nlohmann::json& json, AntiAimConfig& config) {
    if (json.contains("enabled")) config.enabled = json["enabled"];
    if (json.contains("pitchMode")) config.pitchMode = json["pitchMode"];
    if (json.contains("yawMode")) config.yawMode = json["yawMode"];
    if (json.contains("desync")) config.desync = json["desync"];
    if (json.contains("fakelag")) config.fakelag = json["fakelag"];
}

// Visuals Config Serialization
nlohmann::json ConfigManager::SerializeVisualsConfig(const VisualsConfig& config) {
    nlohmann::json json;
    json["glow"] = config.glow;
    json["chams"] = config.chams;
    json["noFlash"] = config.noFlash;
    json["noSmoke"] = config.noSmoke;
    json["fovChanger"] = config.fovChanger;
    return json;
}

void ConfigManager::DeserializeVisualsConfig(const nlohmann::json& json, VisualsConfig& config) {
    if (json.contains("glow")) config.glow = json["glow"];
    if (json.contains("chams")) config.chams = json["chams"];
    if (json.contains("noFlash")) config.noFlash = json["noFlash"];
    if (json.contains("noSmoke")) config.noSmoke = json["noSmoke"];
    if (json.contains("fovChanger")) config.fovChanger = json["fovChanger"];
}

// Misc Config Serialization
nlohmann::json ConfigManager::SerializeMiscConfig(const MiscConfig& config) {
    nlohmann::json json;
    json["bhop"] = config.bhop;
    json["autoStrafe"] = config.autoStrafe;
    json["fastStop"] = config.fastStop;
    json["doubleTap"] = config.doubleTap;
    return json;
}

void ConfigManager::DeserializeMiscConfig(const nlohmann::json& json, MiscConfig& config) {
    if (json.contains("bhop")) config.bhop = json["bhop"];
    if (json.contains("autoStrafe")) config.autoStrafe = json["autoStrafe"];
    if (json.contains("fastStop")) config.fastStop = json["fastStop"];
    if (json.contains("doubleTap")) config.doubleTap = json["doubleTap"];
}

// Helper methods
nlohmann::json ConfigManager::ColorToJson(float r, float g, float b, float a) {
    nlohmann::json json;
    json["r"] = r;
    json["g"] = g;
    json["b"] = b;
    json["a"] = a;
    return json;
}

void ConfigManager::JsonToColor(const nlohmann::json& json, float& r, float& g, float& b, float& a) {
    if (json.contains("r")) r = json["r"];
    if (json.contains("g")) g = json["g"];
    if (json.contains("b")) b = json["b"];
    if (json.contains("a")) a = json["a"];
}
