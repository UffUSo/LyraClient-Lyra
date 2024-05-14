#include "ShinyPots.hpp"
#include "../../../Utils/RenderUtils.h"
#include "../../SDK/SDK.hpp"

void SetGlint(bool glint) {
    auto inventory = SDK::clientInstance->getLocalPlayer()->getSupplies()->getmContainer();
	if (inventory != nullptr) {
		for (int i = 0; i < 36; i++) {
			auto item = inventory->getitem(i);
			if (item->getItem() != nullptr) {
				if (item->getItem()->name == "splash_potion") {
					item->getItem()->Glint = glint;
				}
			}
		}
	}
}

ShinyPots::ShinyPots() : ModuleCtor(0, "Shiny Shiny", "ShinyPots") {
    this->InitModuleSettings();
}

std::string ShinyPots::getModuleName() {
    return "Shiny Pots";
}

void ShinyPots::InitModuleSettings() {
    Settings::addSetting<bool>("Shiny Pots", "enabled", false, true);
}

void ShinyPots::onEnable() {
    Settings::getSettingByName<bool>("Shiny Pots", "enabled")->value = true;
    eventMgr.addListener(this);
}

void ShinyPots::onDisable() {
    Settings::getSettingByName<bool>("Shiny Pots", "enabled")->value = false;
    eventMgr.removeListener(this);
    if(!SDK::clientInstance || !SDK::clientInstance->getLocalPlayer()) return;
    SetGlint(false);
}

void ShinyPots::onTick(const TickEvent &event) {
    if(!SDK::clientInstance || !SDK::clientInstance->getLocalPlayer()) return;
    SetGlint(true);
}