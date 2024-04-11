#pragma once
#include "Pch.h"
#include "PlayerConfig.h"
#include "ZombieConfig.h"
#include "SupplyConfig.h"
#include "TrapConfig.h"
class ConfigInstances
{
public:
	PlayerConfig Player = PlayerConfig(LIT("Player"));
	ZombieConfig Zombie = ZombieConfig(LIT("Zombie"));
	SupplyConfig Supply = SupplyConfig(LIT("Supply"));
	TrapConfig Trap = TrapConfig(LIT("Trap"));


	json ToJson()
	{
		json jsoned;
		jsoned.merge_patch(Player.ToJson());
		jsoned.merge_patch(Zombie.ToJson());
		jsoned.merge_patch(Supply.ToJson());
		jsoned.merge_patch(Trap.ToJson());
		return jsoned;
	}

	void FromJson(json jsoned)
	{
		Player.FromJson(jsoned);
		Zombie.FromJson(jsoned);
		Supply.FromJson(jsoned);
		Trap.FromJson(jsoned);
	}
};