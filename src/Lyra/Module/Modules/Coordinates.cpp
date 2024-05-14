#include "Coordinates.hpp"

#include "../../SDK/SDK.hpp"

Coordinates::Coordinates() : ModuleCtor(0, "Coordinates", "coordinates") {
    this->InitModuleSettings();
}

std::string Coordinates::getModuleName() {
    return "Coordinates";
}

void Coordinates::InitModuleSettings() {
    Settings::addSetting<bool>(this->getModuleName(), "enabled", false, true);
    Settings::addSetting<float>(this->getModuleName(), "PosX", 0, true);
    Settings::addSetting<float>(this->getModuleName(), "PosY", 0, true);
    Settings::addSetting<float>(this->getModuleName(), "size", 1, true);
    Settings::addSetting<float>(this->getModuleName(), "Rounding", 0, true);
    Settings::addSetting<float>(getModuleName(), "Opacity", .5, true);
	Settings::addSetting<std::vector<float>>(getModuleName(), "Text Color", std::vector<float>{1, 1, 1, 1}, true);
    Settings::addSetting<std::vector<float>>(getModuleName(), "Background Color", std::vector<float>{0, 0, 0, .5}, true);

	this->SettingsList["COLOR"]["Background Color"] = "colorpicker";
	this->SettingsList["SCALE"]["Opacity"] = "slider-0-1";
	this->SettingsList["COLOR"]["Text Color"] = "colorpicker";
    this->SettingsList["SCALE"]["size"] = "slider-0.5-2.5";
    this->SettingsList["SCALE"]["Rounding"] = "slider-0-1";
}

void Coordinates::onEnable() {
    eventMgr.addListener(this);
}

void Coordinates::onDisable() {
    eventMgr.removeListener(this);
}

void Coordinates::onRender(const RenderEvent &event) {
    if(!SDK::clientInstance || !SDK::clientInstance->getLocalPlayer()) return;
    if (!(SDK::TopScreen.rfind("hud_screen") != std::string::npos)) return;
    if (SDK::clientInstance->getLocalPlayer() != nullptr) {
        if (SDK::clientInstance->getLocalPlayer()->getAABBShapeComponent() != nullptr) {
            auto Pos = SDK::clientInstance->getLocalPlayer()->getAABBShapeComponent()->aabb.lower;

            std::string cords = std::to_string(static_cast<int>(Pos.x)) + ", " +
                                std::to_string(static_cast<int>(Pos.y)) + ", " +
                                std::to_string(static_cast<int>(Pos.z));
            this->RenderHUD(cords);
        }
    }
}
