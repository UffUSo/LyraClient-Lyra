#include "ArmorStats.hpp"
#include "../../SDK/SDK.hpp"
#include "../../SDK/Client/Render/BaseActorRenderContext.hpp"
#include "../../SDK/Client/Render/ItemRenderer.hpp"
#include "../../SDK/Client/Render/MinecraftUIRenderContext.hpp"
#include "../../../Utils/RenderUtils.h"
#include "../ModuleManager.hpp"

ArmorStats::ArmorStats() : ModuleCtor(0, "Armor status", "ArmourHUD") {
    this->InitModuleSettings();
}

void ArmorStats::InitModuleSettings() {
    Settings::addSetting<bool>("Armor HUD", "enabled", false, true);
    Settings::addSetting<float>("Armor HUD", "PosX", 0, true);
	Settings::addSetting<float>("Armor HUD", "PosY", 0, true);
	Settings::addSetting<float>("Armor HUD", "size", 1, true);
	Settings::addSetting<float>("Armor HUD", "Rounding", 0, true);
	Settings::addSetting<float>(getModuleName(), "Opacity", .5, true);
	Settings::addSetting<std::vector<float>>(getModuleName(), "Text Color", std::vector<float>{1, 1, 1, 1}, true);
    Settings::addSetting<std::vector<float>>(getModuleName(), "Background Color", std::vector<float>{0, 0, 0, .5}, true);

	this->SettingsList["COLOR"]["Background Color"] = "colorpicker";
	this->SettingsList["SCALE"]["Opacity"] = "slider-0-1";
	this->SettingsList["COLOR"]["Text Color"] = "colorpicker";
	this->SettingsList["SCALE"]["size"] = "slider-0.5-2.5";
    this->SettingsList["SCALE"]["Rounding"] = "slider-0-1";
}

std::string ArmorStats::getModuleName() {
    return "Armor HUD";
}

void ArmorStats::onEnable() {
    Settings::getSettingByName<bool>("Armor HUD", "enabled")->value = true;
    eventMgr.addListener(this);
}

void ArmorStats::onDisable() {
    Settings::getSettingByName<bool>("Armor HUD", "enabled")->value = false;
    eventMgr.removeListener(this);
}

// TODO: Fix item texture position
Vec2 currentPos = Vec2();
Vec2 Convert() {

    auto e = SDK::clientInstance->guiData;
    Vec2 xd = Vec2(e->ScreenSize.x, e->ScreenSize.y);
    Vec2 LOL = Vec2(e->ScreenSizeScaled.x, e->ScreenSizeScaled.y);

    return Vec2(currentPos.x * (LOL.x / xd.x), currentPos.y * (LOL.y / xd.y));
}
void ArmorStats::onPostMCRender(const SetupAndRenderEvent &event) {
    return;
    if (!(SDK::TopScreen.rfind("hud_screen") != std::string::npos)) return;

    auto muirc = event.getRenderContext();
    BaseActorRenderContext barc(muirc->screenContext, muirc->clientInstance, muirc->clientInstance->mcgame);
    float SizeModifier = Settings::getSettingByName<float>(this->getModuleName(), "size")->value;
    float s = Constraints::PercentageConstraint(0.04) * SizeModifier;
    float spacing = Constraints::PercentageConstraint(0.0135) * SizeModifier;
    auto e = SDK::clientInstance->guiData;
    Vec2 convert = Convert();
    convert.x = this->pos.x * 640;
    convert.y = this->pos.y * 650;

    if (SDK::clientInstance->getLocalPlayer()->Supplies != nullptr) {
        auto SelectedItem = SDK::clientInstance->getLocalPlayer()->Supplies->mContainer->getitem(SDK::clientInstance->getLocalPlayer()->Supplies->SelectedSlot);
        if (SelectedItem->getItem())
            barc.itemRenderer->renderGuiItemNew(&barc,SelectedItem, 0, convert.x, convert.y, 1.0f, SizeModifier, false);

        spacing = 15 * SizeModifier;

        float xmodifier = 0.0f;
        float ymodifier = 0.0f;

        for (int i = 0; i < 4; i++) {

            if (true) ymodifier += spacing;
            else xmodifier += spacing;

            if (SDK::clientInstance->getLocalPlayer()->getArmor(i)->getItem() != nullptr) {
                barc.itemRenderer->renderGuiItemNew(&barc,
                    SDK::clientInstance->getLocalPlayer()->getArmor(i),
                    0,
                    convert.x + xmodifier, convert.y + ymodifier, 1.0f, SizeModifier, false);
            }
        }
    }
}

