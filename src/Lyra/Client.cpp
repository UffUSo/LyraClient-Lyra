#pragma once
#include "Client.hpp"
#include <kiero.h>
#include <filesystem>
#include <thread>
#include <wingdi.h>
#include <wininet.h>
#define _WIN32_WINNT _WIN32_WINNT_WIN10
#include <initguid.h>
#include <windows.h>
#include <appmodel.h>
#include <shobjidl.h>
#include <cstdlib>
#include "Config/ConfigManager.hpp"
#include "Module/ModuleManager.hpp"
#include "Hook/Manager.hpp"

void DownloadFile(std::string fname, bool png) {
    if (!std::filesystem::exists(Utils::getRoamingPath() + "\\Lyra\\assets\\" + fname + (png ? ".png" : "")))
    {
        std::string from = "http://51.255.80.24:1900/Launcher/Assets/" + fname +  + (png ? ".png" : "");
        std::string file = Utils::getRoamingPath() + "\\Lyra\\assets\\" + fname +  + (png ? ".png" : "");
        URLDownloadToFileA(NULL, from.c_str(), file.c_str(), 0, NULL);
    }
}

void Client::initialize(){
    std::filesystem::path folder_path(Utils::getRoamingPath() + "\\Lyra");
    if (!exists(folder_path))
    {
        create_directory(folder_path);
    }

    std::filesystem::path folder_path2(Utils::getRoamingPath() + "\\Lyra\\assets");
    if (!exists(folder_path2))
    {
        create_directory(folder_path2);
    }

    std::filesystem::path folder_path3(Utils::getRoamingPath() + "\\Lyra\\logs");
    if (!exists(folder_path3))
    {
        create_directory(folder_path3);
    }

    std::filesystem::path folder_path4(Utils::getRoamingPath() + "\\Lyra\\Config");
    if (!exists(folder_path4))
    {
        create_directory(folder_path4);
    }
    std::vector<std::string> AssetFiles = std::vector<std::string>{
        "ArmourHUD",
        "ArrowCounter",
        "BlockOutline",
        "clock",
        "ComboCounter",
        "coordinates",
        "cps",
        "cross",
        "fog color",
        "fps",
        "freelook",
        "Fullbright",
        "HurtColor",
        "IPDisplay",
        "keystroke",
        "Lyra",
        "momentum",
        "MotionBlur",
        "nametag",
        "nohurtcam",
        "ping",
        "Hitbox",
        "playtime",
        "potcounter",
        "ReachDisplay",
        "search",
        "settings",
        "ShinyPots",
        "snaplook",
        "Soupcounter",
        "star",
        "tablist",
        "togglesprint",
        "WeatherChanger",
        "zoom",
        "CPS_Limiter",
        "FoodPoints",
        "add",
        "Profiles",
        "DefaultPlayerHead",
        "hivehelper",
        "Partner",
        "friends_menu",
        "load_profile",
        "reset_config"
    };

    for (std::string fname : AssetFiles) {
        DownloadFile(fname, true);
    }
    DownloadFile("mojangles.ttf", false);
    Settings::load();
    std::vector<std::thread> threads;

    Logger::initialize();

    threads.emplace_back(&ModuleManager::initModules, &moduleMgr);

    for (auto& t : threads) {
        t.join();
    }
    HookManager::initialize();
    Logger::info("Init");

    moduleMgr.EnabledOnStart();

    disable = false;
}

void Client::UnInitialize() {
    kiero::shutdown();
    MH_DisableHook(MH_ALL_HOOKS);
    MH_Uninitialize();
}

void Client::HeartBeatFriend() {
    moduleMgr.HeartBeatFriends();
}