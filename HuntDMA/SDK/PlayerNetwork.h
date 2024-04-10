#pragma once
#include "Memory.h"
#include "PlayerNetworkState.h"
/*
      "Address": 70155144,
      "Name": "PlayerNetwork_TypeInfo",
      "Signature": "PlayerNetwork_c*"
*/
class PlayerNetwork
{
private:
    uint64_t Class = 0;
    uint64_t StaticClass = 0x42E7B88;
    uint64_t StaticField = 0xB8;
    uint64_t FastList = 0x48; // public static FastListA
    uint64_t NetworkState = 0x30; // public PlayerNetworkState
    std::shared_ptr<PlayerNetworkState> PlayerState;
    uint64_t LocalPlayer = 0x1C; // public bool

    bool IsLocalPlayer = false;
public:
    PlayerNetwork(uint64_t address);
    void InitializePlayerList();
    void CachePlayers();
    std::shared_ptr<PlayerNetworkState> GetPlayerNettworkState();
    std::vector<std::shared_ptr<PlayerNetwork>> PlayerList;
};