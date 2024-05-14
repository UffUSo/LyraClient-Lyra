#pragma once
#include "../Module.hpp"

class ComboCounter : public Module {
public:
    ComboCounter();
	std::string getModuleName() override;
    void onEnable() override;
    void onDisable() override;
    void InitModuleSettings() override;
    void onTick(const TickEvent& event) override;
    void onAttack(AttackEvent &event) override;
    void onRender(const RenderEvent& event) override;
};