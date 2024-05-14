#include "FoodPoints.hpp"
#include "../../SDK/SDK.hpp"

FoodPoints::FoodPoints() : ModuleCtor(0, "Food Points", "FoodPoints") {
    this->InitModuleSettings();
}

void FoodPoints::InitModuleSettings() {
    Settings::addSetting<bool>(getModuleName(), "enabled", false, true);
	Settings::addSetting<float>(getModuleName(), "PosX", 0, true);
	Settings::addSetting<float>(getModuleName(), "PosY", 0, true);
	Settings::addSetting<float>(getModuleName(), "size", 1, true);
	Settings::addSetting<float>(getModuleName(), "Rounding", 0, true);
	Settings::addSetting<float>(getModuleName(), "Opacity", .5, true);
	Settings::addSetting<std::vector<float>>(getModuleName(), "Text Color", std::vector<float>{1, 1, 1, 1}, true);
	Settings::addSetting<std::vector<float>>(getModuleName(), "Background Color", std::vector<float>{0, 0, 0, .5}, true);

	this->SettingsList["COLOR"]["Background Color"] = "colorpicker";
	this->SettingsList["SCALE"]["Opacity"] = "slider-0-1";
	this->SettingsList["COLOR"]["Text Color"] = "colorpicker";
	this->SettingsList["SCALE"]["size"] = "slider-0.5-2.5";
    this->SettingsList["SCALE"]["Rounding"] = "slider-0-1";

    hungerLVL["apple"] = 4;
	hungerLVL["baked_potato"] = 5;
	hungerLVL["beetroot"] = 1;
	hungerLVL["beetroot_soup"] = 6;
	hungerLVL["bread"] = 5;
	hungerLVL["cake"] = 14;
	hungerLVL["carrot"] = 3;
	hungerLVL["chorus_fruit"] = 4;
	hungerLVL["cooked_chicken"] = 6;
	hungerLVL["cooked_cod"] = 5;
	hungerLVL["cooked_mutton"] = 6;
	hungerLVL["cooked_porkchop"] = 8;
	hungerLVL["cooked_rabbit"] = 5;
	hungerLVL["cooked_salmon"] = 6;
	hungerLVL["cookie"] = 2;
	hungerLVL["dried_kelp"] = 1;
	hungerLVL["enchanted_golden_apple"] = 4;
	hungerLVL["golden_apple"] = 4;
	hungerLVL["glow_berries"] = 2;
	hungerLVL["golden_carrot"] = 6;
	hungerLVL["honey_bottle"] = 6;
	hungerLVL["melon_slice"] = 2;
	hungerLVL["mushroom_stew"] = 6;
	hungerLVL["poisonous_potato"] = 2;
	hungerLVL["potato"] = 1;
	hungerLVL["puffer_fish"] = 1;
	hungerLVL["pumpkin_pie"] = 8;
	hungerLVL["rabbit_stew"] = 10;
	hungerLVL["beef"] = 3;
	hungerLVL["chicken"] = 2;
	hungerLVL["cod"] = 2;
	hungerLVL["mutton"] = 2;
	hungerLVL["porkchop"] = 3;
	hungerLVL["rabbit"] = 3;
	hungerLVL["salmon"] = 2;
	hungerLVL["rotten_flesh"] = 4;
	hungerLVL["spider_eye"] = 2;
	hungerLVL["steak"] = 8;
	hungerLVL["suspicious_stew"] = 6;
	hungerLVL["sweet_berries"] = 2;
	hungerLVL["tropical_fish"] = 1;
}

std::string FoodPoints::getModuleName() {
    return "Food Points";
}

void FoodPoints::onEnable() {
    Settings::getSettingByName<bool>(getModuleName(), "enabled")->value = true;
    eventMgr.addListener(this);
}

void FoodPoints::onDisable() {
    Settings::getSettingByName<bool>(getModuleName(), "enabled")->value = false;
    eventMgr.removeListener(this);
}

void FoodPoints::onRender(const RenderEvent &event) {
	if (!(SDK::TopScreen.rfind("hud_screen") != std::string::npos)) return;
	if(!SDK::clientInstance || !SDK::clientInstance->getLocalPlayer()) return;
	auto selected = SDK::clientInstance->getLocalPlayer()->getSupplies()->getmContainer()->getitem(
							SDK::clientInstance->getLocalPlayer()->getSupplies()->getSelectedSlot()
						)->getItem();
	int point = 0;
	if (selected != nullptr) point = hungerLVL[selected->name];
	this->RenderHUD(std::to_string(point) + " Points");
}