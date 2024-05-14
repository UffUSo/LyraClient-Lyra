#include <thread>

#include "ModuleManager.hpp"

#include "Modules/FriendSystem/FriendSystem.hpp"

#include "Modules/Zoom.hpp"
#include "Modules/ModMenu.hpp"
#include "Modules/FPS.hpp"
#include "Modules/CPS.hpp"
#include "Modules/ToggleSprint.hpp"
#include "Modules/Clock.hpp"
#include "Modules/FreeLook.hpp"
#include "Modules/SnapLook.hpp"
#include "Modules/ComboCounter.hpp"
#include "Modules/ReachDisplay.hpp"
#include "Modules/ThirdPersonNametag.hpp"
#include "Modules/NoHurtCam.hpp"
#include "Modules/BlockOutline.hpp"
#include "Modules/Ping.hpp"
#include "Modules/Keystroke.hpp"
#include "Modules/FogColour.hpp"
#include "Modules/HurtColor.hpp"
#include "Modules/WeatherChanger.hpp"
#include "Modules/Fullbright.hpp"
#include "Modules/Notif.hpp"
#include "Modules/PotCounter.hpp"
#include "Modules/IPDisplay.hpp"
#include "Modules/Tablist.hpp"
#include "Modules/MotionBlur.hpp"
#include "Modules/Coordinates.hpp"
#include "Modules/Momentum.hpp"
#include "Modules/Playtime.hpp"
#include "Modules/ShinyPots.hpp"
#include "Modules/ArrowCounter.hpp"
#include "Modules/SoupCounter.hpp"
#include "Modules/ArmorStats.hpp"
#include "Modules/Hitbox.hpp"
#include "Modules/CPS_Limiter.hpp"
#include "Modules/FoodPoints.hpp"
#include "Modules/Profiles.hpp"
#include "Modules/hivehelper/helper.hpp"
#include "Modules/CustomChat.hpp"
#include "Modules/SelfDestruct/SelfDestruct.hpp"

ModuleManager moduleMgr;

void ModuleManager::initModules() {

    this->addModule(std::make_shared<Zoom>(Zoom()));
    this->addModule(std::make_shared<ModMenu>(ModMenu()));
    this->addModule(std::make_shared<FPS>(FPS()));
    this->addModule(std::make_shared<CPS>(CPS()));
    this->addModule(std::make_shared<ToggleSprint>(ToggleSprint()));
    this->addModule(std::make_shared<Clock>(Clock()));
    this->addModule(std::make_shared<FreeLook>(FreeLook()));
    this->addModule(std::make_shared<SnapLook>(SnapLook()));
    this->addModule(std::make_shared<IPDisplay>(IPDisplay()));
    this->addModule(std::make_shared<Coordinates>(Coordinates()));
    this->addModule(std::make_shared<Momentum>(Momentum()));
    this->addModule(std::make_shared<ComboCounter>(ComboCounter()));
    this->addModule(std::make_shared<ReachDisplay>(ReachDisplay()));
    this->addModule(std::make_shared<ThirdPersonNametag>(ThirdPersonNametag()));
    this->addModule(std::make_shared<NoHurtCam>(NoHurtCam()));
    this->addModule(std::make_shared<BlockOutline>(BlockOutline()));
    this->addModule(std::make_shared<Ping>(Ping()));
    this->addModule(std::make_shared<Keystroke>(Keystroke()));
    this->addModule(std::make_shared<FogColour>(FogColour()));
    this->addModule(std::make_shared<HurtColor>(HurtColor()));
    this->addModule(std::make_shared<Notif>(Notif()));
    this->addModule(std::make_shared<PotCounter>(PotCounter()));
    this->addModule(std::make_shared<WeatherChanger>(WeatherChanger()));
    this->addModule(std::make_shared<Fullbright>(Fullbright()));
    this->addModule(std::make_shared<Tablist>(Tablist()));
    this->addModule(std::make_shared<Playtime>(Playtime()));
    this->addModule(std::make_shared<ShinyPots>(ShinyPots()));
    this->addModule(std::make_shared<MotionBlur>(MotionBlur()));
    this->addModule(std::make_shared<ArrowCounter>(ArrowCounter()));
    this->addModule(std::make_shared<SoupCounter>(SoupCounter()));
    //this->addModule(std::make_shared<ArmorStats>(ArmorStats()));
    this->addModule(std::make_shared<Hitbox>(Hitbox()));
    this->addModule(std::make_shared<CPS_Limiter>(CPS_Limiter()));
    this->addModule(std::make_shared<FoodPoints>(FoodPoints()));
    this->addModule(std::make_shared<Profiles>(Profiles()));
    //this->addModule(std::make_shared<CustomChat>(CustomChat()));
    this->addModule(std::make_shared<helper>(helper()));
    this->addModule(std::make_shared<SelfDestruct>(SelfDestruct()));
    this->initialized = true;
}

void ModuleManager::addModule(const std::shared_ptr<Module>& mod) {
    std::string name = mod->getRawModuleName();
    std::transform(name.begin(), name.end(), name.begin(), ::tolower);
    this->mods.insert({name, mod});
}

std::shared_ptr<Module> ModuleManager::getModuleG(const std::string &name) {
    std::string n = name;
    std::transform(n.begin(), n.end(), n.begin(), ::tolower);

    return this->mods.at(n);
}

bool ModuleManager::containsModule(const std::string &name) const {
    std::string n = name;
    std::transform(n.begin(), n.end(), n.begin(), ::tolower);

    return this->mods.contains(n);
}

bool ModuleManager::isInitialized() const {
    return this->initialized;
}

std::map<std::string, std::shared_ptr<Module>> &ModuleManager::getModules() {
    return this->mods;
}

void ModuleManager::EnabledOnStart() {
    for (auto it = moduleMgr.getModules().begin(); it != moduleMgr.getModules().end(); ++it) {
        std::shared_ptr<Module> mod = it->second;

        if (Settings::getSettingByName<bool>(mod.get()->getModuleName(), "enabled")->value) {
            mod.get()->setEnabled(true);
        }
    }

}

void ModuleManager::HeartBeatFriends() {
    FriendSys.HeartBeat();
}
