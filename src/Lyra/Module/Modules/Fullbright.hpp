#pragma once
#include "../Module.hpp"

extern bool ShouldFullbright;

class Fullbright : public Module {
public:
    Fullbright();
	std::string getModuleName() override;
    void onEnable() override;
    void onDisable() override;
    void InitModuleSettings() override;
};