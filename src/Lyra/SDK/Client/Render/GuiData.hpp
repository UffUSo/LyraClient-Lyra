#pragma once

#include "../../../../Utils/Memory/Memory.hpp"
#include "../../../../Utils/Utils.hpp"
#include "GuiMessage.hpp"

class GuiData {
public:
	BUILD_ACCESS(this, Vec2, ScreenSize, 0x0030);
	BUILD_ACCESS(this, Vec2, ScreenSizeScaled, 0x0040);
	BUILD_ACCESS(this, float, GuiScale, 0x004C);
    BUILD_ACCESS(this, Vec2, screenResRounded, 0x0038);
    BUILD_ACCESS(this, float, sliderAmount, 0x004C);
    BUILD_ACCESS(this, float, scalingMultiplier, 0x0050);
	BUILD_ACCESS(this, std::vector<GuiMessage*>, guiMessages, 0x140);
};