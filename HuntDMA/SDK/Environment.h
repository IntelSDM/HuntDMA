#pragma once
#include "WorldEntity.h"
class Environment
{
private:
	uint64_t SystemGlobalEnvironment = 0x5EF2FA0;
	uint64_t EntitySystem = 0xA8;
	uint64_t pSystem = 0xC0;
	uint16_t ObjectCount = 0x0;
	uint64_t EntityList = 0x0;
	uint16_t ObjectCountOffset = 0x4006A;
	uint64_t EntityListOffset = 0x40078;
	std::vector<std::shared_ptr<WorldEntity>> PlayerList;
	std::vector<std::shared_ptr<WorldEntity>> ZombieList;
	std::vector<std::shared_ptr<WorldEntity>> StaticList;
public:
	uint64_t GetSystemGlobalEnvironment() { return SystemGlobalEnvironment; }
	uint64_t GetEntitySystem() { return EntitySystem; }
	uint64_t GetpSystem() { return pSystem; }
	uint16_t GetObjectCount() { return ObjectCount; }
	uint64_t GetEntityList() { return EntityList; }
	void GetEntitys();
	void UpdatePlayerList();
	void UpdateZombieList();
	void CacheEntities();
	Environment();
	std::mutex PlayerListMutex;
	std::mutex ZombieListMutex;
	std::mutex StaticListMutex;
	std::vector<std::shared_ptr<WorldEntity>> GetPlayerList() { return PlayerList; }
	std::vector<std::shared_ptr<WorldEntity>> GetZombieList() { return ZombieList; }
	std::vector<std::shared_ptr<WorldEntity>> GetStaticList() { return StaticList; }
};