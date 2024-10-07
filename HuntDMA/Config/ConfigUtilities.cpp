#include "Pch.h"
#include "ConfigUtilities.h"
#include "ConfigInstance.h"

json Config;
ConfigInstances Configs;
std::filesystem::path DefaultConfigPath = std::filesystem::path(LIT("DefaultConfig.json"));
std::filesystem::path ConfigPath = std::filesystem::path(LIT("Config.json"));

void SetUpConfig()
{
    SaveConfig(DefaultConfigPath);
    std::filesystem::exists(ConfigPath) ? LoadConfig(ConfigPath) : SaveConfig(ConfigPath);
}
void SaveConfig(const std::filesystem::path& filePath)
{
    Config = json::array();
    Config = Configs.ToJson();
    std::string jsonstring = Config.dump() + "\n";

    std::ofstream file(filePath);
    if (file.is_open()) {
        file << jsonstring;
        file.close();
    }

}

void LoadConfig(const std::filesystem::path& filePath)
{
    if (!std::filesystem::exists(filePath)) {
        return;
    }

    std::ifstream file(filePath);
    if (file.is_open()) {
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string jsonstring = buffer.str();

        if (json::accept(jsonstring)) {
            Config = json::parse(jsonstring);
            Configs.FromJson(Config);

        }
        file.close();
    }
}