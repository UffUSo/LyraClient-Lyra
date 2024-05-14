#pragma once
#include "../Module.hpp"

class Clock : public Module {
public:
    Clock();
	std::string getModuleName() override;
    void onEnable() override;
    void onDisable() override;
    void InitModuleSettings() override;
    void onRender(const RenderEvent& event) override;
};