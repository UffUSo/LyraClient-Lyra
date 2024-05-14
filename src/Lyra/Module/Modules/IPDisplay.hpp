#pragma once
#include "../Module.hpp"

class IPDisplay : public Module {
public:
    IPDisplay();
	std::string getModuleName() override;
    void InitModuleSettings() override;
    void onEnable() override;
    void onDisable() override;
    void onRender(const RenderEvent& event) override;
    void RenderHUD(std::string text) override;
};