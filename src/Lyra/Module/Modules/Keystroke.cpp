#include "Keystroke.hpp"
#include "../../SDK/SDK.hpp"
#include "../../../Utils/Constraints.hpp"
#include "../../../Utils/RenderUtils.h"
#include "../ModuleManager.hpp"
#include "MenuSelector/MenuSelector.hpp"

ImColor LMB_Col;
ImColor RMB_Col;
ImColor W_Col;
ImColor A_Col;
ImColor S_Col;
ImColor D_Col;
ImColor Space_Col;

Keystroke::Keystroke() : ModuleCtor(0, "Keystroke", "keystroke") {
    this->InitModuleSettings();
}

std::string Keystroke::getModuleName() {
    return "Keystroke";
}

void Keystroke::InitModuleSettings() {
    Settings::addSetting<bool>(this->getModuleName(), "enabled", false, true);
    Settings::addSetting<float>(this->getModuleName(), "PosX", 0, true);
    Settings::addSetting<float>(this->getModuleName(), "PosY", 0, true);
    Settings::addSetting<float>(this->getModuleName(), "size", 1, true);
	Settings::addSetting<float>("Keystroke", "Rounding", 0, true);
	Settings::addSetting<std::vector<float>>(getModuleName(), "Text Color", std::vector<float>{1, 1, 1, 1}, true);
    Settings::addSetting<std::vector<float>>(getModuleName(), "Inactive BG Color", std::vector<float>{0, 0, 0, .5}, true);
    Settings::addSetting<std::vector<float>>(getModuleName(), "Active BG Color", std::vector<float>{.5, .5, .5, .5}, true);
    Settings::addSetting<bool>(getModuleName(), "Mouse Stroke", false, true);

    this->SettingsList["TOGGLE"]["Mouse Stroke"] = "toggle";
	this->SettingsList["COLOR"]["Inactive BG Color"] = "colorpicker";
    this->SettingsList["COLOR"]["Active BG Color"] = "colorpicker";
	this->SettingsList["COLOR"]["Text Color"] = "colorpicker";
    this->SettingsList["SCALE"]["size"] = "slider-0.5-2.5";
    this->SettingsList["SCALE"]["Rounding"] = "slider-0-1";
}

void Keystroke::onEnable() {
    Settings::getSettingByName<bool>(this->getModuleName(), "enabled")->value = true;
    eventMgr.addListener(this);
}

void Keystroke::onDisable() {
    Settings::getSettingByName<bool>(this->getModuleName(), "enabled")->value = false;
    eventMgr.removeListener(this);
}

