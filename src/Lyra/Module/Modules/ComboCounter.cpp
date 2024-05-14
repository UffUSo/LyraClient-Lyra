#include "ComboCounter.hpp"
#include "../../../Utils/RenderUtils.h"
#include "../../SDK/SDK.hpp"

int comboCount = 0;
std::chrono::time_point<std::chrono::high_resolution_clock> last_hit = std::chrono::high_resolution_clock::now();

ComboCounter::ComboCounter() : ModuleCtor(0, "Combo Counter", "ComboCounter") {
	this->InitModuleSettings();
}

std::string ComboCounter::getModuleName() {
	return "Combo Counter";
}

void ComboCounter::InitModuleSettings() {
	Settings::addSetting<bool>("Combo Counter", "enabled", false, true);
	Settings::addSetting<float>("Combo Counter", "PosX", 0, true);
	Settings::addSetting<float>("Combo Counter", "PosY", 0, true);
	Settings::addSetting<float>("Combo Counter", "size", 1, true);
	Settings::addSetting<float>("Combo Counter", "Rounding", 0, true);
	Settings::addSetting<float>(getModuleName(), "Opacity", .5, true);
	Settings::addSetting<std::vector<float>>(getModuleName(), "Text Color", std::vector<float>{1, 1, 1, 1}, true);
	Settings::addSetting<std::vector<float>>(getModuleName(), "Background Color", std::vector<float>{0, 0, 0, .5}, true);

	this->SettingsList["COLOR"]["Background Color"] = "colorpicker";
	this->SettingsList["SCALE"]["Opacity"] = "slider-0-1";
	this->SettingsList["COLOR"]["Text Color"] = "colorpicker";
	this->SettingsList["SCALE"]["size"] = "slider-0.5-2.5";
    this->SettingsList["SCALE"]["Rounding"] = "slider-0-1";
}

void ComboCounter::onEnable() {
	Settings::getSettingByName<bool>("Combo Counter", "enabled")->value = true;
	eventMgr.addListener(this);
}

void ComboCounter::onDisable() {
	Settings::getSettingByName<bool>("Combo Counter", "enabled")->value = false;
	eventMgr.removeListener(this);
}

void ComboCounter::onTick(const TickEvent& event) {
	if(!SDK::clientInstance || !SDK::clientInstance->getLocalPlayer()) return;
    if (SDK::clientInstance->getLocalPlayer()->hurtTime != 0)
        comboCount = 0;
    std::chrono::duration<double> duration = std::chrono::high_resolution_clock::now() - last_hit;
    if (duration.count() >= 5) comboCount = 0;
}
void ComboCounter::onAttack(AttackEvent &event) {
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - last_hit).count();
	if (ms < 450) return;
	comboCount++;
	last_hit = std::chrono::high_resolution_clock::now();
}
void ComboCounter::onRender(const RenderEvent& event) {
	if(!SDK::clientInstance || !SDK::clientInstance->getLocalPlayer()) return;
    if (!(SDK::TopScreen.rfind("hud_screen") != std::string::npos)) return;
	this->RenderHUD(std::to_string(comboCount) + "x Combo");
}