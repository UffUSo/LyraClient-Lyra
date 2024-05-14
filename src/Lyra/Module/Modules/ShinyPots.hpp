#pragma once
#include "../Module.hpp"

class ShinyPots : public Module {
public:
    ShinyPots();
    std::string getModuleName() override;
    void InitModuleSettings() override;
    void onEnable() override;
    void onDisable() override;
    void onTick(const TickEvent &event) override;
};