#pragma once
#include "../Utils/Utils.hpp"
#include "../Utils/Logger/Logger.hpp"
#include "../../lib/include/XorStr.h"
#include <filesystem>
#include <string>
#include <iostream>
#include <windows.h>
#include <chrono>
#include <MinHook.h>
#include <map>

class Client
{
public:
    static inline std::vector<std::string> onlineUsers;
    static inline std::vector<std::string> onlineStaff;
    static inline std::vector<std::string> onlineStaffp;
    static inline std::vector<std::string> online;
    static inline std::map<std::string, std::string> PlayerServerList;
    static inline std::vector<std::string> partners;
    static void initialize();
    static inline bool disable;
    static void UnInitialize();
    static void HeartBeatFriend();
};