#pragma once
#include "Pch.h"
#include "PlayerConfig.h"

class ConfigInstances
{
public:
	PlayerConfig Player = PlayerConfig(LIT("Player"));

	json ToJson()
	{
		json jsoned;
		jsoned.merge_patch(Player.ToJson());
	}

	void FromJson(json jsoned)
	{
		Player.FromJson(jsoned);
	}
};
inline ConfigInstances ConfigInstance;