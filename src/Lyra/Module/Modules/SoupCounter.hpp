#pragma once
#include "../Module.hpp"

class SoupCounter : public Module {
public:
    SoupCounter();
	std::string getModuleName() override;
    void onEnable() override;
    void onDisable() override;
    void InitModuleSettings() override;
    void onRender(const RenderEvent& event) override;
};