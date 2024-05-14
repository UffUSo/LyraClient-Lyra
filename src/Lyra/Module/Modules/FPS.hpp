#pragma once
#include "../Module.hpp"

class FPS : public Module {
public:
    FPS();
	std::string getModuleName() override;
    void onEnable() override;
    void onDisable() override;
    void InitModuleSettings() override;
    void onRender(const RenderEvent& event) override;
};