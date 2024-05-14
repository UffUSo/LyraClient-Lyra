#pragma once

#include "Module.hpp"
#include <map>

#define GET_MODULE(type, name) moduleMgr.getModule<type>(xorstr(name));
#define GET_MODULEG(name) moduleMgr.getModuleG(xorstr(name));

class ModuleManager {
public:
    void addModule(const std::shared_ptr<Module>& mod);

    template <typename T>
    T* getModule(const std::string& name) {
        std::string n = name;
        std::transform(n.begin(), n.end(), n.begin(), ::tolower);

        return reinterpret_cast<T*>(mods.at(n).get());
    }

    std::shared_ptr<Module> getModuleG(const std::string& name);

    [[nodiscard]] bool containsModule(const std::string& name) const;

    void initModules();
    [[nodiscard]] bool isInitialized() const;

    std::map<std::string, std::shared_ptr<Module>>& getModules();

    void EnabledOnStart();

    void HeartBeatFriends();

private:
    bool initialized = false;
    std::map<std::string, std::shared_ptr<Module>> mods;
};

extern ModuleManager moduleMgr;
