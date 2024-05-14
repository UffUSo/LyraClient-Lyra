#pragma once

#include <array>
#include "../Hook.hpp"
#include "../../../Module/ModuleManager.hpp"
#include "../../../Config/ConfigManager.hpp"

class GetFovHook : public Hook
{
private:
    static float GetFovCallback(void* a1, float f, void* a2, void* a3);

public:
    typedef float(__thiscall* FovOriginal)(void* a1, float f, void* a2, void* a3);
    static inline FovOriginal func_original = 0;

    GetFovHook();
    void enableHook() override;
};
