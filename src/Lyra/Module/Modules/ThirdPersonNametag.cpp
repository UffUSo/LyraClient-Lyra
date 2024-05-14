#include "ThirdPersonNametag.hpp"
#include "../../SDK/SDK.hpp"

ThirdPersonNametag::ThirdPersonNametag() : ModuleCtor(67, "Nametag", "nametag") {
	this->InitModuleSettings();
}


std::string ThirdPersonNametag::getModuleName() {
	return "Nametag";
}

void ThirdPersonNametag::InitModuleSettings() {
	Settings::addSetting<bool>("Nametag", "enabled", false, true);
}

static char buf[6];
static void* addr = nullptr;

void ThirdPersonNametag::onEnable() {
	Settings::getSettingByName<bool>("Nametag", "enabled")->value = true;

    static auto sig = Memory::findSig("0F 84 ? ? ? ? 49 8B D6 48 8B CE E8 ? ? ? ? 84 C0 0F 84");

    if (addr == nullptr)
        addr = reinterpret_cast<void*>(sig);

    Memory::copyBytes(addr, buf, 6);
    Memory::nopBytes(addr, 6);
}

void ThirdPersonNametag::onDisable() {
    if(!SDK::clientInstance || !SDK::clientInstance->getLocalPlayer()) return;
	Settings::getSettingByName<bool>("Nametag", "enabled")->value = false;
    Memory::patchBytes(addr, buf, 6);
}