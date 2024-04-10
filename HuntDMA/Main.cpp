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
	/*uint64_t SSystemGlobalEnvironment = TargetProcess.Read<uint64_t>(TargetProcess.GetBaseAddress("GameHunt.dll") + 0x5EF2FA0);
	printf(LIT("SSystemGlobalEnvironment: 0x%X\n"), SSystemGlobalEnvironment);
	uint64_t  EntitySystem = TargetProcess.Read<uint64_t>(SSystemGlobalEnvironment + 0xA8);
	printf(LIT("EntitySystem: 0x%X\n"), EntitySystem);
	uint64_t pSystem = TargetProcess.Read<uint64_t>(SSystemGlobalEnvironment + 0xC0);
	printf(LIT("pSystem: 0x%X\n"), pSystem);

	uint16_t NumberOfObjects = TargetProcess.Read<uint16_t>(EntitySystem + 0x4006A);
	printf(LIT("NumberOfObjects: %d\n"), NumberOfObjects);

	uint64_t EntityList = EntitySystem + 0x40078;
	printf(LIT("EntityList: 0x%X\n"), EntityList);

	uint64_t localPlayerPtr = TargetProcess.Read<uint64_t>(TargetProcess.GetBaseAddress("GameHunt.dll") + 0x59A2698);
	localPlayerPtr = TargetProcess.Read<uint64_t>(localPlayerPtr + 0x8);
	localPlayerPtr = TargetProcess.Read<uint64_t>( localPlayerPtr + 0x18);
	printf(LIT("localPlayerPtr: 0x%X\n"), localPlayerPtr);
	ViewMatrix m_renderViewMatrix = TargetProcess.Read<ViewMatrix>(pSystem + 0x270 + 0x230);

	printf(LIT("m_renderViewMatrix[0][0]: %f\n"), m_renderViewMatrix.matrix[0][0]);
	printf(LIT("m_renderViewMatrix[0][1]: %f\n"), m_renderViewMatrix.matrix[0][1]);
	printf(LIT("m_renderViewMatrix[0][2]: %f\n"), m_renderViewMatrix.matrix[0][2]);
	printf(LIT("m_renderViewMatrix[0][3]: %f\n"), m_renderViewMatrix.matrix[0][3]);
	printf(LIT("m_renderViewMatrix[1][0]: %f\n"), m_renderViewMatrix.matrix[1][0]);
	printf(LIT("m_renderViewMatrix[1][1]: %f\n"), m_renderViewMatrix.matrix[1][1]);
	printf(LIT("m_renderViewMatrix[1][2]: %f\n"), m_renderViewMatrix.matrix[1][2]);
	printf(LIT("m_renderViewMatrix[1][3]: %f\n"), m_renderViewMatrix.matrix[1][3]);
	printf(LIT("m_renderViewMatrix[2][0]: %f\n"), m_renderViewMatrix.matrix[2][0]);
	printf(LIT("m_renderViewMatrix[2][1]: %f\n"), m_renderViewMatrix.matrix[2][1]);
	printf(LIT("m_renderViewMatrix[2][2]: %f\n"), m_renderViewMatrix.matrix[2][2]);
	printf(LIT("m_renderViewMatrix[2][3]: %f\n"), m_renderViewMatrix.matrix[2][3]);
	printf(LIT("m_renderViewMatrix[3][0]: %f\n"), m_renderViewMatrix.matrix[3][0]);
	printf(LIT("m_renderViewMatrix[3][1]: %f\n"), m_renderViewMatrix.matrix[3][1]);
	printf(LIT("m_renderViewMatrix[3][2]: %f\n"), m_renderViewMatrix.matrix[3][2]);
	printf(LIT("m_renderViewMatrix[3][3]: %f\n"), m_renderViewMatrix.matrix[3][3]);

	Vector3 cameraPos = TargetProcess.Read<Vector3>( pSystem + 0x270 + 0x2F0);
	printf(LIT("cameraPos.x: %f\n"), cameraPos.x);
	printf(LIT("cameraPos.y: %f\n"), cameraPos.y);
	printf(LIT("cameraPos.z: %f\n"), cameraPos.z);
	ViewMatrix m_renderProjectionMatrix = TargetProcess.Read<ViewMatrix>( pSystem + 0x270 + 0x270);
	printf(LIT("m_renderProjectionMatrix[0][0]: %f\n"), m_renderProjectionMatrix.matrix[0][0]);
	printf(LIT("m_renderProjectionMatrix[0][1]: %f\n"), m_renderProjectionMatrix.matrix[0][1]);
	printf(LIT("m_renderProjectionMatrix[0][2]: %f\n"), m_renderProjectionMatrix.matrix[0][2]);
	printf(LIT("m_renderProjectionMatrix[0][3]: %f\n"), m_renderProjectionMatrix.matrix[0][3]);
	printf(LIT("m_renderProjectionMatrix[1][0]: %f\n"), m_renderProjectionMatrix.matrix[1][0]);
	printf(LIT("m_renderProjectionMatrix[1][1]: %f\n"), m_renderProjectionMatrix.matrix[1][1]);
	printf(LIT("m_renderProjectionMatrix[1][2]: %f\n"), m_renderProjectionMatrix.matrix[1][2]);
	printf(LIT("m_renderProjectionMatrix[1][3]: %f\n"), m_renderProjectionMatrix.matrix[1][3]);
	printf(LIT("m_renderProjectionMatrix[2][0]: %f\n"), m_renderProjectionMatrix.matrix[2][0]);
	printf(LIT("m_renderProjectionMatrix[2][1]: %f\n"), m_renderProjectionMatrix.matrix[2][1]);
	printf(LIT("m_renderProjectionMatrix[2][2]: %f\n"), m_renderProjectionMatrix.matrix[2][2]);
	printf(LIT("m_renderProjectionMatrix[2][3]: %f\n"), m_renderProjectionMatrix.matrix[2][3]);
	printf(LIT("m_renderProjectionMatrix[3][0]: %f\n"), m_renderProjectionMatrix.matrix[3][0]);
	printf(LIT("m_renderProjectionMatrix[3][1]: %f\n"), m_renderProjectionMatrix.matrix[3][1]);
	printf(LIT("m_renderProjectionMatrix[3][2]: %f\n"), m_renderProjectionMatrix.matrix[3][2]);
	printf(LIT("m_renderProjectionMatrix[3][3]: %f\n"), m_renderProjectionMatrix.matrix[3][3]);
	
	for (unsigned int i = 0; i < NumberOfObjects; ++i) 
	{
		uint64_t entity = TargetProcess.Read<uintptr_t>( EntityList + i * sizeof(uint64_t));
		if (entity == NULL) {
			continue;
		}

		// Get entity Position
		Vector3 pos = TargetProcess.Read<Vector3>(entity + 0x128);
		if(pos == Vector3::Zero())
			continue;
		printf(LIT("Entity %d: %f %f %f\n"), i, pos.x, pos.y, pos.z);

		uintptr_t entityNamePtr = TargetProcess.Read<uintptr_t>( entity + 0x10);
		entityNameStruct entityName = TargetProcess.Read<entityNameStruct>( entityNamePtr);
		entityName.name[99] = '\0';
		printf(LIT("Entity Name: %s\n"), entityName.name);

		uintptr_t entityClassPtr = TargetProcess.Read<uintptr_t>( entity + 0x18);
		entityClassPtr = TargetProcess.Read<uintptr_t>( entityClassPtr + 0x10);
		entityNameStruct entityClassName = TargetProcess.Read<entityNameStruct>( entityClassPtr);
		entityClassName.name[99] = '\0';
		//printf(LIT("Entity Class Name: %s\n"), entityClassName.name);

		//if (strstr(entityName.name, "HunterBasic") != NULL)
	//	{


	//		uintptr_t slotsPtr = TargetProcess.Read<uintptr_t>( entity + 0xA8);
	//		printf(LIT("slotsPtr: 0x%X\n"), slotsPtr);
	//		if (slotsPtr == 0)
	//			continue;
	//		uintptr_t slotPtr = TargetProcess.Read<uintptr_t>(slotsPtr + 0);
//			printf(LIT("slotPtr: 0x%X\n"), slotPtr);
//			if (slotPtr == 0)
	//			continue;
	//		uintptr_t renderNodePtr = TargetProcess.Read<uintptr_t>(slotPtr + 0xA8);
	//		printf(LIT("renderNodePtr: 0x%X\n"), renderNodePtr);
	//		if (renderNodePtr == 0)
	//			continue;
	//		renderNode rNode = TargetProcess.Read<renderNode>(renderNodePtr);

		//	uint32_t RGBAColor = colorType::Red;
		//	TargetProcess.Write<uint32_t>(renderNodePtr + 0x3c, RGBAColor);
			//}
	}*/

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
