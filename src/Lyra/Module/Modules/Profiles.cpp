#include "Profiles.hpp"
#include "ModMenu.hpp"
#include "../../../Utils/RenderUtils.h"
#include "../../SDK/SDK.hpp"
#include "MenuSelector/MenuSelector.hpp"

bool SaveUI, LoadUI, DelUI, ReplaceUI;
std::string name = "";
std::string profile = "";
int sfiles = 0;
float svPosY;
float ldPosY;
float delPosY;

void RenderReplaceUI() {
    ImColor accent = RenderUtils::VecToImColour(Settings::getSettingByName<std::vector<float>>("Mod Menu", "ACCENT COLOR")->value);
    if(DisableModMenu) ReplaceUI = false;
    ImVec2 ScreenCenter(Constraints::PercentageConstraint(.5, 2), Constraints::PercentageConstraint(.5));
    ImVec2 size(Constraints::PercentageConstraint(.3, 2), Constraints::PercentageConstraint(.2));
    ImVec2 pos(ScreenCenter.x - size.x/2, ScreenCenter.y - size.y/2);

    RenderUtils::fillRect(pos, size, ImColor(.15f,.15f,.15f,1.f), Constraints::RoundingConstraint(10));
    
    ImVec2 lbSize(Constraints::PercentageConstraint(.2, 2), Constraints::PercentageConstraint(.05));
    ImVec2 lbPos(pos.x+(size.x-lbSize.x)/2, pos.y+(size.y-lbSize.y*2)/4);

    RenderUtils::RenderText(lbPos, lbSize, IM_COL32_WHITE, "File already exists. Replace config file?", 5 * .5 * .25, 2);

    ImVec2 Size(Constraints::PercentageConstraint(.09, 2), Constraints::PercentageConstraint(.05));
    ImVec2 pos1(pos.x+(size.x-Size.x*2)/4, pos.y+size.y-Size.y-(size.y)/8);
    ImVec2 pos2(pos.x+size.x-Size.x-size.x/8, pos1.y);

    RenderUtils::fillRect(pos1, Size, accent, Constraints::RoundingConstraint(10));
    RenderUtils::fillRect(pos2, Size, accent, Constraints::RoundingConstraint(10));

    RenderUtils::RenderText(pos1, Size, IM_COL32_WHITE, "YES", 5 * .5 * .3, 2);
    RenderUtils::RenderText(pos2, Size, IM_COL32_WHITE, "NO", 5 * .5 * .3, 2);

    if(ImGui::IsMouseClicked(0) && (Utils::IsMouseOverRectangle(Utils::mousepos, pos1, Size) || Utils::IsMouseOverRectangle(Utils::mousepos, pos2, Size))) {
        if(Utils::IsMouseOverRectangle(Utils::mousepos, pos1, Size)) Settings::SaveSettings(profile);
        name = "";
        currentSetting = "Profiles";
        profile = "";
        ReplaceUI = false;
    }
    
    ImVec2 cSize(Constraints::PercentageConstraint(.03), Constraints::PercentageConstraint(.03));
    ImVec2 cPos(pos.x+size.x-cSize.x/2, pos.y-cSize.y/2);
    RenderUtils::fillRect(cPos, cSize, accent, Constraints::RoundingConstraint(10));
    RenderUtils::RenderImage(cPos, cSize, "Lyra\\Assets\\cross.png", IM_COL32_WHITE);

    if (ImGui::IsMouseClicked(0) && Utils::IsMouseOverRectangle(Utils::mousepos, cPos, cSize)) {
        name = "";
        currentSetting = "Profiles";
        profile = "";
        ReplaceUI = false;
    }
}

