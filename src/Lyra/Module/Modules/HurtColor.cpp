#include "HurtColor.hpp"
#include <iostream>
HurtColor::HurtColor() : ModuleCtor(0, "Hit Color", "HurtColor") {
    this->InitModuleSettings();
}

std::string HurtColor::getModuleName() {
    return "Hurt Color";
}

void HurtColor::InitModuleSettings() {
    Settings::addSetting<bool>(this->getModuleName(), "enabled", false, true);
    Settings::addSetting<std::vector<float>>("Hurt Color", "Hurt Color", {1.0f, 1.0f, 1.0f, 0.65f}, true);

    this->SettingsList["COLOR"]["Hurt Color"] = "colorpicker";
}

void HurtColor::onEnable() {
    Settings::getSettingByName<bool>(this->getModuleName(), "enabled")->value = true;
    eventMgr.addListener(this);
}

void HurtColor::onDisable() {
    Settings::getSettingByName<bool>(this->getModuleName(), "enabled")->value = false;
    eventMgr.removeListener(this);
}