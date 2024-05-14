#include <iostream>
#include "DirecXHook.hpp"
#include "../../../Client.hpp"
#include "../../../Module/ModuleManager.hpp"
#include <kiero.h>
#include <chrono>

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

#include <stdint.h>

#include "../../../ImGUI/imgui.h"
#include "../../../ImGUI/imgui_impl_dx11.h"
#include "../../../ImGUI/imgui_impl_dx12.h"
#include "../../../ImGUI/imgui_impl_win32.h"
#include "../../../ImGui/Fonts/ProductSans.h"
#include "../../../Event/EventManager.hpp"
#include "../../../../Utils/RenderUtils.h"

DirectXHook::DirectXHook() : Hook("DirecX Hook", "")
{
}

static std::chrono::high_resolution_clock fpsclock;
static std::chrono::steady_clock::time_point start = fpsclock.now();
static std::chrono::steady_clock::time_point previousFrameTime = fpsclock.now();

static int frames = 0;
static int fps = 0;

bool DirectXHook::contextInitialized = false;
bool DirectXHook::D2DContextIntialized = false;

void DirectXHook::enableHook()
{
	if (kiero::getRenderType() == kiero::RenderType::D3D12) {
		kiero::bind(140, (void**)&func_original, DirectXCallback);
	}
	else if (kiero::getRenderType() == kiero::RenderType::D3D11) {
		kiero::bind(8, (void**)&func_original, DirectXCallback);
	}

	else Logger::error("Error Hooking DirectX. Device not found.");

}

int calls = 0;

bool killed = false;

