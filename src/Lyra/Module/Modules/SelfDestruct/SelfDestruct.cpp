
#include "SelfDestruct.hpp"
#include "../../../Client.hpp"
#include "../../../../Utils/RenderUtils.h"
#include "../ModMenu.hpp"
#include "../../../../Utils/Constraints.hpp"

SelfDestruct::SelfDestruct() : ModuleCtor(0, "SelfDestruct", "selfdestruct") {
    this->InitModuleSettings();
}
bool warning = false;
float warningsvPosY;


std::string SelfDestruct::getModuleName(){
    return "SelfDestruct";
}
void SelfDestruct::onEnable(){
    eventMgr.addListener(this);
    Settings::getSettingByName<bool>(SelfDestruct::getModuleName(), "enabled")->value = false;
    SelfDestruct::setEnabled(false);
    warning = true;
}

void SelfDestruct::onDisable() {

}

void SelfDestruct::InitModuleSettings() {
    Settings::addSetting<bool>(getModuleName(), "enabled", false, true);
}
void Warning() {

    ImColor accent = RenderUtils::VecToImColour(Settings::getSettingByName<std::vector<float>>("Mod Menu", "ACCENT COLOR")->value);
    float speed = Settings::getSettingByName<float>("Mod Menu", "ANIMATION SPEED")->value;
    if(DisableModMenu) warning = false;
    ImVec2 ScreenCenter(Constraints::PercentageConstraint(.5, 2), Constraints::PercentageConstraint(.5));
    ImVec2 size(Constraints::PercentageConstraint(.3, 2), Constraints::PercentageConstraint(.1));
    if (warningsvPosY == NULL) warningsvPosY = -size.y;
    ImVec2 pos(ScreenCenter.x - size.x/2, warningsvPosY);
    warningsvPosY = Utils::animate(ScreenCenter.y - size.y/2, warningsvPosY, MC::Deltatime * .15 * speed);

    RenderUtils::fillRect(pos, size, ImColor(.15f,.15f,.15f,1.f), Constraints::RoundingConstraint(10));

    ImVec2 tbSize(Constraints::PercentageConstraint(.15, 2), Constraints::PercentageConstraint(.05));
    ImVec2 tbPos(pos.x + (size.x-tbSize.x)/6, pos.y + (size.y-tbSize.y)/2);
    RenderUtils::RenderText(tbPos, tbSize, IM_COL32_WHITE, "Your game can crash", 5 * .5 * .3, 2);

    ImVec2 svSize(Constraints::PercentageConstraint(.09, 2), Constraints::PercentageConstraint(.05));
    ImVec2 svPos((pos.x+size.x) - (svSize.x + (size.x-svSize.x)/8), (pos.y+size.y) - (svSize.y + (size.y-svSize.y)/2));

    RenderUtils::fillRect(svPos, svSize, accent, Constraints::RoundingConstraint(10));
    RenderUtils::RenderText(svPos, svSize, IM_COL32_WHITE, "un inject?", 5 * .5 * .3, 2);

    if(ImGui::IsMouseClicked(0) && Utils::IsMouseOverRectangle(Utils::mousepos, svPos, svSize)) {
        Settings::getSettingByName<bool>("SelfDestruct", "enabled")->value = false;
        Client::disable = true;

    }

    ImVec2 cSize(Constraints::PercentageConstraint(.03), Constraints::PercentageConstraint(.03));
    ImVec2 cPos(pos.x+size.x-cSize.x/2, pos.y-cSize.y/2);
    RenderUtils::fillRect(cPos, cSize, accent, Constraints::RoundingConstraint(10));
    RenderUtils::RenderImage(cPos, cSize, "Lyra\\Assets\\cross.png", IM_COL32_WHITE);

    if (ImGui::IsMouseClicked(0) && Utils::IsMouseOverRectangle(Utils::mousepos, cPos, cSize)) {
        currentSetting = "Mod Menu";
        Settings::getSettingByName<bool>("SelfDestruct", "enabled")->value = false;
        warning = false;
    }
}

void SelfDestruct::onRender(const RenderEvent &event) {
    if(warning) Warning();
}
