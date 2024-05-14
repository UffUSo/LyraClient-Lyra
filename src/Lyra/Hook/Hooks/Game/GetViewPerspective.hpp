#pragma once

#include <array>
#include "../Hook.hpp"

class GetViewPerspective : public Hook
{
private:

public:
    static int TickCallback(uintptr_t* a1);
    typedef int(__thiscall* original)(uintptr_t*);

    static inline original func_original = nullptr;

    GetViewPerspective();
    void enableHook() override;
};
