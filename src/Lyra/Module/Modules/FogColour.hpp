#pragma once
#include "../Module.hpp"

class FogColour : public Module {
public:
    FogColour();
    std::string getModuleName() override;
    void onEnable() override;
    void onDisable() override;
    void InitModuleSettings() override;
    void onGetFogColorEvent(GetFogColorEvent&) override;
};