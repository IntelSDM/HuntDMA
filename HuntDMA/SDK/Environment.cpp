#include "Pch.h"
#include "Environment.h"
#include "WorldEntity.h"
#include "Globals.h"
#include "ConfigUtilities.h"

Environment::Environment()
{
	SystemGlobalEnvironment = TargetProcess.Read<uint64_t>(TargetProcess.GetBaseAddress("GameHunt.dll") + SystemGlobalEnvironment);
	printf(LIT("SystemGlobalEnvironment: 0x%X\n"), SystemGlobalEnvironment);
	EntitySystem = TargetProcess.Read<uint64_t>(SystemGlobalEnvironment + EntitySystem);
	printf(LIT("EntitySystem: 0x%X\n"), EntitySystem);
	pSystem = TargetProcess.Read<uint64_t>(SystemGlobalEnvironment + pSystem);
	printf(LIT("pSystem: 0x%X\n"), pSystem);
}

void Environment::GetEntitys()
{
	ObjectCount = TargetProcess.Read<uint16_t>(EntitySystem + ObjectCountOffset);
	printf(LIT("ObjectCount: %d\n"), ObjectCount);
	EntityList = EntitySystem + EntityListOffset;
}

void Environment::UpdatePlayerList()
{
	EnvironmentInstance->PlayerListMutex.lock();
	std::vector<std::shared_ptr<WorldEntity>> templist = EnvironmentInstance->GetPlayerList();
	EnvironmentInstance->PlayerListMutex.unlock();
	if (templist.size() == 0)
		return;
	auto handle = TargetProcess.CreateScatterHandle();
	auto writehandle = TargetProcess.CreateScatterHandle();
	for (std::shared_ptr<WorldEntity> ent : templist)
	{
		if (ent == nullptr)
			continue;
		if (Vector3::Distance(ent->GetPosition(), CameraInstance->GetPosition()) <= 1)
			continue; // local player, ignore
		if (!(ent->GetClass() > 0x2000000 && ent->GetClass() < 0x7FFFFFFFFFFF))
		{
			ent->SetValid(false); // check if the player is still alive/ active
			continue;
		}
		if (Configs.Player.Chams)
		ent->WriteNode(writehandle, Configs.Player.ChamMode);
		ent->UpdateNode(handle);
		ent->UpdatePosition(handle);
		ent->UpdateClass(handle);
	}
	TargetProcess.ExecuteReadScatter(handle);
	TargetProcess.ExecuteWriteScatter(writehandle);
	TargetProcess.CloseScatterHandle(handle);

	EnvironmentInstance->PlayerListMutex.lock();
	PlayerList = templist;
	EnvironmentInstance->PlayerListMutex.unlock();
}

void Environment::UpdateZombieList()
{
	EnvironmentInstance->ZombieListMutex.lock();
	std::vector<std::shared_ptr<WorldEntity>> templist = EnvironmentInstance->GetZombieList();
	EnvironmentInstance->ZombieListMutex.unlock();
	if (templist.size() == 0)
		return;
	auto handle = TargetProcess.CreateScatterHandle();
	auto writehandle = TargetProcess.CreateScatterHandle();
	for (std::shared_ptr<WorldEntity> ent : templist)
	{
		if (ent == nullptr)
			continue;
		if (!(ent->GetClass() > 0x2000000 && ent->GetClass() < 0x7FFFFFFFFFFF))
		{
			ent->SetValid(false);
			continue;
		}
		ent->UpdateNode(handle);
		ent->UpdatePosition(handle);
		ent->UpdateClass(handle);
	}
	TargetProcess.ExecuteReadScatter(handle);
	TargetProcess.ExecuteWriteScatter(writehandle);
	TargetProcess.CloseScatterHandle(handle);

	EnvironmentInstance->ZombieListMutex.lock();
	ZombieList = templist;
	EnvironmentInstance->ZombieListMutex.unlock();
}

