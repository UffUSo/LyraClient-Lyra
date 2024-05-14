#include "Render.hpp"

ID3D11Device* d3d11Device = nullptr;
ID3D12Device* d3d12Device = nullptr;

ID3D12DescriptorHeap* d3d12DescriptorHeapImGuiRender = nullptr;
ID3D12DescriptorHeap* d3d12DescriptorHeapBackBuffers = nullptr;
ID3D12GraphicsCommandList* d3d12CommandList = nullptr;
ID3D12CommandQueue* d3d12CommandQueue = nullptr;
ID3D12CommandAllocator* allocator = nullptr;

uint64_t buffersCounts = -1;
std::vector<FrameContext> frameContexts = {};
ID3D12Device5* d3d12Device5 = {};

ID2D1Bitmap1* D2D1Bitmap = nullptr;