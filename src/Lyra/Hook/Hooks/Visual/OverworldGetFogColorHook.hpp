#pragma once

#include "../Hook.hpp"
#include "../../../SDK/Client/Core/mce.hpp"

class OverworldGetFogColorHook : public Hook
{
private:
    static mce::Color* OverworldGetFogColorCallback(void* a1, void* a2, mce::Color& colour, float a4);

public:
    typedef mce::Color*(__thiscall* FogColorOriginal)(void* a1, void* a2, mce::Color& colour, float a4);
    static inline FogColorOriginal func_original = 0;

    OverworldGetFogColorHook();
    void enableHook() override;
};
