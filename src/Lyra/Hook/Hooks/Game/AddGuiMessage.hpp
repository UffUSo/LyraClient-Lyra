#pragma once

#include <array>
#include "../Hook.hpp"
#include "../../../SDK/SDK.hpp"
#include "../../../SDK/Client/Render/MinecraftUIRenderContext.hpp"
#include "../../../Event/EventManager.hpp"

class AddGuiMessage : public Hook
{
private:
    static __int64 AddGuiMessageCallback(GuiData* _this, GuiMessage* message, int profanityFilterContext);
    static inline bool hasSetup = false;

public:
    typedef __int64(__thiscall* setupAndRenderOriginal)(GuiData* _this, GuiMessage* message, int profanityFilterContext);
    static inline setupAndRenderOriginal func_original = 0;

    AddGuiMessage();
    void enableHook() override;
};
