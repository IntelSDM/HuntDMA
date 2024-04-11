#include "Pch.h"
#include "aimbot.h"
#include "PlayerEsp.h"
#include "globals.h"
#include "CheatFunction.h"
#include "Drawing.h"
#include "ConfigInstance.h"
#include <WorldEntity.h>
#include "ConfigUtilities.h"
#include "Kmbox.h"
#include "InputManager.h"

int ConditionalSwapPlayer(std::vector<std::shared_ptr<WorldEntity>>& entities, int low, int high)
{
	std::shared_ptr<WorldEntity> pivot = entities[high];
	int i = low - 1;
	Vector2 centreofscreen = Vector2(Configs.Overlay.OverrideResolution ? Configs.Overlay.Width / 2 : GetSystemMetrics(SM_CXSCREEN) / 2, Configs.Overlay.OverrideResolution ? Configs.Overlay.Height *0.6f : GetSystemMetrics(SM_CYSCREEN) *0.6f);
	for (int j = low; j < high; ++j)
	{
		if (Configs.Aimbot.Priority == 2)
		{
			if (Vector2::Distance(centreofscreen, CameraInstance->WorldToScreen(entities[j]->GetPosition())) < Vector2::Distance(centreofscreen, CameraInstance->WorldToScreen(pivot->GetPosition())))
			{
				++i;
				std::swap(entities[i], entities[j]);
				continue;
			}
			if (Vector3::Distance(CameraInstance->GetPosition(), entities[j]->GetPosition()) < Vector3::Distance(CameraInstance->GetPosition(), pivot->GetPosition()))
			{
				++i;
				std::swap(entities[i], entities[j]);
			}
		}
		if (Configs.Aimbot.Priority == 0)
		{
			if (Vector3::Distance(CameraInstance->GetPosition(), entities[j]->GetPosition()) < Vector3::Distance(CameraInstance->GetPosition(), pivot->GetPosition()))
			{
				++i;
				std::swap(entities[i], entities[j]);
			}
		}
		if (Configs.Aimbot.Priority == 1)
		{
			if (Vector2::Distance(centreofscreen, CameraInstance->WorldToScreen(entities[j]->GetPosition())) < Vector2::Distance(centreofscreen, CameraInstance->WorldToScreen(pivot->GetPosition())))
			{
				++i;
				std::swap(entities[i], entities[j]);
			}
		}
	}
	std::swap(entities[i + 1], entities[high]);
	return i + 1;
}

void QuickSortPlayers(std::vector<std::shared_ptr<WorldEntity>>& entities, int low, int high)
{
	if (low < high)
	{
		int pi = ConditionalSwapPlayer(entities, low, high);
		QuickSortPlayers(entities, low, pi - 1);
		QuickSortPlayers(entities, pi + 1, high);
	}
}
std::shared_ptr<WorldEntity> AimbotTarget;
bool StickTarget()
{
	Vector2 centreofscreen = Vector2(Configs.Overlay.OverrideResolution ? Configs.Overlay.Width / 2 : GetSystemMetrics(SM_CXSCREEN) / 2, Configs.Overlay.OverrideResolution ? Configs.Overlay.Height * 0.6f : GetSystemMetrics(SM_CYSCREEN) * 0.6f);
	if (CameraInstance == nullptr)
		return false;
	if (EnvironmentInstance == nullptr)
		return false;
	if (AimbotTarget== nullptr)
		return false;	
	if (!AimbotTarget->GetValid())
		return false;
	if (Vector3::Distance(CameraInstance->GetPosition(), AimbotTarget->GetPosition()) > Configs.Aimbot.MaxDistance)
		return false;
	if (CameraInstance->WorldToScreen(AimbotTarget->GetPosition()) == Vector2::Zero())
		return false;
	if (AimbotTarget->GetType() == EntityType::EnemyPlayer && !Configs.Aimbot.TargetPlayers)
		return false;
	if (Vector2::Distance(CameraInstance->WorldToScreen(AimbotTarget->GetPosition()), centreofscreen) > Configs.Aimbot.FOV)
		return false;
	return true;
}

