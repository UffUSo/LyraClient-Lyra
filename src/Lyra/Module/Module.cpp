#include "Module.hpp"
#include "../Client.hpp"
#include "../../Utils/RenderUtils.h"
#include "ModuleManager.hpp"
#include "../SDK/SDK.hpp"
#include "Modules/MenuSelector/MenuSelector.hpp"

Module::Module(const unsigned char key, std::string description, std::string icon) : key(key), description(std::move(description)), icon(icon) {}

std::string Module::getModuleName() {
    return "Module";
}

std::string Module::getRawModuleName() {
    return this->getModuleName();
}

std::string Module::getDescription() {
    return this->description;
}

void Module::onEnable() {}

void Module::onDisable() {}

void Module::setEnabled(const bool enabled) {
    this->enabled = enabled;

    if (enabled) {

        this->onEnable();
    }
    else {
        this->onDisable();
    }
}

void Module::toggle() {
    this->setEnabled(!this->enabled);
}

bool Module::isEnabled() {
    return this->enabled;
}

template <typename T>
SettingType<T>* Module::getSetting(std::string setting_name) {
    return Settings::getSettingByName<T>(this->getModuleName(), setting_name);
}

template <typename T>
void Module::addSetting(std::string setting_name, T defaultVaule, bool check) {
    return Settings::addSetting<T>(this->getModuleName(), setting_name, defaultVaule, check);
}

void Module::InitModuleSettings() { }

void Module::RenderHUD(std::string text) {
    if (!(SDK::TopScreen.rfind("hud_screen") != std::string::npos)) {
        return;
    }
    float SizeModifier = Settings::getSettingByName<float>(this->getModuleName(), "size")->value;
    std::string name = this->getModuleName();
    this->pos = ImVec2(Settings::getSettingByName<float>(this->getModuleName(), "PosX")->value, Settings::getSettingByName<float>(this->getModuleName(), "PosY")->value);
    ImVec2 uiSize = ImGui::GetIO().DisplaySize;
    ImVec2 pos = ImVec2(this->pos.x * uiSize.x, this->pos.y * uiSize.y);
    ImVec2 size = ImVec2(Constraints::PercentageConstraint(0.18 * SizeModifier), Constraints::PercentageConstraint(0.05 * SizeModifier));
    ImVec2 rzPos = ImVec2(pos.x + size.x - 5, pos.y + size.y - 5);
    float opacity = Settings::getSettingByName<float>(this->getModuleName(), "Opacity")->value;
    std::vector<float> tCol = Settings::getSettingByName<std::vector<float>>(this->getModuleName(), "Text Color")->value;
    ImColor textColor = ImColor(tCol.at(0), tCol.at(1), tCol.at(2), tCol.at(3));
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
    if (!(Settings::getSettingByName<bool>("Mod Menu", "enabled")->value || menuSelector.enabled)) {
        selected = false;
        dragging = false;
        
    }
    else {
        RenderUtils::drawRect(pos, size, IM_COL32_WHITE, 3, 0);
        RenderUtils::fillRect(pos, size, selected ? ImColor(0, 234, 255, 75) : ImColor(120, 120, 120, 60), 0);
        RenderUtils::fillRect(rzPos, ImVec2(10, 10), IM_COL32_WHITE, 0);
    }
}