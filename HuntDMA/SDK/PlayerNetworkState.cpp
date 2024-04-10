#include "Pch.h"
#include "PlayerNetworkState.h"
#include "CheatFunction.h"

PlayerNetworkState::PlayerNetworkState(uint64_t address, VMMDLL_SCATTER_HANDLE handle)
{
	Class = address;
	UpdateConnected(handle);
	UpdateHealth(handle);
	UpdateFriendly(handle);
	UpdatePosition(handle);
	UpdatePrimaryWeapon(handle);
	UpdateSecondaryWeapon(handle);
//	UpdateHitboxes(handle);
}


void PlayerNetworkState::UpdateWeapons(VMMDLL_SCATTER_HANDLE handle)
{
	UpdatePrimaryCacheName(handle);
	UpdateSecondaryCacheName(handle);
}
void PlayerNetworkState::UpdateWeapons1(VMMDLL_SCATTER_HANDLE handle)
{
	UpdatePrimaryWeaponName(handle);
	UpdateSecondaryWeaponName(handle);
}

void PlayerNetworkState::UpdatePrimaryCacheName(VMMDLL_SCATTER_HANDLE handle)
{
	TargetProcess.AddScatterReadRequest(handle, PrimaryWeapon + CacheName, reinterpret_cast<void*>(&PrimaryCacheName), sizeof(PrimaryCacheName));
}
void PlayerNetworkState::UpdateHitboxes(VMMDLL_SCATTER_HANDLE handle)
{
	TargetProcess.AddScatterReadRequest(handle, Class + Hitboxes, reinterpret_cast<void*>(&Hitboxes), sizeof(Hitboxes));
}
void PlayerNetworkState::UpdateSecondaryCacheName(VMMDLL_SCATTER_HANDLE handle)
{
	TargetProcess.AddScatterReadRequest(handle, SecondaryWeapon + CacheName, reinterpret_cast<void*>(&SecondaryCacheName), sizeof(SecondaryCacheName));
}
void PlayerNetworkState::UpdateConnected(VMMDLL_SCATTER_HANDLE handle)
{
	TargetProcess.AddScatterReadRequest(handle,Class + Connected,reinterpret_cast<void*>(&IsConnected),sizeof(bool));
}

void PlayerNetworkState::UpdateHealth(VMMDLL_SCATTER_HANDLE handle)
{
	TargetProcess.AddScatterReadRequest(handle, Class + Health, reinterpret_cast<void*>(&CurrentHealth), sizeof(float));
}

void PlayerNetworkState::UpdateFriendly(VMMDLL_SCATTER_HANDLE handle)
{
	TargetProcess.AddScatterReadRequest(handle, Class + Friendly, reinterpret_cast<void*>(&IsFriendly), sizeof(bool));
}

void PlayerNetworkState::UpdatePosition(VMMDLL_SCATTER_HANDLE handle)
{
	TargetProcess.AddScatterReadRequest(handle, Class + PositionOffset, reinterpret_cast<void*>(&Position), sizeof(Vector3));
}

void PlayerNetworkState::UpdatePrimaryWeapon(VMMDLL_SCATTER_HANDLE handle)
{
	TargetProcess.AddScatterReadRequest(handle, Class + PrimaryWeapon, reinterpret_cast<void*>(&PrimaryWeapon), sizeof(uint64_t));
}

void PlayerNetworkState::UpdateSecondaryWeapon(VMMDLL_SCATTER_HANDLE handle)
{
	try // for some reason the &secondaryweapon causes an issue but i can do it to primaryweapon and this exact set of code works everywhere else
	{
		TargetProcess.AddScatterReadRequest(handle, Class + SecondaryWeapon, reinterpret_cast<void*>(&SecondaryWeapon), sizeof(uint64_t));
	}
	catch(std::exception ex){}
}

void PlayerNetworkState::UpdatePrimaryWeaponName(VMMDLL_SCATTER_HANDLE handle)
{
	TargetProcess.AddScatterReadRequest(handle, PrimaryCacheName + 0x14, reinterpret_cast<void*>(&PrimaryWeaponName), sizeof(PrimaryWeaponName));
}

void PlayerNetworkState::UpdateSecondaryWeaponName(VMMDLL_SCATTER_HANDLE handle)
{
	TargetProcess.AddScatterReadRequest(handle, SecondaryCacheName + 0x14, reinterpret_cast<void*>(&SecondaryWeaponName), sizeof(SecondaryWeaponName));
}

float PlayerNetworkState::GetHealth()
{
	return CurrentHealth;
}

bool PlayerNetworkState::GetConnected()
{
	return IsConnected;
}

bool PlayerNetworkState::GetFriendly()
{
	return IsFriendly;
}

Vector3 PlayerNetworkState::GetPosition()
{
	return Position;
}

std::wstring PlayerNetworkState::GetPrimaryWeaponName()
{
	return std::wstring(PrimaryWeaponName);
}

std::wstring PlayerNetworkState::GetSecondaryWeaponName()
{
	return std::wstring(SecondaryWeaponName);
}