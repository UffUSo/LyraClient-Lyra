#include "ModMenu.hpp"
#include "../../../Utils/RenderUtils.h"
#include "../ModuleManager.hpp"
#include "../../SDK/SDK.hpp"
float scroll = 0;
bool scroll_max = false;
bool Sdragging = false;
std::string searchtext = "";
std::string currentSetting = "";

std::string SDK::getServerIP() {
    if (SDK::clientInstance != nullptr) {
        if (SDK::clientInstance->getLocalPlayer() != nullptr) {
            std::string ip{};
            if (SDK::clientInstance->getRakNetConnector() != nullptr) {
                ip = SDK::clientInstance->getRakNetConnector()->JoinedIp;
                if (!ip.empty()) {
                    return ip;
                }
            }
        }
    }
    return "world";
}

void RenderModSettings(std::shared_ptr<Module> mod, float posY, float posX) {
    if(!SDK::clientInstance || !SDK::clientInstance->getLocalPlayer()) return;
    if (!(SDK::TopScreen.rfind("hud_screen") != std::string::npos)) return;
    posX = Constraints::PercentageConstraint(1) - posX;
    ImVec2 Pos = ImVec2(Constraints::PercentageConstraint(0.25, 2) + Constraints::PercentageConstraint(0.04) + posX, posY);
    std::string modName = mod->getModuleName();
    int index = 1;
    bool AnyModInCat = false;
    for (auto it = mod->SettingsList.begin(); it != mod->SettingsList.end(); ++it) {
        std::string cat = it->first;
        std::map<std::string, std::string> settingsInCat = it->second;

        Pos.x = Constraints::PercentageConstraint(0.25, 2) + Constraints::PercentageConstraint(0.04) + posX;
        Pos.y += index == 1 ? 0 : Constraints::PercentageConstraint(true ? 0.05 : 0.09);

        ImVec2 PosForPostCategoryRender = Pos;

        for (auto itt = settingsInCat.begin(); itt != settingsInCat.end(); ++itt) {
            std::string name = itt->first;
            std::string type = itt->second;

            std::string SettingNameSearch = name;


            std::string search = searchtext;
            std::transform(SettingNameSearch.begin(), SettingNameSearch.end(), SettingNameSearch.begin(), ::tolower);
            std::transform(search.begin(), search.end(), search.begin(), ::tolower);
            if (SettingNameSearch.find(search) > 0) {
                continue;
            }
            if (!AnyModInCat)
                Pos.y += Constraints::PercentageConstraint(0.06);

            AnyModInCat = true;

            Pos.x = Constraints::PercentageConstraint(0.25, 2) + Constraints::PercentageConstraint(0.06) + posX;

            RenderUtils::RenderText(Pos, ImVec2(), IM_COL32_WHITE, name, 0.012 * Constraints::PercentageConstraint(0.045));


            if (type.compare(0, ((std::string)"slider").size(), (std::string)"slider") == 0) {

                size_t pos = type.find('-');
                type.substr(0, pos);
                size_t pos2 = type.find('-', pos + 1);
                std::string floatStr = type.substr(pos + 1, pos2 - pos - 1);
                float min = std::stof(floatStr);
                std::string intStr = type.substr(pos2 + 1);
                float max = std::stof(intStr);

                float xOffset = Constraints::PercentageConstraint(0.75, 2) - Constraints::PercentageConstraint(0.34) + posX;
                Settings::getSettingByName<float>(modName, name)->value = RenderUtils::Slider(ImVec2(xOffset, Pos.y), Settings::getSettingByName<float>(modName, name)->value, min, max, index, Constraints::PercentageConstraint(0.30));
            }

            else if (type == "toggle") {
                float xOffset = Constraints::PercentageConstraint(0.75, 2) - Constraints::PercentageConstraint(0.08) + posX;
                Settings::getSettingByName<bool>(modName, name)->value = RenderUtils::ToggleButton(ImVec2(xOffset, Pos.y - Constraints::PercentageConstraint(0.008)), Settings::getSettingByName<bool>(modName, name)->value);
            }

            else if (type == "colorpicker") {
                float xOffset = Constraints::PercentageConstraint(0.75, 2) - Constraints::PercentageConstraint(0.07) + posX;
                RenderUtils::ColourPicker(ImVec2(xOffset, Pos.y - Constraints::PercentageConstraint(0.008)), Settings::getSettingByName<std::vector<float>>(modName, name), index);
            }

			else if (type == "keybind") {
				float xOffset = Constraints::PercentageConstraint(0.75, 2) - Constraints::PercentageConstraint(0.1) + posX;
				Settings::getSettingByName<int>(modName, name)->value = RenderUtils::KeybindSelector(ImVec2(xOffset, Pos.y - Constraints::PercentageConstraint(0.008)), Settings::getSettingByName<int>(modName, name)->value, index, modName == "Mod Menu");
			}
			else if (type == "pushbutton") {
				float xOffset = Constraints::PercentageConstraint(0.75, 2) - Constraints::PercentageConstraint(0.07) + posX - Constraints::PercentageConstraint(.06);
				RenderUtils::PushButton(ImVec2(xOffset, Pos.y - Constraints::PercentageConstraint(0.008)), modName, name);
			}
			else if (type.compare(0, ((std::string)"score").size(), (std::string)"score") == 0) {

                size_t pos = type.find('-');
                type.substr(0, pos);
                size_t pos2 = type.find('-', pos + 1);
                std::string minstr = type.substr(pos + 1, pos2 - pos - 1);
                int min = std::stoi(minstr);
                std::string maxstr = type.substr(pos2 + 1);
                int max = std::stoi(maxstr);

				float xOffset = Constraints::PercentageConstraint(0.75, 2) - Constraints::PercentageConstraint(0.07) + posX - Constraints::PercentageConstraint(.1);
				Settings::getSettingByName<int>(modName, name)->value = RenderUtils::ScoreControl(ImVec2(xOffset, Pos.y), Settings::getSettingByName<int>(modName, name)->value, min, max);
			}

            Pos.y += Constraints::PercentageConstraint(0.04);
            index++;
        }
        if (AnyModInCat) RenderUtils::RenderTextWithOutline(PosForPostCategoryRender, ImVec2(), ImColor(107, 112, 114, 255), cat, 0.02 * Constraints::PercentageConstraint(0.045), ImColor(107, 112, 114, 255), .45);
        AnyModInCat = false;
    }
}

