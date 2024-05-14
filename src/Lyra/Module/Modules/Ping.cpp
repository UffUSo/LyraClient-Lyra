#include "Ping.hpp"
#include "../../SDK/SDK.hpp"
#include <iostream>

Ping::Ping() : ModuleCtor(0, "Ping", "ping") {
    this->InitModuleSettings();
}

std::string Ping::getModuleName() {
    return "Ping";
}

void Ping::InitModuleSettings() {
    Settings::addSetting<bool>(this->getModuleName(), "enabled", false, true);
    Settings::addSetting<float>(this->getModuleName(), "PosX", 0, true);
    Settings::addSetting<float>(this->getModuleName(), "PosY", 0, true);
    Settings::addSetting<float>("Ping", "size", 1, true);
    Settings::addSetting<float>("Ping", "Rounding", 0, true);
    Settings::addSetting<float>(getModuleName(), "Opacity", .5, true);
    Settings::addSetting<std::vector<float>>(getModuleName(), "Text Color", std::vector<float>{1, 1, 1, 1}, true);
    Settings::addSetting<std::vector<float>>(getModuleName(), "Background Color", std::vector<float>{0, 0, 0, .5}, true);

    this->SettingsList["COLOR"]["Background Color"] = "colorpicker";
    this->SettingsList["SCALE"]["Opacity"] = "slider-0-1";
    this->SettingsList["COLOR"]["Text Color"] = "colorpicker";
    this->SettingsList["SCALE"]["size"] = "slider-0.5-2.5";
    this->SettingsList["SCALE"]["Rounding"] = "slider-0-1";
}

void Ping::onEnable() {
    Settings::getSettingByName<bool>(this->getModuleName(), "enabled")->value = true;
    eventMgr.addListener(this);
}

void Ping::onDisable() {
    Settings::getSettingByName<bool>(this->getModuleName(), "enabled")->value = false;
    eventMgr.removeListener(this);
}

void Ping::onRender(const RenderEvent& event) {
    if (!SDK::clientInstance || !SDK::clientInstance->getLocalPlayer()) return;
    if (!(SDK::TopScreen.rfind("hud_screen") != std::string::npos)) return;
    this->RenderHUD(std::to_string(SDK::clientInstance->getRakNetConnector()->peer->getPing()) + " ms");
}