#pragma once
#include "pch.h"
class PlayerConfig
{
	std::string ConfigName;

public:
	PlayerConfig(const std::string& name)
	{
		ConfigName = name;
	}
    bool Enable = true;
    bool Name = true;
	bool Distance = true;
	int MaxDistance = 1500;
	D2D1::ColorF TextColour = Colour(255, 255, 255);
	int FontSize = 12;
    bool Chams = false;
    int ChamMode = 5;
    bool DrawFriends = false;
    D2D1::ColorF FriendColour = Colour(0, 255, 0);
    void ToJsonColour(json* j, const std::string& name, D2D1::ColorF* colour)
    {
        (*j)[ConfigName][name][LIT("r")] = colour->r;
        (*j)[ConfigName][name][LIT("g")] = colour->g;
        (*j)[ConfigName][name][LIT("b")] = colour->b;
        (*j)[ConfigName][name][LIT("a")] = colour->a;

    }
    void FromJsonColour(json j, const std::string& name, D2D1::ColorF* colour)
    {
        if (j[ConfigName].contains(name))
        {
            colour->r = j[ConfigName][name][LIT("r")];
            colour->g = j[ConfigName][name][LIT("g")];
            colour->b = j[ConfigName][name][LIT("b")];
            colour->a = j[ConfigName][name][LIT("a")];
        }
    }

    json ToJson()
    {
        json j;
        j[ConfigName][LIT("Enable")] = Enable;
        j[ConfigName][LIT("Name")] = Name;
        j[ConfigName][LIT("Distance")] = Distance;
        j[ConfigName][LIT("MaxDistance")] = MaxDistance;
        j[ConfigName][LIT("FontSize")] = FontSize;
        j[ConfigName][LIT("Chams")] = Chams;
        j[ConfigName][LIT("ChamMode")] = ChamMode;
        j[ConfigName][LIT("DrawFriends")] = DrawFriends;
        ToJsonColour(&j, LIT("TextColour"), &TextColour);
        ToJsonColour(&j, LIT("FriendColour"), &FriendColour);

        return j;
    }
    void FromJson(const json& j)
    {
        if (!j.contains(ConfigName))
            return;
        if (j[ConfigName].contains(LIT("Enable")))
            Enable = j[ConfigName][LIT("Enable")];
        if (j[ConfigName].contains(LIT("Name")))
            Name = j[ConfigName][LIT("Name")];
        if (j[ConfigName].contains(LIT("Distance")))
            Distance = j[ConfigName][LIT("Distance")];
        if (j[ConfigName].contains(LIT("FontSize")))
            FontSize = j[ConfigName][LIT("FontSize")];
        if (j[ConfigName].contains(LIT("MaxDistance")))
            MaxDistance = j[ConfigName][LIT("MaxDistance")];
        if (j[ConfigName].contains(LIT("Chams")))
            Chams = j[ConfigName][LIT("Chams")];
        if (j[ConfigName].contains(LIT("ChamMode")))
            ChamMode = j[ConfigName][LIT("ChamMode")];
        if (j[ConfigName].contains(LIT("DrawFriends")))
            DrawFriends = j[ConfigName][LIT("DrawFriends")];
        FromJsonColour(j, LIT("FriendColour"), &FriendColour);
        FromJsonColour(j, LIT("TextColour"), &TextColour);
    }
};

