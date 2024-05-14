#include "PotCounter.hpp"
#include "../../../Utils/RenderUtils.h"
#include "../../SDK/SDK.hpp"

PotCounter::PotCounter() : ModuleCtor(0, "PotCounter", "potcounter") {
	this->InitModuleSettings();
}

std::string PotCounter::getModuleName() {
	return "Pot Counter";
}

void PotCounter::InitModuleSettings() {
	Settings::addSetting<bool>("Pot Counter", "enabled", false, true);
	Settings::addSetting<float>("Pot Counter", "PosX", 0, true);
	Settings::addSetting<float>("Pot Counter", "PosY", 0, true);
	Settings::addSetting<float>("Pot Counter", "size", 1, true);
	Settings::addSetting<float>("Pot Counter", "Rounding", 0, true);
	Settings::addSetting<float>(getModuleName(), "Opacity", .5, true);
	Settings::addSetting<std::vector<float>>(getModuleName(), "Text Color", std::vector<float>{1, 1, 1, 1}, true);
    	Settings::addSetting<std::vector<float>>(getModuleName(), "Background Color", std::vector<float>{0, 0, 0, .5}, true);

	this->SettingsList["COLOR"]["Background Color"] = "colorpicker";
	this->SettingsList["SCALE"]["Opacity"] = "slider-0-1";
	this->SettingsList["COLOR"]["Text Color"] = "colorpicker";

	this->SettingsList["SCALE"]["size"] = "slider-0.5-2.5";
    this->SettingsList["SCALE"]["Rounding"] = "slider-0-1";
}

void PotCounter::onEnable() {
	Settings::getSettingByName<bool>("Pot Counter", "enabled")->value = true;
	eventMgr.addListener(this);
}

void PotCounter::onDisable() {
	Settings::getSettingByName<bool>("Pot Counter", "enabled")->value = false;
	eventMgr.removeListener(this);
}

void PotCounter::onRender(const RenderEvent& event) {
    if(!SDK::clientInstance || !SDK::clientInstance->getLocalPlayer()) return;
    if (!(SDK::TopScreen.rfind("hud_screen") != std::string::npos)) return;
	auto inventory = SDK::clientInstance->getLocalPlayer()->getSupplies()->getmContainer();
	int pots = 0;

	if (inventory != nullptr) {
		for (int i = 0; i < 36; i++) {
			auto item = inventory->getitem(i);
			if (item->getItem() != nullptr) {
				if (item->getItem()->name == "splash_potion") {
					pots++;
				}
			}
		}
	}

	this->RenderHUD(std::to_string(pots) + " Pots");
}
