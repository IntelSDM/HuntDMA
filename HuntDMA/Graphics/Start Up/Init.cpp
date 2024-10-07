#include "pch.h"
#include "Init.h"
#include "drawing.h"
#include "GUI.h"
#include "Globals.h"
#include "PlayerEsp.h"
#include "OtherEsp.h"
#include "ConfigUtilities.h"
#include "Aimbot.h"
#include "InputManager.h"
#include "Kmbox.h"
#include "FPS.h"
#include <chrono>
#include "SpectatorAlarm.h"

ID2D1Factory* Factory;
IDWriteFactory* FontFactory;
ID2D1HwndRenderTarget* RenderTarget;
ID2D1SolidColorBrush* Brush;


std::shared_ptr<Environment> EnvironmentInstance;
std::shared_ptr<Camera> CameraInstance;


void CleanD2D()
{
	// ensure pointer is valid
	// then release memory
	if (Factory)
		Factory->Release();
	if (RenderTarget)
		RenderTarget->Release();
	if (Brush)
		Brush->Release();
	for (std::pair dict : Fonts)
	{
		const FontInformation& fontInfo = dict.second;
		if (fontInfo.font)
			fontInfo.font->Release();
	}
	//Clean all cached text layours
	for (std::pair dict : TextLayouts)
	{
		IDWriteTextLayout* layout = dict.second;
		if (layout)
			layout->Release();
	}

	for (std::pair dict : TextCache)
	{
		IDWriteTextLayout* layout = dict.second;
		if (layout)
			layout->Release();
	}
}

int FrameRate()
{
	static int fps = 0;
	static int lastfps = 0;
	static auto lasttime = std::chrono::steady_clock::now();

	auto currenttime = std::chrono::steady_clock::now();
	std::chrono::duration<float> elapsed = currenttime - lasttime;
	fps++;

	if (elapsed.count() >= 1.0f)
	{
		lasttime = currenttime;
		lastfps = fps;
		fps = 0;
	}

	return lastfps;
}

void InitialiseClasses()
{
	EnvironmentInstance = std::make_shared<Environment>();
	CameraInstance = std::make_shared<Camera>();

}

std::shared_ptr<CheatFunction> Cache = std::make_shared<CheatFunction>(8000, [] {
	if (EnvironmentInstance == nullptr)
		return;
	if (EnvironmentInstance->GetObjectCount() == 0)
		return;
	EnvironmentInstance->GetEntityList();
	EnvironmentInstance->CacheEntities();
});

std::shared_ptr<CheatFunction> UpdateCam = std::make_shared<CheatFunction>(5, [] {
	if (EnvironmentInstance == nullptr)
		return;
	if (EnvironmentInstance->GetObjectCount() == 0)
		return;
	auto handle = TargetProcess.CreateScatterHandle();
	CameraInstance->UpdateCamera(handle);
	TargetProcess.ExecuteReadScatter(handle);
	TargetProcess.CloseScatterHandle(handle);
});

void DrawCrosshair()
{
	Vector2 centre = Vector2(Configs.Overlay.OverrideResolution ? Configs.Overlay.Width / 2 : GetSystemMetrics(SM_CXSCREEN) / 2, Configs.Overlay.OverrideResolution ? Configs.Overlay.Height * 0.6f : GetSystemMetrics(SM_CYSCREEN) * 0.6f);
	// drawing aimbot fov here because fuck it
	if (Configs.Aimbot.DrawFOV)
	{
		OutlineCircle(centre.x, centre.y, Configs.Aimbot.FOV, 2,Configs.Aimbot.FOVColour);
	}
	if (Configs.Overlay.CrosshairType == 0)
		return;
	
	if (Configs.Overlay.CrosshairType == 1)
	{
		FilledCircle(centre.x, centre.y, Configs.Overlay.CrosshairSize, Configs.Overlay.CrosshairColour);

	}
	if (Configs.Overlay.CrosshairType == 2)
	{
		OutlineCircle(centre.x, centre.y, Configs.Overlay.CrosshairSize,2, Configs.Overlay.CrosshairColour);

	}
	if (Configs.Overlay.CrosshairType == 3)
	{
		FilledRectangle(centre.x - (Configs.Overlay.CrosshairSize / 2), centre.y - (Configs.Overlay.CrosshairSize / 2), Configs.Overlay.CrosshairSize, Configs.Overlay.CrosshairSize, Configs.Overlay.CrosshairColour);

	}
	if (Configs.Overlay.CrosshairType == 4)
	{
		OutlineRectangle(centre.x - (Configs.Overlay.CrosshairSize/2), centre.y - (Configs.Overlay.CrosshairSize/2),  Configs.Overlay.CrosshairSize , Configs.Overlay.CrosshairSize, 1, Configs.Overlay.CrosshairColour);

	}
}

void CacheThread()
{
	while (true)
	{
		if (EnvironmentInstance == nullptr)
			continue;
		if (EnvironmentInstance->GetObjectCount() == 0)
			continue;
		Cache->Execute();
	}
}

void InitD2D(HWND hWnd)
{
	HRESULT result = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &Factory);
	RECT rect;
	GetClientRect(hWnd, &rect);
	result = Factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED)), D2D1::HwndRenderTargetProperties(hWnd, D2D1::SizeU(rect.right, rect.bottom), D2D1_PRESENT_OPTIONS_IMMEDIATELY), &RenderTarget);
	if (!SUCCEEDED(result))
		return;

	result = DWriteCreateFactory(DWRITE_FACTORY_TYPE_ISOLATED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&FontFactory));
	if (!SUCCEEDED(result))
		return;

	CreateFonts(LIT("Verdana"), LIT(L"Verdana"), 10, DWRITE_FONT_WEIGHT_NORMAL);
	CreateFonts("VerdanaBold", LIT(L"Verdana"), 10, DWRITE_FONT_WEIGHT_SEMI_BOLD);
	RenderTarget->CreateSolidColorBrush(D2D1::ColorF(0, 0, 0, 0), &Brush); // create global brush
	RenderTarget->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE); // set aa mode
	std::thread(CacheThread).detach();
	Keyboard::InitKeyboard();
	kmbox::KmboxInitialize("");
}

void RenderFrame()
{
	if (EnvironmentInstance == nullptr)
	{
		InitialiseClasses();
		Sleep(1000);
	}
	if (EnvironmentInstance->GetObjectCount() == 0)
	{
		InitialiseClasses();
		EnvironmentInstance->GetEntityList();
		auto handle = TargetProcess.CreateScatterHandle();
		CameraInstance->UpdateCamera(handle);
		TargetProcess.ExecuteReadScatter(handle);
		TargetProcess.CloseScatterHandle(handle);
		EnvironmentInstance->CacheEntities();
		Sleep(1000);
	}

	UpdateCam->Execute();
	UpdatePlayers->Execute();
	UpdateBosses->Execute();
	//Aimbot();
	RenderTarget->BeginDraw();
	RenderTarget->Clear(Colour(0, 0, 0, 255)); // clear over the last buffer
	RenderTarget->SetTransform(D2D1::Matrix3x2F::Identity()); // set new transform
	DrawSpectators();
	DrawPlayers();
	DrawBosses();
	DrawOtherEsp();
	//DrawCrosshair();
	DrawFPS();
	Render();
	RenderTarget->EndDraw();
}
