#pragma once
#include "Pch.h"
#include "PlayerConfig.h"
#include "BossesConfig.h"
#include "SupplyConfig.h"
#include "BloodBondsConfig.h"
#include "TrapConfig.h"
#include "POIConfig.h"
#include "OverlayConfig.h"
#include "AimbotConfig.h"
class ConfigInstances
{
public:
	PlayerConfig Player = PlayerConfig(LIT("Player"));
	BossesConfig Bosses = BossesConfig(LIT("Bosses"));
	SupplyConfig Supply = SupplyConfig(LIT("Supply"));
	BloodBondsConfig BloodBonds = BloodBondsConfig(LIT("BloodBouds"));
	TrapConfig Trap = TrapConfig(LIT("Trap"));
	POIConfig POI = POIConfig(LIT("POI"));
	OverlayConfig Overlay = OverlayConfig(LIT("Overlay"));
	AimbotConfig Aimbot = AimbotConfig(LIT("Aimbot"));


	json ToJson()
	{
		json jsoned;
		jsoned.merge_patch(Player.ToJson());
		jsoned.merge_patch(Bosses.ToJson());
		jsoned.merge_patch(Supply.ToJson());
		jsoned.merge_patch(BloodBonds.ToJson());
		jsoned.merge_patch(Trap.ToJson());
		jsoned.merge_patch(POI.ToJson());

		jsoned.merge_patch(Overlay.ToJson());
		jsoned.merge_patch(Aimbot.ToJson());
		return jsoned;
	}

	void FromJson(json jsoned)
	{
		Player.FromJson(jsoned);
		Bosses.FromJson(jsoned);
		Supply.FromJson(jsoned);
		BloodBonds.FromJson(jsoned);
		Trap.FromJson(jsoned);
		POI.FromJson(jsoned);
		Overlay.FromJson(jsoned);
		Aimbot.FromJson(jsoned);
	}
};