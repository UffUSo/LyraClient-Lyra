#pragma once
#include "../Module.hpp"
extern std::string currentSetting;
extern bool DisableModMenu;
class ModMenu : public Module {
public:
    ModMenu();
	std::string getModuleName() override;
    void onEnable() override;
    void onDisable() override;
    void InitModuleSettings() override;
    void onRender(const RenderEvent& event) override;
    void onKey(KeyEvent& event) override;
    void onMouse(MouseEvent& event) override;
    static inline std::unordered_map<std::string, std::vector<std::string>> blocked_modules;
};