void RenderSaveUI() {
    ImColor accent = RenderUtils::VecToImColour(Settings::getSettingByName<std::vector<float>>("Mod Menu", "ACCENT COLOR")->value);
    float speed = Settings::getSettingByName<float>("Mod Menu", "ANIMATION SPEED")->value;
    if(DisableModMenu) SaveUI = false;
    ImVec2 ScreenCenter(Constraints::PercentageConstraint(.5, 2), Constraints::PercentageConstraint(.5));
    ImVec2 size(Constraints::PercentageConstraint(.3, 2), Constraints::PercentageConstraint(.1));
    if (svPosY == NULL) svPosY = -size.y;
    ImVec2 pos(ScreenCenter.x - size.x/2, svPosY);
    svPosY = Utils::animate(ScreenCenter.y - size.y/2, svPosY, MC::Deltatime * .15 * speed);

    RenderUtils::fillRect(pos, size, ImColor(.15f,.15f,.15f,1.f), Constraints::RoundingConstraint(10));
    
    ImVec2 tbSize(Constraints::PercentageConstraint(.15, 2), Constraints::PercentageConstraint(.05));
    ImVec2 tbPos(pos.x + (size.x-tbSize.x)/6, pos.y + (size.y-tbSize.y)/2);
    if(name == "" && !RenderUtils::TextIndex[5]) name = "config";
    if(ImGui::IsMouseClicked(0) && Utils::IsMouseOverRectangle(Utils::mousepos, tbPos, tbSize) && !RenderUtils::TextIndex[5]) name = "";
    RenderUtils::textBox(&name, tbPos, tbSize, IM_COL32_BLACK_TRANS, 5);
    RenderUtils::drawRect(tbPos, tbSize, IM_COL32_WHITE, Constraints::RoundingConstraint(10), 1);
    
    ImVec2 svSize(Constraints::PercentageConstraint(.09, 2), Constraints::PercentageConstraint(.05));
    ImVec2 svPos((pos.x+size.x) - (svSize.x + (size.x-svSize.x)/8), (pos.y+size.y) - (svSize.y + (size.y-svSize.y)/2));

    RenderUtils::fillRect(svPos, svSize, accent, Constraints::RoundingConstraint(10));
    RenderUtils::RenderText(svPos, svSize, IM_COL32_WHITE, "Save", 5 * .5 * .3, 2);

    if(ImGui::IsMouseClicked(0) && Utils::IsMouseOverRectangle(Utils::mousepos, svPos, svSize)) {
        profile = name.size() >= 5 && name.substr(name.size() - 5) == ".json" ? name.substr(0, name.size() - 5) : name;
        if (!std::filesystem::exists(Utils::getRoamingPath() + "\\Lyra\\Config\\"+profile+".json")) Settings::SaveSettings(profile);
        else {ReplaceUI = true;}
        SaveUI = false;
        name = "";
        currentSetting = "Profiles";
        sfiles = 0;
        svPosY = NULL;
    }

    ImVec2 cSize(Constraints::PercentageConstraint(.03), Constraints::PercentageConstraint(.03));
    ImVec2 cPos(pos.x+size.x-cSize.x/2, pos.y-cSize.y/2);
    RenderUtils::fillRect(cPos, cSize, accent, Constraints::RoundingConstraint(10));
    RenderUtils::RenderImage(cPos, cSize, "Lyra\\Assets\\cross.png", IM_COL32_WHITE);

    if (ImGui::IsMouseClicked(0) && Utils::IsMouseOverRectangle(Utils::mousepos, cPos, cSize)) {
        name = "";
        currentSetting = "Profiles";
        profile = "";
        sfiles = 0;
        SaveUI = false;
        svPosY = NULL;
    }
}
void RenderLoadUI() {
    ImColor accent = RenderUtils::VecToImColour(Settings::getSettingByName<std::vector<float>>("Mod Menu", "ACCENT COLOR")->value);
    if(DisableModMenu && !menuSelector.enabled) LoadUI = false;
    float speed = Settings::getSettingByName<float>("Mod Menu", "ANIMATION SPEED")->value;
    ImVec2 ScreenCenter(Constraints::PercentageConstraint(.5, 2), Constraints::PercentageConstraint(.5));
    ImVec2 size(Constraints::PercentageConstraint(.35, 2), Constraints::PercentageConstraint(.8));
    if (ldPosY == NULL) ldPosY = -size.y;
    ImVec2 pos(ScreenCenter.x - size.x/2, ldPosY);
    ldPosY = Utils::animate(ScreenCenter.y - size.y/2, ldPosY, MC::Deltatime * .15 * speed);

    RenderUtils::fillRect(pos, size, ImColor(.15f,.15f,.15f, 1.f), Constraints::RoundingConstraint(10));
    
    std::string path = getenv("AppData") + (std::string)"\\..\\Local\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe\\RoamingState\\Lyra\\Config";

    ImVec2 tbSize(size.x - 20, Constraints::PercentageConstraint(.05));
    ImVec2 tbPos(pos.x + 10, pos.y+size.y-tbSize.y-10);
    RenderUtils::textBox(&name, tbPos, tbSize, IM_COL32_BLACK_TRANS, 6);
    RenderUtils::drawRect(tbPos, tbSize, IM_COL32_WHITE, Constraints::RoundingConstraint(10), 1);

    ImVec2 cellSize(size.x/1.5, (size.y-tbSize.y)/10);
    ImVec2 cellPos(pos.x+Constraints::PercentageConstraint(.01, 2), pos.y);

    std::vector<std::string> files;

    for (const auto& file : std::filesystem::directory_iterator(path)) {
        if (file.is_regular_file() && file.path().filename().string().find(name) != std::string::npos && file.path().filename().string() != "current_config.json") {
            files.push_back(file.path().filename().string());
        }
    }
    int tf = files.size();
    int efiles = std::min(tf - 1, sfiles + 9);
    if (sfiles+efiles >= tf) sfiles = tf - efiles - 1;
    for (int i = sfiles; i < tf && i <= efiles; i++) {
        RenderUtils::RenderText(cellPos, ImVec2(Constraints::PercentageConstraint(.15, 2), cellSize.y), IM_COL32_WHITE, files[i], 5 * .5 * .25);
        ImVec2 lPos(cellPos.x+cellSize.x, cellPos.y+cellSize.y/4);
        ImVec2 lSize(Constraints::PercentageConstraint(.09, 2), cellSize.y/2);
        RenderUtils::fillRect(lPos, lSize, accent, Constraints::RoundingConstraint(10));
        RenderUtils::RenderText(lPos, lSize, IM_COL32_WHITE, "Load", 5 * .5 * .3, 2);

        if(ImGui::IsMouseClicked(0) && Utils::IsMouseOverRectangle(Utils::mousepos, lPos, lSize)) {
            Settings::load(files[i].substr(0, files[i].size() - 5));
            Settings::SaveSettings();
            name = "";
            sfiles = 0;
            LoadUI = false;
            ldPosY = NULL;
            menuSelector.hideUI = false;
        }
        cellPos.y += cellSize.y;
    }

    ImVec2 cSize(Constraints::PercentageConstraint(.03), Constraints::PercentageConstraint(.03));
    ImVec2 cPos(pos.x+size.x-cSize.x/2, pos.y-cSize.y/2);
    RenderUtils::fillRect(cPos, cSize, accent, Constraints::RoundingConstraint(10));
    RenderUtils::RenderImage(cPos, cSize, "Lyra\\Assets\\cross.png", IM_COL32_WHITE);

    if (ImGui::IsMouseClicked(0) && Utils::IsMouseOverRectangle(Utils::mousepos, cPos, cSize)) {
        currentSetting = "Profiles";
        if (menuSelector.enabled) currentSetting = "";
        LoadUI = false;
        name = "";
        sfiles = 0;
        ldPosY = NULL;
        menuSelector.hideUI = false;
    }

}
void RenderDelUI() {
    ImColor accent = RenderUtils::VecToImColour(Settings::getSettingByName<std::vector<float>>("Mod Menu", "ACCENT COLOR")->value);
    if(DisableModMenu) DelUI = false;
    float speed = Settings::getSettingByName<float>("Mod Menu", "ANIMATION SPEED")->value;
    ImVec2 ScreenCenter(Constraints::PercentageConstraint(.5, 2), Constraints::PercentageConstraint(.5));
    ImVec2 size(Constraints::PercentageConstraint(.35, 2), Constraints::PercentageConstraint(.8));
    if (delPosY == NULL) delPosY = -size.y;
    ImVec2 pos(ScreenCenter.x - size.x/2, delPosY);
    delPosY = Utils::animate(ScreenCenter.y - size.y/2, delPosY, MC::Deltatime * .15 * speed);

    RenderUtils::fillRect(pos, size, ImColor(.15f,.15f,.15f, 1.f), Constraints::RoundingConstraint(10));
    
    std::string path = getenv("AppData") + (std::string)"\\..\\Local\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe\\RoamingState\\Lyra\\Config";

    ImVec2 tbSize(size.x - 20, Constraints::PercentageConstraint(.05));
    ImVec2 tbPos(pos.x + 10, pos.y+size.y-tbSize.y-10);
    RenderUtils::textBox(&name, tbPos, tbSize, IM_COL32_BLACK_TRANS, 6);
    RenderUtils::drawRect(tbPos, tbSize, IM_COL32_WHITE, Constraints::RoundingConstraint(10), 1);

    ImVec2 cellSize(size.x/1.5, (size.y-tbSize.y)/10);
    ImVec2 cellPos(pos.x+Constraints::PercentageConstraint(.01, 2), pos.y);

    std::vector<std::string> files;

    for (const auto& file : std::filesystem::directory_iterator(path)) {
        if (file.is_regular_file() && file.path().filename().string().find(name) != std::string::npos && file.path().filename().string() != "current_config.json") {
            files.push_back(file.path().filename().string());
        }
    }
    int tf = files.size();
    int efiles = std::min(tf - 1, sfiles + 9);
    if (sfiles+efiles >= tf) sfiles = tf - efiles - 1;
    for (int i = sfiles; i < tf && i <= efiles; i++) {
        RenderUtils::RenderText(cellPos, ImVec2(Constraints::PercentageConstraint(.15, 2), cellSize.y), IM_COL32_WHITE, files[i], 5 * .5 * .25);
        ImVec2 lPos(cellPos.x+cellSize.x, cellPos.y+cellSize.y/4);
        ImVec2 lSize(Constraints::PercentageConstraint(.09, 2), cellSize.y/2);
        RenderUtils::fillRect(lPos, lSize, accent, Constraints::RoundingConstraint(10));
        RenderUtils::RenderText(lPos, lSize, IM_COL32_WHITE, "Delete", 5 * .5 * .3, 2);

        if(ImGui::IsMouseClicked(0) && Utils::IsMouseOverRectangle(Utils::mousepos, lPos, lSize)) {
            std::filesystem::remove(path+"\\"+files[i]);
            name = "";
            sfiles = 0;
            delPosY = NULL;
        }
        cellPos.y += cellSize.y;
    }
    ImVec2 cSize(Constraints::PercentageConstraint(.03), Constraints::PercentageConstraint(.03));
    ImVec2 cPos(pos.x+size.x-cSize.x/2, pos.y-cSize.y/2);
    RenderUtils::fillRect(cPos, cSize, accent, Constraints::RoundingConstraint(10));
    RenderUtils::RenderImage(cPos, cSize, "Lyra\\Assets\\cross.png", IM_COL32_WHITE);

    if (ImGui::IsMouseClicked(0) && Utils::IsMouseOverRectangle(Utils::mousepos, cPos, cSize)) {
        currentSetting = "Profiles";
        DelUI = false;
        name = "";
        sfiles = 0;
        delPosY = NULL;
    }

}

