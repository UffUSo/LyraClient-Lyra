#include <iostream>
#include "GetFovHook.hpp"
#include "../../../Module/Modules/Zoom.hpp"
#include "../../../../Utils/RenderUtils.h"

GetFovHook::GetFovHook() : Hook("Get_FOV_hook", "48 8B ? 48 89 ? ? 48 89 ? ? 57 48 81 EC ? ? ? ? 0F 29 ? ? 0F 29 ? ? 44 0F ? ? ? 44 0F ? ? ? 48 8B ? ? ? ? ? 48 33 ? 48 89 ? ? ? 41 0F")
{
}

void GetFovHook::enableHook()
{
	this->autoHook((void*)GetFovCallback, (void**)&func_original);
}


float GetFovHook::GetFovCallback(void* a1, float f, void* a2, void* a3)
{
	float fov = func_original(a1, f, a2, a3);
    if(fov==70) return fov;

    float targetZoom = Settings::getSettingByName<float>("Zoom", "Target Zoom")->value;
	if(Settings::getSettingByName<bool>("Zoom", "enabled")->value and ShouldZoom) {
        if(!Settings::getSettingByName<bool>("Zoom", "Smooth Zoom")->value) return targetZoom;
    }else{
        if(!Settings::getSettingByName<bool>("Zoom", "Smooth Zoom")->value) return fov;
        targetZoom = fov;
    }
    auto animationSpeed = Settings::getSettingByName<float>("Zoom", "Animation Speed")->value;
    currentZoom = Utils::animate(targetZoom, currentZoom, MC::Deltatime * 0.15f * animationSpeed);
    return currentZoom;
}