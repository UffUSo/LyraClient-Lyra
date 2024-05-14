#include "ResizeHook.hpp"
#include "DirecXHook.hpp"
#include "../../../ImGUI/imgui.h"
#include "../../../../Utils/RenderUtils.h"

void ResizeHook::enableHook() {

    int index;

    if (kiero::getRenderType() == kiero::RenderType::D3D12)
        index = 145;
    else index = 13;

    auto ResizePtr = (void*)kiero::getMethodsTable()[index];

    this->manualHook(ResizePtr, resizeCallback, (void**)&func_original);

}

ResizeHook::ResizeHook() : Hook("Resize Hook", "") {}

void ResizeHook::resizeCallback(IDXGISwapChain* pSwapChain, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT flags)
{

    Memory::SafeRelease(D2D1Bitmap);

    if (DirectXHook::D2DContextIntialized) {

        Memory::SafeRelease(D2D::context);

        DirectXHook::D2DContextIntialized = false;
    }

    Memory::SafeRelease(RenderUtils::blur);

    RenderUtils::brushCache.clear();

    return func_original(pSwapChain, bufferCount, width, height, newFormat, flags);

}
