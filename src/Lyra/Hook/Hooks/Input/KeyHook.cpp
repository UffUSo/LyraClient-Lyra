#include <iostream>
#include "KeyHook.hpp"
#include "../../../Client.hpp"
#include "../../../Module/ModuleManager.hpp"
#include "../../../Event/EventManager.hpp"
#include "../../../../Utils/RenderUtils.h"
#include "../../../Module/Modules/FriendSystem/FriendSystem.hpp"
#include "../../../Module/Modules/MenuSelector/MenuSelector.hpp"
#include "../../../SDK/SDK.hpp"
#include "../../../Module/Modules/Notif.hpp"
#include "../../../../Utils/SHA256.h"
#include "../../../../Utils/base64.hpp"

KeyHook::KeyHook() : Hook("key_hook", "48 ?? ?? ?? 0F B6 ?? 4C ?? ?? ?? ?? ?? ?? 89 ?? ?? ?? 88")
{
}

void KeyHook::enableHook()
{
	this->autoHook((void*)keyCallback, (void**)&func_original);
}

std::string removeCharFromString(const std::string& inputString, char charToRemove) {
    std::string result;
    for (char c : inputString) {
        if (c != charToRemove) {
            result += c;
        }
    }
    return result;
}

void KeyHook::keyCallback(int key, bool state)
{
	if (state == 1) {
		if (RenderUtils::anyActivetextbox()) RenderUtils::key = key;
		if (RenderUtils::anyKeybindActive()) RenderUtils::KeybindKey = key;
		if (key == 16) RenderUtils::Capital = true;
		if (key == 8) RenderUtils::BackSpace = true;
	}
	else {
		if (key == 16) RenderUtils::Capital = false;
		if (key == 8) RenderUtils::BackSpace = false;
	}
	KeyEvent event = KeyEvent(key, state);
	if (key == Settings::getSettingByName<int>("Mod Menu", "KEYBIND")->value or key == VK_ESCAPE)
		if (RenderUtils::CloseMenu) {
			if (!state and !FriendSys.enabled) {
				if (key == VK_ESCAPE) {
					moduleMgr.getModuleG("Mod Menu")->setEnabled(false);
					if (menuSelector.enabled) {
						menuSelector.toggle();
						menuSelector.hideUI = false;
					}
				}
				else if (!RenderUtils::anyActivetextbox()) { // !RenderUtils::anyKeybindActive() NEEDS FIXING (keybind needs to be set inactive if menu is closed)
					//moduleMgr.getModuleG("Mod Menu").get()->toggle();
					if(!moduleMgr.getModuleG("Mod Menu").get()->isEnabled()) menuSelector.toggle();
					else moduleMgr.getModuleG("Mod Menu").get()->toggle();
				}
				Settings::SaveSettings();
			}
		}
		else {
			RenderUtils::CloseMenu = true;
		}
	if (!moduleMgr.getModuleG("Mod Menu")->isEnabled() and !menuSelector.enabled and (key == Settings::getSettingByName<int>("Mod Menu", "Friend List Keybind")->value or key == VK_ESCAPE) and !state and !RenderUtils::anyActivetextbox() and !RenderUtils::anyKeybindActive()) {
        std::string badthing = "�";
        if(SDK::clientInstance && SDK::clientInstance->getLocalPlayer()) Logger::info(removeCharFromString(base64::to_base64(Utils::removeColorCodes(SDK::clientInstance->getLocalPlayer()->playerName)), *badthing.c_str()));
        if (key == VK_ESCAPE) {
			FriendSys.enabled = false;
            FriendSys.onDisable();
		}
		else {
            std::thread friendThread([]() {
                moduleMgr.HeartBeatFriends();

            });
            friendThread.detach();

			FriendSys.toggle();
		}
	}
	eventMgr.onKey(event);
	if (!event.isCancelled())
		func_original(key, state);

}