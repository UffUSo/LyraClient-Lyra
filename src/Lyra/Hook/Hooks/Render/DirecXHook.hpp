#pragma once

#include <array>
#include "../Hook.hpp"
#include <d2d1_3.h>
#include <d3d12.h>
#include <d3d11.h>
#include <dxgi1_6.h>
#include "../../Render.hpp"

class DirectXHook : public Hook
{
private:
    static HRESULT DirectXCallback(IDXGISwapChain3* pSwapChain, UINT syncInterval, UINT flags);

public:
    static bool contextInitialized;
    static bool D2DContextIntialized;
    typedef HRESULT(__thiscall* DirectXOriginal)(IDXGISwapChain3*, UINT, UINT);
    static inline DirectXOriginal func_original = 0;

    DirectXHook();
    void enableHook() override;
};
