#pragma once
#include "../Hook.hpp"
#include "../../../SDK/Client/Core/mce.hpp"
class ItemStack;

class RenderGuiItemInChunkHook : public Hook
{
    static double Callback(void*, void*, uint32_t, ItemStack*, float, float, float, float, float, int, bool, float);

public:
    inline static mce::Color* color;
    inline static mce::Color original;
    typedef double(__thiscall* Original)(void*, void*, uint32_t, ItemStack*, float, float, float, float, float, int, bool, float);
    static inline Original func_original = 0;

    RenderGuiItemInChunkHook();
    void enableHook() override;
};
