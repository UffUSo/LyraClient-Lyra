#pragma once
#include "../../Utils/Memory/Memory.hpp"
#include "../../Utils/Logger/Logger.hpp"
#include "Hooks/Hook.hpp"
#include <vector>

class HookManager {
public:
    static void initialize();
    static void terminate();
    static std::vector<Hook*> hooks;
};

