#include "Pch.h"
#include "PlayerEsp.h"
#include "globals.h"
#include "CheatFunction.h"
#include "Drawing.h"
#include "ConfigInstance.h"
#include <WorldEntity.h>
#include "ConfigUtilities.h"


void DrawOtherEsp()
{
	if (EnvironmentInstance == nullptr)
		return;
	if (EnvironmentInstance->GetObjectCount() == 0)
		return;
	EnvironmentInstance->StaticListMutex.lock();
	std::vector<std::shared_ptr<WorldEntity>> templist = EnvironmentInstance->GetStaticList();
	EnvironmentInstance->StaticListMutex.unlock();
	if(templist.size() == 0)
		return;
	for (std::shared_ptr<WorldEntity> ent : templist)
	{
		if (ent == nullptr)
			continue;

		if (ent->GetType() == EntityType::BearTrap || ent->GetType() == EntityType::TripMine || ent->GetType() == EntityType::Barrel)
		{
			int distance = (int)Vector3::Distance(ent->GetPosition(), CameraInstance->GetPosition());
			if (distance <= 0)
				continue;
			if (distance > Configs.Trap.MaxDistance)
				continue;
			if (!ent->GetValid())
				continue;
			Vector2 pos = CameraInstance->WorldToScreen(ent->GetPosition());
			if (pos.x == 0 || pos.y == 0)
				continue;
			std::wstring wname = Configs.Trap.Name ? ent->GetName() : L"";
			std::wstring wdistance = Configs.Trap.Distance ? L"[" + std::to_wstring(distance) + L"m]" : L"";
			DrawText(pos.x, pos.y, wname + wdistance, "Verdana", Configs.Trap.FontSize, Configs.Trap.TextColour, Centre);
		}
		else
		{
			int distance = (int)Vector3::Distance(ent->GetPosition(), CameraInstance->GetPosition());
			if (distance <= 0)
				continue;
			if (distance > Configs.Supply.MaxDistance)
				continue;
			if (!ent->GetValid())
				continue;
			Vector2 pos = CameraInstance->WorldToScreen(ent->GetPosition());
			if (pos.x == 0 || pos.y == 0)
				continue;
			std::wstring wname = Configs.Supply.Name ? ent->GetName() : L"";
			std::wstring wdistance = Configs.Supply.Distance ? L"[" + std::to_wstring(distance) + L"m]" : L"";
			DrawText(pos.x, pos.y, wname + wdistance, "Verdana", Configs.Supply.FontSize, Configs.Supply.TextColour, Centre);
		}
		


	}
}