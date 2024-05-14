#include "CPS.hpp"
#include "../../../Utils/RenderUtils.h"

CPS::CPS() : ModuleCtor(0, "CPS", "cps") {
	this->InitModuleSettings();
}

std::string CPS::getModuleName() {
	return "CPS";
}

void CPS::InitModuleSettings() {
	Settings::addSetting<bool>("CPS", "enabled", false, true);
	Settings::addSetting<float>("CPS", "PosX", 0, true);
	Settings::addSetting<float>("CPS", "PosY", 0, true);
	Settings::addSetting<float>("CPS", "size", 1, true);
	Settings::addSetting<float>("CPS", "Rounding", 0, true);
	Settings::addSetting<float>(getModuleName(), "Opacity", .5, true);
	Settings::addSetting<std::vector<float>>(getModuleName(), "Text Color", std::vector<float>{1, 1, 1, 1}, true);
	Settings::addSetting<std::vector<float>>(getModuleName(), "Background Color", std::vector<float>{0, 0, 0, .5}, true);

	this->SettingsList["COLOR"]["Background Color"] = "colorpicker";
	this->SettingsList["SCALE"]["Opacity"] = "slider-0-1";
	this->SettingsList["COLOR"]["Text Color"] = "colorpicker";
	this->SettingsList["SCALE"]["size"] = "slider-0.5-2.5";
    this->SettingsList["SCALE"]["Rounding"] = "slider-0-1";
}

void CPS::onEnable() {
	Settings::getSettingByName<bool>("CPS", "enabled")->value = true;
	eventMgr.addListener(this);
}

void CPS::onDisable() {
	Settings::getSettingByName<bool>("CPS", "enabled")->value = false;
	eventMgr.removeListener(this);
}

void CPS::onRender(const RenderEvent& event) {
	this->RenderHUD(std::to_string(MC::countLeftCps()) + " | " + std::to_string(MC::countRightCps()) + " CPS");
}