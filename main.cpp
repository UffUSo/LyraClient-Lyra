#include <iostream>
#include <windows.h>
#include <thread>
#include <wininet.h>
#include "src/Lyra/Client.hpp"
#include "src/Utils/Logger/Logger.hpp"
#include "src/Lyra/Config/Json/json.hpp"
#include "src/Lyra/SDK/SDK.hpp"
#include "src/Lyra/Config/ConfigManager.hpp"
#include "src/Lyra/Module/Modules/Tablist.hpp"
#include "src/Utils/RenderUtils.h"
#include "src/Lyra/Module/Modules/ModMenu.hpp"
#include "src/Lyra/Module/Modules/Notif.hpp"

DWORD WINAPI initialize(HMODULE instance){

    std::chrono::steady_clock::time_point lastBeatTime;

    if (GetConsoleWindow() == nullptr and true) {
        AllocConsole();
        SetConsoleTitleA("Lyra-Debug");
        FILE *out;
        freopen_s(&out, ("CONOUT$"), ("w"), stdout);
    }
    Client::initialize();


    ImGui::ShowDemoWindow();



    std::string json_string = Utils::DownloadString("http://node2.synthetix.host:5004/blocked");
    try {
        json j = json::parse(json_string);

        for (json::iterator it = j.begin(); it != j.end(); ++it) {
            const std::string& key = it.key();
            std::vector<std::string> v = j[key];
            ModMenu::blocked_modules[key] = v;
        }
    } catch (json::parse_error& e) {
        Logger::info(e.what());

    }

    std::thread statusThread([&lastBeatTime]() {
        while (true) {
            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - lastBeatTime);

            if (!Client::disable) {
                if (SDK::clientInstance != nullptr) {
                    if (SDK::clientInstance->getLocalPlayer() != nullptr && SDK::clientInstance->getLocalPlayer()->playerName != "") {
                        if (elapsed >= std::chrono::seconds(60)) {
                            Client::online.clear();
                            Client::onlineUsers.clear();
                            Client::onlineStaff.clear();
                            Client::onlineStaffp.clear();
                            Client::partners.clear();
                            std::string name = SDK::clientInstance->getLocalPlayer()->playerName;
                            //Client::onlineUsers.push_back(Utils::removeColorCodes(name));

                            std::string ipToSend;

                            Logger::info(Utils::DownloadString(xorstr("http://node2.synthetix.host:5004/heartbeat/") + SDK::getServerIP() + "/" + Utils::removeColorCodes(name)));
                            std::string pp = Utils::DownloadString(xorstr("http://node2.synthetix.host:5004/users"));
                            if(pp != "") {
                                json playersDict = json::parse(pp);

                                int totalPlaytime = 0;
                                int numberOfPlayers = 0;

                                for (const auto &player: playersDict.items()) {

                                    std::time_t unixTimestamp = player.value()["lastbeat"];
                                    std::time_t timePoint = std::chrono::system_clock::to_time_t(std::chrono::system_clock::from_time_t(unixTimestamp));
                                    Client::online.push_back(Utils::removeColorCodes(player.key()));
                                    std::string icon = player.value()["icon"];
                                    Client::PlayerServerList[Utils::removeColorCodes(player.key())] = player.value()["server"];
                                    if (icon == "regular") {
                                        Client::onlineUsers.push_back(Utils::removeColorCodes(player.key()));
                                    }
                                    if (icon == "staff") {

                                        Client::onlineStaff.push_back(Utils::removeColorCodes(player.key()));
                                    }
                                    if (icon == "staffplus") {

                                        Client::onlineStaffp.push_back(Utils::removeColorCodes(player.key()));
                                    }
                                    if (icon == "partner") {

                                        Client::partners.push_back(Utils::removeColorCodes(player.key()));
                                    }


                                }
                                Client::HeartBeatFriend();
                            }else{
                                notifs.push_back("Error while trying to connect to the api");
                            }
                            lastBeatTime = now;
                        }
                    }
                }
                Sleep(60);

            }
            else break;
        }
    });

    while (!Client::disable) {
        Sleep(50);
    }
    Client::UnInitialize();
    statusThread.detach();
    FreeLibraryAndExitThread(instance, 1);
}

BOOL APIENTRY DllMain(HMODULE instance, DWORD reason, LPVOID lpReserved)
{
    switch (reason)
    {
    case DLL_PROCESS_ATTACH: {
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)initialize, instance, 0, 0);
        return TRUE;
        }

    case DLL_PROCESS_DETACH: {
        Logger::info("Saved Config");
        return TRUE;
    }
    }

    return TRUE;
}
