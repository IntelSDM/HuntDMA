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
	//for (std::shared_ptr<WorldEntity> ent : templist)
	for (size_t index = 0; index < templist.size(); ++index)
	{
		std::shared_ptr<WorldEntity> ent = templist[index];
		if (ent == nullptr)
			continue;
		if (Vector3::Distance(ent->GetPosition(), CameraInstance->GetPosition()) <= 1.6f) // Self Player
		{
			continue;
		}	
		if (!(ent->GetClass() > 0x2000000 && ent->GetClass() < 0x7FFFFFFFFFFF))
		{
			ent->SetValid(false); // check if the player is still alive/ active
			continue;
		}
		if (Configs.Player.Chams)
			ent->WriteNode(writehandle, Configs.Player.ChamMode, Configs.Player.Chams);
		ent->UpdateNode(handle);
		ent->UpdatePosition(handle);
		ent->UpdateHealth(handle);
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
		ent->UpdateHealth(handle);
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
	std::vector<std::shared_ptr<WorldEntity>> tempbosseslist;
	std::vector<std::shared_ptr<WorldEntity>> tempstaticlist;
	for (std::shared_ptr<WorldEntity> ent : entitypointerlist)
	{
		if (ent == nullptr)
			continue;
		char* entityName = ent->GetEntityName().name;
		char* entityClassName = ent->GetEntityClassName().name;

		if (((std::string)entityName) == "Hunter_Loot")
		{
			continue;
		}
		else if (strstr(entityClassName, "HunterBasic") != NULL)
		{
			// print ent->GetRenderNode().rnd_flags
		
			ent->SetType(EntityType::EnemyPlayer);
			templayerlist.push_back(ent);
			//printf(LIT("Entity Flags: %d\n"), ent->GetRenderNode().rnd_flags);
			//printf(LIT("Entity Class: %s\n"), entityName);
			continue;
		}
		else if (strstr(entityName, "HunterBasic") != NULL)
		{
			// print ent->GetRenderNode().rnd_flags

			ent->SetType(EntityType::EnemyPlayer);
			templayerlist.push_back(ent);
			//printf(LIT("Entity Ent Name: %s\n"), entityClassName);
			continue;
		}
		else if (strstr(entityClassName, "target_assassin") != NULL)
		{
			ent->SetType(EntityType::Assassin);
			tempbosseslist.push_back(ent);
			continue;
		}
		else if (strstr(entityClassName, "target_butcher") != NULL)
		{
			ent->SetType(EntityType::Butcher);
			tempbosseslist.push_back(ent);
			continue;
		}
		else if (strstr(entityClassName, "special_spider") != NULL)
		{
			ent->SetType(EntityType::Spider);
			tempbosseslist.push_back(ent);
			continue;
		}
		else if (strstr(entityClassName, "target_scrapbeak") != NULL)
		{
			ent->SetType(EntityType::Scrapbeak);
			tempbosseslist.push_back(ent);
			continue;
		}
		else if (strstr(entityClassName, "target_rotjaw") != NULL)
		{
			ent->SetType(EntityType::Rotjaw);
			tempbosseslist.push_back(ent);
			continue;
		}
		else if (strstr(entityClassName, "immolator_elite") != NULL)
		{
			ent->SetType(EntityType::Hellborn);
			tempbosseslist.push_back(ent);
			continue;
		}
		else if (strstr(entityClassName, "ExtractionPoint") != NULL)
		{
			ent->SetType(EntityType::ExtractionPoint);
			tempstaticlist.push_back(ent);
			continue;
		}
		else if (strstr(entityClassName, "cash_register_golden") != NULL)
		{
			ent->SetType(EntityType::GoldCashRegister);
			tempstaticlist.push_back(ent);
			continue;
		}
		else if (strstr(entityClassName, "cash_register") != NULL)
		{
			ent->SetType(EntityType::CashRegister);
			tempstaticlist.push_back(ent);
			continue;
		}
		else if (strstr(entityClassName, "currency_collection") != NULL)
		{
			ent->SetType(EntityType::CurrencyCollection);
			tempstaticlist.push_back(ent);
			continue;
		}
		else if ((std::string)(entityClassName) == "AmmoSwapBox")
		{
			ent->SetType(EntityType::AmmoBox);

			if (strstr(entityName, "bullets_spitzer") != NULL)
			{
				ent->SetType(EntityType::SpitzerBullets);
			}
			else if (strstr(entityName, "bullets_poison") != NULL)
			{
				ent->SetType(EntityType::PoisonBullets);
			}
			else if (strstr(entityName, "bullets_highvelocity") != NULL)
			{
				ent->SetType(EntityType::HighVelocityBullets);

			}
			else if (strstr(entityName, "bullets_incendiary") != NULL)
			{
				ent->SetType(EntityType::IncendiaryBullets);
			}
			else if (strstr(entityName, "bullets_dumdum") != NULL)
			{
				ent->SetType(EntityType::DumDumBullets);
			}
			else if (strstr(entityName, "bullets_explosive") != NULL)
			{
				ent->SetType(EntityType::ExplosiveBullets);
			}
			else if (strstr(entityName, "bullets_fmj") != NULL)
			{
				ent->SetType(EntityType::FMJBullets);
			}
			else if (strstr(entityName, "bolts_arrows_poison") != NULL)
			{
				ent->SetType(EntityType::PoisonBolts);
			}
			else if (strstr(entityName, "bolts_arrows_explosive") != NULL)
			{
				ent->SetType(EntityType::ExplosiveBolts);
			}
			else if (strstr(entityName, "shells_dragonbreath") != NULL)
			{
				ent->SetType(EntityType::IncendiaryShells);
			}
			else if (strstr(entityName, "shells_pennyshot") != NULL)
			{
				ent->SetType(EntityType::PennyShotShells);
			}
			else if (strstr(entityName, "shells_flechette") != NULL)
			{
				ent->SetType(EntityType::FlechetteShells);
			}
			else if (strstr(entityName, "shells_slugs") != NULL)
			{
				ent->SetType(EntityType::SlugShells);
			}

			tempstaticlist.push_back(ent);
			continue;
		}
		else if ((std::string)(entityClassName) == "Explodable_Object")
		{
			ent->SetType(EntityType::Barrel);
			tempstaticlist.push_back(ent);
			continue;
		}
		else if ((std::string)(entityClassName) == "BioBarrel")
		{
			ent->SetType(EntityType::Barrel);
			tempstaticlist.push_back(ent);
			continue;
		}
		else if ((std::string)(entityClassName) == "Supply_Box")
		{
			ent->SetType(EntityType::SupplyBox);
			if (strstr(entityName, "ammo_crate_normal") != NULL) // only in training mode
			{
				ent->SetType(EntityType::AmmoBox);
			}
			else if (strstr(entityName, "ammo_crate_special") != NULL) // only in training mode
			{
				ent->SetType(EntityType::SpecialAmmoCrate);
			}
			else if (strstr(entityName, "ammo_box_special") != NULL)
			{
				ent->SetType(EntityType::SpecialAmmoCrate);
			}
			else if (strstr(entityName, "ammo_box") != NULL)
			{
				ent->SetType(EntityType::AmmoBox);
			}
			else if (strstr(entityName, "ammo_package_medium") != NULL)
			{
				ent->SetType(EntityType::MediumAmmoPackage);
			}
			else if (strstr(entityName, "ammo_package_shotgun") != NULL)
			{
				ent->SetType(EntityType::ShotgunAmmoPackage);
			}
			else if (strstr(entityName, "ammo_package_compact") != NULL)
			{
				ent->SetType(EntityType::CompactAmmoPackage);
			}
			else if (strstr(entityName, "ammo_package_long") != NULL)
			{
				ent->SetType(EntityType::LongAmmoPackage);
			}
			else if (strstr(entityName, "health") != NULL)
			{
				ent->SetType(EntityType::Medkit);
			}

			tempstaticlist.push_back(ent);
			continue;
		}
		else if ((std::string)(entityClassName) == "beartrap")
		{
			ent->SetType(EntityType::BearTrap);
			tempstaticlist.push_back(ent);
			continue;
		}
		else if (((std::string)entityClassName).find("TripMine") != std::string::npos && ((std::string)entityClassName).find("2") == std::string::npos)
		{
			ent->SetType(EntityType::TripMine);
			tempstaticlist.push_back(ent);
			continue;
		}
		//printf(LIT("Entity Position: %f %f %f\n"), ent->GetPosition().x, ent->GetPosition().y, ent->GetPosition().z);
		//printf(LIT("Entity ClassName: %s\n"), entityClassName);
		//printf(LIT("Entity Class: %s\n"), entityName);
		//printf(LIT("Entity Silhouettes: %d\n"), ent->GetRenderNode().silhouettes_param);
		//Vector2 screenpos = CameraInstance->WorldToScreen(ent->GetPosition());
		//printf(LIT("Entity Screen Position: %f %f\n"), screenpos.x, screenpos.y);
	}

	handle = TargetProcess.CreateScatterHandle();
	for (std::shared_ptr<WorldEntity> ent : templayerlist)
	{
		if (ent == nullptr)
			continue;
		ent->SetUp3(handle);
	}
	for (std::shared_ptr<WorldEntity> ent : tempbosseslist)
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
		if (ent->GetRenderNode().silhouettes_param == 0x8CD2FF)
		{
			ent->SetType(EntityType::FriendlyPlayer);
		}
	}

	handle = TargetProcess.CreateScatterHandle();
	for (std::shared_ptr<WorldEntity> ent : templayerlist)
	{
		if (ent == nullptr)
			continue;
		ent->SetUp4(handle);
	}
	TargetProcess.ExecuteReadScatter(handle);
	TargetProcess.CloseScatterHandle(handle);

	PlayerListMutex.lock();
	PlayerList = templayerlist;
	PlayerListMutex.unlock();
	ZombieListMutex.lock();
	ZombieList = tempbosseslist;
	ZombieListMutex.unlock();
	StaticListMutex.lock();
	StaticList = tempstaticlist;
	StaticListMutex.unlock();
}
