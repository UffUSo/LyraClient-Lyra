
#include "Constraints.hpp"

float Constraints::PercentageConstraint(float percentage, const int edge)
{
    float screenWidth = ImGui::GetIO().DisplaySize.x;
    float screenHeight = ImGui::GetIO().DisplaySize.y;
    float x = 0;
    float y = 0;

    float position = 0;

    if (edge == 1)
    {
        position = percentage * screenHeight;
        position += y;
    }
    else if (edge == 2) {
        position = percentage * screenWidth;
        position += x;
    }

    return position;
}
float Constraints::PosToPercent(float position, const int& edge)
{
    float Width = ImGui::GetIO().DisplaySize.x;
    float Height = ImGui::GetIO().DisplaySize.y;
    float percent = 0;

    if (edge == 1)
    {
        percent = position / Height;
    }
    else if (edge == 2) {
        percent = position / Width;
    }

    return percent;
}

float Constraints::RelativeConstraint(float percent, const std::string& dimension)
{
    float length;
    float screenWidth;
    float screenHeight;

    screenWidth = ImGui::GetIO().DisplaySize.x;
    screenHeight = ImGui::GetIO().DisplaySize.y;


    if (dimension == "width")
    {
        length = screenWidth * percent;
    }
    else if (dimension == "height")
    {
        length = screenHeight * percent;
    }

    return length;
}

float Constraints::RoundingConstraint(float radius)
{
    return radius * ImGui::GetIO().DisplaySize.y / 1920.0f;
}

float Constraints::RoundingCalc(ImVec2 rectangleSize, float roundingFactor) {
    // Determine the minimum side of the rectangle
    float min_side = (((rectangleSize.x) < (rectangleSize.y)) ? (rectangleSize.x) : (rectangleSize.y));

    // Calculate the rounding radius
    float rounding_radius = min_side * (roundingFactor/2);

    return rounding_radius;
}