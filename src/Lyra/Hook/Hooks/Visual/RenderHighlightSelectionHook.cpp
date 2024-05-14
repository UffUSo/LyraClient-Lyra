#include <iostream>
#include "RenderHighlightSelectionHook.hpp"
#include "../../../Module/ModuleManager.hpp"
#include "../../../Config/ConfigManager.hpp"
#include "../../../../Utils/RenderUtils.h"

RenderHighlightSelectionHook::RenderHighlightSelectionHook() : Hook("RenderHighlightSelectionHook", "48 8B C4 55 53 56 57 41 56 41 57 48 8D 68 ? 48 81 EC ? ? ? ? 0F 29 70 ? 0F 29 78 ? 44 0F 29 ? ? 44 0F 29 ? ? 48 8B 05")
{
}

void RenderHighlightSelectionHook::enableHook()
{
	this->autoHook((void*)RenderHighlightSelectionHookCallback, (void**)&func_original);
}

void RenderHighlightSelectionHook::RenderHighlightSelectionHookCallback(void* _this, void* screenContext, void* block, void*& region, Vec3 const& pos)
{
    auto mod = moduleMgr.getModuleG("Block Outline");
    bool enabled = false;// Settings::getSettingByName<bool>("Block Outline", "enabled")->value;

    static auto color = Memory::offsetFromSig(Memory::findSig("0F 10 05 ? ? ? ? 0F 11 00 C6 40 10 ? 0F 57 C9 0F 11 4D"), 3);

    if (enabled) {

        ImColor col = RenderUtils::VecToImColour(Settings::getSettingByName<std::vector<float>>("Block Outline", "enabledColour")->value);

        MCCColor c;
        c.r = col.Value.x;
        c.g = col.Value.y;
        c.b = col.Value.z;
        c.a = 1.f;

        Memory::patchBytes((void*)color, &c, 0x10);
    }

    func_original(_this, screenContext, block, region, pos);

    if (enabled) {
        static MCCColor black(0.65f, 0.65f, 0.65f, 1.f);

        Memory::patchBytes((void*)color, &black, 0x10);
    }
}