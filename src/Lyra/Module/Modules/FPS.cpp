#include "FPS.hpp"
#include "../../../Utils/RenderUtils.h"
#include "../../SDK/SDK.hpp"

FPS::FPS() : ModuleCtor(0, "FPS", "fps") {
	this->InitModuleSettings();
}

std::string FPS::getModuleName() {
	return "FPS";
}

void FPS::InitModuleSettings() {
	Settings::addSetting<bool>("FPS", "enabled", false, true);
	Settings::addSetting<float>("FPS", "PosX", 0, true);
	Settings::addSetting<float>("FPS", "PosY", 0, true);
	Settings::addSetting<float>("FPS", "size", 1, true);
	Settings::addSetting<float>("FPS", "Rounding", 0, true);
	Settings::addSetting<float>(getModuleName(), "Opacity", .5, true);
	Settings::addSetting<std::vector<float>>(getModuleName(), "Text Color", std::vector<float>{1, 1, 1, 1}, true);
	Settings::addSetting<std::vector<float>>(getModuleName(), "Background Color", std::vector<float>{0, 0, 0, .5}, true);

	this->SettingsList["COLOR"]["Background Color"] = "colorpicker";
	this->SettingsList["SCALE"]["Opacity"] = "slider-0-1";
	this->SettingsList["COLOR"]["Text Color"] = "colorpicker";
	this->SettingsList["SCALE"]["size"] = "slider-0.5-2.5";
    this->SettingsList["SCALE"]["Rounding"] = "slider-0-1";
}

void FPS::onEnable() {
	Settings::getSettingByName<bool>("FPS", "enabled")->value = true;
	eventMgr.addListener(this);
}

void FPS::onDisable() {
	Settings::getSettingByName<bool>("FPS", "enabled")->value = false;
	eventMgr.removeListener(this);
}

void FPS::onRender(const RenderEvent& event) {
	if(!SDK::clientInstance || !SDK::clientInstance->getLocalPlayer()) return;
    if (!(SDK::TopScreen.rfind("hud_screen") != std::string::npos)) return;
	this->RenderHUD(std::to_string(MC::fps) + " FPS");
}