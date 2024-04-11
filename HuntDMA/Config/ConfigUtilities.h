#pragma once
#include "ConfigInstance.h"
void SaveConfig(const std::wstring& config);
void LoadConfig(const std::wstring& config);
std::vector<std::wstring> GetAllConfigs();
extern ConfigInstances Configs;
extern void SetUpConfig();