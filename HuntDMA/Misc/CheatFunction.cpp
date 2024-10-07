#include "Pch.h"
#include "CheatFunction.h"
#include <chrono>

CheatFunction::CheatFunction(int msTimer, std::function<void()> func)
{
	MsSleep = msTimer;
	Function = func;
}

void CheatFunction::Execute()
{
	auto now = std::chrono::high_resolution_clock::now();
	if (std::chrono::duration_cast<std::chrono::milliseconds>(now - LastExecution).count() > MsSleep)
	{
		Function();
		LastExecution = now;
	}
}