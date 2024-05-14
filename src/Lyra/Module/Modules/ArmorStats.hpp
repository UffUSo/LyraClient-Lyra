#pragma once
#include "../Module.hpp"
#include "../../SDK/Client/Render/MinecraftUIRenderContext.hpp"
class ArmorStats : public Module {
public:
    ArmorStats();
    void InitModuleSettings() override;
    std::string getModuleName() override;
    void onEnable() override;
    void onDisable() override;
    void onPostMCRender(const SetupAndRenderEvent &event) override;
    void onRender(const RenderEvent &event) override;
};