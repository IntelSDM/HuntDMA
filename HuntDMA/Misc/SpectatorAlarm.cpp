#include "Pch.h"
#include "globals.h"
#include "Drawing.h"
#include "ConfigInstance.h"
#include "ConfigUtilities.h"
#include "Init.h"

void DrawSpectators()
{
	EnvironmentInstance->SpectatorCountMutex.lock();
	int spectatorCount = EnvironmentInstance->GetSpectatorCount();
	EnvironmentInstance->SpectatorCountMutex.unlock();
	if (spectatorCount == 0) return;

	int pViewport[4] = { 0, 0, Configs.Overlay.OverrideResolution ? Configs.Overlay.Width : GetSystemMetrics(SM_CXSCREEN), Configs.Overlay.OverrideResolution ? Configs.Overlay.Height : GetSystemMetrics(SM_CYSCREEN) };
	int lineWidth = 50;
	OutlineRectangle(pViewport[0], pViewport[1], pViewport[2], pViewport[3], lineWidth, MyColour(255, 0, 0, 255));
	DrawText(0, 55, std::to_wstring(spectatorCount), "Verdana", 40, Colour(0, 255, 0), None);
}