#include "Notif.hpp"
#include "../../../Utils/Utils.hpp"
#include "../../../Utils/RenderUtils.h"
#include "../ModuleManager.hpp"
#include "../../SDK/SDK.hpp"
#include "ModMenu.hpp"
#include "../../SDK/Client/Network/Packets/ResourcePacksInfo.hpp"

std::vector<std::string> notifs;
std::chrono::time_point<std::chrono::high_resolution_clock> InitTime;
float size = 0;
float posx;
Notif::Notif() : ModuleCtor(0, "", "") {
    this->InitModuleSettings();
}

void Notif::InitModuleSettings() {
    Settings::addSetting<bool>("Notif", "enabled", true, true);
}

std::string Notif::getModuleName() {
    return "Notif";
}

void Notif::onEnable() {
    Settings::getSettingByName<bool>("Notif", "enabled")->value = true;
    eventMgr.addListener(this);
    InitTime = std::chrono::high_resolution_clock::now();
    notifs.push_back("Lyra Client has been launched.");
    notifs.push_back("Press "+Utils::GetKeyAsString(Settings::getSettingByName<int>("Mod Menu", "KEYBIND")->value, true)+" to open menu in-game.");
    notifs.push_back("Press "+Utils::GetKeyAsString(Settings::getSettingByName<int>("Mod Menu", "Friend List Keybind")->value, true)+" to open friend list in-game.");
}

void Notif::onTick(const TickEvent& event) {
    auto it = ModMenu::blocked_modules.find(SDK::getServerIP());

    for (auto it2 = moduleMgr.getModules().begin(); it2 != moduleMgr.getModules().end(); ++it2) {
        std::shared_ptr<Module> modul = it2->second;
        if (it != ModMenu::blocked_modules.end()) {
            std::vector<std::string> &list = it->second;

            if (std::find(list.begin(), list.end(),modul->getModuleName()) != list.end()) {
                if(modul->isEnabled()) {
                    modul->setEnabled(false);

                }

            }
        }
    }


}

void Notif::onRender(const RenderEvent &event) {
    float speed = Settings::getSettingByName<float>("Mod Menu", "ANIMATION SPEED")->value;
    ImColor accent = RenderUtils::VecToImColour(Settings::getSettingByName<std::vector<float>>("Mod Menu", "ACCENT COLOR")->value);
    ImVec2 bSize(Constraints::PercentageConstraint(.18, 2), Constraints::PercentageConstraint(.08));
    ImVec2 Gap(Constraints::PercentageConstraint(.0145, 2), Constraints::PercentageConstraint(.025));
    float tPos = Constraints::PercentageConstraint(1, 2) - bSize.x - Gap.x;

    if(!posx) posx = Constraints::PercentageConstraint(1, 2);
    if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - InitTime).count() < 1) return;
    if (notifs.empty()) {
        ImVec2 sSize = ImGui::GetIO().DisplaySize;
        if (!(SDK::TopScreen.rfind("hud_screen") != std::string::npos) && !(SDK::TopScreen.rfind("chat_screen") != std::string::npos) && SDK::clientInstance && SDK::clientInstance->getLocalPlayer()) {
            ImVec2 bmG_Pos = ImVec2(sSize.x - Constraints::PercentageConstraint(.35), sSize.y - Constraints::PercentageConstraint(.15));
            ImVec2 bm_Icon_Size = ImVec2(Constraints::PercentageConstraint(.08), Constraints::PercentageConstraint(.08));
            ImVec2 bm_Title_Pos = ImVec2(bmG_Pos.x + bm_Icon_Size.x, bmG_Pos.y + 5);
            ImVec2 bm_Title_Size = ImVec2(Constraints::PercentageConstraint(.1), Constraints::PercentageConstraint(.08));
            ImVec2 bm_Sub_Pos = ImVec2(bm_Title_Pos.x + bm_Title_Size.x + 15, bmG_Pos.y + 10);
            ImVec2 bm_Sub_Size = ImVec2(Constraints::PercentageConstraint(.2), Constraints::PercentageConstraint(.08));
            RenderUtils::RenderImage(bmG_Pos, bm_Icon_Size, "Lyra\\assets\\Lyra.png", IM_COL32_WHITE);
            RenderUtils::RenderImage(bmG_Pos, bm_Icon_Size, "Lyra\\assets\\star.png", accent);
            RenderUtils::RenderTextWithOutline(bm_Title_Pos, bm_Title_Size, IM_COL32_WHITE, "LYRA", 20 * .5 * .12, IM_COL32_WHITE, 1);
            RenderUtils::RenderText(bm_Sub_Pos, bm_Sub_Size, ImColor(180, 180, 180), "CLIENT", 15 * .5 * .12);
    }
    }
    if (notifs.empty()) return;
    ImVec2 bPos(posx, Constraints::PercentageConstraint(1) - bSize.y*2);
    ImVec2 h1Size(Constraints::PercentageConstraint(.12, 2), Gap.y+Constraints::PercentageConstraint(.005));
    ImVec2 h2Size(bSize.x-Gap.x, Gap.y-Constraints::PercentageConstraint(.005));
    ImVec2 h1Pos(bPos.x+Gap.x, bPos.y + bSize.y/2-h1Size.y);
    ImVec2 h2Pos(bPos.x+Gap.x, bPos.y + bSize.y/2+h2Size.y/4);

    RenderUtils::fillRect(bPos, bSize, ImColor(.0f, .0f, .0f, .5f), 10);
    posx = Utils::animate(tPos, posx, MC::Deltatime *.15*speed);
    
    std::string ntf = notifs[0];
    RenderUtils::RenderText(h1Pos, h1Size, IM_COL32_WHITE, "NOTIFICATION", 7 * .5 * .25);
    RenderUtils::RenderText(h2Pos, h2Size, IM_COL32_WHITE, ntf, 7 * .5 * .12);

    ImVec2 barSize(size, Constraints::PercentageConstraint(.005));
    ImVec2 barPos(bPos.x, bPos.y+bSize.y-barSize.y);
    RenderUtils::fillRect(barPos, barSize, accent, 10);
    RenderUtils::ShadowRectD2D(barPos, barSize, 10, accent, 2, 5);

    if (posx <= tPos+5) size = Utils::animate(bSize.x, size, MC::Deltatime * .15 * .085);
    if(size >= bSize.x-5) posx = Utils::animate(Constraints::PercentageConstraint(1.5, 2), posx, MC::Deltatime * .15 * speed);
    if (posx >= Constraints::PercentageConstraint(1, 2)-5 && size >= bSize.x-5) {
        posx = NULL;
        size = 0;
        notifs.erase(notifs.begin());
    }
}

void Notif::onPacketReceive(PacketEvent &event) {
    MinecraftPacketIds id = event.getPacket()->getId();
    if (id == MinecraftPacketIds::ResourcePacksInfo) {
        return;
        auto *packet = reinterpret_cast<ResourcePacksInfoPacket *>(event.getPacket());
        std::string tpVersion = "1.0.0";
        std::string uuid = "46b5840e-4517-4fc5-a9d4-3298accc25b4";
        std::string contentKey = "";
        std::string url = "CDN";
        std::string name = uuid + "_" + tpVersion;


        PackInfoData pack(uuid, tpVersion, contentKey, 1000, PackType::Resources);

        packet->addPack(pack);
        packet->mCDNUrls.emplace_back(name,url);
    }
}
