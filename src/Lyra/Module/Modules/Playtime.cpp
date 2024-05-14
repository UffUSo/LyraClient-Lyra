#include "Playtime.hpp"
#include "../ModuleManager.hpp"
#include "../../../Utils/RenderUtils.h"
#include "../../SDK/SDK.hpp"
#include "MenuSelector/MenuSelector.hpp"

std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
Playtime::Playtime() : ModuleCtor(0, "Playtime", "playtime") {
    this->InitModuleSettings();
}

void Playtime::InitModuleSettings() {
    Settings::addSetting<bool>("Playtime", "enabled", false, true);
    Settings::addSetting<float>("Playtime", "PosX", 0, true);
    Settings::addSetting<float>("Playtime", "PosY", 0, true);
    Settings::addSetting<float>("Playtime", "size", 1, true);
    Settings::addSetting<float>("Playtime", "Rounding", 0, true);
    Settings::addSetting<float>(getModuleName(), "Opacity", .5, true);
	Settings::addSetting<std::vector<float>>(getModuleName(), "Text Color", std::vector<float>{1, 1, 1, 1}, true);
    Settings::addSetting<std::vector<float>>(getModuleName(), "Background Color", std::vector<float>{0, 0, 0, .5}, true);

	this->SettingsList["COLOR"]["Background Color"] = "colorpicker";
	this->SettingsList["SCALE"]["Opacity"] = "slider-0-1";
	this->SettingsList["COLOR"]["Text Color"] = "colorpicker";
    this->SettingsList["SCALE"]["size"] = "slider-0.5-2.5";
    this->SettingsList["SCALE"]["Rounding"] = "slider-0-1";
    startTime = std::chrono::high_resolution_clock::now();
}

std::string Playtime::getModuleName() {
    return "Playtime";
}

void Playtime::onEnable() {
    Settings::getSettingByName<bool>("Playtime", "enabled")->value = true;
    eventMgr.addListener(this);
}

void Playtime::onDisable() {
    Settings::getSettingByName<bool>("Playtime", "enabled")->value = false;
    eventMgr.removeListener(this);
}

void Playtime::onRender(const RenderEvent &event) {
    if(!SDK::clientInstance || !SDK::clientInstance->getLocalPlayer()) return;
    if (!(SDK::TopScreen.rfind("hud_screen") != std::string::npos)) return;
    auto currentTime = std::chrono::high_resolution_clock::now();
    auto difference = currentTime - startTime;
    std::string hours = std::to_string(std::chrono::duration_cast<std::chrono::hours>(difference).count());
    std::string mins = std::to_string(std::chrono::duration_cast<std::chrono::minutes>(difference).count() % 60);
    std::string secs = std::to_string(std::chrono::duration_cast<std::chrono::seconds>(difference).count() % 60);

    this->RenderHUD(""+hours+"h, "+mins+"m, "+secs+"s");
}

