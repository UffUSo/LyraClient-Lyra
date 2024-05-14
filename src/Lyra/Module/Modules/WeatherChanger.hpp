#pragma once
#include "../Module.hpp"

class WeatherChanger : public Module {
public:
    WeatherChanger();
	std::string getModuleName() override;
    void onEnable() override;
    void onDisable() override;
    void InitModuleSettings() override;
    void onTick(const TickEvent& event) override;
};