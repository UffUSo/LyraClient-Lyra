#include "CustomChat.hpp"
#include "../../SDK/SDK.hpp"
#include "../../../Utils/RenderUtils.h"

ImVec2 posOriginal = ImVec2();
std::vector<Messages*> DisplayMessages = {};

CustomChat::CustomChat() : ModuleCtor(67, "CustomChat", "CustomChat") {
	this->InitModuleSettings();
}

std::string CustomChat::getModuleName() {
	return "CustomChat";
}

std::map<std::string, std::string> ColorMap = {};

void CustomChat::InitModuleSettings() {
    ColorMap["§0"] = "black";
    ColorMap["§1"] = "dark blue";
    ColorMap["§2"] = "dark green";
    ColorMap["§3"] = "dark aqua";
    ColorMap["§4"] = "dark red";
    ColorMap["§5"] = "dark purple";
    ColorMap["§6"] = "gold";
    ColorMap["§7"] = "gray";
    ColorMap["§8"] = "dark gray";
    ColorMap["§9"] = "blue";
    ColorMap["§a"] = "green";
    ColorMap["§b"] = "aqua";
    ColorMap["§c"] = "red";
    ColorMap["§d"] = "light purple";
    ColorMap["§e"] = "yellow";
    ColorMap["§f"] = "white";
	Settings::addSetting<bool>("CustomChat", "enabled", false, true);
    Settings::addSetting<float>("CustomChat", "PosX", 0, true);
    Settings::addSetting<float>("CustomChat", "PosY", 0.6, true);
    Settings::addSetting<float>("CustomChat", "size", 1, true);
}

void CustomChat::onEnable() {
	if(!SDK::clientInstance || !SDK::clientInstance->getLocalPlayer()) return;
	Settings::getSettingByName<bool>("CustomChat", "enabled")->value = true;
	eventMgr.addListener(this);
}

void CustomChat::onDisable() {
	Settings::getSettingByName<bool>("CustomChat", "enabled")->value = false;
	eventMgr.removeListener(this);
}

void CustomChat::onPreMCRender(const SetupAndRenderEvent& event) {
    SDK::screenView->VisualTree->root->forEachControl([this](std::shared_ptr<UIControl>& control) {
    if(control->LayerName == "chat_stack" or control->LayerName == "chat_panel"){
        if (posOriginal.y == 0){
            posOriginal.x = control->x;
            posOriginal.y = control->y;
        }
        control->x = Constraints::PercentageConstraint(1, 2);
        control->y = Constraints::PercentageConstraint(1, 1);
    }
    });
}

void CustomChat::onRender(const RenderEvent &event) {
    if (!(SDK::TopScreen.rfind("hud_screen") != std::string::npos)) {
        return;
    }
    float SizeModifier = Settings::getSettingByName<float>(this->getModuleName(), "size")->value;
    std::string name = this->getModuleName();
    this->pos = ImVec2(Settings::getSettingByName<float>(this->getModuleName(), "PosX")->value, Settings::getSettingByName<float>(this->getModuleName(), "PosY")->value);
    ImVec2 uiSize = ImGui::GetIO().DisplaySize;
    ImVec2 pos = ImVec2(this->pos.x * uiSize.x, this->pos.y * uiSize.y);
    ImVec2 size = ImVec2(Constraints::PercentageConstraint(0.6 * SizeModifier), Constraints::PercentageConstraint(0.3 * SizeModifier));
    ImVec2 SizeEachMessage = ImVec2(size.x, Constraints::PercentageConstraint(0.03 * SizeModifier));
    ImVec2 rzPos = ImVec2(pos.x + size.x - 5, pos.y + size.y - 5);

    float MessageNextPos = pos.y;

    for (Messages* mes : DisplayMessages){
        std::chrono::duration<double> s = std::chrono::high_resolution_clock::now() - mes->timestamp;
        if (s.count() > 7) continue;

        RenderUtils::fillRect(ImVec2(pos.x, pos.y + MessageNextPos), SizeEachMessage, ImColor(0.0f, 0.0f, 0.0f, 0.5f), 0);

        std::vector<std::string> messages;
        std::string mm = "";
        for (char ch : mes->message) {
            if (ch == '§' and mm != "") {
                messages.push_back(mm);
                mm = "";
            }
            mm  = mm + ch;
        }

        if (mm != "") {
            messages.push_back(mm);
        }

        float lastMessageSize = 0;


        for (std::string message : messages) {
            ImColor col = ImColor(255, 255, 255, 255);
            if (message.starts_with("§0")) {
                col = ImColor(255, 255, 255);
            }
            else if (message.starts_with("§1")) {
                col = ImColor(0, 0, 170);
            }
            else if (message.starts_with("§2")) {
                col = ImColor(0, 170, 0);
            }
            else if (message.starts_with("§3")) {
                col = ImColor(0, 170, 170);
            }
            else if (message.starts_with("§4")) {
                col = ImColor(170, 0, 0);
            }
            else if (message.starts_with("§5")) {
                col = ImColor(170, 0, 170);
            }
            else if (message.starts_with("§6")) {
                col = ImColor(255, 170, 0);
            }
            else if (message.starts_with("§7")) {
                col = ImColor(170, 170, 170);
            }
            else if (message.starts_with("§8")) {
                col = ImColor(85, 85, 85);
            }
            else if (message.starts_with("§a")) {
                col = ImColor(85, 255, 85);
            }
            else if (message.starts_with("§9")) {
                col = ImColor(85, 85, 255);
            }
            else if (message.starts_with("§b")) {
                col = ImColor(85, 255, 255);
            }
            else if (message.starts_with("§c")) {
                col = ImColor(255, 85, 85);
            }
            else if (message.starts_with("§d")) {
                col = ImColor(255, 85, 255);
            }
            else if (message.starts_with("§e")) {
                col = ImColor(255, 255, 85);
            }
            else if (message.starts_with("§f")) {
                col = ImColor(255, 255, 255);
            }

            lastMessageSize += RenderUtils::RenderText(ImVec2(pos.x + lastMessageSize, pos.y + MessageNextPos), SizeEachMessage, col, message, SizeEachMessage.y * 0.015).z;
        }
        MessageNextPos += SizeEachMessage.y;
    }
}