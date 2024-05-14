#pragma once

#include "../Module.hpp"

class Coordinates : public Module {
public:
    Coordinates();

    std::string getModuleName() override;
    void InitModuleSettings() override;
    void onEnable() override;
    void onDisable() override;
    void onRender(const RenderEvent &event) override;
};
