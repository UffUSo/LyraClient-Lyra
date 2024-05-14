#include "Zoom.hpp"
#include "../../SDK/SDK.hpp"

bool ShouldZoom = false;
float currentZoom = 0.0f;

Zoom::Zoom() : ModuleCtor(67, "optfine zoom", "zoom") {
	this->InitModuleSettings();
}

std::string Zoom::getModuleName() {
	return "Zoom";
}

void Zoom::InitModuleSettings() {
	Settings::addSetting<bool>(this->getModuleName(), "enabled", false, true);
	Settings::addSetting<int>(this->getModuleName(), "KEYBIND", 67, true);
    Settings::addSetting<bool>(this->getModuleName(), "Smooth Zoom", false, true);
    Settings::addSetting<float>(this->getModuleName(), "Target Zoom", 30.0f, true);
    Settings::addSetting<float>(this->getModuleName(), "Animation Speed", 0.2f, true);

	this->SettingsList["CONTROLS"]["KEYBIND"] = "keybind";
    this->SettingsList["OTHER"]["Target Zoom"] = "slider-70.0-1.0";
    this->SettingsList["ANIMATION"]["Animation Speed"] = "slider-0.05-0.5";
    this->SettingsList["ANIMATION"]["Smooth Zoom"] = "toggle";
}

void Zoom::onEnable() {
	Settings::getSettingByName<bool>("Zoom", "enabled")->value = true;
	eventMgr.addListener(this);
}

void Zoom::onDisable() {
	Settings::getSettingByName<bool>("Zoom", "enabled")->value = false;
	eventMgr.removeListener(this);
}

void Zoom::onKey(KeyEvent& event) {
	if(!SDK::clientInstance || !SDK::clientInstance->getLocalPlayer()) return;
    if (!(SDK::TopScreen.rfind("hud_screen") != std::string::npos)) return;
	if (event.getKey() == Settings::getSettingByName<int>("Zoom", "KEYBIND")->value and SDK::TopScreen.rfind("hud_screen") != std::string::npos and !Settings::getSettingByName<bool>("Mod Menu", "enabled")->value) {
		ShouldZoom = event.isPressed();
	}
}