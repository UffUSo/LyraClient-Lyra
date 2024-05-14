#include "Tablist.hpp"
#include "../../SDK/SDK.hpp"
#include "../../../Utils/Constraints.hpp"
#include "../../../Utils/RenderUtils.h"
#include "../../Client.hpp"
#include <iostream>

Tablist::Tablist() : ModuleCtor(0, "Tablist", "Tablist") {
    this->InitModuleSettings();
}

std::string Tablist::getModuleName() {
    return "Tablist";
}

void Tablist::InitModuleSettings() {
    Settings::addSetting<bool>(this->getModuleName(), "enabled", false, true);
    Settings::addSetting<float>(this->getModuleName(), "PosX", 0, true);
    Settings::addSetting<float>(this->getModuleName(), "PosY", 0, true);
    Settings::addSetting<float>("Tablist", "size", 1, true);
    Settings::addSetting<int>("Tablist", "keybind", 9, true);
    Settings::addSetting<float>("Tablist", "Rounding", 0, true);

    this->SettingsList["SCALE"]["size"] = "slider-0.65-2.5";
    this->SettingsList["SCALE"]["Rounding"] = "slider-0-0.5";
    this->SettingsList["CONTROLS"]["keybind"] = "keybind";
}

void Tablist::onEnable() {
    Settings::getSettingByName<bool>(this->getModuleName(), "enabled")->value = true;
    eventMgr.addListener(this);
}

void Tablist::onDisable() {
    Settings::getSettingByName<bool>(this->getModuleName(), "enabled")->value = false;
    eventMgr.removeListener(this);
}

void Tablist::onRender(const RenderEvent& event) {
    this->RenderHUD("");
}

bool ShouldTabList = false;

void Tablist::onKey(KeyEvent& event) {
    if (event.getKey() == Settings::getSettingByName<int>("Tablist", "keybind")->value and SDK::TopScreen.rfind("hud_screen") != std::string::npos and !Settings::getSettingByName<bool>("Mod Menu", "enabled")->value)
        ShouldTabList = event.isPressed();
}

void Tablist::RenderHUD(std::string text) {
    if (!ShouldTabList) {
        return;
    }
    if(!SDK::clientInstance || !SDK::clientInstance->getLocalPlayer()) return;
    if (!(SDK::TopScreen.rfind("hud_screen") != std::string::npos)) return;
    std::unordered_map<mcUUID, PlayerListEntry>& Players = SDK::clientInstance->getLocalPlayer()->level->getPlayerMap();

    int rows = Players.size() / 10;
    if (Players.size() % 10 != 0) {
        rows++;
    }
    int cols = (Players.size() >= 10) ? 10 : Players.size();

    float round = Settings::getSettingByName<float>("Tablist", "Rounding")->value;

    float SizeModifier = Settings::getSettingByName<float>(this->getModuleName(), "size")->value;

    ImVec2 sizeEachCell = ImVec2(Constraints::PercentageConstraint(0.2) * SizeModifier, Constraints::PercentageConstraint(0.04) * SizeModifier);
    ImVec2 size = ImVec2(sizeEachCell.x * cols + 10, sizeEachCell.y * rows + 10);
    ImVec2 pos = ImVec2((Constraints::PercentageConstraint(1, 2) / 2) - (size.x / 2), Constraints::PercentageConstraint(0.05));

    RenderUtils::fillRect(pos, size, ImColor(0, 0, 0, 127), Constraints::RoundingCalc(size, round / 2));
    ImVec2 posNames = pos;
    int count = 0;
    for (auto& pair : Players) {
        count++;
        auto it = std::find(Client::onlineUsers.begin(), Client::onlineUsers.end(), Utils::removeColorCodes(pair.second.name));
        auto it5 = std::find(Client::onlineStaff.begin(), Client::onlineStaff.end(), Utils::removeColorCodes(pair.second.name));
        auto it4 = std::find(Client::onlineStaffp.begin(), Client::onlineStaffp.end(), Utils::removeColorCodes(pair.second.name));
        auto it3 = std::find(Client::partners.begin(), Client::partners.end(), Utils::removeColorCodes(pair.second.name));
        std::string prefix = "";
        if (it != Client::onlineUsers.end()) {
            RenderUtils::RenderImage(ImVec2(posNames.x, posNames.y+(sizeEachCell.y)/4-SizeModifier*5), ImVec2(Constraints::PercentageConstraint(.03)*SizeModifier, Constraints::PercentageConstraint(.03)*SizeModifier), "Lyra\\Assets\\Lyra.png", IM_COL32_WHITE);
            RenderUtils::RenderImage(ImVec2(posNames.x, posNames.y+(sizeEachCell.y)/4-SizeModifier*5), ImVec2(Constraints::PercentageConstraint(.03)*SizeModifier, Constraints::PercentageConstraint(.03)*SizeModifier), "Lyra\\Assets\\star.png", ImColor(221, 68, 221));
        }
        if (it5 != Client::onlineStaff.end()) {
            RenderUtils::RenderImage(ImVec2(posNames.x, posNames.y+(sizeEachCell.y)/4-SizeModifier*5), ImVec2(Constraints::PercentageConstraint(.03)*SizeModifier, Constraints::PercentageConstraint(.03)*SizeModifier), "Lyra\\Assets\\Lyra_Dev_ICON.png", IM_COL32_WHITE);
            RenderUtils::RenderImage(ImVec2(posNames.x, posNames.y+(sizeEachCell.y)/4-SizeModifier*5), ImVec2(Constraints::PercentageConstraint(.03)*SizeModifier, Constraints::PercentageConstraint(.03)*SizeModifier), "Lyra\\Assets\\star.png", ImColor(43, 43, 255));
        }
        if (it4 != Client::onlineStaffp.end()) {
            RenderUtils::RenderImage(ImVec2(posNames.x, posNames.y+(sizeEachCell.y)/4-SizeModifier*5), ImVec2(Constraints::PercentageConstraint(.03)*SizeModifier, Constraints::PercentageConstraint(.03)*SizeModifier), "Lyra\\Assets\\Ownersandstufficon.png", IM_COL32_WHITE);
            RenderUtils::RenderImage(ImVec2(posNames.x, posNames.y+(sizeEachCell.y)/4-SizeModifier*5), ImVec2(Constraints::PercentageConstraint(.03)*SizeModifier, Constraints::PercentageConstraint(.03)*SizeModifier), "Lyra\\Assets\\star.png", ImColor(255, 43, 43));
        }
        if (it3 != Client::partners.end()) {
            RenderUtils::RenderImage(ImVec2(posNames.x, posNames.y+(sizeEachCell.y)/4-SizeModifier*5), ImVec2(Constraints::PercentageConstraint(.03)*SizeModifier, Constraints::PercentageConstraint(.03)*SizeModifier), "Lyra\\Assets\\Partner.png", IM_COL32_WHITE);
        }
        std::vector<float> ShadowCol = Settings::getSettingByName<std::vector<float>>("Mod Menu", "TEXT SHADOW COLOR")->value;
        ImColor ShadowColor = ImColor(ShadowCol.at(0), ShadowCol.at(1), ShadowCol.at(2), ShadowCol.at(3));
        if(Settings::getSettingByName<bool>("Mod Menu", "TEXT SHADOWS")->value)
            RenderUtils::RenderText(ImVec2((posNames.x + sizeEachCell.x/10*2) + Constraints::PercentageConstraint(0.0025), (posNames.y + (sizeEachCell.y - 10)/8) + Constraints::PercentageConstraint(0.0025)), sizeEachCell, ShadowColor, Utils::removeNonAlphanumeric(Utils::removeColorCodes(pair.second.name)), sizeEachCell.y * 0.01, 1, ImGui::GetForegroundDrawList(), Settings::getSettingByName<bool>("Mod Menu", "USE MONJANGLES HUD")->value);

        RenderUtils::RenderText(ImVec2(posNames.x + sizeEachCell.x/10*2, posNames.y + (sizeEachCell.y - 10)/8), sizeEachCell, IM_COL32_WHITE, Utils::removeNonAlphanumeric(Utils::removeColorCodes(pair.second.name)), sizeEachCell.y * 0.01, 1, ImGui::GetForegroundDrawList(), Settings::getSettingByName<bool>("Mod Menu", "USE MONJANGLES HUD")->value);
        if (count < 10) {
            posNames.x += sizeEachCell.x;
        }
        else {
            posNames.x = pos.x;
            posNames.y += sizeEachCell.y;
            count = 0;
        }   
    }
}

