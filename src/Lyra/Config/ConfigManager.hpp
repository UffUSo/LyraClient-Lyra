 #pragma once

#include <string>
#include <unordered_map>
#include <typeindex>
#include "json/json.hpp"
#include <fstream>
#include "../../Utils/Logger/Logger.hpp"
#include "../../Utils/Utils.hpp"
#include <iostream>
#include <Windows.h>
#include <filesystem>
#include <sstream>

using json = nlohmann::json;

class Setting {
public:
    virtual ~Setting() = default;
    virtual json ToJson() const = 0;
    virtual void FromJson(const json& jsonData) = 0;
    virtual std::unique_ptr<Setting> clone() const = 0;
};

template<typename T>
class SettingType : public Setting {
public:
    SettingType(std::string name, const T& defaultValue) : value(defaultValue), name(name) {}

    json ToJson() const override {
        json jsonData;
        jsonData["name"] = name;
        jsonData["value"] = value;
        return jsonData;
    }

    void FromJson(const json& jsonData) override {
        if (jsonData.is_object() && jsonData.contains("name") && jsonData.contains("value")) {
            name = jsonData["name"].get<std::string>();
            value = jsonData["value"].get<T>();
        }
    }

    std::unique_ptr<Setting> clone() const override {
        return std::make_unique<SettingType<T>>(name, value);
    }

    std::string name;
    T value;
};

class Settings {
public:

    template<typename T>
    static void addSetting(const std::string& module, const std::string& name, const T& defaultValue, const bool check = false) {
        if (check and getSettingByName<T>(module, name) != nullptr) return;
        settings[module][name] = std::make_unique<SettingType<T>>(name, defaultValue);
    }

    template<typename T>
    static SettingType<T>* getSettingByName(const std::string& module, const std::string& name) {
        auto it = settings[module].find(name);
        if (it != settings[module].end()) {
            return static_cast<SettingType<T>*>(it->second.get());
        }
        return nullptr;
    }

    static std::string ToJson() {
        json jsonData;

        for (const auto& modulePair : settings) {
            const std::string& moduleName = modulePair.first;
            const std::unordered_map<std::string, std::unique_ptr<Setting>>& moduleSettings = modulePair.second;

            json moduleJson;
            for (const auto& settingPair : moduleSettings) {
                moduleJson.push_back(settingPair.second->ToJson());
            }

            jsonData[moduleName] = moduleJson;
        }

        return jsonData.dump(4);
    }


    static void FromJson(const std::string& jsonString) {
        try {
            json jsonData = json::parse(jsonString);

            for (const auto& moduleItem : jsonData.items()) {
                const std::string& moduleName = moduleItem.key();
                const json& moduleSettingsJson = moduleItem.value();

                for (const auto& setting : moduleSettingsJson) {
                    std::string name = setting["name"].get<std::string>();

                    if (setting["value"].is_number_integer()) {
                        int value = setting["value"].get<int>();
                        Settings::addSetting(moduleName, name, value);
                    }else if (setting["value"].is_number_float()) {
                        float value = setting["value"].get<float>();
                        Settings::addSetting(moduleName, name, value);
                    }
                    else if (setting["value"].is_string()) {
                        std::string value = setting["value"].get<std::string>();
                        Settings::addSetting(moduleName, name, value);
                    }
                    else if (setting["value"].is_boolean()) {
                        bool value = setting["value"].get<bool>();
                        Settings::addSetting(moduleName, name, value);
                    }
                    else if (setting["value"].is_array()) {
                        json floatArray = setting["value"];
                        std::vector<float> value = {};
                        int z = 0;
                        for (const auto& value1 : floatArray) {
                            value.emplace_back(value1.get<float>());
                        }
                        Settings::addSetting(moduleName, name, value);

                    }
                    else if (setting["value"].is_null()) {
                        // Handle null value if needed
                    }
                    else {
                        // Handle unsupported value type
                    }
                }
            }
        }
        catch (const std::exception& e) {
            if (!jsonString.empty())
                Logger::error(e.what());
        }
    }


    static void load(std::string name = "current_config") {

        std::string settingspath = Utils::getRoamingPath() + std::format("\\Lyra\\Config\\{0}.json", name);

        if (!std::filesystem::exists(settingspath)) {

            std::filesystem::path filePath(settingspath);
            std::filesystem::create_directories(filePath.parent_path());

            HANDLE fileHandle = CreateFileA(settingspath.c_str(), GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr,
                OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

            if (fileHandle == INVALID_HANDLE_VALUE) {
                Logger::error("Failed to create file: " + settingspath);
                return;
            }

            CloseHandle(fileHandle);
        }

        std::ifstream inputFile(settingspath);
        std::stringstream ss;

        if (inputFile.is_open()) {
            ss << inputFile.rdbuf();
            inputFile.close();
        }
        else {
            Logger::error("File could not be opened. Maybe it doesn't exist?: " + settingspath);
            return;
        }

        std::string settingstring = ss.str();
        FromJson(settingstring);
    }
    static void saveNewConfig(std::string configName) {
        load(configName);
        SaveSettings(configName);
    }
    
    static void SaveSettings(std::string configName = "current_config") {
        try {
            std::ofstream outputFile(Utils::getRoamingPath() + std::format("\\Lyra\\Config\\{0}.json",configName));
            if (outputFile.is_open()) {
                std::string jsonString = ToJson();
                outputFile << jsonString;
                outputFile.close();
            }
            else {
                Logger::error("Failed to open file. Maybe it doesn't exist?: " + Utils::getRoamingPath() + "\\Lyra\\Config\\config.json");
            }
        }
        catch (const std::exception& ex) {
            Logger::error(ex.what());
        }
    }

private:
    static std::unordered_map<std::string, std::unordered_map<std::string, std::unique_ptr<Setting>>> settings;
};
