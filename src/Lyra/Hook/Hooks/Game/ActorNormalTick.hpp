#pragma once

#include <array>
#include "../Hook.hpp"
#include "../../../SDK/Client/Actor/Player.hpp"


class ActorNormalTickHook : public Hook
{
private:

public:
    static void TickCallback(Actor* _this, uintptr_t entt);
    typedef void(__thiscall* KeyOriginal)(Actor* _this, uintptr_t entt);
    static inline KeyOriginal func_original = 0;

    ActorNormalTickHook();
    void enableHook() override;
};
