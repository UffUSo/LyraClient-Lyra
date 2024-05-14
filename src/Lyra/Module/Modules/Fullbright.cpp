#include "Fullbright.hpp"
#include "../../SDK/SDK.hpp"

bool ShouldFullbright = false;

Fullbright::Fullbright() : ModuleCtor(67, "Fullbright", "Fullbright") {
	this->InitModuleSettings();
}

std::string Fullbright::getModuleName() {
	return "Fullbright";
}

void Fullbright::InitModuleSettings() {
	Settings::addSetting<bool>("Fullbright", "enabled", false, true);
}

void Fullbright::onEnable() {
	if(!SDK::clientInstance || !SDK::clientInstance->getLocalPlayer()) return;
	Settings::getSettingByName<bool>("Fullbright", "enabled")->value = true;
	eventMgr.addListener(this);
	ShouldFullbright = true;
}

void Fullbright::onDisable() {
	Settings::getSettingByName<bool>("Fullbright", "enabled")->value = false;
	eventMgr.removeListener(this);
	ShouldFullbright = false;
}
