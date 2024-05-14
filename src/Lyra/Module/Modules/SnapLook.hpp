#pragma once
#include "../Module.hpp"

extern bool ShouldSnapLook;

class SnapLook : public Module {
public:
    SnapLook();
	std::string getModuleName() override;
    void onEnable() override;
    void onDisable() override;
    void InitModuleSettings() override;
    void onKey(KeyEvent& event) override;
};