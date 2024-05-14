#include <regex>
#include "helper.hpp"
#include "../../../Event/Events/Network/PacketEvent.hpp"
#include "../../../SDK/Client/Network/Packets/CommandRequestPacket.hpp"
#include "../../../SDK/Client/Network/Packets/PlaySoundPacket.hpp"
#include "../../../SDK/Client/Network/Packets/TextPacket.hpp"
#include "../../../SDK/SDK.hpp"
#include "../Notif.hpp"
#include "../ModMenu.hpp"
#include "../../../../Utils/RenderUtils.h"

std::string gamemode = "";
bool chs = false;
bool ggtext = false;

std::string GGText ="";
float GGsvPosY;

std::list<uint64_t> ctns;

helper::helper() : ModuleCtor(0, "Hive Helper", "hivehelper") {
    this->InitModuleSettings();
}

void RenderGGTEXTUI() {

    ImColor accent = RenderUtils::VecToImColour(Settings::getSettingByName<std::vector<float>>("Mod Menu", "ACCENT COLOR")->value);
    float speed = Settings::getSettingByName<float>("Mod Menu", "ANIMATION SPEED")->value;
    if(DisableModMenu) ggtext = false;
    ImVec2 ScreenCenter(Constraints::PercentageConstraint(.5, 2), Constraints::PercentageConstraint(.5));
    ImVec2 size(Constraints::PercentageConstraint(.3, 2), Constraints::PercentageConstraint(.1));
    if (GGsvPosY == NULL) GGsvPosY = -size.y;
    ImVec2 pos(ScreenCenter.x - size.x/2, GGsvPosY);
    GGsvPosY = Utils::animate(ScreenCenter.y - size.y/2, GGsvPosY, MC::Deltatime * .15 * speed);

    RenderUtils::fillRect(pos, size, ImColor(.15f,.15f,.15f,1.f), Constraints::RoundingConstraint(10));

    ImVec2 tbSize(Constraints::PercentageConstraint(.15, 2), Constraints::PercentageConstraint(.05));
    ImVec2 tbPos(pos.x + (size.x-tbSize.x)/6, pos.y + (size.y-tbSize.y)/2);
    if(GGText == "" && !RenderUtils::TextIndex[6]) GGText = Settings::getSettingByName<std::string>("Hive Helper", "Auto GG Text")->value;
    if(ImGui::IsMouseClicked(0) && Utils::IsMouseOverRectangle(Utils::mousepos, tbPos, tbSize) && !RenderUtils::TextIndex[5]) GGText = "";
    RenderUtils::textBox(&GGText, tbPos, tbSize, IM_COL32_BLACK_TRANS, 6);
    RenderUtils::drawRect(tbPos, tbSize, IM_COL32_WHITE, Constraints::RoundingConstraint(10), 1);

    ImVec2 svSize(Constraints::PercentageConstraint(.09, 2), Constraints::PercentageConstraint(.05));
    ImVec2 svPos((pos.x+size.x) - (svSize.x + (size.x-svSize.x)/8), (pos.y+size.y) - (svSize.y + (size.y-svSize.y)/2));

    RenderUtils::fillRect(svPos, svSize, accent, Constraints::RoundingConstraint(10));
    RenderUtils::RenderText(svPos, svSize, IM_COL32_WHITE, "Save", 5 * .5 * .3, 2);

    if(ImGui::IsMouseClicked(0) && Utils::IsMouseOverRectangle(Utils::mousepos, svPos, svSize)) {
        Settings::getSettingByName<std::string>("Hive Helper", "Auto GG Text")->value = GGText;
        GGText = "";
        currentSetting = "Hive Helper";
        ggtext = false;
        GGsvPosY = NULL;
    }

    ImVec2 cSize(Constraints::PercentageConstraint(.03), Constraints::PercentageConstraint(.03));
    ImVec2 cPos(pos.x+size.x-cSize.x/2, pos.y-cSize.y/2);
    RenderUtils::fillRect(cPos, cSize, accent, Constraints::RoundingConstraint(10));
    RenderUtils::RenderImage(cPos, cSize, "Lyra\\Assets\\cross.png", IM_COL32_WHITE);

    if (ImGui::IsMouseClicked(0) && Utils::IsMouseOverRectangle(Utils::mousepos, cPos, cSize)) {
        GGText = "";
        currentSetting = "Hive Helper";
        ggtext = false;
        GGsvPosY = NULL;
    }
}

bool triggered = false;
std::string helper::getModuleName(){
    return "Hive Helper";
}
void helper::onEnable(){
    eventMgr.addListener(this);
}

void helper::onDisable() {
    eventMgr.removeListener(this);
}
void  Set() {
    ggtext = true;
}

void helper::InitModuleSettings() {
    Settings::addSetting<bool>(getModuleName(), "enabled", true, true);
    Settings::addSetting<bool>(getModuleName(), "Auto RE Q", false, true);
    Settings::addSetting<bool>(getModuleName(), "Auto GG", false, true);
    Settings::addSetting<std::string>(getModuleName(), "Auto GG Text", "GG", true);

    //Settings::addSetting<bool>(getModuleName(), "Show Player Levels", false, true);

    this->SettingsList["Auto RE Q"]["Auto RE Q"] = "toggle";
    this->SettingsList["Auto GG"]["Auto GG"] = "toggle";

    this->SettingsList["Auto GG"]["GG Text"] = "pushbutton";

    //this->SettingsList["Player Levels"]["Show Player Levels"] = "toggle";


    this->pbConfig["GG Text"]["Set"] = &Set;

//    Settings::addSetting<bool>(getModuleName(), "Levels", false, true);
//    this->SettingsList["Levels"]["Levels"] = "toggle";
}





