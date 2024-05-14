#pragma once
#include "../Module.hpp"

class PotCounter : public Module {
public:
    PotCounter();
	std::string getModuleName() override;
    void onEnable() override;
    void onDisable() override;
    void InitModuleSettings() override;
    void onRender(const RenderEvent& event) override;
};