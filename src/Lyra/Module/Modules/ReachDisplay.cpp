#include "ReachDisplay.hpp"
#include "../../SDK/SDK.hpp"
#include <iostream>

ReachDisplay::ReachDisplay() : ModuleCtor(0, "Reach Display", "ReachDisplay") {
    this->InitModuleSettings();
}

std::string ReachDisplay::getModuleName() {
    return "Reach Display";
}

void ReachDisplay::InitModuleSettings() {
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

void ReachDisplay::onEnable() {
    Settings::getSettingByName<bool>(this->getModuleName(), "enabled")->value = true;
    eventMgr.addListener(this);
}

void ReachDisplay::onDisable() {
    Settings::getSettingByName<bool>(this->getModuleName(), "enabled")->value = false;
    eventMgr.removeListener(this);
}

void ReachDisplay::onAttack(AttackEvent &event) {
    reach = event.getActor()->getlevel()->getHitResult().distance();
    last_hit = std::chrono::high_resolution_clock::now();
}

void ReachDisplay::onTick(const TickEvent &event) {
    if(!SDK::clientInstance || !SDK::clientInstance->getLocalPlayer()) return;
    std::chrono::duration<double> duration = std::chrono::high_resolution_clock::now() - last_hit;
    if (duration.count() >= 5) reach = 0.0f;
}

void ReachDisplay::onRender(const RenderEvent& event) {
    if(!SDK::clientInstance || !SDK::clientInstance->getLocalPlayer()) return;
    if (!(SDK::TopScreen.rfind("hud_screen") != std::string::npos)) return;
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << reach;
    std::string reachString = oss.str();
    this->RenderHUD(reachString + " Blocks");
}


