#include "Pch.h"
#include "PlayerEsp.h"
#include "globals.h"
#include "CheatFunction.h"
#include "Drawing.h"
#include "ConfigInstance.h"
#include <WorldEntity.h>
#include "ConfigUtilities.h"

void DrawSupply()
{
	EnvironmentInstance->SupplyListMutex.lock();
	std::vector<std::shared_ptr<WorldEntity>> templist = EnvironmentInstance->GetSupplyList();
	EnvironmentInstance->SupplyListMutex.unlock();

	if (templist.size() == 0)
		return;

	for (std::shared_ptr<WorldEntity> ent : templist)
	{
		if (ent == nullptr)
			continue;

		auto type = ent->GetType();
		if (!Configs.Supply.ShowAmmoSwapBox &&
			(type == EntityType::AmmoSwapBox ||
				type == EntityType::SpitzerBullets ||
				type == EntityType::PoisonBullets ||
				type == EntityType::HighVelocityBullets ||
				type == EntityType::IncendiaryBullets ||
				type == EntityType::DumDumBullets ||
				type == EntityType::ExplosiveBullets ||
				type == EntityType::FMJBullets ||
				type == EntityType::PoisonBolts ||
				type == EntityType::ExplosiveBolts ||
				type == EntityType::IncendiaryShells ||
				type == EntityType::PennyShotShells ||
				type == EntityType::FlechetteShells ||
				type == EntityType::SlugShells))
			continue;
		if (!Configs.Supply.ShowCompactAmmo && (type == EntityType::CompactAmmoPackage))
			continue;
		if (!Configs.Supply.ShowMediumAmmo && (type == EntityType::MediumAmmoPackage))
			continue;
		if (!Configs.Supply.ShowLongAmmo && (type == EntityType::LongAmmoPackage))
			continue;
		if (!Configs.Supply.ShowShortgunAmmo && (type == EntityType::ShotgunAmmoPackage))
			continue;
		if (!Configs.Supply.ShowSpecialAmmo && (type == EntityType::SpecialAmmoCrate))
			continue;
		if ((!Configs.Supply.ShowCompactAmmo &&
			!Configs.Supply.ShowMediumAmmo &&
			!Configs.Supply.ShowLongAmmo &&
			!Configs.Supply.ShowShortgunAmmo) && (type == EntityType::AmmoCrate))
			continue;
		if (!Configs.Supply.ShowMedkit && (type == EntityType::Medkit))
			continue;
		if (!Configs.Supply.ShowSupplyBox && (type == EntityType::SupplyBox))
			continue;

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

void DrawBloodBonds()
{
	EnvironmentInstance->BloodBondsListMutex.lock();
	std::vector<std::shared_ptr<WorldEntity>> templist = EnvironmentInstance->GetBloodBondsList();
	EnvironmentInstance->BloodBondsListMutex.unlock();

	if (templist.size() == 0)
		return;

	for (std::shared_ptr<WorldEntity> ent : templist)
	{
		if (ent == nullptr)
			continue;

		int distance = (int)Vector3::Distance(ent->GetPosition(), CameraInstance->GetPosition());
		if (distance <= 0)
			continue;
		if (distance > Configs.BloodBonds.MaxDistance)
			continue;
		if (!ent->GetValid())
			continue;
		Vector2 pos = CameraInstance->WorldToScreen(ent->GetPosition());
		if (pos.x == 0 || pos.y == 0)
			continue;

		std::wstring wname = Configs.BloodBonds.Name ? ent->GetName() : L"";
		std::wstring wdistance = Configs.BloodBonds.Distance ? L"[" + std::to_wstring(distance) + L"m]" : L"";
		DrawText(pos.x, pos.y, wname + wdistance, "Verdana", Configs.BloodBonds.FontSize, Configs.BloodBonds.TextColour, Centre);
	}
}

void DrawTraps()
{
	EnvironmentInstance->TrapListMutex.lock();
	std::vector<std::shared_ptr<WorldEntity>> templist = EnvironmentInstance->GetTrapList();
	EnvironmentInstance->TrapListMutex.unlock();

	if (templist.size() == 0)
		return;

	for (std::shared_ptr<WorldEntity> ent : templist)
	{
		if (ent == nullptr)
			continue;

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
}

void DrawPOI()
{
	EnvironmentInstance->POIListMutex.lock();
	std::vector<std::shared_ptr<WorldEntity>> templist = EnvironmentInstance->GetPOIList();
	EnvironmentInstance->POIListMutex.unlock();

	if (templist.size() == 0)
		return;

	for (std::shared_ptr<WorldEntity> ent : templist)
	{
		if (ent == nullptr)
			continue;

		int distance = (int)Vector3::Distance(ent->GetPosition(), CameraInstance->GetPosition());
		if (distance <= 0)
			continue;
		if (distance > Configs.POI.MaxDistance)
			continue;
		if (!ent->GetValid())
			continue;

		Vector2 pos = CameraInstance->WorldToScreen(ent->GetPosition());
		if (pos.x == 0 || pos.y == 0)
			continue;

		std::wstring wname = Configs.POI.Name ? ent->GetName() : L"";
		std::wstring wdistance = Configs.POI.Distance ? L"[" + std::to_wstring(distance) + L"m]" : L"";
		DrawText(pos.x, pos.y, wname + wdistance, "Verdana", Configs.POI.FontSize, Configs.POI.TextColour, Centre);
	}
}

void DrawOtherEsp()
{
	if (EnvironmentInstance == nullptr)
		return;

	if (EnvironmentInstance->GetObjectCount() == 0)
		return;

	if (Configs.Supply.Enable)
		DrawSupply();

	if (Configs.BloodBonds.Enable)
		DrawBloodBonds();

	if (Configs.Trap.Enable)
		DrawTraps();

	if (Configs.POI.Enable)
		DrawPOI();
}