void save(){SaveUI = true;}
void load(){LoadUI = true;}
void del(){DelUI = true;}

Profiles::Profiles() : ModuleCtor(0, "Profile Manager", "Profiles") {
    this->InitModuleSettings();
}

std::string Profiles::getModuleName() {
    return "Profiles";
}

void Profiles::InitModuleSettings() {
    Settings::addSetting<bool>(getModuleName(), "enabled", true, true);
    this->SettingsList["MANAGE CONFIG FILES"]["Save UI Configuration"] = "pushbutton";
    this->SettingsList["MANAGE CONFIG FILES"]["Load UI Configuration"] = "pushbutton";
    this->SettingsList["MANAGE CONFIG FILES"]["Delete UI Configuration"] = "pushbutton";

    this->pbConfig["Save UI Configuration"]["Save"] = &save;
    this->pbConfig["Load UI Configuration"]["Load"] = &load;
    this->pbConfig["Delete UI Configuration"]["Delete"] = &del;
}

void Profiles::onEnable() {
    Settings::getSettingByName<bool>(getModuleName(), "enabled")->value = true;
    eventMgr.addListener(this);
}

void Profiles::onDisable() {
    this->setEnabled(true);
    currentSetting = getModuleName();
}

void Profiles::onRender(const RenderEvent &event) {
    if(!SDK::clientInstance || !SDK::clientInstance->getLocalPlayer()) return;
    if (!(SDK::TopScreen.rfind("hud_screen") != std::string::npos)) return;
    float speed = Settings::getSettingByName<float>("Mod Menu", "ANIMATION SPEED")->value;
    if (ReplaceUI) RenderReplaceUI();
    if (SaveUI) RenderSaveUI();
    if (LoadUI) RenderLoadUI();
    if (DelUI) RenderDelUI();
}

void Profiles::onMouse(MouseEvent &event) {
    if (LoadUI || DelUI) {
        if (event.GetAction() != MouseAction::SCROLL_UP && event.GetButton() == MouseButton::Scroll)
            sfiles++;
        if (event.GetAction() == MouseAction::SCROLL_UP)
            if(sfiles > 0) sfiles--;
    }
}