ModMenu::ModMenu() : ModuleCtor(67, "Mod Menu", "modmenu") {
    this->InitModuleSettings();
}

std::string ModMenu::getModuleName() {
    return "Mod Menu";
}

void ModMenu::InitModuleSettings() {
	Settings::addSetting<bool>("Mod Menu", "enabled", false, true);
	Settings::addSetting<int>("Mod Menu", "KEYBIND", 76, true);
	Settings::addSetting<std::vector<float>>("Mod Menu", "ACCENT COLOR", {0.866f, 0.266f, 0.866f, 1.f}, true);
	Settings::addSetting<float>("Mod Menu", "ANIMATION SPEED", 1, true);
	Settings::addSetting<bool>("Mod Menu", "DISABLE ANIMATIONS", false, true);
	Settings::addSetting<float>("Mod Menu", "BLUR INTENISTY", 15, true);
    Settings::addSetting<bool>("Mod Menu", "DISABLE VSYNC", false, true);
    Settings::addSetting<int>("Mod Menu", "Friend List Keybind", 77, true);
    Settings::addSetting<bool>("Mod Menu", "DISABLE SHADOWS", false, true);
    Settings::addSetting<bool>("Mod Menu", "USE MONJANGLES HUD", false, true);
    Settings::addSetting<bool>("Mod Menu", "TEXT SHADOWS", false, true);
    Settings::addSetting<std::vector<float>>("Mod Menu", "TEXT SHADOW COLOR", {0.3f, 0.3f, 0.3f, 1.0f}, true);

    this->SettingsList["USER INTERFACE"]["Friend List Keybind"] = "keybind";
	this->SettingsList["USER INTERFACE"]["KEYBIND"] = "keybind";
	this->SettingsList["USER INTERFACE"]["ACCENT COLOR"] = "colorpicker";
	this->SettingsList["USER INTERFACE"]["ANIMATION SPEED"] = "slider-0.5-5";
	this->SettingsList["USER INTERFACE"]["BLUR INTENISTY"] = "slider-0-15";
    this->SettingsList["USER INTERFACE"]["DISABLE VSYNC"] = "toggle";
	this->SettingsList["USER INTERFACE"]["DISABLE ANIMATIONS"] = "toggle";
    this->SettingsList["USER INTERFACE"]["DISABLE SHADOWS"] = "toggle";
    this->SettingsList["USER INTERFACE"]["USE MONJANGLES HUD"] = "toggle";
    this->SettingsList["USER INTERFACE"]["TEXT SHADOWS"] = "toggle";
    this->SettingsList["USER INTERFACE"]["TEXT SHADOW COLOR"] = "colorpicker";
}

