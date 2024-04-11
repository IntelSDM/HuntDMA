#pragma once
#include "pch.h"
class OverlayConfig
{
    std::string ConfigName;

public:
    OverlayConfig(const std::string& name)
    {
        ConfigName = name;
    }
    bool OverrideResolution = false;
    int Width = 1920;
    int Height = 1080;
    int CrosshairType = 1;
    int CrosshairSize = 3;
    D2D1::ColorF CrosshairColour = Colour(0, 150, 255);
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
        j[ConfigName][LIT("OverrideResolution")] = OverrideResolution;
        j[ConfigName][LIT("Width")] = Width;
        j[ConfigName][LIT("Height")] = Height;
        j[ConfigName][LIT("CrosshairType")] = CrosshairType;
        j[ConfigName][LIT("CrosshairSize")] = CrosshairSize;
        ToJsonColour(&j, LIT("CrosshairColour"), &CrosshairColour);

        return j;
    }
    void FromJson(const json& j)
    {
        if (!j.contains(ConfigName))
            return;
        if (j[ConfigName].contains(LIT("OverrideResolution")))
            OverrideResolution = j[ConfigName][LIT("OverrideResolution")];
        if (j[ConfigName].contains(LIT("Width")))
            Width = j[ConfigName][LIT("Width")];
        if (j[ConfigName].contains(LIT("Height")))
            Height = j[ConfigName][LIT("Height")];
        if (j[ConfigName].contains(LIT("CrosshairType")))
            CrosshairType = j[ConfigName][LIT("CrosshairType")];
        if (j[ConfigName].contains(LIT("CrosshairSize")))
            CrosshairSize = j[ConfigName][LIT("CrosshairSize")];
        FromJsonColour(j, LIT("CrosshairColour"), &CrosshairColour);
    }
};