void ArmorStats::onRender(const RenderEvent &event) {
    if(!SDK::clientInstance || !SDK::clientInstance->getLocalPlayer()) return;
    if (!(SDK::TopScreen.rfind("hud_screen") != std::string::npos)) return;
	auto helmet = SDK::clientInstance->getLocalPlayer()->getArmor(0);
    auto chest = SDK::clientInstance->getLocalPlayer()->getArmor(1);
    auto legs = SDK::clientInstance->getLocalPlayer()->getArmor(2);
    auto boots = SDK::clientInstance->getLocalPlayer()->getArmor(3);
    
    int hMax = helmet->getItem() != nullptr ? helmet->getMaxDamage() : 1;
    int cMax = chest->getItem() != nullptr ? chest->getMaxDamage() : 1;
    int lMax = legs->getItem() != nullptr ? legs->getMaxDamage() : 1;
    int bMax = boots->getItem() != nullptr ? boots->getMaxDamage() : 1;

    int hDura = helmet->getItem() != nullptr ? hMax - helmet->getDamageValue() : 0;
    int cDura = chest->getItem() != nullptr ? cMax - chest->getDamageValue() : 0;
    int lDura = legs->getItem() != nullptr ? lMax - legs->getDamageValue() : 0;
    int bDura = boots->getItem() != nullptr ? bMax - boots->getDamageValue() : 0;


    float SizeModifier = Settings::getSettingByName<float>(this->getModuleName(), "size")->value;
    float opacity = Settings::getSettingByName<float>(this->getModuleName(), "Opacity")->value;
    std::vector<float> tCol = Settings::getSettingByName<std::vector<float>>(this->getModuleName(), "Text Color")->value;
    ImColor textColor = ImColor(tCol.at(0), tCol.at(1), tCol.at(2), tCol.at(3));
    std::string name = this->getModuleName();
    this->pos = ImVec2(Settings::getSettingByName<float>(this->getModuleName(), "PosX")->value, Settings::getSettingByName<float>(this->getModuleName(), "PosY")->value);
    ImVec2 uiSize = ImGui::GetIO().DisplaySize;
    ImVec2 pos = ImVec2(this->pos.x * uiSize.x, this->pos.y * uiSize.y);
    ImVec2 size = ImVec2(Constraints::PercentageConstraint(0.2 * SizeModifier), Constraints::PercentageConstraint(0.3 * SizeModifier));
    ImVec2 rzPos = ImVec2(pos.x + size.x - 5, pos.y + size.y - 5);
    float Rounding = Settings::getSettingByName<float>(this->getModuleName(), "Rounding")->value;
    std::vector<float> bCol = Settings::getSettingByName<std::vector<float>>(this->getModuleName(), "Background Color")->value;
    ImColor bgColor = ImColor(bCol.at(0), bCol.at(1), bCol.at(2), bCol.at(3));
    Settings::getSettingByName<float>(this->getModuleName(), "Opacity")->value = bCol.at(3);
    Settings::getSettingByName<std::vector<float>>(this->getModuleName(), "Background Color")->value = std::vector<float>{bCol.at(0), bCol.at(1), bCol.at(2), opacity};
     bool isHovered = Utils::IsMouseOverRectangle(Utils::mousepos, pos, size);
     bool isHoveredResize = Utils::IsMouseOverRectangle(Utils::mousepos, rzPos, ImVec2(10, 10));

    if (Settings::getSettingByName<bool>("Mod Menu", "enabled")->value) {

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
    ImVec2 Pos_H = ImVec2(pos.x, pos.y);
    ImVec2 Pos_C = ImVec2(pos.x, pos.y + size.y / 4);
    ImVec2 Pos_L = ImVec2(pos.x, pos.y + size.y / 4 * 2);
    ImVec2 Pos_B = ImVec2(pos.x, pos.y + size.y / 4 * 3);
    ImVec2 Size = ImVec2(size.x, size.y/4);

    RenderUtils::fillRect(Pos_H, Size, bgColor, Rounding);
    RenderUtils::fillRect(Pos_C, Size, bgColor, Rounding);
    RenderUtils::fillRect(Pos_L, Size, bgColor, Rounding);
    RenderUtils::fillRect(Pos_B, Size, bgColor, Rounding);

    RenderUtils::RenderText(Pos_H, Size, textColor, std::to_string(hDura)+"/"+std::to_string(hMax), size.y/4 * .015 *.75, 3);
    RenderUtils::RenderText(Pos_C, Size, textColor, std::to_string(cDura)+"/"+std::to_string(cMax), size.y/4 * .015 *.75, 3);
    RenderUtils::RenderText(Pos_L, Size, textColor, std::to_string(lDura)+"/"+std::to_string(lMax), size.y/4 * .015 *.75, 3);
    RenderUtils::RenderText(Pos_B, Size, textColor, std::to_string(bDura)+"/"+std::to_string(bMax), size.y/4 * .015 *.75, 3);

    //RenderUtils::RenderText(pos, size, ImColor(255, 255, 255, 255), "W", size.y * 0.015, 2);
    if (Settings::getSettingByName<bool>("Mod Menu", "enabled")->value) {
        RenderUtils::drawRect(pos, size, IM_COL32_WHITE, 3, 0);
        RenderUtils::fillRect(pos, size, selected ? ImColor(0, 234, 255, 75) : ImColor(120, 120, 120, 60), 0);
        RenderUtils::fillRect(rzPos, ImVec2(10, 10), IM_COL32_WHITE, 0);
    }
}
