#include "MenuSelector.hpp"
#include "../../../SDK/SDK.hpp"
#include "../../../../Utils/RenderUtils.h"
#include "../../ModuleManager.hpp"
#include "../Profiles.hpp"
#include "../FriendSystem/FriendSystem.hpp"
#include <filesystem>

float blur = 0;

float ModsSizeX = 0;

float ProfilesSizeY = 0;

float FriendsSizeY = 0;

float ResetSizeY = 0;

ImVec2 SC;
float IconSize = 0;
float bSize;
float gap;

std::string path = getenv("AppData") + (std::string)"\\..\\Local\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe\\RoamingState\\Lyra\\Config\\current_config.json";

void MenuSelector::toggle() {
    enabled = !enabled;
    if(enabled) onEnable();
}

void MenuSelector::onEnable() {
    SC = ImVec2(Constraints::PercentageConstraint(.5, 2), Constraints::PercentageConstraint(.5));
    bSize = Constraints::PercentageConstraint(.05);
    gap = bSize/2;
    if (SDK::TopScreen.rfind("hud_screen") != std::string::npos) {
        blur = 0;

        if (SDK::clientInstance) {
            SDK::clientInstance->releaseMouse();
        }

        eventMgr.addListener(this);
    }
    else {
        this->enabled = false;
    }
}

void MenuSelector::onKey(KeyEvent &event) {
    if(enabled) event.cancel();
}

void MenuSelector::onMouse(MouseEvent &event) {
    if(enabled) event.cancel();
}

