#pragma once
#include "../Module.hpp"

class Tablist : public Module {
public:
    Tablist();
    std::string getModuleName() override;
    void InitModuleSettings() override;
    void onEnable() override;
    void onDisable() override;
    void onRender(const RenderEvent& event) override;
    void RenderHUD(std::string text) override;
    void onKey(KeyEvent& event) override;
    void onTick(const TickEvent& event) override;
};