#pragma once
#include "../Module.hpp"

class FoodPoints : public Module {
public:
    FoodPoints();
    void InitModuleSettings() override;
    std::string getModuleName() override;
    void onEnable() override;
    void onDisable() override;
    void onRender(const RenderEvent &event);
};

static std::map<std::string, int> hungerLVL;