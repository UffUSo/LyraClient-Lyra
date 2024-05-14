#pragma once
#include "../Module.hpp"

class MotionBlur : public Module {
public:
    MotionBlur();
	std::string getModuleName() override;
    void onEnable() override;
    void onDisable() override;
    void InitModuleSettings() override;
    void onRender(const RenderEvent& event) override;
};