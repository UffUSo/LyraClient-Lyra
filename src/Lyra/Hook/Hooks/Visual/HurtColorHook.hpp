#pragma once

#include "../Hook.hpp"
#include "../../../../Utils/Memory/Memory.hpp"
#include "../../../../Utils/Utils.hpp"
#include "../../../Config/ConfigManager.hpp"

class HurtColorHook : public Hook {
private:

    static MCCColor *HurtColorCallback(void *a1, MCCColor *color, void *a3) {
        MCCColor* col = funcOriginal(a1, color, a3);
        if (Settings::getSettingByName<bool>("Hurt Color", "enabled")->value) {
            std::vector<float> color = Settings::getSettingByName<std::vector<float>>("Hurt Color", "Hurt Color")->value;
            col->r = color.at(0);
            col->g = color.at(1);
            col->b = color.at(2);
            col->a = color.at(3);
        }

        return col;
    }

public:
    typedef MCCColor *(__thiscall *HurtColorOriginal)(void *a1, MCCColor *color, void *a3);

    static inline HurtColorOriginal funcOriginal = nullptr;

    HurtColorHook() : Hook("Hurt Color Hook", "") {}

    void enableHook() override {

        auto RefAddr = Memory::findSig("E8 ? ? ? ? E9 ? ? ? ? 8B 43 ? 48 8D 54 24");
        auto RealFunc = Memory::offsetFromSig(RefAddr, 1);


        this->manualHook((void *) RealFunc, HurtColorCallback, (void **) &funcOriginal);
    }
};
