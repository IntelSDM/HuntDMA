#include "pch.h"
#include "InputManager.h"
#include "Registry.h"
#include "Memory/Memory.h"

namespace Keyboard
{
	uint64_t gafAsyncKeyStateExport = 0;
	uint8_t state_bitmap[64]{ };
	uint8_t previous_state_bitmap[256 / 8]{ };
	uint64_t win32kbase = 0;
	int win_logon_pid = 0;
}

bool Keyboard::InitKeyboard()
{
	c_registry registry;
	std::string win = registry.QueryValue("HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\CurrentBuild", e_registry_type::sz);
	if (win == "")
		return false;
	int Winver = 0;
	if (!win.empty())
		Winver = std::stoi(win);
	else
		return false;

	win_logon_pid = TargetProcess.GetPidFromName("winlogon.exe");
	if (Winver > 22000)
	{
		auto pids = TargetProcess.GetPidListFromName("csrss.exe");
		for (size_t i = 0; i < pids.size(); i++)
		{
			auto pid = pids[i];
			uintptr_t tmp = VMMDLL_ProcessGetModuleBaseU(TargetProcess.vHandle, pid, (LPSTR)"win32ksgd.sys");
			uintptr_t g_session_global_slots = tmp + 0x3110;
			uintptr_t user_session_state = TargetProcess.Read<uintptr_t>(TargetProcess.Read<uintptr_t>(TargetProcess.Read<uintptr_t>(g_session_global_slots, pid), pid), pid);
			gafAsyncKeyStateExport = user_session_state + 0x3690;
			if (gafAsyncKeyStateExport > 0x7FFFFFFFFFFF)
				break;
		}
		if (gafAsyncKeyStateExport > 0x7FFFFFFFFFFF)
			return true;
		return false;
	}
	else
	{
		printf("win10\n");
		PVMMDLL_MAP_EAT eat_map = NULL;
		PVMMDLL_MAP_EATENTRY eat_map_entry;
		bool result = VMMDLL_Map_GetEATU(TargetProcess.vHandle, TargetProcess.GetPidFromName("winlogon.exe") | VMMDLL_PID_PROCESS_WITH_KERNELMEMORY, (LPSTR)"win32kbase.sys", &eat_map);
		if (!result)
			return false;

		if (eat_map->dwVersion != VMMDLL_MAP_EAT_VERSION)
		{
			VMMDLL_MemFree(eat_map);
			eat_map_entry = NULL;
			return false;
		}
		printf("eat_map->cMap: %d\n", eat_map->cMap);
		for (int i = 0; i < eat_map->cMap; i++)
		{
			eat_map_entry = eat_map->pMap + i;
			if (strcmp(eat_map_entry->uszFunction, "gafAsyncKeyState") == 0)
			{
				gafAsyncKeyStateExport = eat_map_entry->vaFunction;

				break;
			}
		}

		VMMDLL_MemFree(eat_map);
		eat_map = NULL;
		if (gafAsyncKeyStateExport > 0x7FFFFFFFFFFF)
			return true;
		return false;
	}
}

void Keyboard::UpdateKeys()
{
	uint8_t previous_key_state_bitmap[64] = { 0 };
	memcpy(previous_key_state_bitmap, state_bitmap, 64);

	VMMDLL_MemReadEx(TargetProcess.vHandle, win_logon_pid | VMMDLL_PID_PROCESS_WITH_KERNELMEMORY, gafAsyncKeyStateExport, (PBYTE)&state_bitmap, 64, NULL, VMMDLL_FLAG_NOCACHE);
	for (int vk = 0; vk < 256; ++vk)
		if ((state_bitmap[(vk * 2 / 8)] & 1 << vk % 4 * 2) && !(previous_key_state_bitmap[(vk * 2 / 8)] & 1 << vk % 4 * 2))
			previous_state_bitmap[vk / 8] |= 1 << vk % 8;
}

auto start = std::chrono::system_clock::now();

bool Keyboard::IsKeyDown(uint32_t virtual_key_code)
{
	if (gafAsyncKeyStateExport < 0x7FFFFFFFFFFF)
		return false;
	if (std::chrono::system_clock::now() - start > std::chrono::milliseconds(1))
	{
		UpdateKeys();
		start = std::chrono::system_clock::now();
	}
	return state_bitmap[(virtual_key_code * 2 / 8)] & 1 << virtual_key_code % 4 * 2;
}
