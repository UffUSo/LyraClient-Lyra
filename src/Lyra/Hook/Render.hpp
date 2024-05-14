#pragma once

#include <Windows.h>
#include <MinHook.h>
#include <algorithm>
#include <optional>
#include <vector>

#include <dxgi.h>
#include <d3d11.h>
#include <d3d12.h>
#include <d2d1_1.h>
#include <dxgi1_4.h>
#include <initguid.h>
#include <wincodec.h>

#include <stdint.h>

struct FrameContext {
	ID3D12CommandAllocator* commandAllocator = nullptr;
	ID3D12Resource* main_render_target_resource = nullptr;
	D3D12_CPU_DESCRIPTOR_HANDLE main_render_target_descriptor;
};

namespace D2D {

    inline ID2D1DeviceContext* context;
    inline IDXGISurface1* surface = nullptr;
};


extern ID3D11Device* d3d11Device;
extern ID3D12Device* d3d12Device;

extern ID3D12DescriptorHeap* d3d12DescriptorHeapImGuiRender;
extern ID3D12DescriptorHeap* d3d12DescriptorHeapBackBuffers;
extern ID3D12GraphicsCommandList* d3d12CommandList;
extern ID3D12CommandQueue* d3d12CommandQueue;
extern ID3D12CommandAllocator* allocator;

extern uint64_t buffersCounts;
extern std::vector<FrameContext> frameContexts;
extern ID3D12Device5* d3d12Device5;

extern ID2D1Bitmap1* D2D1Bitmap;
