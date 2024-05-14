#pragma once
#include "../Module.hpp"

extern bool ShouldZoom;
extern float currentZoom;

class Zoom : public Module {
public:
    Zoom();
	std::string getModuleName() override;
    void onEnable() override;
    void onDisable() override;
    void InitModuleSettings() override;
    void onKey(KeyEvent& event) override;
};