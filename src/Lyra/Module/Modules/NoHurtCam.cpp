#include "NoHurtCam.hpp"
#include "../../SDK/SDK.hpp"

NoHurtCam::NoHurtCam() : ModuleCtor(67, "No Hurt Cam", "nohurtcam") {
	this->InitModuleSettings();
    originalCameraAngle.resize(3);
    memcpy(originalCameraAngle.data(), (LPVOID) sigOffset, 3);

}


std::string NoHurtCam::getModuleName() {
	return "No Hurt Cam";
}

void NoHurtCam::InitModuleSettings() {
	Settings::addSetting<bool>("No Hurt Cam", "enabled", false, true);
}

void NoHurtCam::onEnable() {
    if(!SDK::clientInstance || !SDK::clientInstance->getLocalPlayer()) return;
	Settings::getSettingByName<bool>("No Hurt Cam", "enabled")->value = true;
    eventMgr.addListener(this);

    originalCameraAngle.resize(3);
    memcpy(originalCameraAngle.data(), (LPVOID) sigOffset, 3);

    Memory::patchBytes((LPVOID) sigOffset, newCameraAngle, 3);
}

void NoHurtCam::onDisable() {
	Settings::getSettingByName<bool>("No Hurt Cam", "enabled")->value = false;
	eventMgr.removeListener(this);
    
    Memory::patchBytes((LPVOID) sigOffset, originalCameraAngle.data(), 3);
}