void Keystroke::onRender(const RenderEvent &event) {
    if(!SDK::clientInstance || !SDK::clientInstance->getLocalPlayer()) return;
    if (!(SDK::TopScreen.rfind("hud_screen") != std::string::npos)) return;
    float SizeModifier = Settings::getSettingByName<float>(this->getModuleName(), "size")->value;
    std::vector<float> tCol = Settings::getSettingByName<std::vector<float>>(this->getModuleName(), "Text Color")->value;
    ImColor textColor = ImColor(tCol.at(0), tCol.at(1), tCol.at(2), tCol.at(3));
    std::string name = this->getModuleName();
    this->pos = ImVec2(Settings::getSettingByName<float>(this->getModuleName(), "PosX")->value, Settings::getSettingByName<float>(this->getModuleName(), "PosY")->value);
    ImVec2 uiSize = ImGui::GetIO().DisplaySize;
    ImVec2 pos = ImVec2(this->pos.x * uiSize.x, this->pos.y * uiSize.y);
    ImVec2 size = ImVec2(Constraints::PercentageConstraint(0.25 * SizeModifier), Constraints::PercentageConstraint(0.2 * SizeModifier));
    ImVec2 rzPos = ImVec2(pos.x + size.x - 5, pos.y + size.y - 5);
    float Rounding = Settings::getSettingByName<float>(this->getModuleName(), "Rounding")->value;
    std::vector<float> bCol = Settings::getSettingByName<std::vector<float>>(this->getModuleName(), "Inactive BG Color")->value;
    ImColor bgColor = ImColor(bCol.at(0), bCol.at(1), bCol.at(2), bCol.at(3));
    std::vector<float> bACol = Settings::getSettingByName<std::vector<float>>(this->getModuleName(), "Active BG Color")->value;
    ImColor bgAColor = ImColor(bACol.at(0), bACol.at(1), bACol.at(2), bACol.at(3));
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
    ImVec2 Pos_W = ImVec2(pos.x + size.x / 3 + 5, pos.y);
    ImVec2 Pos_A = ImVec2(pos.x + 5, pos.y + size.y / 2.5);
    ImVec2 Pos_S = ImVec2(pos.x + size.x / 3 + 5, pos.y + size.y / 2.5);
    ImVec2 Pos_D = ImVec2(pos.x + size.x / 3 * 2 + 5, pos.y + size.y / 2.5);
    ImVec2 Pos_LMB = ImVec2(pos.x + 5, pos.y);
    ImVec2 Pos_RMB = ImVec2(pos.x + size.x / 3 * 2 + 5, pos.y);
    ImVec2 Size = ImVec2(size.x / 3 - 5, size.y / 2.5 - 5);

    ImVec2 Pos_Space = ImVec2(pos.x + 5, pos.y + size.y / 2.5 * 2);
    ImVec2 Size_Space = ImVec2(size.x - 5, size.y / 3 / 1.65);

    bool LMB = ImGui::IsMouseDown(0);
    bool RMB = ImGui::IsMouseDown(1);
    bool W = false, A = false, S = false, D = false, Space = false;
    if (SDK::clientInstance && SDK::clientInstance->getLocalPlayer()){
        MoveInputComponent *MV = SDK::clientInstance->getLocalPlayer()->getMoveInputHandler();
        W = MV->forward;
        A = MV->left;
        S = MV->backward;
        D = MV->right;
        Space = MV->jumping;
    }

    float rounding = Constraints::RoundingCalc(Size, Rounding);

    bool mstroke = Settings::getSettingByName<bool>(getModuleName(), "Mouse Stroke")->value;

    if(!LMB_Col || !RMB_Col || !W_Col || !A_Col || !S_Col || !D_Col || !Space_Col) LMB_Col = RMB_Col = W_Col = A_Col = S_Col = D_Col = Space_Col = bgColor;

    float speed = Settings::getSettingByName<float>("Mod Menu", "ANIMATION SPEED")->value * MC::Deltatime * .15 * 2;
    LMB_Col = LMB ? Utils::LerpColor(LMB_Col, bgAColor, speed) : Utils::LerpColor(LMB_Col, bgColor, speed);
    RMB_Col = RMB ? Utils::LerpColor(RMB_Col, bgAColor, speed) : Utils::LerpColor(RMB_Col, bgColor, speed);
    W_Col = W ? Utils::LerpColor(W_Col, bgAColor, speed) : Utils::LerpColor(W_Col, bgColor, speed);
    A_Col = A ? Utils::LerpColor(A_Col, bgAColor, speed) : Utils::LerpColor(A_Col, bgColor, speed);
    S_Col = S ? Utils::LerpColor(S_Col, bgAColor, speed) : Utils::LerpColor(S_Col, bgColor, speed);
    D_Col = D ? Utils::LerpColor(D_Col, bgAColor, speed) : Utils::LerpColor(D_Col, bgColor, speed);
    Space_Col = Space ? Utils::LerpColor(Space_Col, bgAColor, speed) : Utils::LerpColor(Space_Col, bgColor, speed);

    if(mstroke){
        RenderUtils::fillRect(Pos_LMB, Size, LMB_Col, rounding);
        RenderUtils::fillRect(Pos_RMB, Size, RMB_Col, rounding);
    }
    RenderUtils::fillRect(Pos_W, Size, W_Col, rounding);
    RenderUtils::fillRect(Pos_A, Size, A_Col, rounding);
    RenderUtils::fillRect(Pos_S, Size, S_Col, rounding);
    RenderUtils::fillRect(Pos_D, Size, D_Col, rounding);
    RenderUtils::fillRect(Pos_Space, Size_Space, Space_Col, Constraints::RoundingCalc(Size_Space, Rounding));

    if (Settings::getSettingByName<bool>("Mod Menu", "TEXT SHADOWS")->value) {
        std::vector<float> ShadowCol = Settings::getSettingByName<std::vector<float>>("Mod Menu", "TEXT SHADOW COLOR")->value;
        ImColor ShadowColor = ImColor(ShadowCol.at(0), ShadowCol.at(1), ShadowCol.at(2), ShadowCol.at(3));
        RenderUtils::RenderText(ImVec2(Pos_W.x + Constraints::PercentageConstraint(0.0025), Pos_W.y + Constraints::PercentageConstraint(0.0025)), Size, ShadowColor, "W", size.y * .015 / 4, 2, ImGui::GetForegroundDrawList(), Settings::getSettingByName<bool>("Mod Menu", "USE MONJANGLES HUD")->value);
        RenderUtils::RenderText(ImVec2(Pos_A.x + Constraints::PercentageConstraint(0.0025), Pos_A.y + Constraints::PercentageConstraint(0.0025)), Size, ShadowColor, "A", size.y * .015 / 4, 2, ImGui::GetForegroundDrawList(), Settings::getSettingByName<bool>("Mod Menu", "USE MONJANGLES HUD")->value);
        RenderUtils::RenderText(ImVec2(Pos_S.x + Constraints::PercentageConstraint(0.0025), Pos_S.y + Constraints::PercentageConstraint(0.0025)), Size, ShadowColor, "S", size.y * .015 / 4, 2, ImGui::GetForegroundDrawList(), Settings::getSettingByName<bool>("Mod Menu", "USE MONJANGLES HUD")->value);
        RenderUtils::RenderText(ImVec2(Pos_D.x + Constraints::PercentageConstraint(0.0025), Pos_D.y + Constraints::PercentageConstraint(0.0025)), Size, ShadowColor, "D", size.y * .015 / 4, 2, ImGui::GetForegroundDrawList(), Settings::getSettingByName<bool>("Mod Menu", "USE MONJANGLES HUD")->value);
        if (mstroke) {
            RenderUtils::RenderText(ImVec2(Pos_LMB.x + Constraints::PercentageConstraint(0.0025), Pos_LMB.y + Constraints::PercentageConstraint(0.0025)), Size, ShadowColor, "LMB", size.y * .015 / 6, 2, ImGui::GetForegroundDrawList(), Settings::getSettingByName<bool>("Mod Menu", "USE MONJANGLES HUD")->value);
            RenderUtils::RenderText(ImVec2(Pos_RMB.x + Constraints::PercentageConstraint(0.0025), Pos_RMB.y + Constraints::PercentageConstraint(0.0025)), Size, ShadowColor, "RMB", size.y * .015 / 6, 2, ImGui::GetForegroundDrawList(), Settings::getSettingByName<bool>("Mod Menu", "USE MONJANGLES HUD")->value);
        }
    }

    if(mstroke){
        RenderUtils::RenderText(Pos_LMB, Size, textColor, "LMB", size.y * .015 / 6, 2, ImGui::GetForegroundDrawList(), Settings::getSettingByName<bool>("Mod Menu", "USE MONJANGLES HUD")->value);
        RenderUtils::RenderText(Pos_RMB, Size, textColor, "RMB", size.y * .015 / 6, 2, ImGui::GetForegroundDrawList(), Settings::getSettingByName<bool>("Mod Menu", "USE MONJANGLES HUD")->value);
    }

    RenderUtils::RenderText(Pos_W, Size, textColor, "W", size.y * .015 / 4, 2, ImGui::GetForegroundDrawList(), Settings::getSettingByName<bool>("Mod Menu", "USE MONJANGLES HUD")->value);
    RenderUtils::RenderText(Pos_A, Size, textColor, "A", size.y * .015 / 4, 2, ImGui::GetForegroundDrawList(), Settings::getSettingByName<bool>("Mod Menu", "USE MONJANGLES HUD")->value);
    RenderUtils::RenderText(Pos_S, Size, textColor, "S", size.y * .015 / 4, 2, ImGui::GetForegroundDrawList(), Settings::getSettingByName<bool>("Mod Menu", "USE MONJANGLES HUD")->value);
    RenderUtils::RenderText(Pos_D, Size, textColor, "D", size.y * .015 / 4, 2, ImGui::GetForegroundDrawList(), Settings::getSettingByName<bool>("Mod Menu", "USE MONJANGLES HUD")->value);
    RenderUtils::fillRect(ImVec2(Pos_Space.x + Size_Space.x/4, Pos_Space.y + Size_Space.y/2 - 2.5), ImVec2(Size_Space.x/4*2, 5), textColor, 20);
    //RenderUtils::RenderText(pos, size, ImColor(255, 255, 255, 255), "W", size.y * 0.015, 2);
    if (Settings::getSettingByName<bool>("Mod Menu", "enabled")->value || menuSelector.enabled) {
        RenderUtils::drawRect(pos, size, IM_COL32_WHITE, 3, 0);
        RenderUtils::fillRect(pos, size, selected ? ImColor(0, 234, 255, 75) : ImColor(120, 120, 120, 60), 0);
        RenderUtils::fillRect(rzPos, ImVec2(10, 10), IM_COL32_WHITE, 0);
    }
}
