#pragma once

#include <string>
#include <vector>
#include <filesystem>
#include <nlohmann/json.hpp>

// Forward declarations
struct ESPConfig;
struct LegitConfig;
struct RageConfig;
struct AntiAimConfig;
struct VisualsConfig;
struct MiscConfig;

// Config Manager - Task 19
// Handles saving/loading configurations to/from JSON files

class ConfigManager {
public:
    ConfigManager();
    ~ConfigManager() = default;

    // Config file operations
    bool SaveConfig(const std::string& name);
    bool LoadConfig(const std::string& name);
    std::vector<std::string> GetConfigList();
    bool DeleteConfig(const std::string& name);
    
    // Get config directory path
    std::string GetConfigPath() const { return configPath; }
    
    // Version info
    static constexpr int CONFIG_VERSION = 1;

private:
    std::string configPath;
    
    // Ensure config directory exists
    void EnsureConfigDirectory();
    
    // Serialization methods
    nlohmann::json SerializeESPConfig(const ESPConfig& config);
    void DeserializeESPConfig(const nlohmann::json& json, ESPConfig& config);
    
    nlohmann::json SerializeLegitConfig(const LegitConfig& config);
    void DeserializeLegitConfig(const nlohmann::json& json, LegitConfig& config);
    
    nlohmann::json SerializeRageConfig(const RageConfig& config);
    void DeserializeRageConfig(const nlohmann::json& json, RageConfig& config);
    
    nlohmann::json SerializeAntiAimConfig(const AntiAimConfig& config);
    void DeserializeAntiAimConfig(const nlohmann::json& json, AntiAimConfig& config);
    
    nlohmann::json SerializeVisualsConfig(const VisualsConfig& config);
    void DeserializeVisualsConfig(const nlohmann::json& json, VisualsConfig& config);
    
    nlohmann::json SerializeMiscConfig(const MiscConfig& config);
    void DeserializeMiscConfig(const nlohmann::json& json, MiscConfig& config);
    
    // Helper methods
    nlohmann::json ColorToJson(float r, float g, float b, float a);
    void JsonToColor(const nlohmann::json& json, float& r, float& g, float& b, float& a);
};
