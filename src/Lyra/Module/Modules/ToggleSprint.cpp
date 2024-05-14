#include "ToggleSprint.hpp"
#include "../../../Utils/RenderUtils.h"
#include "../../SDK/SDK.hpp"

ToggleSprint::ToggleSprint() : ModuleCtor(0, "Toggle Sprint", "togglesprint") {
	this->InitModuleSettings();
}

std::string ToggleSprint::getModuleName() {
	return "Toggle Sprint";
}

void ToggleSprint::InitModuleSettings() {
	Settings::addSetting<bool>("Toggle Sprint", "enabled", false, true);
	Settings::addSetting<float>("Toggle Sprint", "PosX", 0, true);
	Settings::addSetting<float>("Toggle Sprint", "PosY", 0, true);
}

void ToggleSprint::onEnable() {
	Settings::getSettingByName<bool>("Toggle Sprint", "enabled")->value = true;
	eventMgr.addListener(this);
}

void ToggleSprint::onDisable() {
	Settings::getSettingByName<bool>("Toggle Sprint", "enabled")->value = false;
	eventMgr.removeListener(this);
}

void ToggleSprint::onTick(const TickEvent& event) {
	if(!SDK::clientInstance || !SDK::clientInstance->getLocalPlayer()) return;
    if (!(SDK::TopScreen.rfind("hud_screen") != std::string::npos)) return;
	SDK::clientInstance->getLocalPlayer()->getMoveInputHandler()->sprinting = true;
}