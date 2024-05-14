#include "Clock.hpp"
#include "../../../Utils/RenderUtils.h"
#include "../../SDK/SDK.hpp"

Clock::Clock() : ModuleCtor(0, "Clock", "clock") {
	this->InitModuleSettings();
}

std::string Clock::getModuleName() {
	return "Clock";
}

void Clock::InitModuleSettings() {
	Settings::addSetting<bool>("Clock", "enabled", false, true);
	Settings::addSetting<float>("Clock", "PosX", 0, true);
	Settings::addSetting<float>("Clock", "PosY", 0, true);
	Settings::addSetting<float>("Clock", "size", 1, true);
	Settings::addSetting<float>("Clock", "Rounding", 0, true);
	Settings::addSetting<float>(getModuleName(), "Opacity", .5, true);
	Settings::addSetting<std::vector<float>>(getModuleName(), "Text Color", std::vector<float>{1, 1, 1, 1}, true);
	Settings::addSetting<std::vector<float>>(getModuleName(), "Background Color", std::vector<float>{0, 0, 0, .5}, true);
	Settings::addSetting<bool>("Clock", "24 Hour", false, true);
	
	this->SettingsList["COLOR"]["Background Color"] = "colorpicker";
	this->SettingsList["SCALE"]["Opacity"] = "slider-0-1";
	this->SettingsList["COLOR"]["Text Color"] = "colorpicker";
	this->SettingsList["SCALE"]["size"] = "slider-0.5-2.5";
    this->SettingsList["SCALE"]["Rounding"] = "slider-0-1";
	this->SettingsList["Other"]["24 Hour"] = "toggle";
}

void Clock::onEnable() {
	Settings::getSettingByName<bool>("Clock", "enabled")->value = true;
	eventMgr.addListener(this);
}

void Clock::onDisable() {
	Settings::getSettingByName<bool>("Clock", "enabled")->value = false;
	eventMgr.removeListener(this);
}

void Clock::onRender(const RenderEvent& event) { 
	if(!SDK::clientInstance || !SDK::clientInstance->getLocalPlayer()) return;
    if (!(SDK::TopScreen.rfind("hud_screen") != std::string::npos)) return;
	time_t currentTime;
	struct tm* timeInfo;
	char timeBuffer[10];

	time(&currentTime);
	timeInfo = localtime(&currentTime);
	if (Settings::getSettingByName<bool>("Clock", "24 Hour")->value) {
		strftime(timeBuffer, sizeof(timeBuffer), "%H:%M", timeInfo);
	}
	else {
		strftime(timeBuffer, sizeof(timeBuffer), "%I:%M %p", timeInfo);
	}
	
	std::string formattedTime(timeBuffer);
	this->RenderHUD(formattedTime);
}