void Tablist::onTick(const TickEvent &event) {
    return;
    if(!SDK::clientInstance || !SDK::clientInstance->getLocalPlayer()) return;
    std::unordered_map<mcUUID, PlayerListEntry>& Players = SDK::clientInstance->getLocalPlayer()->level->getPlayerMap();
    auto entities = SDK::clientInstance->getLocalPlayer()->level->getRuntimeActorList();
    for (auto& pair : Players) {
        auto nonColoredNameClean = Utils::removeColorCodes(pair.second.name);
        auto cleanName = Utils::removeNonAlphanumeric(nonColoredNameClean);
        Actor* entity = nullptr;
        for (auto& actor : entities) {
            if(!actor->isValidTarget()) continue;
            std::string name = actor->playerName;
            if(name == cleanName){
                entity = actor;
                break;
            }
        }

        if(!entity) continue;

        auto nonColoredName = Utils::removeColorCodes(*entity->getNametag());

        auto cleanEntityName = Utils::removeNonAlphanumeric(nonColoredName);

        if(cleanEntityName.find(" LYRA ") != std::string::npos) continue;

        auto it = std::find(Client::onlineUsers.begin(), Client::onlineUsers.end(),
                            nonColoredNameClean);
        auto it5 = std::find(Client::onlineStaff.begin(), Client::onlineStaff.end(),
                             nonColoredNameClean);
        auto it4 = std::find(Client::onlineStaffp.begin(), Client::onlineStaffp.end(),
                             nonColoredNameClean);

        auto it3 = std::find(Client::partners.begin(), Client::partners.end(), Utils::removeColorCodes(pair.second.name));

        if (it != Client::onlineUsers.end()) {
            auto name = "§l§0[§d LYRA §0] §r" + *entity->getNametag(); //"\uE800"
            entity->setNametag(&name);
        }else if (it5 != Client::onlineStaff.end()) {
            auto name = "§l§0[§b LYRA §0] §r" + *entity->getNametag(); //"\uE801"
            entity->setNametag(&name);
        }else if (it4 != Client::onlineStaffp.end()) {
            auto name = "§l§0[§c LYRA §0] §r" + *entity->getNametag(); //"\uE802"
            entity->setNametag(&name);
        }else if (it3 != Client::partners.end()) {
            auto name = "§l§0[§6 LYRA §0] §r" + *entity->getNametag();
            entity->setNametag(&name);
        }
    }
}
