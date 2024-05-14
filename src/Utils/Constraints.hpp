#pragma once

#include "../Lyra/ImGUI/imgui.h"
#include <string>

class Constraints
{
public:
    static float PercentageConstraint(float percentage, const int edge = 1);
    static float PosToPercent(float Pos, const int& edge);
    static float RelativeConstraint(float percent, const std::string& dimension = "height");
    static float RoundingConstraint(float radius);
    static float RoundingCalc(ImVec2 size, float radius);
};