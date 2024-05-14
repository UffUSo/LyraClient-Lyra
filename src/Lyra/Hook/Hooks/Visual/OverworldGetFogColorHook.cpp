#include "OverworldGetFogColorHook.hpp"
#include "../../../Event/EventManager.hpp"

OverworldGetFogColorHook::OverworldGetFogColorHook() : Hook("Overworld_fog_colour_hook", "41 0F 10 08 48 8B C2 0F 28 D3")
{
}

void OverworldGetFogColorHook::enableHook()
{
	this->autoHook((void*)OverworldGetFogColorCallback, (void**)&func_original);
}

mce::Color* OverworldGetFogColorHook::OverworldGetFogColorCallback(void* a1, void* a2, mce::Color& colour, float a4)
{
	auto result = func_original(a1, a2, colour, a4);
	auto e = GetFogColorEvent(*result);
	eventMgr.onGetFogColorEvent(e);
	*result = e.getColour();
	return result;
}