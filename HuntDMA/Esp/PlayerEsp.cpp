#include "Pch.h"
#include "PlayerEsp.h"
#include "globals.h"
#include "CheatFunction.h"
#include "Drawing.h"
#include "ConfigInstance.h"
#include <WorldEntity.h>
#include "ConfigUtilities.h"

std::shared_ptr<CheatFunction> UpdatePlayers = std::make_shared<CheatFunction>(10, [] {
	
	EnvironmentInstance->UpdatePlayerList();
});
std::shared_ptr<CheatFunction> UpdateBosses = std::make_shared<CheatFunction>(20, [] {
	EnvironmentInstance->UpdateBossesList();
});

void DrawBosses()
{
	if (EnvironmentInstance == nullptr)
		return;

	if (EnvironmentInstance->GetObjectCount() == 0)
		return;

	if (Configs.Bosses.Enable)
	{
		EnvironmentInstance->BossesListMutex.lock();
		std::vector<std::shared_ptr<WorldEntity>> templist = EnvironmentInstance->GetBossesList();
		EnvironmentInstance->BossesListMutex.unlock();
		if (templist.size() != 0)
		{
			for (std::shared_ptr<WorldEntity> ent : templist)
			{

				if (ent == nullptr)
					continue;
				int distance = (int)Vector3::Distance(ent->GetPosition(), CameraInstance->GetPosition());
				if (distance <= 0)
					continue;
				if (distance > Configs.Bosses.MaxDistance)
					continue;
				if (!ent->GetValid())
					continue;
				Vector2 pos = CameraInstance->WorldToScreen(ent->GetPosition());
				if (pos.x == 0 || pos.y == 0)
					continue;
				std::wstring wname = Configs.Bosses.Name ? ent->GetName() : L"";
				std::wstring wdistance = Configs.Bosses.Distance ? L"[" + std::to_wstring(distance) + L"m]" : L"";
				DrawText(pos.x, pos.y, wname + wdistance, "Verdana", Configs.Bosses.FontSize, Configs.Bosses.TextColour, Centre);
			}
		}
	}
}
void DrawPlayers()
{
	if (EnvironmentInstance == nullptr)
		return;
	
	if (EnvironmentInstance->GetObjectCount() == 0)
		return;
	
	if (Configs.Player.Enable)
	{
		EnvironmentInstance->PlayerListMutex.lock();
		std::vector<std::shared_ptr<WorldEntity>> templist = EnvironmentInstance->GetPlayerList();
		EnvironmentInstance->PlayerListMutex.unlock();
		if (templist.size() == 0)
			return;
		for (std::shared_ptr<WorldEntity> ent : templist)
		{
			if (ent == nullptr)
				continue;
			if (!Configs.Player.DrawFriends && ent->GetType() == EntityType::FriendlyPlayer)
				continue;
			int distance = (int)Vector3::Distance(ent->GetPosition(), CameraInstance->GetPosition());
			if (distance <= 0)
				continue;
			if (Vector3::Distance(CameraInstance->GetPosition(), ent->GetPosition()) <= 2.0f)
				continue;
			if (distance > Configs.Player.MaxDistance)
				continue;
			if (!ent->GetValid())
				continue;
			if (!IsValidHP(ent->GetHealth().current_hp) ||
				!IsValidHP(ent->GetHealth().current_max_hp) ||
				!IsValidHP(ent->GetHealth().regenerable_max_hp))
				continue;
			Vector2 pos = CameraInstance->WorldToScreen(ent->GetPosition());
			if (pos.x == 0 || pos.y == 0)
				continue;
			std::wstring wname = Configs.Player.Name ? ent->GetName() : L"";
			std::wstring wdistance = Configs.Player.Distance ? L"[" + std::to_wstring(distance) + L"m]" : L"";
			std::wstring whealth = std::to_wstring(ent->GetHealth().current_hp) + L"/" + std::to_wstring(ent->GetHealth().current_max_hp) + L"[" + std::to_wstring(ent->GetHealth().regenerable_max_hp) + L"]";
			//DrawText(pos.x, pos.y, L".", "Verdana", Configs.Player.FontSize, Colour(255, 0, 0), Centre); // dot
			DrawText(pos.x, pos.y, wname + wdistance + L"\n" + whealth, "Verdana", Configs.Player.FontSize, ent->GetType() == EntityType::FriendlyPlayer ? Configs.Player.FriendColour : Configs.Player.TextColour, Centre);
		}
	}
}

bool IsValidHP(int hp)
{
	return hp > 0 && hp <= 150;
}