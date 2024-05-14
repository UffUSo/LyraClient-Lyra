#pragma once

#include <array>
#include "../Hook.hpp"
#include "../../../../Utils/Utils.hpp"

class RenderHighlightSelectionHook : public Hook
{
private:
    static void RenderHighlightSelectionHookCallback(void* _this, void* screenContext, void* block, void*& region, Vec3 const& pos);

public:
    typedef float(__thiscall* FovOriginal)(void* _this, void* screenContext, void* block, void* region, Vec3 const& pos);
    static inline FovOriginal func_original = 0;

    RenderHighlightSelectionHook();
    void enableHook() override;
};
