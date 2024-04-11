#include "Pch.h"
#include "ConfigUtilities.h"
#include "ConfigInstance.h"

json Config;
std::string Path;
ConfigInstances Configs;

void SetUpConfig()
{
    std::filesystem::path filePath = std::filesystem::path(getenv(LIT("USERPROFILE"))) / LIT("Documents") / LIT("Hunt") / (LIT("Default.json"));
    std::filesystem::create_directories(filePath.parent_path());
    std::filesystem::exists(filePath) ? LoadConfig(LIT(L"Default")) : SaveConfig(LIT(L"Default"));
}
void SaveConfig(const std::wstring& config)
{
    std::string cfg(config.begin(), config.end());
    Config = json::array();
    Config = Configs.ToJson();
    std::string jsonstring = Config.dump() + "\n";

    std::filesystem::path filePath = std::filesystem::path(getenv(LIT("USERPROFILE"))) / LIT("Documents") / LIT("Hunt") / (cfg + LIT(".json"));

    std::filesystem::create_directories(filePath.parent_path());

    std::ofstream file(filePath);
    if (file.is_open()) {
        file << jsonstring;
        file.close();
    }

}

void LoadConfig(const std::wstring& config)
{
    std::string cfg(config.begin(), config.end());

    std::filesystem::path filePath = std::filesystem::path(getenv(LIT("USERPROFILE"))) / LIT("Documents") / LIT("Hunt") / (cfg + LIT(".json"));

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
        else {

        }
        file.close();
    }
    else {

    }
}

std::vector<std::wstring> GetAllConfigs()
{
    std::vector<std::wstring> confignames;

    std::filesystem::path directory = std::filesystem::path(getenv(LIT("USERPROFILE"))) / LIT("Documents") / LIT("Hunt");

    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        if (entry.is_regular_file()) {
            std::wstring configname = entry.path().filename().stem();
            confignames.push_back(configname);
        }
    }

    return confignames;
}