void MenuSelector::onRender(const RenderEvent &event) {
    if(!SDK::clientInstance || !SDK::clientInstance->getLocalPlayer()) return;
    if (!(SDK::TopScreen.rfind("hud_screen") != std::string::npos)) return;
    float animationSpeed = Settings::getSettingByName<float>("Mod Menu", "ANIMATION SPEED")->value;
    if (enabled) {
        blur = Utils::animate(10, blur, MC::Deltatime * 0.15 * animationSpeed);
        ModsSizeX = Utils::animate(bSize*3+4*gap, ModsSizeX, MC::Deltatime * 0.15 * animationSpeed);
        IconSize = Utils::animate(bSize*3/1.5, IconSize, MC::Deltatime * .15 * animationSpeed);
        if (IconSize >= (bSize*3/1.5-10)) {
            FriendsSizeY = Utils::animate(bSize, FriendsSizeY, MC::Deltatime * .15 * animationSpeed);
            if (FriendsSizeY >= bSize-5) ProfilesSizeY = Utils::animate(bSize, ProfilesSizeY, MC::Deltatime * .15 * animationSpeed);
            if (ProfilesSizeY >= bSize-5) ResetSizeY = Utils::animate(bSize, ResetSizeY, MC::Deltatime * .15 * animationSpeed);
        }
    }
    else {
        blur = Utils::animate(-1, blur, MC::Deltatime * 0.15 * animationSpeed);
        ModsSizeX = Utils::animate(-(bSize*3+4*gap), ModsSizeX, MC::Deltatime * 0.15 * animationSpeed);
        IconSize = Utils::animate(-bSize*3/1.5, IconSize, MC::Deltatime * .15 * animationSpeed);
        FriendsSizeY = Utils::animate(-bSize, FriendsSizeY, MC::Deltatime * .15 * animationSpeed);
        ProfilesSizeY = ResetSizeY = FriendsSizeY;
        if (blur <= 0 || ModsSizeX <= 0 || ProfilesSizeY <= 0 || FriendsSizeY <= 0 || ResetSizeY <= 0){
            eventMgr.removeListener(this);
            if (SDK::TopScreen.rfind("hud_screen") != std::string::npos) {
                if (SDK::clientInstance) {
                    SDK::clientInstance->grabMouse();
                }
            }
            return;
        }
    }
    ImColor accent = RenderUtils::VecToImColour(Settings::getSettingByName<std::vector<float>>("Mod Menu", "ACCENT COLOR")->value);

    ImVec2 ModsPos(SC.x-ModsSizeX/2, SC.y-bSize-gap/2);
    ImVec2 ModsSize(ModsSizeX, bSize);

    ImVec2 FriendsPos(SC.x-gap-bSize/2-bSize, SC.y+gap/2);
    ImVec2 FriendsSize(bSize, FriendsSizeY);

    ImVec2 ProfilesPos(SC.x-bSize/2, SC.y+gap/2);
    ImVec2 ProfilesSize(bSize, ProfilesSizeY);

    ImVec2 ResetPos(SC.x+gap+bSize/2, SC.y+gap/2);
    ImVec2 ResetSize(bSize, ResetSizeY);

    RenderUtils::Blur(blur);
    if(!this->hideUI){
        RenderUtils::RenderImage(ImVec2(SC.x-IconSize/2, SC.y-(gap/2+bSize+(bSize*3/2))-IconSize/2), ImVec2(IconSize, IconSize), "Lyra\\Assets\\Lyra.png", IM_COL32_WHITE);
        RenderUtils::RenderImage(ImVec2(SC.x-IconSize/2, SC.y-(gap/2+bSize+(bSize*3/2))-IconSize/2), ImVec2(IconSize, IconSize), "Lyra\\Assets\\star.png", accent);
        if(ModsSizeX > 0){
            RenderUtils::fillRect(ModsPos, ModsSize, Utils::IsMouseOverRectangle(Utils::mousepos, ModsPos, ModsSize) ? accent : ImColor(.0f, .0f, .0f, .75f), Constraints::RoundingCalc(ImVec2(ModsSizeX, bSize),.5));
            RenderUtils::RenderTextWithOutline(ModsPos, ModsSize, IM_COL32_WHITE, "MOD MENU", ModsSize.y / 100, IM_COL32_WHITE, .5, 2);
        }
        if(FriendsSizeY > 0){
            RenderUtils::fillRect(FriendsPos, FriendsSize, Utils::IsMouseOverRectangle(Utils::mousepos, FriendsPos, FriendsSize) ? accent : ImColor(.0f, .0f, .0f, .75f), Constraints::RoundingCalc(ImVec2(bSize, FriendsSizeY), .5));
            RenderUtils::RenderImage(ImVec2(FriendsPos.x, FriendsPos.y), ImVec2(FriendsSize.x, FriendsSize.y), "Lyra\\Assets\\friends_menu.png", IM_COL32_WHITE);
        }
        if(ProfilesSizeY > 0){
            RenderUtils::fillRect(ProfilesPos, ProfilesSize, Utils::IsMouseOverRectangle(Utils::mousepos, ProfilesPos, ProfilesSize) ? accent : ImColor(.0f, .0f, .0f, .75f), Constraints::RoundingCalc(ImVec2(bSize, ProfilesSizeY), .5));
            RenderUtils::RenderImage(ImVec2(ProfilesPos.x, ProfilesPos.y), ImVec2(ProfilesSize.x, ProfilesSize.y), "Lyra\\Assets\\load_profile.png", IM_COL32_WHITE);
        }
        if(ResetSizeY > 0){
            //RenderUtils::fillRect(ResetPos, ResetSize, Utils::IsMouseOverRectangle(Utils::mousepos, ResetPos, ResetSize) ? ImColor(200, 50, 50, 255) : ImColor(.0f, .0f, .0f, .75f), Constraints::RoundingCalc(ImVec2(bSize, ResetSizeY), .5));
            RenderUtils::fillRect(ResetPos, ResetSize, ImColor(.0f, .0f, .0f, .75f), Constraints::RoundingCalc(ImVec2(bSize, ResetSizeY), .5)); // will change later
            RenderUtils::RenderImage(ImVec2(ResetPos.x, ResetPos.y), ImVec2(ResetSize.x, ResetSize.y), "Lyra\\Assets\\reset_config.png", ImColor(100, 100, 100, 255)); // will change later
        }
        if (ImGui::IsMouseClicked(0)) {
            if(Utils::IsMouseOverRectangle(Utils::mousepos, ModsPos, ModsSize)) {
                toggle();
                moduleMgr.getModuleG("Mod Menu").get()->toggle();
            }
            if (Utils::IsMouseOverRectangle(Utils::mousepos, ProfilesPos, ProfilesSize)) {
                LoadUI = true;
                this->hideUI = true;
            }
            if (Utils::IsMouseOverRectangle(Utils::mousepos, FriendsPos, FriendsSize)) {
                toggle();
                FriendSys.toggle();
            }
            if (Utils::IsMouseOverRectangle(Utils::mousepos, ResetPos, ResetSize)) {
                // Do this thing pls
            }
        }
    }
}

MenuSelector menuSelector;