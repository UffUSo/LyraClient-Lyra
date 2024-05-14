#pragma once
#include "../Module.hpp"

class CPS : public Module {
public:
    CPS();
	std::string getModuleName() override;
    void onEnable() override;
    void onDisable() override;
    void InitModuleSettings() override;
    void onRender(const RenderEvent& event) override;
};