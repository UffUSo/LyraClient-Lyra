#include <sstream>
#include <string>
#include <algorithm>
#include <stdlib.h>
#include <windows.h>
#include <thread>
#include <wininet.h>
#include "FriendSystem.hpp"
#include "../../../Config/ConfigManager.hpp"
#include "../../../SDK/SDK.hpp"
#include "../../../../Utils/RenderUtils.h"
#include "../../../Client.hpp"
#include "../../ModuleManager.hpp"
#include "../Notif.hpp"
#include "../../../../Utils/base64.hpp"
#include "../../../../Utils/SHA256.h"


float MenuPosX = 0;
float tMenuPosX = 0;
float Blur = 0;
std::string Search = "";
std::string AddFriendName = "";
bool inPendingMenu = false;
bool disabled = false;
bool pNotif = false;
int scroll = 0;


FriendSystem FriendSys;

std::string key(std::string v){
    std::string s = base64::to_base64(Utils::removeColorCodes(v));
    SHA256 sha;
    sha.update(s);
    std::array<uint8_t, 32> digest = sha.digest();
    return SHA256::toString(digest);
}
void FriendSystem::onEnable() {
    if (SDK::TopScreen.rfind("hud_screen") != std::string::npos) {
        disabled = false;
        MenuPosX = Constraints::PercentageConstraint(-0.4);
        tMenuPosX = 0;
        Blur = 0;
        Search = "";
        AddFriendName = "";

        if (SDK::clientInstance) {
            SDK::clientInstance->releaseMouse();
        }

        eventMgr.addListener(this);
    }
    else {
        this->enabled = false;
    }
}

void FriendSystem::onDisable() {
    disabled = true;
}

void FriendSystem::toggle() {
    if (enabled) {
        this->onDisable();
        enabled = false;
    }
    else {
        this->onEnable();
        enabled = true;
    }
}

void FriendSystem::HeartBeat() {
    LocalPlayerName = SDK::clientInstance->getLocalPlayer()->playerName;
    std::string pp = Utils::DownloadString(xorstr("http://node2.synthetix.host:5004/friends/") + Utils::removeColorCodes(LocalPlayerName));
    if (pp != "") {
        json playersDict = json::parse(pp);

        std::vector<std::string> Friends = {};

        int totalPlaytime = 0;
        int numberOfPlayers = 0;

        for (const auto& player : playersDict.items()) {
            json floatArray = player.value();
            int z = 0;
            for (const auto& value1 : floatArray) {
                Friends.push_back(value1.get<std::string>());
            }
        }
        friends = Friends;
    }

    pp = Utils::DownloadString(xorstr("http://node2.synthetix.host:5004/pendingfriendrequests/") + Utils::removeColorCodes(LocalPlayerName));
    if (pp != "") {
        json playersDict = json::parse(pp);

        std::vector<std::string> Pendings = {};

        for (const auto& player : playersDict.items()) {
            json arraY = player.value();
            for (const auto& value1 : arraY) {
                if (Utils::removeColorCodes(LocalPlayerName) == value1.get<std::string>())
                    continue;
                Pendings.push_back(value1.get<std::string>());
            }
        }
        if(!pNotif && Pendings.size() > 0) notifs.push_back("You have "+std::to_string(Pendings.size())+" pending friend requests.");
        pNotif = true;
        pendings = Pendings;
    }
}

