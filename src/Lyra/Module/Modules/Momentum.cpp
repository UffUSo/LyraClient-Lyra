#include "Momentum.hpp"
#include "../../SDK/SDK.hpp"
#include <iostream>

Momentum::Momentum() : ModuleCtor(0, "Momentum", "momentum") {
    this->InitModuleSettings();
}

std::string Momentum::getModuleName() {
    return "Speed Display";
}

void Momentum::InitModuleSettings() {
    Settings::addSetting<bool>(this->getModuleName(), "enabled", false, true);
    Settings::addSetting<float>(this->getModuleName(), "PosX", 0, true);
    Settings::addSetting<float>(this->getModuleName(), "PosY", 0, true);
    Settings::addSetting<float>(this->getModuleName(), "size", 1, true);
    Settings::addSetting<float>(this->getModuleName(), "Rounding", 0, true);
    Settings::addSetting<float>(getModuleName(), "Opacity", .5, true);
	Settings::addSetting<std::vector<float>>(getModuleName(), "Text Color", std::vector<float>{1, 1, 1, 1}, true);
    Settings::addSetting<std::vector<float>>(getModuleName(), "Background Color", std::vector<float>{0, 0, 0, .5}, true);

	this->SettingsList["COLOR"]["Background Color"] = "colorpicker";
	this->SettingsList["SCALE"]["Opacity"] = "slider-0-1";
	this->SettingsList["COLOR"]["Text Color"] = "colorpicker";
    this->SettingsList["SCALE"]["size"] = "slider-0.5-2.5";
    this->SettingsList["SCALE"]["Rounding"] = "slider-0-1";
}

void Momentum::onEnable() {
    Settings::getSettingByName<bool>(this->getModuleName(), "enabled")->value = true;
    eventMgr.addListener(this);
}

void Momentum::onDisable() {
    Settings::getSettingByName<bool>(this->getModuleName(), "enabled")->value = false;
    eventMgr.removeListener(this);
}

void Momentum::onRender(const RenderEvent& event) {
    if(!SDK::clientInstance || !SDK::clientInstance->getLocalPlayer()) return;
    if (!(SDK::TopScreen.rfind("hud_screen") != std::string::npos)) return;
    if (SDK::clientInstance && SDK::clientInstance->getLocalPlayer() != nullptr) {
        if (SDK::clientInstance->getLocalPlayer()->getstateVector() != nullptr) {
            auto speed = SDK::clientInstance->getLocalPlayer()->getstateVector()->pos.dist(SDK::clientInstance->getLocalPlayer()->getstateVector()->posPrev) * 20;

            this->RenderHUD(std::format("{:.2f}", speed)+" m/s");
        }
    }
}
