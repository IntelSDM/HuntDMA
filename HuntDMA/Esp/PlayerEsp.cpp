#include "Pch.h"
#include "PlayerEsp.h"
#include "globals.h"
#include "CheatFunction.h"
#include "Drawing.h"
#include "ConfigInstance.h"

std::shared_ptr<CheatFunction> UpdatePlayers = std::make_shared<CheatFunction>(10, [] {
	if (CurrentLocalPlayer == nullptr)
		return;
	auto handle = TargetProcess.CreateScatterHandle();
	for (auto player : BasePlayer->PlayerList)
	{

		if (player->GetPlayerNettworkState()->GetHealth() <= 0)
			continue;
		player->GetPlayerNettworkState()->UpdatePosition(handle);

		
	}
	TargetProcess.ExecuteReadScatter(handle);
	TargetProcess.CloseScatterHandle(handle);
	

});



void DrawPlayers()
{
	if (CurrentLocalPlayer == nullptr)
		return;
	for (auto player : BasePlayer->PlayerList)
	{
		if (player == CurrentLocalPlayer)
			continue;
		if (player->GetPlayerNettworkState()->GetHealth() <= 0)
			continue;
		if (player->GetPlayerNettworkState()->GetFriendly() == true)
			continue;

		Vector3 position = player->GetPlayerNettworkState()->GetPosition();
		int distance = Vector3::Distance(position, CurrentLocalPlayer->GetPlayerNettworkState()->GetPosition());
		if(distance > ConfigInstance.Player.MaxDistance)
					continue;
		Vector2 screenpos = Camera->WorldToScreen(position);
		if (screenpos == Vector2::Zero())
			continue;

		std::wstring distancestr = ConfigInstance.Player.Distance ? LIT(L"[") + std::to_wstring(distance) + LIT(L"m]") : LIT(L"");
		std::wstring healthstr = ConfigInstance.Player.Health ? LIT(L"[") + std::to_wstring((int)player->GetPlayerNettworkState()->GetHealth()) + LIT(L"❤]") : LIT(L"");;
		std::wstring primary = ConfigInstance.Player.PrimaryWeapon ? player->GetPlayerNettworkState()->GetPrimaryWeaponName() : LIT(L"");
		std::wstring secondary = ConfigInstance.Player.SecondaryWeapon ? player->GetPlayerNettworkState()->GetSecondaryWeaponName() : LIT(L"");

		DrawText(screenpos.x, screenpos.y, distancestr + healthstr, LIT("Verdana"), ConfigInstance.Player.FontSize, ConfigInstance.Player.TextColour, FontAlignment::Centre);
		DrawText(screenpos.x, screenpos.y + ConfigInstance.Player.FontSize + 2, primary, LIT("Verdana"), ConfigInstance.Player.FontSize, ConfigInstance.Player.TextColour, FontAlignment::Centre);
		DrawText(screenpos.x, screenpos.y + (ConfigInstance.Player.FontSize*2), secondary, LIT("Verdana"), ConfigInstance.Player.FontSize-1, ConfigInstance.Player.TextColour, FontAlignment::Centre);
	}

}