float currentModMenuPos = 0;
float currentTopBarPos = 0;

float currentUnderBarSize = 0;
float currentUnderBarPos = 0;

float currentModListPos = 0;

float CurrentBlurAmount = 0;

bool DisableModMenu = false;


void ModMenu::onEnable() {
    if (SDK::TopScreen.rfind("hud_screen") != std::string::npos) {
        DisableModMenu = false;
        Settings::getSettingByName<bool>("Mod Menu", "enabled")->value = true;
        eventMgr.addListener(this);
        if (SDK::clientInstance) {
            SDK::clientInstance->releaseMouse();
        }
        currentModListPos -= Constraints::PercentageConstraint(1);
    }
    else
        this->setEnabled(false);
}

void ModMenu::onDisable() {
    DisableModMenu = true;
    RenderUtils::DisableTextBoxes();
}

void ModMenu::onKey(KeyEvent& event) {
    event.setCancelled(true);
}

void ModMenu::onMouse(MouseEvent& event) {
    if (event.GetAction() == MouseAction::SCROLL_UP) {
        scroll += 0.032;
        if (scroll > 0)
            scroll = 0;
    }
    if (event.GetAction() != MouseAction::SCROLL_UP && event.GetButton() == MouseButton::Scroll) {
        if (!scroll_max)
            scroll -= 0.032;
    }
    event.setCancelled(true);
}

