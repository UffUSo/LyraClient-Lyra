#include "CPS_Limiter.hpp"
#include "../../../Utils/RenderUtils.h"
#include "../../SDK/SDK.hpp"

CPS_Limiter::CPS_Limiter() : ModuleCtor(0, "Limit CPS", "CPS_Limiter") {
    this->InitModuleSettings();
}

void CPS_Limiter::InitModuleSettings() {
    Settings::addSetting<bool>(this->getModuleName(), "enabled", false, true);
    Settings::addSetting<int>(this->getModuleName(), "Left Limit", 10, true);
    Settings::addSetting<int>(this->getModuleName(), "Right Limit", 10, true);

    this->SettingsList["SCORE"]["Left Limit"] = "score-1-20";
    this->SettingsList["SCORE"]["Right Limit"] = "score-1-20";
}

std::string CPS_Limiter::getModuleName() {
    return "CPS Limiter";
}

void CPS_Limiter::onEnable() {
    Settings::getSettingByName<bool>(this->getModuleName(), "enabled")->value = true;
    eventMgr.addListener(this);
}

void CPS_Limiter::onDisable() {
    Settings::getSettingByName<bool>(this->getModuleName(), "enabled")->value = false;
    eventMgr.removeListener(this);
}

void CPS_Limiter::onMouse(MouseEvent &event) {
    if(!SDK::clientInstance || !SDK::clientInstance->getLocalPlayer()) return;
    if (!(SDK::TopScreen.rfind("hud_screen") != std::string::npos)) return;

    int l_limit = Settings::getSettingByName<int>(this->getModuleName(), "Left Limit")->value;
    int r_limit = Settings::getSettingByName<int>(this->getModuleName(), "Right Limit")->value;

    long lms = std::chrono::duration_cast<std::chrono::milliseconds>(MC::leftClick[0].ClickTimeStamp - MC::leftClick[1].ClickTimeStamp).count();
    long rms = std::chrono::duration_cast<std::chrono::milliseconds>(MC::rightClick[0].ClickTimeStamp - MC::rightClick[1].ClickTimeStamp).count();
    
    if (lms < 1000/l_limit && event.GetButton() == MouseButton::Left && MC::countLeftCps() >= l_limit) {
        MC::leftClick.erase(MC::leftClick.begin());
        return event.cancel();
    }

    if (rms < 1000/r_limit && event.GetButton() == MouseButton::Right && MC::countRightCps() >= r_limit) {
        MC::rightClick.erase(MC::rightClick.begin());
        return event.cancel();
    }
}