#include "Hitbox.hpp"
#include "../../../Utils/RenderUtils.h"
#include "../../SDK/SDK.hpp"
#include "../../../Utils/TessUtils/TessUtils.hpp"
#include "../../../Utils/Vector.hpp"

Hitbox::Hitbox() : ModuleCtor(0, "Hitbox", "Hitbox") {
	this->InitModuleSettings();
}

std::string Hitbox::getModuleName() {
	return "Hitbox";
}

void Hitbox::InitModuleSettings() {
	Settings::addSetting<bool>("Hitbox", "enabled", false, true);
	Settings::addSetting<std::vector<float>>("Hitbox", "Color", std::vector<float>{1, 1, 1, 1}, true);

	this->SettingsList["COLOR"]["Color"] = "colorpicker";
}

void Hitbox::onEnable() {
	Settings::getSettingByName<bool>("Hitbox", "enabled")->value = true;
	eventMgr.addListener(this);
}

void Hitbox::onDisable() {
	Settings::getSettingByName<bool>("Hitbox", "enabled")->value = false;
	eventMgr.removeListener(this);
}

void Hitbox::onRenderLevel(const RenderLevelEvent& event) {
	if(!SDK::clientInstance || !SDK::clientInstance->getLocalPlayer()) return;
	if (SDK::clientInstance->getLocalPlayer()) {
		TessUtils::begin(VertexFormat::LINE_LIST);
		for (auto p : SDK::clientInstance->getLocalPlayer()->getlevel()->getRuntimeActorList()) {
            if (p != nullptr && p != SDK::clientInstance->getLocalPlayer()) {
                if (p->getActorFlag(ActorFlags::FLAG_INVISIBLE))
                    continue;
                AABB aabb = p->getAABBShapeComponent()->aabb;
                std::vector<float> col = Settings::getSettingByName<std::vector<float>>("Hitbox", "Color")->value;
                TessUtils::addBox3D(aabb.lower, aabb.upper, 1, mce::Color(col.at(0), col.at(1), col.at(2), col.at(3)));
            }
		}
		TessUtils::render();
	}
}
