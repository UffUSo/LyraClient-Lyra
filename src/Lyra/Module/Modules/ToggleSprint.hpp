#pragma once
#include "../Module.hpp"

class ToggleSprint : public Module {
public:
    ToggleSprint();
	std::string getModuleName() override;
    void onEnable() override;
    void onDisable() override;
    void InitModuleSettings() override;
    void onTick(const TickEvent& event) override;
};