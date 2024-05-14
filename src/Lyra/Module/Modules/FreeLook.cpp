#include "FreeLook.hpp"
#include "../../SDK/SDK.hpp"
#include "ModMenu.hpp"

bool shouldFreelook = false;

FreeLook::FreeLook() : ModuleCtor(67, "Freelook", "freelook") {
	this->InitModuleSettings();
    OriginalYaw1.resize(4);
    memcpy(OriginalYaw1.data(), (LPVOID)yaw1, 4);

    OriginalYaw2.resize(4);
    memcpy(OriginalYaw2.data(), (LPVOID)yaw2, 4);

    OriginalPitch.resize(4);
    memcpy(OriginalPitch.data(), (LPVOID)pitch, 4);

    Originalmovement.resize(4);
    memcpy(Originalmovement.data(), (LPVOID)movement, 4);

    PatchedYaw1.push_back(0x90);
    PatchedYaw1.push_back(0x90);
    PatchedYaw1.push_back(0x90);
    PatchedYaw1.push_back(0x90);

    PatchedYaw2.push_back(0x90);
    PatchedYaw2.push_back(0x90);
    PatchedYaw2.push_back(0x90);
    PatchedYaw2.push_back(0x90);

    PatchedPitch.push_back(0x90);
    PatchedPitch.push_back(0x90);
    PatchedPitch.push_back(0x90);
    PatchedPitch.push_back(0x90);

    Patchedmovement.push_back(0x90);
    Patchedmovement.push_back(0x90);
    Patchedmovement.push_back(0x90);
    Patchedmovement.push_back(0x90);
}


std::string FreeLook::getModuleName() {
	return "Freelook";
}

void FreeLook::InitModuleSettings() {
	Settings::addSetting<bool>("Freelook", "enabled", false, true);
    Settings::addSetting<int>("Freelook", "KEYBIND", 70, true);

	this->SettingsList["CONTROLS"]["KEYBIND"] = "keybind";
}

void FreeLook::onEnable() {
	Settings::getSettingByName<bool>("Freelook", "enabled")->value = true;
	eventMgr.addListener(this);
}

void FreeLook::onDisable() {
	Settings::getSettingByName<bool>("Freelook", "enabled")->value = false;
	eventMgr.removeListener(this);
}

void FreeLook::onKey(KeyEvent& event) {
	if (event.getKey() == Settings::getSettingByName<int>("Freelook", "KEYBIND")->value and SDK::TopScreen.rfind("hud_screen") != std::string::npos and !Settings::getSettingByName<bool>("Mod Menu", "enabled")->value) {
        shouldFreelook = event.isPressed();
	}
}

void FreeLook::onTick(const TickEvent& event) {
    if(!SDK::clientInstance || !SDK::clientInstance->getLocalPlayer()) return;
	if (shouldFreelook) {
        patch();
    }
    else {unpatch();}


}

void FreeLook::patch() const {
    Memory::patchBytes((LPVOID)yaw1, PatchedYaw1.data(), PatchedYaw1.size());

    Memory::patchBytes((LPVOID)yaw2, PatchedYaw2.data(), PatchedYaw2.size());

    Memory::patchBytes((LPVOID)pitch, PatchedPitch.data(), PatchedPitch.size());

    Memory::patchBytes((LPVOID)movement, Patchedmovement.data(), Patchedmovement.size());
}

void FreeLook::unpatch() const {
    Memory::patchBytes((LPVOID)yaw1, OriginalYaw1.data(), OriginalYaw1.size());

    Memory::patchBytes((LPVOID)yaw2, OriginalYaw2.data(), OriginalYaw2.size());

    Memory::patchBytes((LPVOID)pitch, OriginalPitch.data(), OriginalPitch.size());

    Memory::patchBytes((LPVOID)movement, Originalmovement.data(), Originalmovement.size());
}