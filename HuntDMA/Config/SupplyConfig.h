#pragma once
#include "pch.h"
class SupplyConfig
{
    std::string ConfigName;

public:
    SupplyConfig(const std::string& name)
    {
        ConfigName = name;
    }
    bool Enable = true;
    bool Name = true;
    bool Distance = true;
    bool ShowAmmoSwapBox = true;
    bool ShowCompactAmmo = true;
    bool ShowMediumAmmo = true;
    bool ShowLongAmmo = true;
    bool ShowShortgunAmmo = true;
    bool ShowSpecialAmmo = true;
    bool ShowMedkit = true;
    bool ShowSupplyBox = true;
    int MaxDistance = 100;
    D2D1::ColorF TextColour = Colour(0, 255, 0);
    int FontSize = 11;
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
        j[ConfigName][LIT("ShowAmmoSwapBox")] = ShowAmmoSwapBox;
        j[ConfigName][LIT("ShowCompactAmmo")] = ShowCompactAmmo;
        j[ConfigName][LIT("ShowMediumAmmo")] = ShowMediumAmmo;
        j[ConfigName][LIT("ShowLongAmmo")] = ShowLongAmmo;
        j[ConfigName][LIT("ShowShortgunAmmo")] = ShowShortgunAmmo;
        j[ConfigName][LIT("ShowSpecialAmmo")] = ShowSpecialAmmo;
        j[ConfigName][LIT("ShowMedkit")] = ShowMedkit;
        j[ConfigName][LIT("ShowSupplyBox")] = ShowSupplyBox;
        j[ConfigName][LIT("MaxDistance")] = MaxDistance;
        j[ConfigName][LIT("FontSize")] = FontSize;
        ToJsonColour(&j, LIT("TextColour"), &TextColour);

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
        if (j[ConfigName].contains(LIT("ShowAmmoSwapBox")))
            ShowAmmoSwapBox = j[ConfigName][LIT("ShowAmmoSwapBox")];
        if (j[ConfigName].contains(LIT("ShowCompactAmmo")))
            ShowCompactAmmo = j[ConfigName][LIT("ShowCompactAmmo")];
        if (j[ConfigName].contains(LIT("ShowMediumAmmo")))
            ShowMediumAmmo = j[ConfigName][LIT("ShowMediumAmmo")];
        if (j[ConfigName].contains(LIT("ShowLongAmmo")))
            ShowLongAmmo = j[ConfigName][LIT("ShowLongAmmo")];
        if (j[ConfigName].contains(LIT("ShowShortgunAmmo")))
            ShowShortgunAmmo = j[ConfigName][LIT("ShowShortgunAmmo")];
        if (j[ConfigName].contains(LIT("ShowSpecialAmmo")))
            ShowSpecialAmmo = j[ConfigName][LIT("ShowSpecialAmmo")];
        if (j[ConfigName].contains(LIT("ShowMedkit")))
            ShowMedkit = j[ConfigName][LIT("ShowMedkit")];
        if (j[ConfigName].contains(LIT("ShowSupplyBox")))
            ShowSupplyBox = j[ConfigName][LIT("ShowSupplyBox")];
        if (j[ConfigName].contains(LIT("FontSize")))
            FontSize = j[ConfigName][LIT("FontSize")];
        if (j[ConfigName].contains(LIT("MaxDistance")))
            MaxDistance = j[ConfigName][LIT("MaxDistance")];
        FromJsonColour(j, LIT("TextColour"), &TextColour);
    }
};