float currentSearchbarSize = 0;
float slider = 0;
ImColor col = ImColor(0, 0, 0, 0);
void ModMenu::onRender(const RenderEvent& event) {
    if(!SDK::clientInstance || !SDK::clientInstance->getLocalPlayer()) return;
    if (!(SDK::TopScreen.rfind("hud_screen") != std::string::npos)) {
        this->setEnabled(false);
        eventMgr.removeListener(this);
        return;
    }

    float animationSpeed = Settings::getSettingByName<float>("Mod Menu", "ANIMATION SPEED")->value;

    std::vector<float> accentCol = Settings::getSettingByName<std::vector<float>>("Mod Menu", "ACCENT COLOR")->value;

    ImColor accentColor = ImColor(accentCol.at(0), accentCol.at(1), accentCol.at(2), 1.0f);

    float TargetModMenuPos;
    float TargetTopBarPos;
    float TargetModListPos;
    float TargetBlurAmount;

    if (DisableModMenu) {
        TargetModMenuPos = Constraints::PercentageConstraint(1) + 5;
        TargetTopBarPos = Constraints::PercentageConstraint(-0.08) - 5;
        TargetModListPos = Constraints::PercentageConstraint(.4) + 5;
        TargetBlurAmount = 0;

        if (currentModMenuPos > Constraints::PercentageConstraint(1) and currentTopBarPos < Constraints::PercentageConstraint(-0.08) and currentModListPos > Constraints::PercentageConstraint(.4) and CurrentBlurAmount < 1) {
            Settings::getSettingByName<bool>("Mod Menu", "enabled")->value = false;
            eventMgr.removeListener(this);

            RenderUtils::DisableTextBoxes();

            if (SDK::TopScreen.rfind("hud_screen") != std::string::npos) {
                if (SDK::clientInstance) {
                    SDK::clientInstance->grabMouse();
                }
            }
            currentModMenuPos = 0;
            currentTopBarPos = 0;
            currentModListPos = 0;
            currentSetting = "";
        }
    }

    else {
        TargetModMenuPos = Constraints::PercentageConstraint(0.2955 - 0.04);
        TargetTopBarPos = Constraints::PercentageConstraint(0.16);
        TargetModListPos = -1;
        TargetBlurAmount = Settings::getSettingByName<float>("Mod Menu", "BLUR INTENISTY")->value;
        SDK::clientInstance->releaseMouse();
    }

    ImVec2 displaySize = ImGui::GetIO().DisplaySize;

    CurrentBlurAmount = Utils::animate(TargetBlurAmount, CurrentBlurAmount, MC::Deltatime * 0.15 * animationSpeed);

    RenderUtils::Blur(CurrentBlurAmount);

    if (currentModMenuPos == 0)
        currentModMenuPos = Constraints::PercentageConstraint(1);
    if (currentTopBarPos == 0)
        currentTopBarPos -= Constraints::PercentageConstraint(0.08);

    currentModMenuPos = Utils::animate(TargetModMenuPos, currentModMenuPos, MC::Deltatime * 0.15 * animationSpeed);
    currentTopBarPos = Utils::animate(TargetTopBarPos, currentTopBarPos, MC::Deltatime * 0.15 * animationSpeed);

    // The Size for TOP BAR
    ImVec2 Size_TOPBAR = ImVec2(Constraints::PercentageConstraint(1), Constraints::PercentageConstraint(0.078));
    // The Position for TOP BAR (calculates the X coords so top bar is on the middle)
    ImVec2 Pos_TOPBAR = ImVec2(Constraints::PercentageConstraint(0.5, 2) - (Size_TOPBAR.x / 2), currentTopBarPos);

    // Position of Lyra Logo (uses top bar pos as refernce)
    ImVec2 Pos_LOGO = ImVec2(Pos_TOPBAR.x + (Size_TOPBAR.y * 0.18), Pos_TOPBAR.y + (Size_TOPBAR.y  * 0.13));
    // Size of Lyra Logo (2/3 of top bar Y size)
    ImVec2 Size_LOGO = ImVec2(Size_TOPBAR.y * 0.75, Size_TOPBAR.y * 0.75);

    if (currentSearchbarSize == 0) currentSearchbarSize = Size_TOPBAR.y * 0.423;

    // Search bar size
    ImVec2 Size_SearchBar = ImVec2(Constraints::PercentageConstraint(0.25), Size_TOPBAR.y * 0.423);
    ImVec2 Size_SearchBarDisabled = ImVec2(Size_TOPBAR.y * 0.423, Size_SearchBar.y);
    // Search bar pos (make sures that search bar is in the middle of top bar from y axis)
    ImVec2 Pos_SearchBar = ImVec2((Pos_TOPBAR.x + Size_TOPBAR.x) - Constraints::PercentageConstraint(0.35), Pos_TOPBAR.y + ((Size_TOPBAR.y / 2) - (Size_SearchBar.y / 2)));

    ImVec2 Size_SearchBartarget = ImVec2(currentSearchbarSize, Size_SearchBar.y);
    ImVec2 Pos_SearchBartarget = ImVec2(Size_SearchBar.x + Pos_SearchBar.x - currentSearchbarSize, Pos_SearchBar.y);

    ImVec2 Size_SearchIcon = ImVec2(Size_SearchBarDisabled.x - 5, Size_SearchBarDisabled.y - 5);// ImVec2(Size_SearchBarDisabled.x * (8 / 10), Size_SearchBarDisabled.x * (8 / 10));
    ImVec2 Pos_SearchIcon = ImVec2(Pos_SearchBar.x + Size_SearchBar.x - Size_SearchBar.y + 2.5,Pos_SearchBar.y + 4);

    // Search bar text box pos
    ImVec2 Pos_SearchBarTextBox = ImVec2(Pos_SearchBartarget.x + Constraints::PercentageConstraint(0.008), Pos_SearchBartarget.y);
    // Search bar text box size
    ImVec2 Size_SearchBarTextBox = ImVec2(Size_SearchBartarget.x - Constraints::PercentageConstraint(0.016) - Size_SearchIcon.x, Size_SearchBartarget.y);

    ImVec2 Pos_Mods_Cat = ImVec2(Pos_LOGO.x + Constraints::PercentageConstraint(.08), Pos_TOPBAR.y);

    ImVec2 Pos_Settings_Cat = ImVec2(Pos_Mods_Cat.x + Constraints::PercentageConstraint(.12), Pos_TOPBAR.y);

    float targetSize;
    float targetPos;
    ImVec2 ModCatSize;
    ImGui::SetWindowFontScale((7.5 * .7 * .12 * Constraints::PercentageConstraint(0.001))/2);
    if (currentSetting == "") {
        ModCatSize = ImGui::CalcTextSize("Modules");
        targetSize = ModCatSize.x;
        targetPos = Pos_Mods_Cat.x;
    }
    else {
        targetSize = ImGui::CalcTextSize("Settings").x;
        targetPos = Pos_Settings_Cat.x;
    }

    currentUnderBarPos = Utils::animate(targetPos, currentUnderBarPos, MC::Deltatime * 0.15 * animationSpeed);
    currentUnderBarSize = Utils::animate(targetSize, currentUnderBarSize, MC::Deltatime * 0.15 * animationSpeed);

    ImVec2 Pos_Mods_Cat_U = ImVec2(currentUnderBarPos, Pos_Mods_Cat.y + Constraints::PercentageConstraint(.055));
    ImVec2 Size_Mods_Cat_U = ImVec2(currentUnderBarSize, 3.5);

    // Space Between SearchBar (right) and Topbar
    float SBST = (Pos_TOPBAR.x + Size_TOPBAR.x) - (Pos_SearchBar.x + Size_SearchBar.x);
    // Close Gui button size (same as searchbar's Y axis)
    ImVec2 Size_closeButton = ImVec2(Size_SearchBar.y, Size_SearchBar.y);
    // Position of Close Gui button
    ImVec2 Pos_closeButton = ImVec2(Pos_SearchBar.x + Size_SearchBar.x + ((SBST / 2) - (Size_closeButton.y / 2)), Pos_SearchBar.y);

    // Rendering TOP BAR with their logic

    RenderUtils::ShadowRectD2D(Pos_TOPBAR, Size_TOPBAR, 30, ImColor(0.f, 0.f, 0.f, 0.4f), Constraints::PercentageConstraint(0.006), 10);
    RenderUtils::fillRect(Pos_TOPBAR, Size_TOPBAR, ImColor(0.f, 0.f, 0.f, 0.5f), 30);

    RenderUtils::fillRect(Pos_Mods_Cat_U, Size_Mods_Cat_U, accentColor, 0);
    RenderUtils::RenderText(Pos_Mods_Cat, ImVec2(0, Size_TOPBAR.y), currentSetting == "" ? accentColor : ImColor(0.75f, 0.75f, 0.75f, 1.0f), "Modules", 7.5 * .7 * .12 * Constraints::PercentageConstraint(0.001));

    ImVec4 textPosSize = RenderUtils::RenderText(Pos_Settings_Cat, ImVec2(0, Size_TOPBAR.y), currentSetting != "" ? accentColor : ImColor(0.75f, 0.75f, 0.75f, 1.0f), "Settings", 7.5 * .7 * .12 * Constraints::PercentageConstraint(0.001));

    if (ImGui::IsMouseClicked(0)){
        if (Utils::IsMouseOverRectangle(Utils::mousepos, ImVec2(Pos_Mods_Cat.x, Pos_TOPBAR.y), ImVec2(targetSize, Size_TOPBAR.y)) and currentSetting != "") {
            currentSetting = "";
            searchtext = "";
        }
    }

    if (ImGui::IsMouseClicked(0)){
        if (Utils::IsMouseOverRectangle(Utils::mousepos, ImVec2(Pos_Settings_Cat.x, Pos_TOPBAR.y), ImVec2(targetSize, Size_TOPBAR.y)) and currentSetting == "") {
            currentSetting = this->getModuleName();
        }
    }

    RenderUtils::RenderImage(Pos_LOGO, Size_LOGO, "Lyra\\Assets\\Lyra.png", IM_COL32_WHITE);
    RenderUtils::RenderImage(Pos_LOGO, Size_LOGO, "Lyra\\Assets\\star.png", accentColor);

    //RenderUtils::ShadowCircle(ImVec2(Pos_closeButton.x + (Size_closeButton.x / 2), Pos_closeButton.y + (Size_closeButton.x / 2)), Size_closeButton.x / 2, accentColor, 30, ImVec2(0, 0), 0);

    RenderUtils::fillRect(Pos_closeButton, Size_closeButton, accentColor, 1000);
    RenderUtils::ShadowRectD2D(Pos_closeButton, Size_closeButton, 1000, accentColor, Constraints::PercentageConstraint(0.006), 10);

    RenderUtils::RenderImage(Pos_closeButton, Size_closeButton, "Lyra\\Assets\\cross.png", IM_COL32_WHITE);

    if (ImGui::IsMouseClicked(0)){
        if (Utils::IsMouseOverRectangle(Utils::mousepos, Pos_closeButton, Size_closeButton)) {
            this->setEnabled(false);
            return;
        }
    }

    std::string searchText = searchtext;

    bool textBoxEnabled = RenderUtils::textBox(&searchtext, Pos_SearchBarTextBox, Size_SearchBarTextBox, ImColor(0, 0, 0, 0), 1);
    if (ImGui::IsMouseClicked(0)){
        if (Utils::IsMouseOverRectangle(Utils::mousepos, Pos_SearchBartarget, Size_SearchBartarget)) {
            RenderUtils::EnableTextBox(1);
            ImGui::GetIO().MouseDown[0] = false;
        }
    }
    if (textBoxEnabled) {

    }
    RenderUtils::drawRect(Pos_SearchBartarget, Size_SearchBartarget, textBoxEnabled ? accentColor : ImColor(0.75f, 0.75f, 0.75f, 1.0f), 1000, 2);

    if (searchtext == "" and textBoxEnabled)
        RenderUtils::RenderText(Pos_SearchBarTextBox, Size_SearchBarTextBox, ImColor(0.75f, 0.75f, 0.75f, 1.0f), "Search...", Size_SearchBarTextBox.y * 0.01 * Constraints::PercentageConstraint(0.001));

    if (searchText != searchtext) {
        scroll = 0;
    }

    if (textBoxEnabled) {
        currentSearchbarSize = Utils::animate(Size_SearchBar.x, currentSearchbarSize, MC::Deltatime * 0.15 * animationSpeed);
    }
    else {
        currentSearchbarSize = Utils::animate(Size_SearchBarDisabled.x, currentSearchbarSize, MC::Deltatime * 0.15 * animationSpeed);
    }

    RenderUtils::RenderImage(Pos_SearchIcon, Size_SearchIcon, "Lyra\\Assets\\search.png", textBoxEnabled ? ImColor(1.0f, 1.0f, 1.0f, 1.0f) : ImColor(0.75f, 0.75f, 0.75f, 1.0f));

    //TOP BAR finished

    // Pos Menu (same X coords as top bar
    ImVec2 Pos_Menu = ImVec2(Pos_TOPBAR.x, currentModMenuPos);
    // Size Menu
    ImVec2 Size_MENU = ImVec2(Size_TOPBAR.x, Constraints::PercentageConstraint(0.558));

    //Rendering Menu

    //RenderUtils::ColourPicker(ImVec2(200, 200), col, 1);

    RenderUtils::ShadowRectD2D(Pos_Menu, Size_MENU, 30, ImColor(0.f, 0.f, 0.f, 0.4f), Constraints::PercentageConstraint(0.012), 10);
    RenderUtils::fillRect(Pos_Menu, Size_MENU, ImColor(0.f, 0.f, 0.f, 0.5f), 30);

    if (currentSetting != "") {
        RenderUtils::PushClippingArea(Pos_Menu, Size_MENU);
        RenderModSettings(moduleMgr.getModuleG(currentSetting), Pos_Menu.y + Constraints::PercentageConstraint(0.055), currentModListPos);
        currentModListPos = Utils::animate(Size_MENU.x + 10, currentModListPos, MC::Deltatime * 0.15 * animationSpeed);
        RenderUtils::PopClippingArea();

        if (currentModListPos > Size_MENU.x)
            return;
    }
    else {
        currentModListPos = Utils::animate(TargetModListPos, currentModListPos, MC::Deltatime * 0.15 * animationSpeed);
    }

    RenderUtils::PushClippingArea(Pos_Menu, Size_MENU);

    auto renderModCard = [&](ImVec2 Pos, std::shared_ptr<Module> mod) {
        auto module = mod.get();
        //Mod Card Size
        ImVec2 Size_ModCard = ImVec2(Constraints::PercentageConstraint(0.287), Constraints::PercentageConstraint(0.217));

        // Module icon size
        ImVec2 Size_ModuleIcon = ImVec2(Constraints::PercentageConstraint(0.06), Constraints::PercentageConstraint(0.06));
        // Module icon pos
        ImVec2 Pos_ModuleIcon = ImVec2(Pos.x + (Size_ModCard.x / 2) - (Size_ModuleIcon.x / 2), Pos.y + (Size_ModCard.x / 4) - (Size_ModuleIcon.y / 2) - 10);

        // Module name pos
        ImVec2 Pos_ModuleName = ImVec2(Pos.x, Pos_ModuleIcon.y + Size_ModuleIcon.y);
        // Module name padding size
        ImVec2 Padding_ModuleName = ImVec2(Size_ModCard.x+2, Constraints::PercentageConstraint(0.0555));

        // Pos toggle button
        ImVec2 Pos_toggleButton = ImVec2(Pos.x + Constraints::PercentageConstraint(0.0196), Pos_ModuleName.y + Padding_ModuleName.y + Constraints::PercentageConstraint(.0057));
        // Size Toggle Button
        ImVec2 Size_toggleButton = ImVec2(Constraints::PercentageConstraint(0.18), Constraints::PercentageConstraint(0.0412));

        // Pos settings button
        ImVec2 Pos_SettingsButton = ImVec2(Pos_toggleButton.x + Size_toggleButton.x + Constraints::PercentageConstraint(0.0196), Pos_toggleButton.y);
        // Size settings button
        ImVec2 Size_SettingsButton = ImVec2(Size_toggleButton.y, Size_toggleButton.y);

        // Colour depedent on module enabled or not
        ImColor col = module->isEnabled() ? accentColor : ImColor(102, 101, 102, 255);

        RenderUtils::fillRect(Pos, Size_ModCard, ImColor(0.f, 0.f, 0.f, 0.45f), 15);

        RenderUtils::RenderImage(Pos_ModuleIcon, Size_ModuleIcon, "Lyra\\Assets\\" + module->icon + ".png", IM_COL32_WHITE);

        RenderUtils::RenderTextWithOutline(Pos_ModuleName, Padding_ModuleName, ImColor(255, 255, 255, 255), module->getModuleName(), 5 * 0.7 * 0.165 * Constraints::PercentageConstraint(0.001), IM_COL32_WHITE, .3, 2);

        //RenderUtils::ShadowRect(Pos_toggleButton, Size_toggleButton, col, 15, ImVec2(0, 0), 100);

        if (Pos_toggleButton.y > Pos_Menu.y + Size_MENU.y or Pos_toggleButton.y + Size_toggleButton.y < Pos_Menu.y)
            return;

		RenderUtils::fillRect(Pos_toggleButton, Size_toggleButton, col, 10);
        RenderUtils::ShadowRectD2D(Pos_toggleButton, Size_toggleButton, 10, col, Constraints::PercentageConstraint(0.008), 10);
        auto it = ModMenu::blocked_modules.find(SDK::getServerIP());
        if (it != ModMenu::blocked_modules.end()) {
            std::vector<std::string> &list = it->second;
            if (std::find(list.begin(), list.end(), mod->getModuleName()) != list.end()) {
                RenderUtils::RenderTextWithOutline(Pos_toggleButton, Size_toggleButton, ImColor(255, 255, 255, 255),
                                                   "LOCKED", 5.5 * 0.7 * 0.13 * Constraints::PercentageConstraint(0.001),
                                                   IM_COL32_WHITE, .3, 2);
            }else{
                RenderUtils::RenderTextWithOutline(Pos_toggleButton, Size_toggleButton, ImColor(255, 255, 255, 255),
                                                   module->getModuleName() != "Profiles"? (module->isEnabled() ? "ENABLED" : "DISABLED") : "MANAGE", 5.5 * 0.7 * 0.13 * Constraints::PercentageConstraint(0.001),
                                                   IM_COL32_WHITE, .3, 2);
                if (Utils::IsMouseOverRectangle(ImVec2(Utils::mousepos.x, Utils::mousepos.y), Pos_toggleButton, Size_toggleButton) and ImGui::IsMouseClicked(0)) {
                    module->toggle();
                }
            }
        }else{
            RenderUtils::RenderTextWithOutline(Pos_toggleButton, Size_toggleButton, ImColor(255, 255, 255, 255),
                                               module->getModuleName() != "Profiles"? (module->isEnabled() ? "ENABLED" : "DISABLED") : "MANAGE", 5.5 * 0.7 * 0.13 * Constraints::PercentageConstraint(0.001),
                                               IM_COL32_WHITE, .3, 2);
            if (Utils::IsMouseOverRectangle(ImVec2(Utils::mousepos.x, Utils::mousepos.y), Pos_toggleButton, Size_toggleButton) and ImGui::IsMouseClicked(0)) {
                module->toggle();
            }
        }

        //RenderUtils::ShadowRect(Pos_SettingsButton, Size_SettingsButton, col, 15, ImVec2(0, 0), 100);
        RenderUtils::fillRect(Pos_SettingsButton, Size_SettingsButton, col, 10);
        RenderUtils::ShadowRectD2D(Pos_SettingsButton, Size_SettingsButton, 10, col, Constraints::PercentageConstraint(0.008), 10);
        RenderUtils::RenderImage(ImVec2(Pos_SettingsButton.x + 5, Pos_SettingsButton.y + 5), ImVec2(Size_SettingsButton.x - 10, Size_SettingsButton.y - 10), "Lyra\\Assets\\settings.png", ImColor(255, 255, 255, 255));
        if (Utils::IsMouseOverRectangle(ImVec2(Utils::mousepos.x, Utils::mousepos.y), Pos_SettingsButton,
                                                Size_SettingsButton) and ImGui::IsMouseClicked(0)) {
            scroll = 0;
            currentSetting = mod->getModuleName();
            searchtext = "";
        }
    };
    int num = 1;
    int vert = 10;
    //Module Position
    ImVec2 ModulePostion = ImVec2(Pos_Menu.x + Constraints::PercentageConstraint(0.052) - currentModListPos, Pos_Menu.y + Constraints::PercentageConstraint(0.015));
    ModulePostion.y += Constraints::PercentageConstraint(scroll);

    for (auto it = moduleMgr.getModules().begin(); it != moduleMgr.getModules().end(); ++it) {
        std::shared_ptr<Module> mod = it->second;
        std::string modName = mod->getModuleName();
        if (modName == "Mod Menu" || modName == "Notif") continue;
        std::string search = searchtext;
        std::transform(modName.begin(), modName.end(), modName.begin(), ::tolower);
        std::transform(search.begin(), search.end(), search.begin(), ::tolower);
        if (modName.find(search) > 0) {
            continue;
        }
        if (ModulePostion.y > Pos_Menu.y + Size_MENU.y)
            break;
        if (num == 3) {
            renderModCard(ModulePostion, mod);
            ModulePostion.x = Pos_Menu.x + Constraints::PercentageConstraint(0.052) - currentModListPos;
            ModulePostion.y += displaySize.y * 0.237;
            num = 1;
        }
        else {
            renderModCard(ModulePostion, mod);
            ModulePostion.x += displaySize.y * 0.307 - currentModListPos;
            num++;
        }
    };
    RenderUtils::PopClippingArea();
    ImVec2 SCB_Pos = ImVec2(Pos_Menu.x + Size_MENU.x - 30, Pos_Menu.y + 30);
    ImVec2 SCB_Size = ImVec2(5, Size_MENU.y - 60 - vert);
    ImVec2 SC_Pos = ImVec2(Pos_Menu.x + Size_MENU.x - 30, Pos_Menu.y + 30 - Constraints::PercentageConstraint(scroll) / Constraints::PercentageConstraint(.045) * vert);
    ImVec2 SC_Size = ImVec2(5, (Size_MENU.y - 5) / vert);
    RenderUtils::fillRect(SCB_Pos, SCB_Size, ImColor(120, 120, 120), 20);
    RenderUtils::fillRect(SC_Pos, SC_Size, accentColor, 20);
    RenderUtils::ShadowRectD2D(SC_Pos, SC_Size, 20, accentColor, 3, 5);
    if (SC_Pos.y >= (Pos_Menu.y + Size_MENU.y - 30 - SC_Size.y - vert)) {
        SC_Pos.y = (Pos_Menu.y + Size_MENU.y - 30 - SC_Size.y - vert);
        scroll_max = true;
    }
    else scroll_max = false;
    if (Utils::IsMouseOverRectangle(Utils::mousepos, SCB_Pos, SCB_Size) && ImGui::IsMouseClicked(0)) Sdragging = true;
    if (ImGui::IsMouseDown(0) && !selected && !resizing && !dragging) {
        if (Sdragging){
            if (Utils::mousepos.y <= (Pos_Menu.y + 30)) {
                scroll = 0;
                return;
            }
            if (Utils::mousepos.y >= (Pos_Menu.y + Size_MENU.y - 30 - SC_Size.y - vert)) {
                scroll = ((60 + SC_Size.y + vert - Size_MENU.y) * Constraints::PercentageConstraint(.045) / vert)/displaySize.y;
                return;
            }
            scroll = ((Pos_Menu.y + 30 - Utils::mousepos.y) * Constraints::PercentageConstraint(.045) / vert)/displaySize.y;
        }
    }
    else Sdragging = false;
}