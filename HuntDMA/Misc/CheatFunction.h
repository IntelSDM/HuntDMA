#include <chrono>
#pragma once
class CheatFunction
{
	int MsSleep = 0;
	std::function<void()> Function;
	std::chrono::time_point<std::chrono::high_resolution_clock> LastExecution;
public:
	CheatFunction(int time, std::function<void()>func);
	void Execute();
};