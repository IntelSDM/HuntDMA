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

		/*
		* static std::unordered_map<entity_type_v2, entity_type_match_criteria> entity_type_to_match_criteria_map = {
		{
			{
				entity_type_v2::HUNTER, {
				.entity_name_substrings = { },
				.entity_class_name_substrings = { "HunterBasic" }
			}
		},
		{
					entity_type_v2::ZOMBIE, {
						.entity_name_substrings = { },
						.entity_class_name_substrings = { "grunt_base", "Immolator", "Waterdevil", "special_meathead", "dog_base" }
					},
		},
		{
					entity_type_v2::BOSS_ZOMBIE, {
						.entity_name_substrings = { },
						.entity_class_name_substrings = { "target_assassin", "target_butcher", "target_rotjaw", "special_spider", "target_scrapbeak" }
					},
		},
		{
					entity_type_v2::CLUE, {
						.entity_name_substrings = { },
						.entity_class_name_substrings = { "Clue_Base" }
					},
		},
		{
					entity_type_v2::EXTRACTION, {
						.entity_name_substrings = { },
						.entity_class_name_substrings = { "ExtractionPoint" }
					},
		},
		{
					entity_type_v2::WORLD_WEAPON, {
						.entity_name_substrings = { "lootable_saddle_a_weapon" },
						.entity_class_name_substrings = { "1rPistol", "1rRevolver", "2rRifle", "2rShotgun" }
					},
		},
		{
					entity_type_v2::WORLD_CONSUMABLE, {
						.entity_name_substrings = { },
						.entity_class_name_substrings = { "2tConsumable" }
					},
		},
		{
					entity_type_v2::WORLD_ITEM, {
						.entity_name_substrings = { },
						.entity_class_name_substrings = { "2eWorldLight", "2eWorldTrap", "2mWorldBlunt", "2mWorldStab" }
					},
		},
		{
					entity_type_v2::AMMO_BOX, {
						.entity_name_substrings = { "ammo_box", "ammo_package" },
						.entity_class_name_substrings = { }
					},
		},
		{
					entity_type_v2::AMMO_SWAP, {
						.entity_name_substrings = { },
						.entity_class_name_substrings = { "AmmoSwapBox" }
					},
		},
		{
					entity_type_v2::MEDKIT, {
						.entity_name_substrings = { "health_box" },
						.entity_class_name_substrings = { }
					},
		},
		{
					entity_type_v2::TOOLBOX, {
						.entity_name_substrings = { "item_box", "lootable_saddle_a_bag" },
						.entity_class_name_substrings = { }
					},
		},
		{
					entity_type_v2::TRAIT, {
						.entity_name_substrings = { },
						.entity_class_name_substrings = { "TraitCharm" }
					},
		},
		{
					entity_type_v2::BARREL, {
						.entity_name_substrings = { },
						.entity_class_name_substrings = { "BioBarrel", "Explodable_Object" }
					},
		},
		{
					entity_type_v2::LOOTABLE_MONEY, {
						.entity_name_substrings = { "currency_collection", "cash_register" },
						.entity_class_name_substrings = { }
					},
		},
		{
					entity_type_v2::POSTER, {
						.entity_name_substrings = { "poster" },
						.entity_class_name_substrings = { }
					},
		},
		{
					entity_type_v2::DESTROYABLE_REWARD, {
						.entity_name_substrings = { },
						.entity_class_name_substrings = { "DestroyableReward" }
					},
		},
		{
					entity_type_v2::HALLOWEEN_ROTTEN_ALTAR, {
						.entity_name_substrings = { },
						.entity_class_name_substrings = { "RottenAltar" }
					}
		}
		}
	};
		
	}

	
		*/
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
		
		if (strstr(ent->GetEntityClassName().name, "Immolator") != NULL)
		{
			ent->SetType(EntityType::Immolator);
			tempzombielist.push_back(ent);
			continue;
		}
		if (strstr(ent->GetEntityClassName().name, "grunt_base") != NULL)
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
		if (strstr(ent->GetEntityClassName().name, "ExtractionPoint") != NULL)
		{
			ent->SetType(EntityType::ExtractionPoint);
			tempzombielist.push_back(ent);
			continue;
		}
		if (strstr(ent->GetEntityClassName().name, "cash_register") != NULL)
		{
			ent->SetType(EntityType::CashRegister);
			tempzombielist.push_back(ent);
			continue;
		}
		if (strstr(ent->GetEntityClassName().name, "currency_collection") != NULL)
		{
			ent->SetType(EntityType::CurrencyCollection);
			tempzombielist.push_back(ent);
			continue;
		}
		if (strstr(ent->GetEntityClassName().name, "target_assassin") != NULL)
		{
			ent->SetType(EntityType::Assassin);
			tempzombielist.push_back(ent);
			continue;
		}
		if (strstr(ent->GetEntityClassName().name, "target_rotjaw") != NULL)
		{
			ent->SetType(EntityType::RotJaw);
			tempzombielist.push_back(ent);
			continue;
		}
		if (strstr(ent->GetEntityClassName().name, "target_scrapbeak") != NULL)
		{
			ent->SetType(EntityType::ScrapBeak);
			tempzombielist.push_back(ent);
			continue;
		}
		if (strstr(ent->GetEntityClassName().name, "dog_base") != NULL)
		{
			ent->SetType(EntityType::Dog);
			tempzombielist.push_back(ent);
			continue;
		}
		if (strstr(ent->GetEntityClassName().name, "Waterdevil") != NULL)
		{
			ent->SetType(EntityType::WaterDevil);
			tempzombielist.push_back(ent);
			continue;
		}
		if (((std::string)ent->GetEntityClassName().name) == "special_meathead")
		{
			ent->SetType(EntityType::MeatHead);
			tempzombielist.push_back(ent);
			continue;
		}
		if (((std::string)ent->GetEntityClassName().name) == "AmmoSwapBox")
		{
			ent->SetType(EntityType::AmmoBox);

				if (strstr(ent->GetEntityName().name, "bullets_spitzer") != NULL)
				{
					ent->SetType(EntityType::SpitzerBullets);
				}
				else if (strstr(ent->GetEntityName().name, "bullets_poison") != NULL)
				{
					ent->SetType(EntityType::PoisonBullets);
				}
				else if (strstr(ent->GetEntityName().name, "bullets_highvelocity") != NULL)
				{
					ent->SetType(EntityType::HighVelocityBullets);
					
				}
				else if (strstr(ent->GetEntityName().name, "bullets_incendiary") != NULL)
				{
					ent->SetType(EntityType::IncendiaryBullets);
				}
				else if (strstr(ent->GetEntityName().name, "bullets_dumdum") != NULL)
				{
					ent->SetType(EntityType::DumDumBullets);
				}
				else if (strstr(ent->GetEntityName().name, "bullets_explosive") != NULL)
				{
					ent->SetType(EntityType::ExplosiveBullets);
				}
				else if (strstr(ent->GetEntityName().name, "bullets_fmj") != NULL)
				{
					ent->SetType(EntityType::FMJBullets);
				}
				else if (strstr(ent->GetEntityName().name, "bolts_arrows_poison") != NULL)
				{
					ent->SetType(EntityType::PoisonBolts);
				}
				else if (strstr(ent->GetEntityName().name, "bolts_arrows_explosive") != NULL)
				{
					ent->SetType(EntityType::ExplosiveBolts);
				}
				else if (strstr(ent->GetEntityName().name, "shells_dragonbreath") != NULL)
				{
					ent->SetType(EntityType::IncendiaryShells);
				}
				else if (strstr(ent->GetEntityName().name, "shells_pennyshot") != NULL)
				{
					ent->SetType(EntityType::PennyShotShells);
				}
				else if (strstr(ent->GetEntityName().name, "shells_flechette") != NULL)
				{
					ent->SetType(EntityType::FlechetteShells);
				}
				else if (strstr(ent->GetEntityName().name, "shells_slugs") != NULL)
				{
					ent->SetType(EntityType::SlugShells);
				}

			tempstaticlist.push_back(ent);
			continue;
		}
		if (((std::string)ent->GetEntityClassName().name) == "Explodable_Object")
		{
			ent->SetType(EntityType::Explosive);
			tempstaticlist.push_back(ent);
			continue;
		}
		if (((std::string)ent->GetEntityClassName().name) == "BioBarrel")
		{
			ent->SetType(EntityType::Explosive);
			tempstaticlist.push_back(ent);
			continue;
		}
		if (((std::string)ent->GetEntityClassName().name) == "Supply_Box")
		{
			ent->SetType(EntityType::SupplyBox);
			if (strstr(ent->GetEntityName().name, "ammo_crate_normal") != NULL) // only in training mode
			{
				ent->SetType(EntityType::AmmoBox);
			}
			else if (strstr(ent->GetEntityName().name, "ammo_crate_special") != NULL) // only in training mode
			{
				ent->SetType(EntityType::SpecialAmmoCrate);
			}
			else if (strstr(ent->GetEntityName().name, "ammo_box_special") != NULL)
			{
				ent->SetType(EntityType::SpecialAmmoCrate);
			}
			else if (strstr(ent->GetEntityName().name, "ammo_box") != NULL)
			{
				ent->SetType(EntityType::AmmoBox);
			}
			else if (strstr(ent->GetEntityName().name, "ammo_package_medium") != NULL)
			{
				ent->SetType(EntityType::MediumAmmoPackage);
			}
			else if (strstr(ent->GetEntityName().name, "ammo_package_shotgun") != NULL)
			{
				ent->SetType(EntityType::ShotgunAmmoPackage);
			}
			else if (strstr(ent->GetEntityName().name, "ammo_package_compact") != NULL)
			{
				ent->SetType(EntityType::CompactAmmoPackage);
			}
			else if (strstr(ent->GetEntityName().name, "ammo_package_long") != NULL)
			{
				ent->SetType(EntityType::LongAmmoPackage);
			}
			else if (strstr(ent->GetEntityName().name, "health") != NULL)
			{
				ent->SetType(EntityType::Medkit);
			}

			tempstaticlist.push_back(ent);
			continue;
		}
		if (((std::string)ent->GetEntityClassName().name) == "beartrap")
		{
			ent->SetType(EntityType::BearTrap);
			tempstaticlist.push_back(ent);
			continue;
		}
		if (((std::string)ent->GetEntityClassName().name).find("TripMine") != std::string::npos && ((std::string)ent->GetEntityClassName().name).find("2") == std::string::npos)
		{
			ent->SetType(EntityType::TripMine);
			tempstaticlist.push_back(ent);
			continue;
		}
		if (strstr(ent->GetEntityName().name, "Grunts") != NULL)
		{
			ent->SetType(EntityType::Zombie);
			tempzombielist.push_back(ent);
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