#include "pch.h"
#include "Memory.h"
#include "CheatFunction.h"
#include "Environment.h"
#include "Globals.h"
#include "Camera.h"
#include "Init.h"
#include "GUI.h"



void main()
{
	while (!TargetProcess.Init("HuntGame.exe", true, true))
	{
		std::printf("Cant Attach To Game\n");
		Sleep(2000);
		continue;
	}
	TargetProcess.FixCr3();
	std::printf("HuntGame: 0x%X\n", TargetProcess.GetBaseAddress("HuntGame.exe"));
	std::printf("GameHunt: 0x%X\n", TargetProcess.GetBaseAddress("GameHunt.dll"));
}
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	InputWndProc(hWnd, message, wParam, lParam);
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}
struct renderNode {
	char pad_01[0x28]; // 0x00(0x28)
	unsigned __int64 rnd_flags; // 0x28(0x08)
	char pad_02[0xc]; // 0x30(0xc)
	unsigned int silhouettes_param; // 0x3c(0x04)
};

struct entityNameStruct {
	char name[100];
};

const char* hunter = "HunterBasic"; // Hunter
const char* targetSensor = "TargetSensor"; // Boss Lair Grunts.Assassin.Target_Assassin_scripted008_TargetSensor
const char* eventItem = "tide_of_shadows"; // Event liveevents/tide_of_shadows_2023/ship_altar_scripted330
const char* grunts = "Grunts"; // Zombie

enum colorType : uint32_t {
	Red = 0xFF0000FF,
	Green = 0x00FF00FF,
	Blue = 0x0000FFFF,
	Cyan = 0x00FFFFFF,
	Orange = 0xFFA500FF,
	Yellow = 0xFFFF00FF,
	White = 0xFFFFFFFF
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd;
	WNDCLASSEX wc;
	AllocConsole();
	FILE* fDummy;
	freopen_s(&fDummy, LIT("CONIN$"), LIT("r"), stdin);
	freopen_s(&fDummy, LIT("CONOUT$"), LIT("w"), stderr);
	freopen_s(&fDummy, LIT("CONOUT$"), LIT("w"), stdout);
	printf(LIT("Debugging Window:\n"));

	main();
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = L"GUI Framework";
	RegisterClassEx(&wc);

	hWnd = CreateWindowEx(WS_EX_APPWINDOW, wc.lpszClassName, L"GUI Framework",
		WS_POPUP,
		0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), NULL, NULL, hInstance, NULL);

	if (!hWnd)
		return -1;


	SetLayeredWindowAttributes(hWnd, RGB(0, 0, 0), 255, LWA_ALPHA);

	ShowWindow(hWnd, nCmdShow);

	InitD2D(hWnd);
	CreateGUI();
	MSG msg;
	SetProcessDPIAware();
	SetInput();
	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
				break;
		}
		RenderFrame();
	}
	CleanD2D();
	return msg.wParam;
}
