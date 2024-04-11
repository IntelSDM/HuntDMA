#include "pch.h"
#include "GUI.h"
#include "entity.h"
#include "Form.h"
#include "Button.h"
#include "ColourPicker.h"
#include "Label.h"
#include "tab.h"
#include "TabController.h"
#include "Toggle.h"
#include "Slider.h"
#include "DropDown.h"
#include "ComboBox.h"
#include "KeyBind.h"
#include "TabListBox.h"
#include "TabListBoxController.h"
#include "TextBox.h"
#include "ConfigInstance.h"
#include "ConfigUtilities.h"
#include "Kmbox.h"
int SelectedTab = 0;
int SelectedSubTab = 0;
int TabCount = 0;
int KeyBindClipBoard = 0;
EntityVector MenuEntity;
bool MenuOpen = true;
D2D1::ColorF ColourPickerClipBoard = Colour(255,255,255);
D2D1::ColorF ColourPick = Colour(0, 150, 255, 255);
std::wstring ScreenWidth = std::to_wstring(Configs.Overlay.Width);
std::wstring ScreenHeight = std::to_wstring(Configs.Overlay.Height);
void CreateGUI()
{
	ScreenWidth = std::to_wstring(Configs.Overlay.Width);
	ScreenHeight = std::to_wstring(Configs.Overlay.Height);
	MenuEntity = std::make_shared<Container>();
	auto form = std::make_shared<Form>(200, 300.0f, 450, 250, 2, 30, LIT(L"Hunt DMA"), false);
	{
		auto tabcontroller = std::make_shared<TabController>();
		form->Push(tabcontroller);

		auto playeresptab = std::make_shared<Tab>(LIT(L"Player ESP"), 5, 5, &SelectedTab, 0, 20);
		{
			auto enable = std::make_shared<Toggle>( 100, 5, LIT(L"Enable"), &Configs.Player.Enable);
			playeresptab->Push(enable);
			auto textcolour = std::make_shared<ColourPicker>(160, 6, &Configs.Player.TextColour);
			playeresptab->Push(textcolour);
			auto name = std::make_shared<Toggle>(100, 25, LIT(L"Name"), &Configs.Player.Name);
			playeresptab->Push(name);
			auto distance = std::make_shared<Toggle>(100, 45, LIT(L"Distance"), &Configs.Player.Distance);
			playeresptab->Push(distance);
			auto chams = std::make_shared<Toggle>(100, 65, LIT(L"Chams"), &Configs.Player.Chams);
			playeresptab->Push(chams);
			auto drawfriendly = std::make_shared<Toggle>(100, 85, LIT(L"Draw Friendly"), &Configs.Player.DrawFriends);
			playeresptab->Push(drawfriendly);
			auto friendcolour = std::make_shared<ColourPicker>(200, 86, &Configs.Player.FriendColour);
			playeresptab->Push(friendcolour);
			auto chammode = std::make_shared<DropDown>(100, 115, LIT(L"Cham Mode"), &Configs.Player.ChamMode,
				std::vector<std::wstring>{LIT(L"Outline Red"), LIT(L"Outline Blue"), LIT(L"Outline Yellow"), LIT(L"Outline Orange"), LIT(L"Outline Cyan"), LIT(L"Outline White"),
				LIT(L"Filled Red"), LIT(L"Filled Blue"), LIT(L"Filled Yellow"), LIT(L"Filled Orange"), LIT(L"Filled Cyan"), LIT(L"Filled White")});
			playeresptab->Push(chammode);
			auto maxdistance = std::make_shared<Slider<int>>(100, 140, 150, LIT(L"Max Distance"), LIT(L"m"), 0, 1000, &Configs.Player.MaxDistance);
			playeresptab->Push(maxdistance);
			auto textsize = std::make_shared<Slider<int>>(100, 165,150, LIT(L"Text Size"), LIT(L"px"), 4, 16, &Configs.Player.FontSize);
			playeresptab->Push(textsize);
		


		}
		tabcontroller->Push(playeresptab);
		auto zombieesp = std::make_shared<Tab>(LIT(L"Zombie ESP"), 5, 30, &SelectedTab, 0, 20);
		{
			auto enable = std::make_shared<Toggle>(100, 5, LIT(L"Enable"), &Configs.Zombie.Enable);
			zombieesp->Push(enable);
			auto textcolour = std::make_shared<ColourPicker>(160, 6, &Configs.Zombie.TextColour);
			zombieesp->Push(textcolour);
			auto name = std::make_shared<Toggle>(100, 25, LIT(L"Name"), &Configs.Zombie.Name);
			zombieesp->Push(name);
			auto distance = std::make_shared<Toggle>(100, 45, LIT(L"Distance"), &Configs.Zombie.Distance);
			zombieesp->Push(distance);
			auto chams = std::make_shared<Toggle>(100, 65, LIT(L"Chams"), &Configs.Zombie.Chams);
			zombieesp->Push(chams);
			auto chammode = std::make_shared<DropDown>(100, 95, LIT(L"Cham Mode"), &Configs.Zombie.ChamMode,
				std::vector<std::wstring>{LIT(L"Outline Red"), LIT(L"Outline Blue"), LIT(L"Outline Yellow"), LIT(L"Outline Orange"), LIT(L"Outline Cyan"), LIT(L"Outline White"),
				LIT(L"Filled Red"), LIT(L"Filled Blue"), LIT(L"Filled Yellow"), LIT(L"Filled Orange"), LIT(L"Filled Cyan"), LIT(L"Filled White")});
			zombieesp->Push(chammode);
			auto maxdistance = std::make_shared<Slider<int>>(100, 120, 150, LIT(L"Max Distance"), LIT(L"m"), 0, 1000, &Configs.Zombie.MaxDistance);
			zombieesp->Push(maxdistance);
			auto textsize = std::make_shared<Slider<int>>(100, 145, 150, LIT(L"Text Size"), LIT(L"px"), 4, 16, &Configs.Zombie.FontSize);
			zombieesp->Push(textsize);
		}
		tabcontroller->Push(zombieesp);
		auto supplyesptab = std::make_shared<Tab>(LIT(L"Supply ESP"), 5, 55, &SelectedTab, 0, 20);
		{
			auto enable = std::make_shared<Toggle>(100, 5, LIT(L"Enable"), &Configs.Supply.Enable);
			supplyesptab->Push(enable);
			auto textcolour = std::make_shared<ColourPicker>(160, 6, &Configs.Supply.TextColour);
			supplyesptab->Push(textcolour);
			auto name = std::make_shared<Toggle>(100, 25, LIT(L"Name"), &Configs.Supply.Name);
			supplyesptab->Push(name);
			auto distance = std::make_shared<Toggle>(100, 45, LIT(L"Distance"), &Configs.Supply.Distance);
			supplyesptab->Push(distance);
			auto maxdistance = std::make_shared<Slider<int>>(100, 65, 150, LIT(L"Max Distance"), LIT(L"m"), 0, 1000, &Configs.Supply.MaxDistance);
			supplyesptab->Push(maxdistance);
			auto textsize = std::make_shared<Slider<int>>(100, 90, 150, LIT(L"Text Size"), LIT(L"px"), 4, 16, &Configs.Supply.FontSize);
			supplyesptab->Push(textsize);
		}
		tabcontroller->Push(supplyesptab);
		auto trapesptab = std::make_shared<Tab>(LIT(L"Trap ESP"), 5, 80, &SelectedTab, 0, 20);
		{
			auto enable = std::make_shared<Toggle>(100, 5, LIT(L"Enable"), &Configs.Trap.Enable);
			trapesptab->Push(enable);
			auto textcolour = std::make_shared<ColourPicker>(160, 6, &Configs.Trap.TextColour);
			trapesptab->Push(textcolour);
			auto name = std::make_shared<Toggle>(100, 25, LIT(L"Name"), &Configs.Trap.Name);
			trapesptab->Push(name);
			auto distance = std::make_shared<Toggle>(100, 45, LIT(L"Distance"), &Configs.Trap.Distance);
			trapesptab->Push(distance);
			auto maxdistance = std::make_shared<Slider<int>>(100, 65, 150, LIT(L"Max Distance"), LIT(L"m"), 0, 1000, &Configs.Trap.MaxDistance);
			trapesptab->Push(maxdistance);
			auto textsize = std::make_shared<Slider<int>>(100, 90, 150, LIT(L"Text Size"), LIT(L"px"), 4, 16, &Configs.Trap.FontSize);
			trapesptab->Push(textsize);
		}
		tabcontroller->Push(trapesptab);
		auto overlaytab = std::make_shared<Tab>(LIT(L"Overlay"), 5, 105, &SelectedTab, 0, 20);
		{
			auto overrideresolution = std::make_shared<Toggle>(100, 5, LIT(L"Override W2S Resolution"), &Configs.Overlay.OverrideResolution);
			overlaytab->Push(overrideresolution);
			auto screenwidth = std::make_shared<TextBox>(100, 35, LIT(L"Screen Width"), &ScreenWidth);
			screenwidth->SetValueChangedEvent([]()
				{
					try
					{
						Configs.Overlay.Width = std::stoi(ScreenWidth);
					}
					catch (std::exception ex)
					{
					}
				});
			overlaytab->Push(screenwidth);
			auto screenheight = std::make_shared<TextBox>(100, 70, LIT(L"Screen Height"), &ScreenHeight);
			screenheight->SetValueChangedEvent([]()
				{
					try
					{
						Configs.Overlay.Height = std::stoi(ScreenHeight);
					}
					catch (std::exception ex)
					{
					}
				});
			overlaytab->Push(screenheight);
			auto crosshaircolour = std::make_shared<ColourPicker>(400, 25, &Configs.Overlay.CrosshairColour);
			overlaytab->Push(crosshaircolour);
			auto crosshairtype = std::make_shared<DropDown>(270, 20, LIT(L"Crosshair Mode"), &Configs.Overlay.CrosshairType,
				std::vector<std::wstring>{LIT(L"None"), LIT(L"Filled Circle"), LIT(L"Outline Circle"), LIT(L"Filled Rect"), LIT(L"Outline Rect") });
			overlaytab->Push(crosshairtype);
			auto textsize = std::make_shared<Slider<int>>(270, 45, 100, LIT(L"Size"), LIT(L"px"), 1, 20, &Configs.Overlay.CrosshairSize);
			overlaytab->Push(textsize);
			
			
		}
		tabcontroller->Push(overlaytab);
		auto aimbottab = std::make_shared<Tab>(LIT(L"Aimbot"), 5, 130, &SelectedTab, 0, 20);
		{
			auto enable = std::make_shared<Toggle>(100, 5, LIT(L"Enable"), &Configs.Aimbot.Enable);
			aimbottab->Push(enable);
			auto targetplayers = std::make_shared<Toggle>(100, 25, LIT(L"Target Players"), &Configs.Aimbot.TargetPlayers);
			aimbottab->Push(targetplayers);
			auto targetzombies = std::make_shared<Toggle>(100, 45, LIT(L"Target Zombies"), &Configs.Aimbot.TargetZombies);
			aimbottab->Push(targetzombies);
			auto drawfov = std::make_shared<Toggle>(100, 65, LIT(L"Draw FOV"), &Configs.Aimbot.DrawFOV);
			aimbottab->Push(drawfov);
			auto colourpicker = std::make_shared<ColourPicker>(180, 66, &Configs.Aimbot.FOVColour);
			aimbottab->Push(colourpicker);
			auto fov = std::make_shared<Slider<int>>(100, 85, 150, LIT(L"FOV"), LIT(L"°"), 1, 1000, &Configs.Aimbot.FOV);
			aimbottab->Push(fov);
			auto maxdistance = std::make_shared<Slider<int>>(100, 110, 150, LIT(L"Max Distance"), LIT(L"m"), 0, 1000, &Configs.Aimbot.MaxDistance);
			aimbottab->Push(maxdistance);
			auto priority = std::make_shared<DropDown>(100, 150, LIT(L"Priority"), &Configs.Aimbot.Priority,
								std::vector<std::wstring>{LIT(L"Crosshair"), LIT(L"Distance"), LIT(L"Both")});
			aimbottab->Push(priority);
			auto keybind = std::make_shared<KeyBind>(100, 195, LIT(L"Aim Key"), &Configs.Aimbot.Aimkey);
			aimbottab->Push(keybind);

			auto connecttokmbox = std::make_shared<Button>(210, 5, LIT(L"Connect To Kmbox"), []()
				{
					kmbox::KmboxInitialize("");

				

				});
			aimbottab->Push(connecttokmbox);
		}
		tabcontroller->Push(aimbottab);
		auto configtab = std::make_shared<Tab>(LIT(L"Config"), 5, 155, &SelectedTab, 0, 20);
		{
			auto saveconfig = std::make_shared<Button>(100, 5, LIT(L"Save"), []()
				{
					
					
					SaveConfig(L"Default.json");
					CreateGUI(); // reinit/ reload
					SelectedTab = 1;
					
				});
			configtab->Push(saveconfig);

			auto loadconfig = std::make_shared<Button>(165, 5, LIT(L"Load"), []()
				{


					LoadConfig(L"Default.json");
					CreateGUI(); // reinit/ reload
					SelectedTab = 1;

				});
			configtab->Push(loadconfig);
		}
		tabcontroller->Push(configtab);

	}

	MenuEntity->Push(form);
	MenuEntity->Draw();
	MenuEntity->Update();
}

void SetFormPriority()
{
	// This sorts the host container (containerptr) which contains forms, as long as a form isn't parented to another form then this will allow it to draw over when clicked.
	// I swear to god if i need to make this work for forms inside forms for some odd reason in the future then i am going to throw a monitor out the window.
	std::sort(MenuEntity->GetContainer().begin(), MenuEntity->GetContainer().end(),
	          [](child a, child b) { return b->GetLastClick() < a->GetLastClick(); }
	);
}

float LastOpen = 0;

void Render()
{
	if (IsKeyClicked(VK_INSERT) && LastOpen < clock() * 0.00001f)
	{
		LastOpen = (clock() * 0.00001f) + 0.002f;
		MenuOpen = !MenuOpen;
	}

	MenuEntity->Draw();
	MenuEntity->GetContainer()[0]->Update(); // only allow stretching,dragging and other update stuff if it is the main form, prevents dragging and sizing the wrong forms.
	SetFormPriority();
}
