#include "ConfigManager.hpp"

// Define the static member
std::unordered_map<std::string, std::unordered_map<std::string, std::unique_ptr<Setting>>> Settings::settings;
