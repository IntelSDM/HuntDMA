#pragma once
#include "pch.h"
class AimbotConfig
{
    std::string ConfigName;

public:
    AimbotConfig(const std::string& name)
    {
        ConfigName = name;
    }
    bool Enable = true;
    int MaxDistance = 250;
    bool TargetZombies = true;
    bool TargetPlayers = true;
    int Priority = 0;
    int FOV = 200;
    int Aimkey = 4;
    bool DrawFOV = true;
    D2D1::ColorF FOVColour = D2D1::ColorF::White;
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
        j[ConfigName][LIT("MaxDistance")] = MaxDistance;
        j[ConfigName][LIT("TargetZombies")] = TargetZombies;
        j[ConfigName][LIT("TargetPlayers")] = TargetPlayers;
        j[ConfigName][LIT("Priority")] = Priority;
        j[ConfigName][LIT("FOV")] = FOV;
        j[ConfigName][LIT("Aimkey")] = Aimkey;
        j[ConfigName][LIT("DrawFOV")] = DrawFOV;
        ToJsonColour(&j, LIT("FOVColour"), &FOVColour);



        return j;
    }
    void FromJson(const json& j)
    {
        if (!j.contains(ConfigName))
            return;
        if (j[ConfigName].contains(LIT("Enable")))
            Enable = j[ConfigName][LIT("Enable")];
        if (j[ConfigName].contains(LIT("MaxDistance")))
            MaxDistance = j[ConfigName][LIT("MaxDistance")];
        if (j[ConfigName].contains(LIT("TargetZombies")))
            TargetZombies = j[ConfigName][LIT("TargetZombies")];
        if (j[ConfigName].contains(LIT("TargetPlayers")))
            TargetPlayers = j[ConfigName][LIT("TargetPlayers")];
        if (j[ConfigName].contains(LIT("Priority")))
            Priority = j[ConfigName][LIT("Priority")];
        if (j[ConfigName].contains(LIT("FOV")))
            FOV = j[ConfigName][LIT("FOV")];
        if (j[ConfigName].contains(LIT("Aimkey")))
            Aimkey = j[ConfigName][LIT("Aimkey")];
        if (j[ConfigName].contains(LIT("DrawFOV")))
            DrawFOV = j[ConfigName][LIT("DrawFOV")];
        FromJsonColour(j, LIT("FOVColour"), &FOVColour);
    }
};