void helper::onPacketReceive(PacketEvent &event){
    MinecraftPacketIds id = event.getPacket()->getId();

    if (id == MinecraftPacketIds::PlaySoundA) {
        auto *pkt = reinterpret_cast<PlaySoundPacket *>(event.getPacket());

        auto player = SDK::clientInstance->getLocalPlayer();
        if(Settings::getSettingByName<bool>(getModuleName(), "Auto GG")->value && pkt->mName == "raid.horn") {
            std::shared_ptr<Packet> packet2 = SDK::createPacket(9);
            auto *text = reinterpret_cast<TextPacket *>(packet2.get());


            text->type = TextPacketType::CHAT;
            text->message = Settings::getSettingByName<std::string>(getModuleName(), "Auto GG Text")->value;
            text->platformId = "";
            text->translationNeeded = false;
            text->xuid = "";
            text->name = player->playerName;

            SDK::clientInstance->getPacketSender()->sendToServer(text);
        }
        if(Settings::getSettingByName<bool>(getModuleName(), "Auto RE Q")->value) {

            if (pkt->mName == "raid.horn") {
                triggered = true;




                notifs.push_back("Re queuing into " + gamemode);




                std::shared_ptr<Packet> packet = SDK::createPacket(77);
                auto *command_packet = reinterpret_cast<CommandRequestPacket *>(packet.get());
                command_packet->command = "/q " + gamemode;

                command_packet->origin.type = CommandOriginType::Player;

                command_packet->InternalSource = true;
                SDK::clientInstance->getPacketSender()->sendToServer(command_packet);
            }
        }
    }

    if (id == MinecraftPacketIds::ChangeDimension) {

        ctns.clear();

            if(chs){

                std::thread ts([]() {
                    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                    triggered = true;
                    std::shared_ptr<Packet> packet = SDK::createPacket(77);
                    auto *command_packet = reinterpret_cast<CommandRequestPacket *>(packet.get());

                    command_packet->command = "/connection";

                    command_packet->origin.type = CommandOriginType::Player;

                    command_packet->InternalSource = true;

                    SDK::clientInstance->getPacketSender()->sendToServer(command_packet);
                    //notifs.push_back("Detecting gamemode " + gamemode);
                    chs = false;
                });
                ts.detach();
            }else{
                chs = true;

            }



    }
    else if (id == MinecraftPacketIds::Text) {
        auto* pkt = reinterpret_cast<TextPacket*>(event.getPacket());

        std::string f1 = "You killed";

        size_t found = pkt->message.find(f1);

        std::string textToCheck = "You are connected to server name ";
        std::string textToCheckToSilence = "You are connected";
        if (pkt->message.find(textToCheck) != std::string::npos && triggered) {
            std::string server = pkt->message.replace(0, textToCheck.length(), "");
            std::regex pattern("\\d+");
            gamemode = std::regex_replace(server, pattern, "");
            triggered = false;
            event.cancel();

        }
        else if (pkt->message.find(textToCheckToSilence) != std::string::npos) {
            event.cancel();
        }
    }

}

//void helper::onPacketReceive(PacketEvent &event){
//    MinecraftPacketIds id = event.getPacket()->getId();
//
//    if (id == MinecraftPacketIds::PlaySoundA) {
//    auto *pkt = reinterpret_cast<PlaySoundPacket *>(event.getPacket());
//
//    if (pkt->mName == "raid.horn") {
//        if(Settings::getSettingByName<bool>(getModuleName(), "Auto RE Q")->value) {
//            triggered = true;
//            std::shared_ptr<Packet> packet = SDK::createPacket(77);
//            auto *command_packet = reinterpret_cast<CommandRequestPacket *>(packet.get());
//
//            command_packet->command = "/connection";
//
//            command_packet->origin.type = CommandOriginType::Player;
//
//            command_packet->InternalSource = true;
//
//            SDK::clientInstance->getPacketSender()->sendToServer(command_packet);
//        }
//
//        } //std::cout << pkt->mName << std::endl;
//
//    }
//    else if (id == MinecraftPacketIds::Text) {
//    auto* pkt = reinterpret_cast<TextPacket*>(event.getPacket());
//    std::string textToCheck = "You are connected to server name ";
//    std::string textToCheckToSilence = "You are connected";
//    if (pkt->message.find(textToCheck) != std::string::npos && triggered) {
//        std::string server = pkt->message.replace(0, textToCheck.length(), "");
//        std::regex pattern("\\d+");
//        std::string name = std::regex_replace(server, pattern, "");
//        std::thread t([name]() {
//            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//
//            notifs.push_back("Re queuing into " + name);
//
//
//            std::shared_ptr<Packet> packet = SDK::createPacket(77);
//            auto* command_packet = reinterpret_cast<CommandRequestPacket*>(packet.get());
//            command_packet->command = "/q " + name;
//
//            command_packet->origin.type = CommandOriginType::Player;
//
//            command_packet->InternalSource = true;
//            SDK::clientInstance->getPacketSender()->sendToServer(command_packet);
//        });
//        t.detach();
//        triggered = false;
//        pkt->message = "";
//
//    }
//        else if (pkt->message.find(textToCheckToSilence) != std::string::npos) {
//            event.cancel();
//        }
//    }
//}

void helper::onRender(const RenderEvent &event) {
    if(ggtext) RenderGGTEXTUI();
}

