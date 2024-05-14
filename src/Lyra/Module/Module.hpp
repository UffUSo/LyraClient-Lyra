#pragma once

#include "../Event/EventManager.hpp"
#include <string.h>
#include "../../Utils/Utils.hpp"
#include "../Config/ConfigManager.hpp"

#define ModuleCtor(key, description, icon) Module(key, description, icon)

class Module : public Listener {
public:
    Module() = default;
    Module(unsigned char key, std::string description, std::string icon);

    virtual ~Module() = default;

    virtual std::string getModuleName();
    virtual std::string getRawModuleName();
    virtual std::string getDescription();
    virtual void onEnable();
    virtual void onDisable();
    virtual void setEnabled(bool enabled);
    virtual void toggle();
    virtual bool isEnabled();
    template<typename T>
    SettingType<T>* getSetting(std::string setting_name);
    template<typename T>
    void addSetting(std::string setting_name, T defaultValue, bool check);
    virtual void InitModuleSettings();
    virtual void RenderHUD(std::string text);

    bool dragging = false;
    bool resizing = false;
    bool selected = false;
    std::map<std::string, std::map<std::string, std::string>> SettingsList;
    std::map<std::string, std::map<std::string, void(*)()>> pbConfig;
    float mousePosDiff = 0;
    ImVec2 pos = ImVec2(0, 0);
    ImVec2 diff = ImVec2(0, 0);

    std::string icon;

private:
    unsigned char key = 0;
    std::string description{};
    bool enabled = false;
};