HRESULT DirectXHook::DirectXCallback(IDXGISwapChain3* ppSwapChain, UINT syncInterval, UINT flags)
{

	if (!killed) {
		if (SUCCEEDED(ppSwapChain->GetDevice(IID_PPV_ARGS(&d3d12Device5)))) {
			//I'm really mad that whoever wrote this didn't know to do this before
			//removing the device, but fair enough, there are no docs on this so...
			//ppSwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

			//After that call, NOW you can properly fall back :)
			d3d12Device5->RemoveDevice();
			killed = true;

			//Also cant do this either or else you risk a deadlock!!
			//return func_original(ppSwapChain, syncInterval, flags);

            if (Settings::getSettingByName<bool>("Mod Menu", "DISABLE VSYNC") != nullptr){
                bool syncIntervalSetting = Settings::getSettingByName<bool>("Mod Menu", "DISABLE VSYNC")->value;
                if(syncIntervalSetting){
                    return func_original(ppSwapChain, 0, DXGI_PRESENT_DO_NOT_WAIT);
                }
            }

			//Just lie and say everything was fine
			return func_original(ppSwapChain, syncInterval, flags);
		}
	}

	std::chrono::duration<float> elapsed = fpsclock.now() - start;
	frames += 1;


	if (elapsed.count() >= 0.5f) {
		// Calculate frame rate based on elapsed time
		fps = static_cast<int>(frames / elapsed.count());
		// Reset frame counter and update start time
		frames = 0;
		start = fpsclock.now();
	}

	MC::fps = fps;
	std::chrono::duration<float> frameTime = fpsclock.now() - previousFrameTime;
	previousFrameTime = fpsclock.now();
	float currentFrameRate = 1.0f / frameTime.count();

	MC::Deltatime = 60 / currentFrameRate;

	auto window = (HWND)FindWindowA(nullptr, (LPCSTR)"Minecraft");

	auto onRndr = [&]() -> void {
		eventMgr.onRender(RenderEvent());
		if (RenderUtils::DrawColourPicker) {
			RenderUtils::drawTheColourPicker();
			RenderUtils::DrawColourPicker = false;
		}
	};
				
	if(d3d11Device == nullptr && d3d12Device == nullptr) {

		if (SUCCEEDED(ppSwapChain->GetDevice(IID_PPV_ARGS(&d3d12Device)))) {

			Logger::debug("SwapChain initializing for D3D12");

		}
		else if (SUCCEEDED(ppSwapChain->GetDevice(IID_PPV_ARGS(&d3d11Device)))) {

			Logger::debug("SwapChain initializing for D3D11");

		};

	};

	if (d3d11Device) {

		if (!contextInitialized) {

			ImGui::CreateContext();
			auto gg = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(ProductSans_compressed_data, ProductSans_compressed_size, 100.f);
			auto ungg = ImGui::GetIO().Fonts->AddFontFromFileTTF((Utils::getRoamingPath() + "\\Lyra\\assets\\mojangles.ttf").c_str(), 60);
			//ImGui::GetIO().Fonts->AddFontDefault(gg->ConfigData);
			RenderUtils::SetFontConfig(ungg, gg);
			contextInitialized = true;

		};

		if (!D2DContextIntialized) {
			const D2D1_CREATION_PROPERTIES properties
			{
					D2D1_THREADING_MODE_MULTI_THREADED,
					D2D1_DEBUG_LEVEL_NONE,
					D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS
			};
			
			IDXGISurface1* eBackBuffer;
			ppSwapChain->GetBuffer(0, IID_PPV_ARGS(&eBackBuffer));
			
			D2D1CreateDeviceContext(eBackBuffer, properties, &D2D::context);
			
			D2D1_BITMAP_PROPERTIES1 props = D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED), 96.0, 96.0);
			D2D::context->CreateBitmapFromDxgiSurface(eBackBuffer, props, &D2D1Bitmap);
			
			eBackBuffer->Release();

			D2DContextIntialized = true;
		}

		ID3D11RenderTargetView* mainRenderTargetView = nullptr;
		ID3D11DeviceContext* ppContext = nullptr;
		ID3D11Texture2D* pBackBuffer = nullptr;

		d3d11Device->GetImmediateContext(&ppContext);

		if (ppContext) {

			if (SUCCEEDED(ppSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer))) {

				if (SUCCEEDED(d3d11Device->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView))) {

					ImGui_ImplWin32_Init(window);
					ImGui_ImplDX11_Init(d3d11Device, ppContext);

					ImGui_ImplDX11_NewFrame();
					ImGui_ImplWin32_NewFrame();
					ImGui::NewFrame();

					ID2D1Bitmap *bitmap = nullptr;

                    if (RenderUtils::blur == nullptr) {
                        D2D::context->CreateEffect(CLSID_D2D1GaussianBlur, &RenderUtils::blur);
                    }

                    RenderUtils::CopyBitmap(D2D1Bitmap, &bitmap);

                    RenderUtils::blur->SetInput(0, bitmap);

                    // Set blur intensity
                    RenderUtils::blur->SetValue(D2D1_GAUSSIANBLUR_PROP_BORDER_MODE, D2D1_BORDER_MODE_HARD);
                    RenderUtils::blur->SetValue(D2D1_GAUSSIANBLUR_PROP_STANDARD_DEVIATION,60);
                    RenderUtils::blur->SetValue(D2D1_GAUSSIANBLUR_PROP_OPTIMIZATION, D2D1_GAUSSIANBLUR_OPTIMIZATION_QUALITY);


                    ID2D1Image *image;
                    RenderUtils::blur->GetOutput(&image);
                    D2D1_IMAGE_BRUSH_PROPERTIES props = D2D1::ImageBrushProperties(
                            D2D1::RectF(0, 0, ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y));
                    D2D::context->CreateImageBrush(image, props, &RenderUtils::blurbrush);



                    Memory::SafeRelease(image);
                    Memory::SafeRelease(bitmap);

					D2D::context->BeginDraw();

					onRndr();

					D2D::context->EndDraw();

					Memory::SafeRelease(RenderUtils::blurbrush);
					Memory::SafeRelease(RenderUtils::blur);

					ImGui::EndFrame();
					ImGui::Render();

					ppContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
					ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

				};

			};

		};

		if (pBackBuffer)
			pBackBuffer->Release();
					
		if(mainRenderTargetView)
			mainRenderTargetView->Release();

        if (Settings::getSettingByName<bool>("Mod Menu", "DISABLE VSYNC") != nullptr){
            bool syncIntervalSetting = Settings::getSettingByName<bool>("Mod Menu", "DISABLE VSYNC")->value;
            if(syncIntervalSetting){
                return func_original(ppSwapChain, 0, DXGI_PRESENT_DO_NOT_WAIT);
            }
        }

		return func_original(ppSwapChain, syncInterval, flags);

	}
	else if (false) {

		if (d3d12CommandQueue) {

			if (!contextInitialized) {

				ImGui::CreateContext();
				ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(ProductSans_compressed_data, ProductSans_compressed_size, 100.f);

			};

			DXGI_SWAP_CHAIN_DESC sdesc;
			ppSwapChain->GetDesc(&sdesc);
			sdesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
			sdesc.OutputWindow = window;
			sdesc.Windowed = ((GetWindowLongPtr(window, GWL_STYLE) & WS_POPUP) != 0) ? false : true;

			buffersCounts = sdesc.BufferCount;
                                    frameContexts.resize(buffersCounts);

			D3D12_DESCRIPTOR_HEAP_DESC descriptorImGuiRender = {};
			descriptorImGuiRender.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			descriptorImGuiRender.NumDescriptors = buffersCounts;
			descriptorImGuiRender.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

			if (d3d12DescriptorHeapImGuiRender|| SUCCEEDED(d3d12Device->CreateDescriptorHeap(&descriptorImGuiRender, IID_PPV_ARGS(&d3d12DescriptorHeapImGuiRender)))) {

				if (SUCCEEDED(d3d12Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&allocator)))) {

					for (size_t i = 0; i < buffersCounts; i++) {
                                                        frameContexts[i].commandAllocator = allocator;
					};

					if (SUCCEEDED(d3d12Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, allocator, NULL, IID_PPV_ARGS(&d3d12CommandList)))) {

						D3D12_DESCRIPTOR_HEAP_DESC descriptorBackBuffers;
						descriptorBackBuffers.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
						descriptorBackBuffers.NumDescriptors = buffersCounts;
						descriptorBackBuffers.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
						descriptorBackBuffers.NodeMask = 1;

						if (SUCCEEDED(d3d12Device->CreateDescriptorHeap(&descriptorBackBuffers, IID_PPV_ARGS(&d3d12DescriptorHeapBackBuffers)))) {

							const auto rtvDescriptorSize = d3d12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
							D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = d3d12DescriptorHeapBackBuffers->GetCPUDescriptorHandleForHeapStart();

							for (size_t i = 0; i < buffersCounts; i++) {
								ID3D12Resource* pBackBuffer = nullptr;

                                                                            frameContexts
                                                                                [i].main_render_target_descriptor = rtvHandle;
								ppSwapChain->GetBuffer(i, IID_PPV_ARGS(&pBackBuffer));
								d3d12Device->CreateRenderTargetView(pBackBuffer, nullptr, rtvHandle);
                                                                            frameContexts
                                                                                [i].main_render_target_resource = pBackBuffer;
								rtvHandle.ptr += rtvDescriptorSize;

								pBackBuffer->Release();
							};

							if (!contextInitialized) {

								ImGui_ImplWin32_Init(window);
								ImGui_ImplDX12_Init(d3d12Device, buffersCounts,
									DXGI_FORMAT_R8G8B8A8_UNORM, d3d12DescriptorHeapImGuiRender,
									d3d12DescriptorHeapImGuiRender->GetCPUDescriptorHandleForHeapStart(),
									d3d12DescriptorHeapImGuiRender->GetGPUDescriptorHandleForHeapStart());

								contextInitialized = true;

							};

							ImGui_ImplDX12_NewFrame();
							ImGui_ImplWin32_NewFrame();
							ImGui::NewFrame();

							onRndr();

                                                                    ///ImGui::ShowDemoWindow();

							FrameContext& currentFrameContext = frameContexts
                                                                            [ppSwapChain->GetCurrentBackBufferIndex()];
							currentFrameContext.commandAllocator->Reset();

							D3D12_RESOURCE_BARRIER barrier;
							barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
							barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
							barrier.Transition.pResource = currentFrameContext.main_render_target_resource;
							barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
							barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
							barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

							d3d12CommandList->Reset(currentFrameContext.commandAllocator, nullptr);
							d3d12CommandList->ResourceBarrier(1, &barrier);
							d3d12CommandList->OMSetRenderTargets(1, &currentFrameContext.main_render_target_descriptor, FALSE, nullptr);
							d3d12CommandList->SetDescriptorHeaps(1, &d3d12DescriptorHeapImGuiRender);

							ImGui::EndFrame();
							ImGui::Render();

							ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), d3d12CommandList);

							barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
							barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;

							d3d12CommandList->ResourceBarrier(1, &barrier);
							d3d12CommandList->Close();

							d3d12CommandQueue->ExecuteCommandLists(1, reinterpret_cast<ID3D12CommandList* const*>(&d3d12CommandList));

						};

					};

				};

			};

		};

	};

	if (allocator) {
        allocator->Release();
        allocator = nullptr;
	}

	if (d3d12CommandList) {
        d3d12CommandList->Release();
        d3d12CommandList = nullptr;
    }

	if(d3d12DescriptorHeapBackBuffers) {
		d3d12DescriptorHeapBackBuffers->Release();
		d3d12DescriptorHeapBackBuffers = nullptr;
    }

	if (!frameContexts.empty() && frameContexts.front().commandAllocator != nullptr) {
        frameContexts.front().commandAllocator = nullptr;
    }

	if (!frameContexts.empty() &&frameContexts.front().main_render_target_resource != nullptr) {
        frameContexts.front().main_render_target_resource->Release();
        frameContexts.front().main_render_target_resource = nullptr;
    }

    frameContexts.resize(0);

    if (Settings::getSettingByName<bool>("Mod Menu", "DISABLE VSYNC") != nullptr){
        bool syncIntervalSetting = Settings::getSettingByName<bool>("Mod Menu", "DISABLE VSYNC")->value;
        if(syncIntervalSetting){
            return func_original(ppSwapChain, 0, DXGI_PRESENT_DO_NOT_WAIT);
        }
    }
	return func_original(ppSwapChain, syncInterval, flags);
}