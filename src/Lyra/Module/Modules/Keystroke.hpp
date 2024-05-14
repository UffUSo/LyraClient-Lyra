#pragma once
#include "../Module.hpp"

class Keystroke : public Module {
public:
    Keystroke();
    std::string getModuleName() override;
    void InitModuleSettings() override;
    void onEnable() override;
    void onDisable() override;
    void onRender(const RenderEvent &event) override;
};