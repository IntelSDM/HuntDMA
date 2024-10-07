#pragma once
extern std::shared_ptr<CheatFunction> UpdatePlayers;
extern std::shared_ptr<CheatFunction> UpdateBosses;
extern void DrawPlayers();
extern void DrawBosses();

bool IsValidHP(int hp);