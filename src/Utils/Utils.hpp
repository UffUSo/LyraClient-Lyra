#pragma once
#include <string>
#include <cstdlib>
#include <cmath>
#include <vector>
#include "../Lyra/ImGui/imgui.h"
#include "Vector.hpp"

class Utils
{
public:
    static ImVec2 mousepos;

    static std::string getRoamingPath();

    static std::string GetKeyAsString(int key, bool isCapital = false, bool isKeybind = true);

    static int GetStringAsKey(const std::string &str);

    static std::vector<int> GetStringAsKeys(const std::string str);

    static std::string removeNonAlphanumeric(const std::string& input);

    static std::string remomveNonNumeric(const std::string &input);

    static bool IsMouseOverRectangle(ImVec2 mouse, ImVec2 rectPos, ImVec2 size);

    static float animate(float endPoint, float current, float speed);

    static ImColor LerpColor(ImColor color1, ImColor color2, float t);

    static std::string removeColorCodes(const std::string& input);

    static std::string replaceAll(std::string subject, const std::string& search, const std::string& replace);

    static std::string DownloadString(std::string URL);


};

struct MCCColor {
    union {
        struct {
            float r, g, b, a;
        };
        float arr[4];
    };
    bool shouldDelete = true;

    MCCColor() {
        this->r = 1;
        this->g = 1;
        this->b = 1;
        this->a = 1;
    };

    MCCColor(const MCCColor& other) {
        this->r = other.r;
        this->g = other.g;
        this->b = other.b;
        this->a = other.a;
        this->shouldDelete = other.shouldDelete;
    }

    MCCColor(const float* arr) {
        this->arr[0] = arr[0];
        this->arr[1] = arr[1];
        this->arr[2] = arr[2];
        this->arr[3] = arr[3];
    };

    MCCColor(const float r, const float g, const float b, const float a = 1) {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    };

    MCCColor(const int r, const int g, const int b, const int a = 255) {
        this->r = r / 255.0f;
        this->g = g / 255.0f;
        this->b = b / 255.0f;
        this->a = a / 255.0f;
    };

    MCCColor(const float r, const float g, const float b, const float a, const bool shouldDelete) {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
        this->shouldDelete = shouldDelete;
    };
};

#pragma once

struct CaretMeasureData
{
    int Position;
    bool isSingleline;// false|insert

    CaretMeasureData()
    {
        CaretMeasureData(0xFFFFFFFF, true);
    };

    CaretMeasureData(int position, bool singlelines)
    {
        this->Position = position;
        this->isSingleline = singlelines;
    };
};