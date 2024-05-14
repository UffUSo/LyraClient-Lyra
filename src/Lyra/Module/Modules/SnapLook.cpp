#include "SnapLook.hpp"
#include "../../SDK/SDK.hpp"

bool ShouldSnapLook = false;

SnapLook::SnapLook() : ModuleCtor(67, "Snaplook", "snaplook") {
	this->InitModuleSettings();
}

std::string SnapLook::getModuleName() {
	return "Snaplook";
}

void SnapLook::InitModuleSettings() {
	Settings::addSetting<bool>("Snaplook", "enabled", false, true);
	Settings::addSetting<int>("Snaplook", "KEYBIND", 71, true);

	this->SettingsList["CONTROLS"]["KEYBIND"] = "keybind";
}

void SnapLook::onEnable() {
	Settings::getSettingByName<bool>("Snaplook", "enabled")->value = true;
	eventMgr.addListener(this);
}

void SnapLook::onDisable() {
	Settings::getSettingByName<bool>("Snaplook", "enabled")->value = false;
	eventMgr.removeListener(this);
}

void SnapLook::onKey(KeyEvent& event) {
	if(!SDK::clientInstance || !SDK::clientInstance->getLocalPlayer()) return;
    if (!(SDK::TopScreen.rfind("hud_screen") != std::string::npos)) return;
	if (event.getKey() == Settings::getSettingByName<int>("Snaplook", "KEYBIND")->value and SDK::TopScreen.rfind("hud_screen") != std::string::npos and !Settings::getSettingByName<bool>("Mod Menu", "enabled")->value)
		ShouldSnapLook = event.isPressed();
}