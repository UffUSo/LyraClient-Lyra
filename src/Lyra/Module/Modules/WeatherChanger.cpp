#include "WeatherChanger.hpp"
#include "../../../Utils/RenderUtils.h"
#include "../../SDK/SDK.hpp"

WeatherChanger::WeatherChanger() : ModuleCtor(0, "Weather Changer", "WeatherChanger") {
	this->InitModuleSettings();
}

std::string WeatherChanger::getModuleName() {
	return "Weather Changer";
}

void WeatherChanger::InitModuleSettings() {
	Settings::addSetting<bool>("Weather Changer", "enabled", false, true);
	Settings::addSetting<float>("Weather Changer", "rain", 0, true);
	Settings::addSetting<float>("Weather Changer", "lighting", 0, true);
	Settings::addSetting<bool>("Weather Changer", "snow", false, true);

	this->SettingsList["CONTROLS"]["rain"] = "slider-0-10";
    this->SettingsList["CONTROLS"]["lighting"] = "slider-0-10";
	this->SettingsList["CONTROLS"]["snow"] = "toggle";
}

void WeatherChanger::onEnable() {
	Settings::getSettingByName<bool>("Weather Changer", "enabled")->value = true;
	eventMgr.addListener(this);
}

void WeatherChanger::onDisable() {
	Settings::getSettingByName<bool>("Weather Changer", "enabled")->value = false;
	eventMgr.removeListener(this);
}

void WeatherChanger::onTick(const TickEvent& event) {
	if(!SDK::clientInstance || !SDK::clientInstance->getLocalPlayer()) return;
	if (!SDK::clientInstance->getBlockSource())
		return;

	SDK::clientInstance->getBlockSource()->dimension->weather->rainLevel = Settings::getSettingByName<float>("Weather Changer", "rain")->value;
	SDK::clientInstance->getBlockSource()->dimension->weather->lightingLevel = Settings::getSettingByName<float>("Weather Changer", "lighting")->value;

	if (Settings::getSettingByName<bool>("Weather Changer", "snow")->value) {
		Vec3 pos = SDK::clientInstance->getLocalPlayer()->stateVector->Pos;
		pos = Vec3(std::round(pos.x), std::round(pos.y), std::round(pos.z)); 
		SDK::clientInstance->getBlockSource()->getBiome(pos)->temparature = 0.0f;
	}
}