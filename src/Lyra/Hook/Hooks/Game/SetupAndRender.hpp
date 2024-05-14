#pragma once

#include <array>
#include "../Hook.hpp"
#include "../../../SDK/SDK.hpp"
#include "../../../SDK/Client/Render/MinecraftUIRenderContext.hpp"
#include "../../../Event/EventManager.hpp"

class mcuirc {

};

class SetupAndRenderHook : public Hook
{
private:
    static void SetupAndRendeCallback(ScreenView* pScreenContext, MinecraftUIRenderContext* mcuirc);
    static inline bool hasSetup = false;

public:
    typedef void(__thiscall* setupAndRenderOriginal)(ScreenView* pScreenContext, MinecraftUIRenderContext* mcuirc);
    static inline setupAndRenderOriginal func_original = 0;

    SetupAndRenderHook();
    void enableHook() override;
};
