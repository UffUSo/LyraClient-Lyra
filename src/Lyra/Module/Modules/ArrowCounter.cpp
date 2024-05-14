#include "ArrowCounter.hpp"
#include "../../../Utils/RenderUtils.h"
#include "../../SDK/SDK.hpp"

ArrowCounter::ArrowCounter() : ModuleCtor(0, "ArrowCounter", "ArrowCounter") {
	this->InitModuleSettings();
}

std::string ArrowCounter::getModuleName() {
	return "Arrow Counter";
}

void ArrowCounter::InitModuleSettings() {
	Settings::addSetting<bool>("Arrow Counter", "enabled", false, true);
	Settings::addSetting<float>("Arrow Counter", "PosX", 0, true);
	Settings::addSetting<float>("Arrow Counter", "PosY", 0, true);
	Settings::addSetting<float>("Arrow Counter", "size", 1, true);
	Settings::addSetting<float>("Arrow Counter", "Rounding", 0, true);
	Settings::addSetting<float>(getModuleName(), "Opacity", .5, true);
	Settings::addSetting<std::vector<float>>(getModuleName(), "Text Color", std::vector<float>{1, 1, 1, 1}, true);
    Settings::addSetting<std::vector<float>>(getModuleName(), "Background Color", std::vector<float>{0, 0, 0, .5}, true);

    this->SettingsList["COLOR"]["Background Color"] = "colorpicker";
	this->SettingsList["SCALE"]["Opacity"] = "slider-0-1";
	this->SettingsList["COLOR"]["Text Color"] = "colorpicker";
	this->SettingsList["SCALE"]["size"] = "slider-0.5-2.5";
    this->SettingsList["SCALE"]["Rounding"] = "slider-0-1";
}

void ArrowCounter::onEnable() {
	Settings::getSettingByName<bool>("Arrow Counter", "enabled")->value = true;
	eventMgr.addListener(this);
}

void ArrowCounter::onDisable() {
	Settings::getSettingByName<bool>("Arrow Counter", "enabled")->value = false;
	eventMgr.removeListener(this);
}

void ArrowCounter::onRender(const RenderEvent& event) {
    if(!SDK::clientInstance || !SDK::clientInstance->getLocalPlayer()) return;
	if (!(SDK::TopScreen.rfind("hud_screen") != std::string::npos)) return;
	auto inventory = SDK::clientInstance->getLocalPlayer()->getSupplies()->getmContainer();
	int arrows = 0;

	if (inventory != nullptr) {
		for (int i = 0; i < 36; i++) {
			auto item = inventory->getitem(i);
			if (item->getItem() != nullptr) {
				if (item->getItem()->name.find("arrow") != std::string::npos) {
					arrows += item->count;
				}
			}
		}
	}

	this->RenderHUD(std::to_string(arrows) + " Arrows");
}
