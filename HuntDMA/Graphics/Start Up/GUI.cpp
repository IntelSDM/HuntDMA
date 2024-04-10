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
int SelectedTab = 1;
int SelectedSubTab = 0;
int TabCount = 0;
int KeyBindClipBoard = 0;
EntityVector MenuEntity;
bool MenuOpen = true;
D2D1::ColorF ColourPickerClipBoard = Colour(255,255,255);
D2D1::ColorF ColourPick = Colour(0, 150, 255, 255);

void CreateGUI()
{
	MenuEntity = std::make_shared<Container>();
	auto form = std::make_shared<Form>(100, 100.0f, 300, 200, 2, 30, LIT(L"Cool Hack"), false);
	{
		auto health = std::make_shared<Toggle>(10, 10, LIT(L"Health"), &ConfigInstance.Player.Health);
		auto colourpicker = std::make_shared<ColourPicker>(80, 10, &ConfigInstance.Player.TextColour);
		auto distance = std::make_shared<Toggle>(10, 30, LIT(L"Distance"), &ConfigInstance.Player.Distance);
		auto primary = std::make_shared<Toggle>(10, 50, LIT(L"Primary Weapon"), &ConfigInstance.Player.PrimaryWeapon);
		auto secondary = std::make_shared<Toggle>(10, 70, LIT(L"Secondary Weapon"), &ConfigInstance.Player.SecondaryWeapon);
		auto maxdistance = std::make_shared<Slider<int>>(10, 90,150, LIT(L"Max Distance"),LIT(L"m"), 0, 1500, &ConfigInstance.Player.MaxDistance);
		auto fontsize = std::make_shared<Slider<int>>(10, 115,150, LIT(L"Font Size"),LIT(L"px"), 1, 16, &ConfigInstance.Player.FontSize);
		auto info = std::make_shared<Label>(LIT(L"WinKey + Shift + Arrow To Change Monitor"), 10, 140);
		form->Push(health);
		form->Push(colourpicker);
		form->Push(distance);
		form->Push(primary);
		form->Push(secondary);
		form->Push(maxdistance);
		form->Push(fontsize);
		form->Push(info);



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
