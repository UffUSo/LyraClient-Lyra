#include "FogColour.hpp"
#include "../../Event/Events/GetFogColorEvent.hpp"
#include "../../SDK/SDK.hpp"

FogColour::FogColour() : ModuleCtor(0, "Fog Color", "fog color") {
	this->InitModuleSettings();
}
std::string FogColour::getModuleName() {
	return "Fog Color";
}
void FogColour::onEnable() {
	Settings::getSettingByName<bool>("Fog Color", "enabled")->value = true;
	eventMgr.addListener(this);
}

void FogColour::onDisable() {
	Settings::getSettingByName<bool>("Fog Color", "enabled")->value = false;
	eventMgr.removeListener(this);
}

void FogColour::onGetFogColorEvent(GetFogColorEvent& e) {
	if(!SDK::clientInstance || !SDK::clientInstance->getLocalPlayer()) return;
	std::vector<float> Color = Settings::getSettingByName<std::vector<float>>("Fog Color", "Fog Color")->value;

	e.setColour(mce::Color(Color.at(0), Color.at(1), Color.at(2), Color.at(3)));
}


void FogColour::InitModuleSettings() {
	Settings::addSetting<bool>("Fog Color", "enabled", false, true);
	Settings::addSetting<std::vector<float>>("Fog Color", "Fog Color", {1.0f, 1.0f, 1.0f, 0.65f}, true);

    this->SettingsList["COLOR"]["Fog Color"] = "colorpicker";
}
