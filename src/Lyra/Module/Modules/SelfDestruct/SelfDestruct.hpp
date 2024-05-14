#pragma once

#include <string>
#include "../../Module.hpp"

class SelfDestruct : public Module {
public:
    SelfDestruct();
    std::string getModuleName() override;
    void onEnable() override;
    void onDisable() override;
    void InitModuleSettings() override;
    void onRender(const RenderEvent &event) override;
};