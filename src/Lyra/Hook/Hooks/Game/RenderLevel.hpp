#pragma once

#include <array>
#include "../Hook.hpp"
#include "../../../SDK/SDK.hpp"
#include "../../../SDK/Client/Render/MinecraftUIRenderContext.hpp"
#include "../../../Event/EventManager.hpp"

class RenderLevel : public Hook
{
private:
    static __int64 RenderLevelCallback(LevelRender* _this, ScreenContext* context, const __int64& renderObj);
    static inline bool hasSetup = false;

public:
    typedef __int64(__thiscall* setupAndRenderOriginal)(LevelRender* _this, ScreenContext* context, const __int64& renderObj);
    static inline setupAndRenderOriginal func_original = 0;

    RenderLevel();
    void enableHook() override;
};
