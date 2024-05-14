#pragma once
#include "../Module.hpp"

class ThirdPersonNametag : public Module {
public:
    ThirdPersonNametag();
	std::string getModuleName() override;
    void onEnable() override;
    void onDisable() override;
    void InitModuleSettings() override;
};