std::string DownloadString(std::string URL, const CHAR *cd) {
    HINTERNET interwebs = InternetOpenA(cd, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
    HINTERNET urlFile;
    std::string rtn;
    if (interwebs) {
        urlFile = InternetOpenUrlA(interwebs, URL.c_str(), NULL, NULL, NULL, NULL);
        if (urlFile) {
            char buffer[2000];
            DWORD bytesRead;
            do {
                InternetReadFile(urlFile, buffer, 2000, &bytesRead);
                rtn.append(buffer, bytesRead);
                memset(buffer, 0, 2000);
            } while (bytesRead);
            InternetCloseHandle(interwebs);
            InternetCloseHandle(urlFile);
            std::string p = Utils::replaceAll(rtn, "|n", "\r\n");
            return p;
        }
    }
    InternetCloseHandle(interwebs);
    std::string p = Utils::replaceAll(rtn, "|n", "\r\n");
    return p;
}

void FriendSystem::AddFriend(std::string name) {
    LocalPlayerName = SDK::clientInstance->getLocalPlayer()->playerName;
    if (name == Utils::removeColorCodes(LocalPlayerName)) return notifs.push_back("Can't add yourself as a friend.");
    if(std::find(friends.begin(), friends.end(), name) != friends.end()) return notifs.push_back(name+" is already a friend.");
    std::string pp = DownloadString(xorstr("http://node2.synthetix.host:5004/sendfriendrequest/") + name + "/" + Utils::removeColorCodes(LocalPlayerName), base64::to_base64(Utils::removeColorCodes(LocalPlayerName)).c_str());


    if (pp != "") {
        printf(pp.c_str());
    }
    notifs.push_back("Friend request sent to "+name+".");
    moduleMgr.HeartBeatFriends();
}

void FriendSystem::AcceptFriend(std::string name) {
    LocalPlayerName = SDK::clientInstance->getLocalPlayer()->playerName;
    if(std::find(friends.begin(), friends.end(), name) != friends.end()) return notifs.push_back(name+" is already a friend.");
    std::string pp = DownloadString(xorstr("http://node2.synthetix.host:5004/acceptfriendrequest/") + name + "/" + Utils::removeColorCodes(LocalPlayerName), base64::to_base64(Utils::removeColorCodes(LocalPlayerName)).c_str());
    if (pp != "") {
        printf(pp.c_str());
    }
    notifs.push_back("Added "+name+" to friend list.");
    moduleMgr.HeartBeatFriends();
}

void FriendSystem::RemoveFriend(std::string name) {
    LocalPlayerName = SDK::clientInstance->getLocalPlayer()->playerName;
    std::string pp = DownloadString(xorstr("http://node2.synthetix.host:5004/friendremove/") + name + "/" + Utils::removeColorCodes(LocalPlayerName), base64::to_base64(Utils::removeColorCodes(LocalPlayerName)).c_str());

    if (pp != "") {
        printf(pp.c_str());
    }
    notifs.push_back("Removed "+name+" from friend list.");
    moduleMgr.HeartBeatFriends();
}

void FriendSystem::onKey(KeyEvent& event) {
    if (!disabled)
        event.setCancelled(true);
}

void FriendSystem::onMouse(MouseEvent& event) {
    if (!disabled) {
        if(event.GetAction() != MouseAction::SCROLL_UP && event.GetButton() == MouseButton::Scroll)
            scroll++;
        if(event.GetAction() == MouseAction::SCROLL_UP)
            if(scroll > 0) scroll--;
        event.setCancelled(true);
    }
}

void FriendSystem::onRender(const RenderEvent& event) {
    float animationSpeed = Settings::getSettingByName<float>("Mod Menu", "ANIMATION SPEED")->value;
    ImColor accent = RenderUtils::VecToImColour(Settings::getSettingByName<std::vector<float>>("Mod Menu", "ACCENT COLOR")->value);
    if(!SDK::clientInstance || !SDK::clientInstance->getLocalPlayer()) return;
    if (!(SDK::TopScreen.rfind("hud_screen") != std::string::npos)) return;
    if (!disabled) {
        Blur = Utils::animate(10, Blur, MC::Deltatime * 0.15 * animationSpeed);
        MenuPosX = Utils::animate(Constraints::PercentageConstraint(.05), MenuPosX, MC::Deltatime * 0.15 * animationSpeed);
    }
    else {
        Blur = Utils::animate(-1, Blur, MC::Deltatime * 0.15 * animationSpeed);
        MenuPosX = Utils::animate(Constraints::PercentageConstraint(-0.4)-5, MenuPosX, MC::Deltatime * 0.15 * animationSpeed);

        if (Blur < 0 and MenuPosX < Constraints::PercentageConstraint(-0.4)){
            eventMgr.removeListener(this);
            if (SDK::TopScreen.rfind("hud_screen") != std::string::npos) {
                if (SDK::clientInstance) {
                    SDK::clientInstance->grabMouse();
                }
            }
            return;
        }
    }

    ImVec2 PosMenu = ImVec2(MenuPosX, Constraints::PercentageConstraint(.05));
    ImVec2 Sizemenu = ImVec2(Constraints::PercentageConstraint(0.4), Constraints::PercentageConstraint(.82));

    ImVec2 SizeTopButtons = ImVec2(Constraints::PercentageConstraint(0.15), Constraints::PercentageConstraint(0.06));

    ImVec2 PosFriendsButton = ImVec2(MenuPosX+Constraints::PercentageConstraint(.0325), PosMenu.y+Constraints::PercentageConstraint(0.03));
    ImVec2 PosPendingButton = ImVec2(PosFriendsButton.x+SizeTopButtons.x+Constraints::PercentageConstraint(.02), PosFriendsButton.y);

    ImVec2 PosSearchBar = ImVec2(PosFriendsButton.x, PosFriendsButton.y + SizeTopButtons.y + Constraints::PercentageConstraint(0.01));
    ImVec2 SizeSearchBar = ImVec2(SizeTopButtons.x*2+Constraints::PercentageConstraint(.025), Constraints::PercentageConstraint(0.06));

    ImVec2 PosSearchBarTextBox = ImVec2(PosSearchBar.x + Constraints::PercentageConstraint(0.015), PosSearchBar.y);
    ImVec2 SizeSearchBarTextBox = ImVec2(SizeSearchBar.x - Constraints::PercentageConstraint(0.03), SizeSearchBar.y);

    ImVec2 SizeAddFriendButton = ImVec2(SizeSearchBar.x, Constraints::PercentageConstraint(0.06));
    ImVec2 PosAddFriendButton = ImVec2(PosSearchBar.x, PosMenu.y+Sizemenu.y-SizeAddFriendButton.y-Constraints::PercentageConstraint(.02));

    ImVec2 SizeAddFriendBar = ImVec2(SizeSearchBar.x, Constraints::PercentageConstraint(0.06));
    ImVec2 PosAddFriendBar = ImVec2(PosSearchBar.x, PosAddFriendButton.y-Constraints::PercentageConstraint(.01)-SizeAddFriendBar.y);

    ImVec2 PosAddFriendTextBox = ImVec2(PosAddFriendBar.x + Constraints::PercentageConstraint(0.015), PosAddFriendBar.y);
    ImVec2 SizeAddFriendTextBox = ImVec2(SizeAddFriendBar.x - Constraints::PercentageConstraint(0.03), SizeAddFriendBar.y);

    RenderUtils::Blur(Blur);
    RenderUtils::fillRect(PosMenu, Sizemenu, ImColor(0.f, 0.f, 0.f, 0.5f), Constraints::RoundingCalc(Sizemenu, .20));
    RenderUtils::fillRect(PosFriendsButton, SizeTopButtons, (!Utils::IsMouseOverRectangle(Utils::mousepos, PosFriendsButton, SizeTopButtons) && inPendingMenu) ? ImColor(0.f, 0.f, 0.f, 0.3f) : accent, Constraints::RoundingConstraint(20));
    RenderUtils::fillRect(PosPendingButton, SizeTopButtons, (!Utils::IsMouseOverRectangle(Utils::mousepos, PosPendingButton, SizeTopButtons) && !inPendingMenu) ? ImColor(0.f, 0.f, 0.f, 0.3f) : accent, Constraints::RoundingConstraint(20));

    RenderUtils::RenderText(PosFriendsButton, SizeTopButtons, IM_COL32_WHITE, std::to_string(friends.size()) + " Friends", SizeTopButtons.y * 0.011, 2);
    RenderUtils::RenderText(PosPendingButton, SizeTopButtons, IM_COL32_WHITE, std::to_string(pendings.size()) + " Pending", SizeTopButtons.y * 0.011, 2);

    if (ImGui::IsMouseClicked(0) and Utils::IsMouseOverRectangle(Utils::mousepos, PosFriendsButton, SizeTopButtons)) {
        inPendingMenu = false;
    }
    else if (ImGui::IsMouseClicked(0) and Utils::IsMouseOverRectangle(Utils::mousepos, PosPendingButton, SizeTopButtons)) {
        inPendingMenu = true;
    }

    RenderUtils::fillRect(PosSearchBar, SizeSearchBar, ImColor(0.f, 0.f, 0.f, 0.3f), Constraints::RoundingConstraint(20));

    RenderUtils::textBox(&Search, PosSearchBarTextBox, SizeSearchBarTextBox, ImColor(0, 0, 0, 0), 12);

    if (Search == "")
        RenderUtils::RenderText(PosSearchBarTextBox, SizeSearchBarTextBox, ImColor(0.75f, 0.75f, 0.75f, 1.0f), "Search...", SizeTopButtons.y / 100, 1);

    ImVec2 pos = ImVec2(PosSearchBar.x+Constraints::PercentageConstraint(.01), PosSearchBar.y + SizeSearchBar.y + Constraints::PercentageConstraint(0.04));

    if (!inPendingMenu) {
        int ps = friends.size();
        int es = std::min(ps - 1, scroll + 5);
        if (scroll+es >= ps) scroll = ps - es - 1;
        for (int i = scroll; i < ps && i <= es; i++) {
            std::string player = friends[i];
            std::string lplayer = player;
            std::transform(lplayer.begin(), lplayer.end(), lplayer.begin(), ::tolower);
            std::transform(Search.begin(), Search.end(), Search.begin(), ::tolower);
            if (lplayer.find(Search) == std::string::npos) return;
            ImVec2 PlayerHeadPos = pos;
            ImVec2 PlayerHeadSize = ImVec2(Constraints::PercentageConstraint(0.05), Constraints::PercentageConstraint(0.05));

            ImVec2 PlayerNamePos = ImVec2(PlayerHeadPos.x + PlayerHeadSize.x + Constraints::PercentageConstraint(0.02), PlayerHeadPos.y);
            ImVec2 PlayerNamePadding = ImVec2(0, PlayerHeadSize.x / 1.7);

            ImVec2 OnlineStatusPos = ImVec2(PlayerHeadPos.x + PlayerHeadSize.x + Constraints::PercentageConstraint(0.02), PlayerHeadPos.y + PlayerHeadSize.x / 2);
            ImVec2 OnlineStatusPadding = ImVec2(0, PlayerHeadSize.x / 2);

            ImVec2 SizeRemoveButton = ImVec2(Constraints::PercentageConstraint(0.035), Constraints::PercentageConstraint(0.035));
            ImVec2 PosRemoveButton = ImVec2(Sizemenu.x - SizeRemoveButton.x - Constraints::PercentageConstraint(0.03) + MenuPosX, PlayerHeadPos.y + ((PlayerHeadSize.y / 2) - (SizeRemoveButton.y / 2)));

            auto it = std::find(Client::online.begin(), Client::online.end(),Utils::removeColorCodes(player));

            RenderUtils::drawRect(ImVec2(PosSearchBar.x, pos.y - Constraints::PercentageConstraint(0.01)), ImVec2(SizeSearchBar.x, Constraints::PercentageConstraint(0.07)), ImColor(.35f, .35f, .35f, 1.f), Constraints::RoundingConstraint(15), Constraints::PercentageConstraint(0.0015));
            RenderUtils::fillRect(ImVec2(PosSearchBar.x, pos.y - Constraints::PercentageConstraint(0.01)), ImVec2(SizeSearchBar.x, Constraints::PercentageConstraint(0.07)), ImColor(.0f, .0f, .0f, .4f), Constraints::RoundingConstraint(15));
            RenderUtils::RenderImage(PlayerHeadPos, PlayerHeadSize, "Lyra\\Assets\\DefaultPlayerHead.png", IM_COL32_WHITE);
            RenderUtils::RenderText(PlayerNamePos, PlayerNamePadding, IM_COL32_WHITE, player, PlayerNamePadding.y * 0.015);

            if (it != Client::online.end()) {
                RenderUtils::RenderText(OnlineStatusPos, OnlineStatusPadding, ImColor(0, 255, 0), "Online", OnlineStatusPadding.y * 0.015);
            }
            else {
                RenderUtils::RenderText(OnlineStatusPos, OnlineStatusPadding, ImColor(255, 0, 0), "Offline", OnlineStatusPadding.y * 0.015);
            }

            if (Utils::IsMouseOverRectangle(Utils::mousepos, pos, ImVec2(Constraints::PercentageConstraint(0.38), Constraints::PercentageConstraint(0.05)))) {
                bool morb = Utils::IsMouseOverRectangle(Utils::mousepos, PosRemoveButton, SizeRemoveButton);
                RenderUtils::fillRect(PosRemoveButton, SizeRemoveButton, !morb ? ImColor(0.5f, 0.f, 0.f, 0.5f) : ImColor(1.f, 0.f, 0.f, 1.f), Constraints::RoundingConstraint(15));
                RenderUtils::drawRect(PosRemoveButton, SizeRemoveButton, ImColor(1.f, 0.f, 0.f, 1.f), Constraints::RoundingConstraint(15), Constraints::PercentageConstraint(0.001));
                RenderUtils::RenderImage(PosRemoveButton, SizeRemoveButton, "Lyra\\Assets\\cross.png", IM_COL32_WHITE);
                //RenderUtils::RenderText(PosRemoveButton, SizeRemoveButton, IM_COL32_WHITE, "Remove", SizeRemoveButton.y * 0.014, 2);

                if (ImGui::IsMouseClicked(0) and Utils::IsMouseOverRectangle(Utils::mousepos, PosRemoveButton, SizeRemoveButton)) {
                    RemoveFriend(player);
                }
            }

            pos.y += Constraints::PercentageConstraint(0.08);
        }

        RenderUtils::fillRect(PosAddFriendBar, SizeAddFriendBar, ImColor(0.f, 0.f, 0.f, 0.3f), Constraints::RoundingConstraint(20));

        RenderUtils::textBox(&AddFriendName, PosAddFriendTextBox, SizeAddFriendTextBox, ImColor(0, 0, 0, 0), 13);

        if (AddFriendName == "")
            RenderUtils::RenderText(PosAddFriendTextBox, SizeAddFriendTextBox, ImColor(0.75f, 0.75f, 0.75f, 1.0f), "Add Friend...", SizeTopButtons.y / 100, 1);
        bool moaf = Utils::IsMouseOverRectangle(Utils::mousepos, PosAddFriendButton, SizeAddFriendButton);
        RenderUtils::fillRect(PosAddFriendButton, SizeAddFriendButton, !moaf ? ImColor(0.f, 0.f, 0.f, 0.3f) : accent, Constraints::RoundingConstraint(20));
        RenderUtils::RenderText(PosAddFriendButton, SizeAddFriendButton, IM_COL32_WHITE, "ADD FRIEND", SizeAddFriendButton.y/100, 2);
        if(moaf){
            if (ImGui::IsMouseClicked(0) and AddFriendName != "") {
                AddFriend(AddFriendName);
                AddFriendName = "";
            }
        }
    }
    else {
        int ps = pendings.size();
        int es = std::min(ps - 1, scroll + 7);
        if (scroll+es >= ps) scroll = ps - es - 1;
        for (int i = scroll; i < ps && i <= es; i++) {
            std::string player = pendings[i];
            std::string lplayer = player;
            std::transform(lplayer.begin(), lplayer.end(), lplayer.begin(), ::tolower);
            std::transform(Search.begin(), Search.end(), Search.begin(), ::tolower);
            if (lplayer.find(Search) == std::string::npos) return;
            ImVec2 PlayerHeadPos = pos;
            ImVec2 PlayerHeadSize = ImVec2(Constraints::PercentageConstraint(0.05), Constraints::PercentageConstraint(0.05));

            ImVec2 PlayerNamePos = ImVec2(PlayerHeadPos.x + PlayerHeadSize.x + Constraints::PercentageConstraint(0.02), PlayerHeadPos.y);
            ImVec2 PlayerNamePadding = ImVec2(0, PlayerHeadSize.x/1.4);

            ImVec2 SizeAcceptButton = ImVec2(Constraints::PercentageConstraint(0.08), Constraints::PercentageConstraint(0.035));
            ImVec2 PosAcceptButton = ImVec2(Sizemenu.x - SizeAcceptButton.x - Constraints::PercentageConstraint(0.03) + MenuPosX, PlayerHeadPos.y + ((PlayerHeadSize.y / 2) - (SizeAcceptButton.y / 2)));

            RenderUtils::drawRect(ImVec2(pos.x - Constraints::PercentageConstraint(0.01), pos.y - Constraints::PercentageConstraint(0.01)), ImVec2(Constraints::PercentageConstraint(0.38), Constraints::PercentageConstraint(0.07)), ImColor(.35f, .35f, .35f, 1.f), Constraints::RoundingConstraint(15), Constraints::PercentageConstraint(0.0015));
            RenderUtils::fillRect(ImVec2(pos.x - Constraints::PercentageConstraint(0.01), pos.y - Constraints::PercentageConstraint(0.01)), ImVec2(Constraints::PercentageConstraint(0.38), Constraints::PercentageConstraint(0.07)), ImColor(.0f, .0f, .0f, .4f), Constraints::RoundingConstraint(15));
            RenderUtils::RenderImage(PlayerHeadPos, PlayerHeadSize, "Lyra\\Assets\\DefaultPlayerHead.png", IM_COL32_WHITE);
            RenderUtils::RenderText(PlayerNamePos, PlayerNamePadding, IM_COL32_WHITE, player, PlayerNamePadding.y * 0.015);

            RenderUtils::fillRect(PosAcceptButton, SizeAcceptButton, !Utils::IsMouseOverRectangle(Utils::mousepos, PosAcceptButton, SizeAcceptButton) ? ImColor(0.f, .5f, 0.f, 0.5f) : ImColor(0.f, 1.f, 0.f, 1.f), Constraints::RoundingConstraint(15));
            RenderUtils::drawRect(PosAcceptButton, SizeAcceptButton, ImColor(0.f, 1.f, 0.f, 1.f), Constraints::RoundingConstraint(15), Constraints::PercentageConstraint(0.002));
            RenderUtils::RenderText(PosAcceptButton, SizeAcceptButton, IM_COL32_WHITE, "Accept", SizeAcceptButton.y * 0.015, 2);

            if (ImGui::IsMouseClicked(0) and Utils::IsMouseOverRectangle(Utils::mousepos, PosAcceptButton, SizeAcceptButton)) {
                AcceptFriend(player);
                for (auto it = this->pendings.begin(); it < this->pendings.end(); it++) {
                    if (*it == player) {
                        this->pendings.erase(it);
                    }
                }
            }

             pos.y += Constraints::PercentageConstraint(0.08);
        }
    }
}