void GetAimbotTarget()
{
	if (CameraInstance == nullptr)
		return;
	if (EnvironmentInstance == nullptr)
		return;
	if (!Configs.Aimbot.Enable)
		return;
	if(StickTarget())
		return;
	Vector2 centreofscreen = Vector2(Configs.Overlay.OverrideResolution ? Configs.Overlay.Width / 2 : GetSystemMetrics(SM_CXSCREEN) / 2, Configs.Overlay.OverrideResolution ? Configs.Overlay.Height * 0.6f : GetSystemMetrics(SM_CYSCREEN) * 0.6f);

	std::vector<std::shared_ptr<WorldEntity>> templist;
	Vector3 localpos = CameraInstance->GetPosition();
	std::lock_guard<std::mutex> lock(EnvironmentInstance->PlayerListMutex);
	{
		templist = EnvironmentInstance->GetPlayerList();
	}
	if (templist.size() == 0)
	{
		return;
	}

	QuickSortPlayers(templist, 0, templist.size() - 1);

	for (std::shared_ptr<WorldEntity> player : templist)
	{
		if(player == nullptr)
			continue;
		if (!Configs.Aimbot.TargetPlayers)
			continue;
		if (!player->GetValid())
			continue;
		if (player->GetType()  == EntityType::FriendlyPlayer)
			continue;
		if (Vector3::Distance(CameraInstance->GetPosition(), player->GetPosition()) <= 2.0f) // local player
			continue;
		if (Vector3::Distance(CameraInstance->GetPosition(), player->GetPosition()) > Configs.Aimbot.MaxDistance)
			continue;
		if (CameraInstance->WorldToScreen(player->GetPosition()) == Vector2::Zero())
			continue;
		if (Vector2::Distance(CameraInstance->WorldToScreen(player->GetPosition()), centreofscreen) >Configs.Aimbot.FOV)
			continue;
		AimbotTarget = player;
		//	printf("Targeting: %s\n", AimbotTarget->GetName().c_str());
		return;
	}
	AimbotTarget = nullptr;
}
bool AimKeyDown = false;
std::shared_ptr<CheatFunction> UpdateAimKey = std::make_shared<CheatFunction>(50, [] {
	if (EnvironmentInstance == nullptr)
		return;
	if (EnvironmentInstance->GetObjectCount() == 0)
		return;
	if (Keyboard::IsKeyDown(Configs.Aimbot.Aimkey))
	{
		AimKeyDown = true;
	}
	else
	{
		AimKeyDown = false;
	}
	});
std::chrono::system_clock::time_point KmboxStart;
void Aimbot()
{  
	UpdateAimKey->Execute();
	if (!kmbox::connected || !AimKeyDown)
	{
		AimbotTarget = nullptr;
		return;
	}
		GetAimbotTarget();
		if (AimbotTarget == nullptr)
			return;
		
		if (AimbotTarget->GetPosition() == Vector3::Zero())
		{
			AimbotTarget = nullptr;
			return;
		}
		Vector2 screenpos = CameraInstance->WorldToScreen(AimbotTarget->GetPosition());
		Vector2 centreofscreen = Vector2(Configs.Overlay.OverrideResolution ? Configs.Overlay.Width / 2 : GetSystemMetrics(SM_CXSCREEN) / 2, Configs.Overlay.OverrideResolution ? Configs.Overlay.Height * 0.6f : GetSystemMetrics(SM_CYSCREEN) * 0.6f);
		if (Vector2::Distance(screenpos, centreofscreen) > Configs.Aimbot.FOV)
			return;
		if (screenpos == Vector2::Zero())
		{
			AimbotTarget = nullptr;
			return;
		}
		
		float x = screenpos.x - centreofscreen.x;
		
		if (KmboxStart + std::chrono::milliseconds(55) < std::chrono::system_clock::now())
		{
			kmbox::move(x,0);
			KmboxStart = std::chrono::system_clock::now();
		}
	
}