void Playtime::RenderHUD(std::string text) {
    float SizeModifier = Settings::getSettingByName<float>(this->getModuleName(), "size")->value;
    std::string name = this->getModuleName();
    this->pos = ImVec2(Settings::getSettingByName<float>(this->getModuleName(), "PosX")->value, Settings::getSettingByName<float>(this->getModuleName(), "PosY")->value);
    ImVec2 uiSize = ImGui::GetIO().DisplaySize;
    ImVec2 pos = ImVec2(this->pos.x * uiSize.x, this->pos.y * uiSize.y);
    ImVec2 size = ImVec2(Constraints::PercentageConstraint(0.18 * SizeModifier), Constraints::PercentageConstraint(0.05 * SizeModifier));
    int len = text.size();
    size.x += len * size.y * 0.1;
    float opacity = Settings::getSettingByName<float>(this->getModuleName(), "Opacity")->value;
    std::vector<float> tCol = Settings::getSettingByName<std::vector<float>>(this->getModuleName(), "Text Color")->value;
    ImColor textColor = ImColor(tCol.at(0), tCol.at(1), tCol.at(2), tCol.at(3));
    ImVec2 rzPos = ImVec2(pos.x + size.x - 5, pos.y + size.y - 5);
    std::vector<float> bCol = Settings::getSettingByName<std::vector<float>>(this->getModuleName(), "Background Color")->value;
    ImColor bgColor = ImColor(bCol.at(0), bCol.at(1), bCol.at(2), bCol.at(3));
    Settings::getSettingByName<float>(this->getModuleName(), "Opacity")->value = bCol.at(3);
    Settings::getSettingByName<std::vector<float>>(this->getModuleName(), "Background Color")->value = std::vector<float>{bCol.at(0), bCol.at(1), bCol.at(2), opacity};
     bool isHovered = Utils::IsMouseOverRectangle(Utils::mousepos, pos, size);
     bool isHoveredResize = Utils::IsMouseOverRectangle(Utils::mousepos, rzPos, ImVec2(10, 10));

    if (Settings::getSettingByName<bool>("Mod Menu", "enabled")->value || menuSelector.enabled) {

        if (isHoveredResize && ImGui::IsMouseClicked(0)) {
            resizing = true;
            for (auto it = moduleMgr.getModules().begin(); it != moduleMgr.getModules().end(); ++it) {
                std::shared_ptr<Module> mod = it->second;
                if ((mod->resizing or mod->dragging) and mod->getModuleName() != this->getModuleName()) {
                    resizing = false;
                    break;
                }
            }
            mousePosDiff = Utils::mousepos.x + Utils::mousepos.y;
        }

        if (isHovered && ImGui::IsMouseClicked(0)) {
            this->dragging = true;
            for (auto it = moduleMgr.getModules().begin(); it != moduleMgr.getModules().end(); ++it) {
                std::shared_ptr<Module> mod = it->second;
                if (mod->getModuleName() != this->getModuleName()) {
                    if (mod->resizing or mod->dragging) {
                        dragging = false;
                        selected = false;
                        break;
                    }
                    else {
                        selected = true;
                    }
                }
            }
            diff.x = Utils::mousepos.x - pos.x;
		    diff.y = Utils::mousepos.y - pos.y;
        }

        else if (!isHovered && ImGui::IsMouseClicked(0))
            selected = false;

        if (resizing) {
            selected = true;
            Settings::getSettingByName<float>(this->getModuleName(), "size")->value += Constraints::PosToPercent(((Utils::mousepos.x + Utils::mousepos.y)) - mousePosDiff, 1) * 2;
            if (Settings::getSettingByName<float>(this->getModuleName(), "size")->value > 2.5) {
                Settings::getSettingByName<float>(this->getModuleName(), "size")->value = 2.5;
            }
            else if (Settings::getSettingByName<float>(this->getModuleName(), "size")->value < 0.5) {
                Settings::getSettingByName<float>(this->getModuleName(), "size")->value = 0.5;
            }
            mousePosDiff = Utils::mousepos.x + Utils::mousepos.y;
            dragging = false;
        }

        if (dragging) {

            pos.x = Utils::mousepos.x - diff.x;
		    pos.y = Utils::mousepos.y - diff.y;

            Settings::getSettingByName<float>(name, "PosX")->value = Constraints::PosToPercent(pos.x, 2);
	        Settings::getSettingByName<float>(name, "PosY")->value = Constraints::PosToPercent(pos.y, 1);

	        if (pos.x + size.x > Constraints::PercentageConstraint(1, 2)) {
		        Settings::getSettingByName<float>(name, "PosX")->value = Constraints::PosToPercent(Constraints::PercentageConstraint(1, 2) - size.x, 2);
		        pos.x = Constraints::PercentageConstraint(1, 2) - size.x;
	        }
	        else if (Settings::getSettingByName<float>(name, "PosX")->value < 0) {
		        Settings::getSettingByName<float>(name, "PosX")->value = 0;
		        pos.x = (Constraints::PercentageConstraint(0, 2));
	        }
	        if (pos.y + size.y > Constraints::PercentageConstraint(1, 1)) {
		        Settings::getSettingByName<float>(name, "PosY")->value = Constraints::PosToPercent(Constraints::PercentageConstraint(1, 1) - size.y, 1);
		        pos.y = Constraints::PercentageConstraint(1, 1) - size.y;
	        }
	        else if (Settings::getSettingByName<float>(name, "PosY")->value < 0) {
		        Settings::getSettingByName<float>(name, "PosY")->value = 0;
		        pos.y = (Constraints::PercentageConstraint(0, 1));
	        }
        }

        if (ImGui::IsMouseReleased(0)) {
            dragging = false;
            resizing = false;
        }
    }
    std::vector<float> ShadowCol = Settings::getSettingByName<std::vector<float>>("Mod Menu", "TEXT SHADOW COLOR")->value;
    ImColor ShadowColor = ImColor(ShadowCol.at(0), ShadowCol.at(1), ShadowCol.at(2), ShadowCol.at(3));

    RenderUtils::fillRect(pos, size, bgColor, Constraints::RoundingCalc(size, Settings::getSettingByName<float>(name, "Rounding")->value));
    if(Settings::getSettingByName<bool>("Mod Menu", "TEXT SHADOWS")->value)
        RenderUtils::RenderText(ImVec2(pos.x + Constraints::PercentageConstraint(0.0025), pos.y + Constraints::PercentageConstraint(0.0025)), size, ShadowColor, text, size.y * 0.015, 2, ImGui::GetForegroundDrawList(), Settings::getSettingByName<bool>("Mod Menu", "USE MONJANGLES HUD")->value);
    RenderUtils::RenderText(pos, size, textColor, text, size.y * 0.015, 2, ImGui::GetForegroundDrawList(), Settings::getSettingByName<bool>("Mod Menu", "USE MONJANGLES HUD")->value);
    if (!Settings::getSettingByName<bool>("Mod Menu", "enabled")->value && !menuSelector.enabled) {
        selected = false;
        dragging = false;
        
    }
    else {
        RenderUtils::drawRect(pos, size, IM_COL32_WHITE, 3, 0);
        RenderUtils::fillRect(pos, size, selected ? ImColor(0, 234, 255, 75) : ImColor(120, 120, 120, 60), 0);
        RenderUtils::fillRect(rzPos, ImVec2(10, 10), IM_COL32_WHITE, 0);
    }
}