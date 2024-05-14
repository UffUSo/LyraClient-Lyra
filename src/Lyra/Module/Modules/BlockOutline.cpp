#include "BlockOutline.hpp"
#include "../../../Utils/Memory/CustomAllocator/Buffer.hpp"
#include "../../SDK/SDK.hpp"

BlockOutline::BlockOutline() : ModuleCtor(67, "Block Outline", "BlockOutline") {
	this->InitModuleSettings();
}

std::string BlockOutline::getModuleName() {
	return "Block Outline";
}

void BlockOutline::InitModuleSettings() {
    highlightColorRipRelAddr = Memory::findSig("? ? ? ? 0F 11 45 ? 0F 11 00 C6 40 10 ? 45 38 96"); // blockHighlightColor RIP REL 4BYTE FROM FUNC OFFSET ADDR
    highlightColorOrigRipRel = *(UINT32*)highlightColorRipRelAddr;

    outlineColorRipRelAddr = Memory::findSig("? ? ? ? 0F 11 00 C6 40 10 ? 0F 57 C9 0F 11 4D"); // mce::Color::BLACK
    outlineColorOrigRipRel = *(UINT32*)outlineColorRipRelAddr;

    selectionColor = (MCCColor*)AllocateBuffer((void*)highlightColorRipRelAddr); // allocate space for color
    highlightColorNewRipRel = Memory::getRipRel(highlightColorRipRelAddr, reinterpret_cast<uintptr_t>((void*)selectionColor));
    outlineColorNewRipRel= Memory::getRipRel(outlineColorRipRelAddr, reinterpret_cast<uintptr_t>((void*)selectionColor));
    *selectionColor = MCCColor{};

	Settings::addSetting<bool>(getModuleName(), "enabled", false, true);
	Settings::addSetting<std::vector<float>>(getModuleName(), "Outline Color", std::vector<float>{1, 1, 1, 1}, true);
    this->SettingsList["COLOR"]["Outline Color"] = "colorpicker";
}

void BlockOutline::onEnable() {
	Settings::getSettingByName<bool>(getModuleName(), "enabled")->value = true;
    Memory::patchBytes((void *) highlightColorRipRelAddr, highlightColorNewRipRel.data(), sizeof(UINT32));
    Memory::patchBytes((void *) outlineColorRipRelAddr, outlineColorNewRipRel.data(), sizeof(UINT32));
	eventMgr.addListener(this);
}

void BlockOutline::onDisable() {
	Settings::getSettingByName<bool>(getModuleName(), "enabled")->value = false;
    Memory::patchBytes((void *) highlightColorRipRelAddr, &highlightColorOrigRipRel, sizeof(UINT32));
    Memory::patchBytes((void *) outlineColorRipRelAddr, &outlineColorOrigRipRel, sizeof(UINT32));
	eventMgr.removeListener(this);
}

void BlockOutline::onTick(const TickEvent& event) {
    if(!SDK::clientInstance || !SDK::clientInstance->getLocalPlayer()) return;
    auto color = Settings::getSettingByName<std::vector<float>>(getModuleName(), "Outline Color")->value;

    selectionColor->r = color.at(0);
    selectionColor->g = color.at(1);
    selectionColor->b = color.at(2);
    selectionColor->a = color.at(3);
}