void Environment::CacheEntities()
{
	GetEntitys();
	std::vector<uint64_t> entitylist;
	entitylist.resize(ObjectCount);
	std::unique_ptr<uint64_t[]> object_raw_ptr = std::make_unique<uint64_t[]>(ObjectCount);
	TargetProcess.Read(EntityList, object_raw_ptr.get(), ObjectCount * sizeof(uint64_t));
	for (size_t i = 0; i < ObjectCount; i++)
	{
		entitylist[i] = object_raw_ptr[i];
	}
	std::vector<std::shared_ptr<WorldEntity>> entitypointerlist;
	auto handle = TargetProcess.CreateScatterHandle();
	for (int i = 0; i < ObjectCount; i++)
	{
		uint64_t entity = entitylist[i];
		
		if (entity == NULL) 
		{
			continue;
		}
		entitypointerlist.push_back(std::make_shared<WorldEntity>(entity));
	}
	TargetProcess.ExecuteReadScatter(handle);
	TargetProcess.CloseScatterHandle(handle);
	handle = TargetProcess.CreateScatterHandle();
	for (std::shared_ptr<WorldEntity> ent : entitypointerlist)
	{
		if (ent == nullptr)
			continue;
		ent->SetUp(handle);
	}
	TargetProcess.ExecuteReadScatter(handle);
	TargetProcess.CloseScatterHandle(handle);
	handle = TargetProcess.CreateScatterHandle();
	for (std::shared_ptr<WorldEntity> ent : entitypointerlist)
	{
		if (ent == nullptr)
			continue;
		ent->SetUp1(handle);
	}
	TargetProcess.ExecuteReadScatter(handle);
	TargetProcess.CloseScatterHandle(handle);

	handle = TargetProcess.CreateScatterHandle();
	for (std::shared_ptr<WorldEntity> ent : entitypointerlist)
	{
		if (ent == nullptr)
			continue;
		ent->SetUp2(handle);
	}
	TargetProcess.ExecuteReadScatter(handle);
	TargetProcess.CloseScatterHandle(handle);

	// doing this after we have read class names fully to avoid reading things we don't need

	std::vector<std::shared_ptr<WorldEntity>> templayerlist;
	std::vector<std::shared_ptr<WorldEntity>> tempzombielist;
	std::vector<std::shared_ptr<WorldEntity>> tempstaticlist;
	for (std::shared_ptr<WorldEntity> ent : entitypointerlist)
	{
		if (ent == nullptr)
			continue;
		if (((std::string)ent->GetEntityName().name) == "Hunter_Loot")
		{
			continue;
		}
		if (strstr(ent->GetEntityClassName().name, "HunterBasic") != NULL)
		{
			// print ent->GetRenderNode().rnd_flags
		
			ent->SetType(EntityType::EnemyPlayer);
			templayerlist.push_back(ent);
			//printf(LIT("Entity Flags: %d\n"), ent->GetRenderNode().rnd_flags);
			//printf(LIT("Entity Class: %s\n"), ent->GetEntityName().name);
			continue;
		}
		if (strstr(ent->GetEntityName().name, "HunterBasic") != NULL)
		{
			// print ent->GetRenderNode().rnd_flags

			ent->SetType(EntityType::EnemyPlayer);
			templayerlist.push_back(ent);
		//	printf(LIT("Entity Ent Name: %s\n"), ent->GetEntityClassName().name);
			continue;
		}
		if (strstr(ent->GetEntityClassName().name, "target_assassin") != NULL)
		{
			ent->SetType(EntityType::Zombie);
			tempzombielist.push_back(ent);
			continue;
		}
		if (strstr(ent->GetEntityClassName().name, "target_butcher") != NULL)
		{
			ent->SetType(EntityType::Butcher);
			tempzombielist.push_back(ent);
			continue;
		}
		if (strstr(ent->GetEntityClassName().name, "special_spider") != NULL)
		{
			ent->SetType(EntityType::Spider);
			tempzombielist.push_back(ent);
			continue;
		}
		if (strstr(ent->GetEntityClassName().name, "target_scrapbeak") != NULL)
		{
			ent->SetType(EntityType::Dog);
			tempzombielist.push_back(ent);
			continue;
		}
		if (strstr(ent->GetEntityClassName().name, "target_rotjaw") != NULL)
		{
			ent->SetType(EntityType::WaterDevil);
			tempzombielist.push_back(ent);
			continue;
		}
		/* if (((std::string)ent->GetEntityClassName().name) == "special_meathead")
		{
			ent->SetType(EntityType::MeatHead);
			tempzombielist.push_back(ent);
			continue;
		}
		*/
		if (((std::string)ent->GetEntityClassName().name) == "AmmoSwapBox")
		{
			ent->SetType(EntityType::AmmoBox);
			tempstaticlist.push_back(ent);
			continue;
		}
		if (((std::string)ent->GetEntityClassName().name) == "Supply_Box")
		{
			ent->SetType(EntityType::SupplyBox);
			tempstaticlist.push_back(ent);
			continue;
		}
		if (((std::string)ent->GetEntityClassName().name) == "beartrap")
		{
			ent->SetType(EntityType::BearTrap);
			tempstaticlist.push_back(ent);
			continue;
		}
		if (((std::string)ent->GetEntityClassName().name) == "poisonbomb")
		{
			ent->SetType(EntityType::PoisonTrap);
			tempstaticlist.push_back(ent);
			continue;
		}
		//	printf(LIT("Entity Position: %f %f %f\n"), ent->GetPosition().x, ent->GetPosition().y, ent->GetPosition().z);
		//	printf(LIT("Entity ClassName: %s\n"), ent->GetEntityClassName().name);
		//	printf(LIT("Entity Class: %s\n"), ent->GetEntityName().name);
		//	printf(LIT("Entity Silhouettes: %d\n"), ent->GetRenderNode().silhouettes_param);
		//	Vector2 screenpos = CameraInstance->WorldToScreen(ent->GetPosition());
		//	printf(LIT("Entity Screen Position: %f %f\n"), screenpos.x, screenpos.y);
	}

	handle = TargetProcess.CreateScatterHandle();
	for (std::shared_ptr<WorldEntity> ent : templayerlist)
	{
		if (ent == nullptr)
			continue;
		ent->SetUp3(handle);
	}
	for (std::shared_ptr<WorldEntity> ent : tempzombielist)
	{
		if (ent == nullptr)
			continue;
		ent->SetUp3(handle);
	}
	for (std::shared_ptr<WorldEntity> ent : tempstaticlist)
	{
		if (ent == nullptr)
			continue;
		ent->SetUp3(handle);
	}
	TargetProcess.ExecuteReadScatter(handle);
	TargetProcess.CloseScatterHandle(handle);

	for (std::shared_ptr<WorldEntity> ent : templayerlist) // got to do this after set up 3
	{
		if (ent->GetRenderNode().silhouettes_param == 0x00FFFFFF)
		{
			ent->SetType(EntityType::FriendlyPlayer);
		}
	}

	PlayerListMutex.lock();
	PlayerList = templayerlist;
	PlayerListMutex.unlock();
	ZombieListMutex.lock();
	ZombieList = tempzombielist;
	ZombieListMutex.unlock();
	StaticListMutex.lock();
	StaticList = tempstaticlist;
	StaticListMutex.unlock();
}
