#include "Utils.hpp"
#include "Logger/Logger.hpp"
#include <sstream>
#include <string>
#include <algorithm>
#include <stdlib.h>
#include <windows.h>
#include <thread>
#include <wininet.h>

#include "../Lyra/Config/ConfigManager.hpp"

ImVec2 Utils::mousepos = ImVec2();

std::string Utils::getRoamingPath()
{
    return (getenv("AppData") + (std::string)"\\..\\Local\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe\\RoamingState");
};

std::string Utils::removeNonAlphanumeric(const std::string& input)
{
    std::string result;
    std::copy_if(input.begin(), input.end(), std::back_inserter(result), [](char c) {
        return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_' || c == ' ' || c == '-';
    });
    return result;
}

float Utils::animate(float endPoint, float current, float speed) {
    //speed = MC::Deltatime * speed;
    if (Settings::getSettingByName<bool>("Mod Menu", "DISABLE ANIMATIONS")->value) {
        return endPoint;
    }
    if (speed < 0.0) speed = 0.0;
    else if (speed > 1.0) speed = 1.0;

    float dif = (((endPoint) > (current)) ? (endPoint) : (current)) - (((endPoint) < (current)) ? (endPoint) : (current));
    float factor = dif * speed;
    return current + (endPoint > current ? factor : -factor);
}

ImColor Utils::LerpColor(ImColor color1, ImColor color2, float t) {
    if (!Settings::getSettingByName<bool>("Mod Menu", "DISABLE ANIMATIONS")->value) {
        float r = color1.Value.x + (color2.Value.x - color1.Value.x) * t;
        float g = color1.Value.y + (color2.Value.y - color1.Value.y) * t;
        float b = color1.Value.z + (color2.Value.z - color1.Value.z) * t;
        float a = color1.Value.w + (color2.Value.w - color1.Value.w) * t;

        return ImColor(r, g, b, a);
    } else return ImColor(color2);
}

std::string Utils::removeColorCodes(const std::string &input) {
    std::string result;
    bool skipNext = false;

    int wchars_num = MultiByteToWideChar(CP_ACP, 0, input.c_str(), -1, NULL, 0);
    std::wstring wide;
    wide.resize(wchars_num);
    MultiByteToWideChar(CP_ACP, 0, input.c_str(), -1, &wide[0], wchars_num);

    for (wchar_t c: wide) {
        if (skipNext) {
            skipNext = false;
        } else if (c == L'§') {
            skipNext = true;
        } else {
            result += c;
        }
    }

    return result;
}

std::string Utils::remomveNonNumeric(const std::string& input)
{
    std::string result;
    std::copy_if(input.begin(), input.end(), std::back_inserter(result), [](char c) {
        return (c >= '0' && c <= '9') || c == '.';
    });
    return result;
}

bool Utils::IsMouseOverRectangle(ImVec2 mouse, ImVec2 rectPos, ImVec2 size) {
    // Check if the mouse coordinates are within the bounds of the rectangle
    if (mouse.x >= rectPos.x && mouse.x <= (rectPos.x + size.x) &&
        mouse.y >= rectPos.y && mouse.y <= (rectPos.y + size.y)) {
        return true; // Mouse is over the rectangle
    }

    return false; // Mouse is not over the rectangle
}

std::string Utils::replaceAll(std::string subject, const std::string& search,
                       const std::string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
        subject.replace(pos, search.length(), replace);
        pos += replace.length();
    }
    return subject;
}


std::string Utils::DownloadString(std::string URL) {
    HINTERNET interwebs = InternetOpenA("Samsung Smart Fridge", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
    HINTERNET urlFile;
    std::string rtn;
    if (interwebs) {
        urlFile = InternetOpenUrlA(interwebs, URL.c_str(), NULL, NULL, NULL, NULL);
        if (urlFile) {
            char buffer[2000];
            DWORD bytesRead;
            do {
                InternetReadFile(urlFile, buffer, 2000, &bytesRead);
                rtn.append(buffer, bytesRead);
                memset(buffer, 0, 2000);
            } while (bytesRead);
            InternetCloseHandle(interwebs);
            InternetCloseHandle(urlFile);
            std::string p = replaceAll(rtn, "|n", "\r\n");
            return p;
        }
    }
    InternetCloseHandle(interwebs);
    std::string p = replaceAll(rtn, "|n", "\r\n");
    return p;
}


std::string Utils::GetKeyAsString(int key, bool isCapital, bool isKeybind) {

    if (key == 32) { // Spacebar key
        return " ";
    }

    if(isKeybind) {
        if (key == 18) return "ALT";
        if (key == 17) return "CTRL";
        if (key == 16) return "SHIFT";
        if (key == 9) return "TAB";
        if (key == 45) return "INS";
        if (key == 46) return "DEL";
        if (key == 36) return "HOME";
        if (key == 35) return "END";
        if (key == 33) return "PG UP";
        if (key == 34) return "PG DOWN";
    }
    // 0-9
    if (key >= '0' && key <= '9') return std::string(1, (char)key);
    else if (isCapital) {
        // A-Z
        if (key >= 'A' && key <= 'Z') return std::string(1, (char)key);
        else switch (key) {
            // symbols
            case 188: return "<";
            case 190: return ">";
            case 192: return "~";
            case 40: return "(";
            case 41: return ")";
            case 37: return "%";
            case 91: return "{";
            case 93: return "}";
            case 220: return "|";
            case 191: return "?";
            case 186: return ":";
            case 187: return "+";
            case 219: return "{";
            case 221: return "}";
        }
    } else {
        // a - z
        if (key >= 'A' && key <= 'Z') return std::string(1, (char)(key+32));
        switch (key) {
            // symbols
            case 188: return ",";
            case 190: return ".";
            case 192: return "`";
            case 219: return "[";
            case 221: return "]";
            case 220: return "\\";
            case 191: return "/";
            case 187: return "=";
            case 186: return (std::string)";";
        }
    }
    // Return empty string for unsupported key codes or non-alphabetic keys
    return "";
};

int Utils::GetStringAsKey(const std::string& str) {


    if(str == "ALT" || str == "alt") return 18;
    if(str == "CTRL" || str == "ctrl") return 17;
    if(str == "SHIFT" || str == "shift") return 16;
    if(str == "INS" || str == "ins") return 45;
    if(str == "DEL" || str == "del") return 46;
    if(str == "HOME" || str == "home") return 36;
    if(str == "PG UP" || str == "pg up") return 33;
    if(str == "PG DOWN" || str == "pg down") return 34;
    if(str == "TAB") return 9;
    if(str == "{") return 219;
    if(str == "}") return 221;
    if(str == "+") return 187;
    if(str == ":") return 186;
    if(str == "?") return 191;
    if(str == "|") return 220;
    if(str == "`" || str == "~") return 192;
    if(str == " ") return 32;

    char c = str[0];

    return static_cast<int>(std::toupper(c));

    // Unsupported character or non-alphabetic key
    return -1;
}

 std::vector<int> Utils::GetStringAsKeys(const std::string str) {
    std::vector<int> keys;

    if (str.empty()) {
        // Empty string
        return keys;
    }

    std::stringstream iss(str);
    std::string token;
    while (std::getline(iss, token, '+')) {
        int key = Utils::GetStringAsKey(token);
        if (key != -1) {
            keys.push_back(key);
        }
    }

    return keys;
}
