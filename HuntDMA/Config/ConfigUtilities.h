#pragma once
#include "ConfigInstance.h"

extern std::filesystem::path DefaultConfigPath;
extern std::filesystem::path ConfigPath;
extern ConfigInstances Configs;
extern void SetUpConfig();

void SaveConfig(const std::filesystem::path& filePath);
void LoadConfig(const std::filesystem::path& filePath);