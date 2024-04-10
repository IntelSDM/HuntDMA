#include "Pch.h"
#include "PlayerNetwork.h"
#include "globals.h"

PlayerNetwork::PlayerNetwork(uint64_t address)
{
	Class = address;
}
void PlayerNetwork::InitializePlayerList()
{
	StaticClass = TargetProcess.Read<uint64_t>(TargetProcess.GetBaseAddress("GameAssembly.dll") + StaticClass);
	printf("StaticClass: 0x%llX\n", StaticClass);
	StaticField = TargetProcess.Read<uint64_t>(StaticClass + StaticField);
	FastList = TargetProcess.Read<uint64_t>(StaticField + FastList);
	printf("FastList: 0x%llX\n", FastList);
}
void PlayerNetwork::CachePlayers()
{
	
	auto handle = TargetProcess.CreateScatterHandle();
	uint32_t fastlistsize = 0;
	uint64_t fastlistbuffer = 0;
	TargetProcess.AddScatterReadRequest(handle, FastList + 0x10, reinterpret_cast<void*>(&fastlistbuffer), sizeof(uint64_t));
	TargetProcess.AddScatterReadRequest(handle, FastList + 0x18, reinterpret_cast<void*>(&fastlistsize), sizeof(uint32_t));
	TargetProcess.ExecuteReadScatter(handle);
	TargetProcess.CloseScatterHandle(handle);

	std::vector<uint64_t> players;
	players.resize(fastlistsize);
	handle = TargetProcess.CreateScatterHandle();
	for (int i = 0; i < fastlistsize; i++)
	{
		TargetProcess.AddScatterReadRequest(handle,fastlistbuffer + (0x20 + (i * 0x8)), reinterpret_cast<void*>(&players[i]),sizeof(uint64_t));

	}
	TargetProcess.ExecuteReadScatter(handle);
	TargetProcess.CloseScatterHandle(handle);
	PlayerList.resize(fastlistsize);
	for (int i = 0; i < fastlistsize; i++)
	{
		PlayerList[i] = std::make_shared<PlayerNetwork>(players[i]);
	}
	handle = TargetProcess.CreateScatterHandle();
	for (auto player : PlayerList)
	{
		TargetProcess.AddScatterReadRequest(handle, player->Class + player->NetworkState, reinterpret_cast<void*>(&player->NetworkState), sizeof(uint64_t));
		TargetProcess.AddScatterReadRequest(handle, player->Class + player->LocalPlayer, reinterpret_cast<void*>(&player->IsLocalPlayer), sizeof(bool));
	}
	TargetProcess.ExecuteReadScatter(handle);
	TargetProcess.CloseScatterHandle(handle);
	handle = TargetProcess.CreateScatterHandle();
	for (auto player : PlayerList)
	{
		if(player->IsLocalPlayer)
			CurrentLocalPlayer = player;

		player->PlayerState = std::make_shared<PlayerNetworkState>(player->NetworkState,handle);
	}
	TargetProcess.ExecuteReadScatter(handle);
	TargetProcess.CloseScatterHandle(handle);
	handle = TargetProcess.CreateScatterHandle();
	for (auto player : PlayerList)
	{
		player->PlayerState->UpdateWeapons(handle);
	
	}
	TargetProcess.ExecuteReadScatter(handle);
	TargetProcess.CloseScatterHandle(handle);

	handle = TargetProcess.CreateScatterHandle();
	for (auto player : PlayerList)
	{
		player->PlayerState->UpdateWeapons1(handle);
	}
	TargetProcess.ExecuteReadScatter(handle);
	TargetProcess.CloseScatterHandle(handle);


}

std::shared_ptr<PlayerNetworkState> PlayerNetwork::GetPlayerNettworkState()
{
	return PlayerState;
}