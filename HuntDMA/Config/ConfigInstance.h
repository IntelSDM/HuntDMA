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
		return jsoned;
	}

	void FromJson(json jsoned)
	{
		Player.FromJson(jsoned);
	}
};