#include "Pch.h"
#include "globals.h"
#include "Drawing.h"
#include "ConfigInstance.h"
#include "ConfigUtilities.h"
#include "Init.h"

void DrawFPS()
{
	if (Configs.Overlay.ShowFPS)
	{
		DrawText(25, 25, L"DMA FPS: " + std::to_wstring(FrameRate()), "Verdana", Configs.Overlay.FpsFontSize, Colour(128, 128, 128), None);
	}
}