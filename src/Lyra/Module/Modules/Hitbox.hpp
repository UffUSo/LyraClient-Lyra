#pragma once
#include "../Module.hpp"

class Hitbox : public Module {
public:
    Hitbox();
	std::string getModuleName() override;
    void onEnable() override;
    void onDisable() override;
    void InitModuleSettings() override;
    void onRenderLevel(const RenderLevelEvent& event) override;
};