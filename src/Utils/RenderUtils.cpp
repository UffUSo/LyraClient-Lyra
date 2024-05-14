#include "RenderUtils.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../Lyra/ImGui/stb.h"
#include "Utils.hpp"
#include "Memory/Memory.hpp"
#include "../Lyra/Module/ModuleManager.hpp"
#include "../Lyra/Module/Modules/CPS_Limiter.hpp"
std::map<std::string, ID3D11ShaderResourceView*> RenderUtils::images = {};
std::map<std::string, ImTextureID> RenderUtils::imagesDX12 = {};

std::map<int, bool> RenderUtils::TextIndex = {};
int RenderUtils::key = -1;
bool RenderUtils::Capital = false;
bool RenderUtils::BackSpace = false;
std::chrono::time_point<std::chrono::high_resolution_clock> RenderUtils::last_deleted = std::chrono::high_resolution_clock::now();
std::map<int, bool> RenderUtils::SliderDrag = {};

bool RenderUtils::hueDragging = false;
bool RenderUtils::colourDragging = false;

std::map<int, bool> ColourPickerEnable = {};
std::map<int, ImVec2> ColourPickerMarkerPos = {};
std::map<int, float> ColourPickerGradientMarkerPos = {};
std::map<int, ImColor> ColourPickerColor = {};

std::map<int, bool> RenderUtils::keybindIndex = {};
int RenderUtils::KeybindKey = -1;

bool RenderUtils::DrawColourPicker = false;
ImVec2 colourPickerPos = ImVec2();
int CurrentColourPickerIndex = 1;
SettingType<std::vector<float>> w = SettingType<std::vector<float>>("", {});
SettingType<std::vector<float>>* CurrentColour = &w;

std::map<std::tuple<int, int, int>, std::string> RenderUtils::rgbToHexMap = {};

ID2D1Factory* RenderUtils::factory = nullptr;
ID2D1ImageBrush* RenderUtils::blurbrush = nullptr;
ID2D1Effect* RenderUtils::blur = nullptr;
std::map<std::string, ID2D1SolidColorBrush*> RenderUtils::brushCache = {};

bool RenderUtils::CloseMenu = true;
bool CScoreP = false;
bool CScoreN = false;
std::chrono::time_point<std::chrono::high_resolution_clock> clickTS;

SwapChainDeviceType RenderUtils::type = SwapChainDeviceType::INVALID;

ImFont* mojangles = nullptr;
ImFont* NOTmojangles = nullptr;

void RenderUtils::SetFontConfig(ImFont* mojanglse, ImFont* notMojanglse) {
	mojangles = mojanglse;
	NOTmojangles = notMojanglse;
}

bool googoo = false;

ImVec4 RenderUtils::RenderText(ImVec2 position, ImVec2 PaddingSize, ImU32 color, std::string text, float fontSize, int alignment, ImDrawList* drawList, bool Mojangles) {
	ImFont* font = (Mojangles ? mojangles : NOTmojangles);
	//ImGui::GetIO().Fonts->Fonts[textIndex]; //->RenderText(drawList, fontSize / 2, position, color, ImVec4(0, 0, Constraints::PercentageConstraint(1, 2), Constraints::PercentageConstraint(1)), text.c_str(), (const char*)0);
	//drawList->AddText(position, color, text.c_str());
	ImGui::PushFont(font);
    ImGui::SetWindowFontScale(fontSize/2);
	switch (alignment) {
		case 1: break;
		case 2: {
			position.x += (PaddingSize.x / 2) - (ImGui::CalcTextSize(text.c_str()).x / 2);
			break;
		}
		case 3: {
			position.x += (PaddingSize.x - ImGui::CalcTextSize(text.c_str()).x); 
			break;
		}
	}
	position.y += (PaddingSize.y / 2) - (ImGui::CalcTextSize(text.c_str()).y / 2);
    ImVec2 textPos = ImVec2(position.x, position.y);
    drawList->AddText(textPos, color, text.c_str());
    ImGui::PopFont();

	return ImVec4(position.x, position.y, ImGui::CalcTextSize(text.c_str()).x, ImGui::CalcTextSize(text.c_str()).y);
}

void RenderUtils::RenderTextWithOutline(ImVec2 pos, ImVec2 padSize, ImU32 color, std::string text, float fontSize, ImU32 outlineColor, float outlineWidth, int alignment, ImDrawList* drawList) {
	// outline
	RenderText(ImVec2(pos.x + outlineWidth, pos.y), padSize, outlineColor, text, fontSize, alignment, drawList);
	RenderText(ImVec2(pos.x - outlineWidth, pos.y), padSize, outlineColor, text, fontSize, alignment, drawList);
	RenderText(ImVec2(pos.x, pos.y + outlineWidth), padSize, outlineColor, text, fontSize, alignment, drawList);
	RenderText(ImVec2(pos.x, pos.y - outlineWidth), padSize, outlineColor, text, fontSize, alignment, drawList);
	RenderText(ImVec2(pos.x + outlineWidth, pos.y + outlineWidth), padSize, outlineColor, text, fontSize, alignment, drawList);
	RenderText(ImVec2(pos.x - outlineWidth, pos.y - outlineWidth), padSize, outlineColor, text, fontSize, alignment, drawList);
	RenderText(ImVec2(pos.x - outlineWidth, pos.y + outlineWidth), padSize, outlineColor, text, fontSize, alignment, drawList);
	RenderText(ImVec2(pos.x + outlineWidth, pos.y - outlineWidth), padSize, outlineColor, text, fontSize, alignment, drawList);

	// actual text
	RenderText(pos, padSize, color, text, fontSize, alignment, drawList);
}

auto RenderUtils::getTextSize(std::string text, float fontSize) -> ImVec2 {
    auto font = ImGui::GetFont();
    return font->CalcTextSizeA(fontSize, 1000.f, -1.f, text.c_str());
};

auto RenderUtils::fillRect(ImVec2 rectPos, ImVec2 rectSize, ImColor color, float round = 0.f, ImDrawList* drawList) -> void {
	drawList->AddRectFilled(
		rectPos,
		ImVec2(rectPos.x + rectSize.x, rectPos.y + rectSize.y),
		color,
		round,
		240
	);
	ImGui::ColorPicker4;
};

auto RenderUtils::drawRect(ImVec2 rectPos, ImVec2 rectSize, ImColor color, float round = 0.f, float thickness = 1.f, ImDrawList* drawList) -> void {
    drawList->AddRect(rectPos, ImVec2(rectPos.x + rectSize.x, rectPos.y + rectSize.y), color, round, ImDrawFlags_RoundCornersAll, thickness);
};

void RenderUtils::RenderImage(ImVec2 Pos, ImVec2 size, std::string imagePath, ImColor imgColour, ImDrawList* drawList) {
	if (d3d12Device == nullptr) {
		if (images[imagePath] == nullptr) {
			std::string path = getenv("AppData") + (std::string)"\\..\\Local\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe\\RoamingState\\" + imagePath;
			bool ret = LoadTextureFromFile(path.c_str(), &images[imagePath], (int*)&size.x, (int*)&size.y);
			if (!ret) {
				return;
			}
		}
		else if (images[imagePath] != nullptr) {
			drawList->AddImage(images[imagePath], Pos, ImVec2(Pos.x + size.x, Pos.y + size.y), ImVec2(0, 0), ImVec2(1, 1), imgColour);
		}
	}
	else {
		if (imagesDX12[imagePath] == nullptr) {
			static_assert(sizeof(ImTextureID) >= sizeof(D3D12_CPU_DESCRIPTOR_HANDLE), "D3D12_CPU_DESCRIPTOR_HANDLE is too large to fit in an ImTextureID");

			// We presume here that we have our D3D device pointer in g_pd3dDevice

			int my_image_width = 0;
			int my_image_height = 0;
			ID3D12Resource* my_texture = NULL;

			// Get CPU/GPU handles for the shader resource view
			// Normally your engine will have some sort of allocator for these - here we assume that there's an SRV descriptor heap in
			// g_pd3dSrvDescHeap with at least two descriptors allocated, and descriptor 1 is unused
			UINT handle_increment = d3d12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			int descriptor_index = imagesDX12.size() + 1; // The descriptor table index to use (not normally a hard-coded constant, but in this case we'll assume we have slot 1 reserved for us)
			D3D12_CPU_DESCRIPTOR_HANDLE cpu = d3d12DescriptorHeapImGuiRender->GetCPUDescriptorHandleForHeapStart();
			cpu.ptr += (handle_increment * descriptor_index);
			D3D12_GPU_DESCRIPTOR_HANDLE gpu = d3d12DescriptorHeapImGuiRender->GetGPUDescriptorHandleForHeapStart();
			gpu.ptr += (handle_increment * descriptor_index);

			std::string path = getenv("AppData") + (std::string)"\\..\\Local\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe\\RoamingState" + "\\" + imagePath;
			bool ret = LoadTextureFromFileDX12(path.c_str(), d3d12Device, cpu, &my_texture, &my_image_width, &my_image_height);
			IM_ASSERT(ret);
			imagesDX12[imagePath] = (ImTextureID)gpu.ptr;

		}
		else {
			drawList->AddImage(imagesDX12[imagePath], Pos, ImVec2(Pos.x + size.x, Pos.y + size.y), ImVec2(0, 0), ImVec2(1, 1), imgColour);
		}
	}
}

bool RenderUtils::LoadTextureFromFileDX12(const char* filename, ID3D12Device* d3d_device, D3D12_CPU_DESCRIPTOR_HANDLE srv_cpu_handle, ID3D12Resource** out_tex_resource, int* out_width, int* out_height)
{
	// Load from disk into a raw RGBA buffer
	int image_width = 0;
	int image_height = 0;
	unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
	if (image_data == NULL)
		return false;

	// Create texture resource
	D3D12_HEAP_PROPERTIES props;
	memset(&props, 0, sizeof(D3D12_HEAP_PROPERTIES));
	props.Type = D3D12_HEAP_TYPE_DEFAULT;
	props.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	props.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	D3D12_RESOURCE_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	desc.Alignment = 0;
	desc.Width = image_width;
	desc.Height = image_height;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	desc.Flags = D3D12_RESOURCE_FLAG_NONE;

	ID3D12Resource* pTexture = NULL;
	d3d_device->CreateCommittedResource(&props, D3D12_HEAP_FLAG_NONE, &desc,
		D3D12_RESOURCE_STATE_COPY_DEST, NULL, IID_PPV_ARGS(&pTexture));

	// Create a temporary upload resource to move the data in
	UINT uploadPitch = (image_width * 4 + D3D12_TEXTURE_DATA_PITCH_ALIGNMENT - 1u) & ~(D3D12_TEXTURE_DATA_PITCH_ALIGNMENT - 1u);
	UINT uploadSize = image_height * uploadPitch;
	desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Alignment = 0;
	desc.Width = uploadSize;
	desc.Height = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.Flags = D3D12_RESOURCE_FLAG_NONE;

	props.Type = D3D12_HEAP_TYPE_UPLOAD;
	props.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	props.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	ID3D12Resource* uploadBuffer = NULL;
	HRESULT hr = d3d_device->CreateCommittedResource(&props, D3D12_HEAP_FLAG_NONE, &desc,
		D3D12_RESOURCE_STATE_GENERIC_READ, NULL, IID_PPV_ARGS(&uploadBuffer));
	IM_ASSERT(SUCCEEDED(hr));

	// Write pixels into the upload resource
	void* mapped = NULL;
	D3D12_RANGE range = { 0, uploadSize };
	hr = uploadBuffer->Map(0, &range, &mapped);
	IM_ASSERT(SUCCEEDED(hr));
	for (int y = 0; y < image_height; y++)
		memcpy((void*)((uintptr_t)mapped + y * uploadPitch), image_data + y * image_width * 4, image_width * 4);
	uploadBuffer->Unmap(0, &range);

	// Copy the upload resource content into the real resource
	D3D12_TEXTURE_COPY_LOCATION srcLocation = {};
	srcLocation.pResource = uploadBuffer;
	srcLocation.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
	srcLocation.PlacedFootprint.Footprint.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srcLocation.PlacedFootprint.Footprint.Width = image_width;
	srcLocation.PlacedFootprint.Footprint.Height = image_height;
	srcLocation.PlacedFootprint.Footprint.Depth = 1;
	srcLocation.PlacedFootprint.Footprint.RowPitch = uploadPitch;

	D3D12_TEXTURE_COPY_LOCATION dstLocation = {};
	dstLocation.pResource = pTexture;
	dstLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	dstLocation.SubresourceIndex = 0;

	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = pTexture;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

	// Create a temporary command queue to do the copy with
	ID3D12Fence* fence = NULL;
	hr = d3d_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	IM_ASSERT(SUCCEEDED(hr));

	HANDLE event = CreateEvent(0, 0, 0, 0);
	IM_ASSERT(event != NULL);

	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.NodeMask = 1;

	ID3D12CommandQueue* cmdQueue = NULL;
	hr = d3d_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&cmdQueue));
	IM_ASSERT(SUCCEEDED(hr));

	ID3D12CommandAllocator* cmdAlloc = NULL;
	hr = d3d_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&cmdAlloc));
	IM_ASSERT(SUCCEEDED(hr));

	ID3D12GraphicsCommandList* cmdList = NULL;
	hr = d3d_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, cmdAlloc, NULL, IID_PPV_ARGS(&cmdList));
	IM_ASSERT(SUCCEEDED(hr));

	cmdList->CopyTextureRegion(&dstLocation, 0, 0, 0, &srcLocation, NULL);
	cmdList->ResourceBarrier(1, &barrier);

	hr = cmdList->Close();
	IM_ASSERT(SUCCEEDED(hr));

	// Execute the copy
	cmdQueue->ExecuteCommandLists(1, (ID3D12CommandList* const*)&cmdList);
	hr = cmdQueue->Signal(fence, 1);
	IM_ASSERT(SUCCEEDED(hr));

	// Wait for everything to complete
	fence->SetEventOnCompletion(1, event);
	WaitForSingleObject(event, INFINITE);

	// Tear down our temporary command queue and release the upload resource
	cmdList->Release();
	cmdAlloc->Release();
	cmdQueue->Release();
	CloseHandle(event);
	fence->Release();
	uploadBuffer->Release();

	// Create a shader resource view for the texture
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = desc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	d3d_device->CreateShaderResourceView(pTexture, &srvDesc, srv_cpu_handle);

	// Return results
	*out_tex_resource = pTexture;
	*out_width = image_width;
	*out_height = image_height;
	stbi_image_free(image_data);

	return true;
}

bool RenderUtils::LoadTextureFromFile(const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height)
{
	// Load from disk into a raw RGBA buffer
	int image_width = 0;
	int image_height = 0;
	unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
	if (image_data == NULL)
		return false;

	// Create texture
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = image_width;
	desc.Height = image_height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;

	ID3D11Texture2D* pTexture = NULL;
	D3D11_SUBRESOURCE_DATA subResource;
	subResource.pSysMem = image_data;
	subResource.SysMemPitch = desc.Width * 4;
	subResource.SysMemSlicePitch = 0;
	d3d11Device->CreateTexture2D(&desc, &subResource, &pTexture);

	// Create texture view
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = desc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	d3d11Device->CreateShaderResourceView(pTexture, &srvDesc, out_srv);
	pTexture->Release();

	*out_width = image_width;
	*out_height = image_height;
	stbi_image_free(image_data);

	return true;
}

void RenderUtils::PushClippingArea(ImVec2 position, ImVec2 size, ImDrawList* drawList) {
	drawList->PushClipRect(position, ImVec2(position.x + size.x, position.y + size.y), true);
	D2D::context->PushAxisAlignedClip(D2D1::RectF(position.x, position.y, position.x + size.x, position.y + size.y), D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
}

void RenderUtils::PopClippingArea() {
	ImGui::GetForegroundDrawList()->PopClipRect();
	D2D::context->PopAxisAlignedClip();
}

bool RenderUtils::textBox(std::string* text, ImVec2 pos, ImVec2 size, ImColor color, int index, ImDrawList* drawList) {
	std::string Text = *text;
	if (ImGui::IsMouseClicked(0)) {
		if (Utils::IsMouseOverRectangle(Utils::mousepos, pos, size)) {
			DisableTextBoxes();
			TextIndex[index] = true;
		}
		else
		{
			TextIndex[index] = false;
		}
	}
	if (TextIndex[index]) {
		fillRect(pos, size, color, 100, drawList);
		if (BackSpace and !Text.empty() and ((std::chrono::duration<double>)(std::chrono::high_resolution_clock::now() - last_deleted)).count() > 0.16) {
			last_deleted = std::chrono::high_resolution_clock::now();
			Text.pop_back();
		}
		else {
			Text = Text + Utils::GetKeyAsString(key, Capital, false);
		}
		key = -1;
	}
	else fillRect(pos, size, color, 100, drawList);
	ImGui::SetWindowFontScale(0.015 * size.y);
	PushClippingArea(pos, size, drawList);
	if ((ImGui::CalcTextSize(Text.c_str()).x/2) > size.x)
		RenderText(pos, size, ImColor(255, 255, 255, 255), Text, 0.015 * size.y, 3, drawList);
	else
		RenderText(pos, size, ImColor(255, 255, 255, 255), Text , 0.015 * size.y, 1, drawList);
	PopClippingArea();
	*text = Text;
	return TextIndex[index];
}

void RenderUtils::DisableTextBoxes(){
	for (auto map : TextIndex) {
		TextIndex[map.first] = false;
	}
}

bool RenderUtils::anyActivetextbox() {
	for (auto map : TextIndex) {
		if (TextIndex[map.first]) return true;
	}
	return false;
}

void RenderUtils::EnableTextBox(int index) {
	TextIndex[index] = true;
}

float RenderUtils::Slider(ImVec2 position, float value, float minVal, float maxVal, int index, float sizeX) {
	ImVec2 size(sizeX, Constraints::PercentageConstraint(0.006));
	ImVec2 mousePos = Utils::mousepos;

	float valuePercent = (value - minVal) / (maxVal - minVal);
	float thingPosition = size.x * valuePercent;
	const float handleRadius = size.y; // Adjust the handle's size

	if (Utils::IsMouseOverRectangle(mousePos, position, ImVec2(size.x, size.y*2)) && ImGui::IsMouseClicked(0)) {
		SliderDrag[index] = true;
	}

	if (SliderDrag[index] && ImGui::IsMouseDown(0)) {
		float x = mousePos.x - position.x;
		if (x > size.x) {
			thingPosition = size.x;
		}
		else if (x < 0) {
			thingPosition = 0;
		}
		else {
			thingPosition = x;
		}

		// Update value based on the new position
		valuePercent = thingPosition / size.x;
		value = minVal + (maxVal - minVal) * valuePercent;
	}

	if (ImGui::IsMouseReleased(0)) {
		SliderDrag[index] = false;
	}

	// Draw the slider background
	ImColor col = VecToImColour(Settings::getSettingByName<std::vector<float>>("Mod Menu", "ACCENT COLOR")->value);
	col.Value.w = 1.0f;
	fillRect(position, size, col, Constraints::RoundingConstraint(20));

	// Calculate the handle position as a circle
	ImVec2 handleCenter = ImVec2(position.x + thingPosition, position.y + size.y * 0.5f);

	// Check if the mouse is hovering over the handle circle (increased hitbox)
	float handleRadiusWithHitbox = handleRadius * 2.0f; // Increase hitbox size
	bool isHovered = ImGui::IsMouseHoveringRect(
		ImVec2(handleCenter.x - handleRadiusWithHitbox, handleCenter.y - handleRadiusWithHitbox),
		ImVec2(handleCenter.x + handleRadiusWithHitbox, handleCenter.y + handleRadiusWithHitbox));

	// Draw the slider handle as a circle handleCenter - ImVec2(handleRadius, handleRadius)
	fillRect(ImVec2(handleCenter.x - handleRadius, handleCenter.y - handleRadius), ImVec2(size.y, handleRadius * 2), ImColor(200, 200, 200, 255), 10000);

	if (isHovered && ImGui::IsMouseClicked(0)) {
		SliderDrag[index] = true;
	}

	if (SliderDrag[index] && ImGui::IsMouseDragging(0)) {
		float x = mousePos.x - position.x;
		if (x > size.x) {
			thingPosition = size.x;
		}
		else if (x < 0) {
			thingPosition = 0;
		}
		else {
			thingPosition = x;
		}

		// Update value based on the new position
		valuePercent = thingPosition / size.x;
		value = minVal + (maxVal - minVal) * valuePercent;
	}

	return value;
}

bool RenderUtils::ToggleButton(ImVec2 position, bool value) {
	ImVec2 toggleSize = ImVec2(Constraints::PercentageConstraint(0.04), Constraints::PercentageConstraint(0.02));

	ImVec2 toggleIndicatorSize = ImVec2(Constraints::PercentageConstraint(0.016), Constraints::PercentageConstraint(0.016));
	ImVec2 toggleIndicatorPosDisabled = ImVec2(position.x + Constraints::PercentageConstraint(0.002), position.y + Constraints::PercentageConstraint(0.002));
	ImVec2 toggleIndicatorPosEnabled = ImVec2(position.x + toggleSize.x -  toggleIndicatorSize.x - Constraints::PercentageConstraint(0.002), position.y + Constraints::PercentageConstraint(0.002));

	ImColor col = value ? VecToImColour(Settings::getSettingByName<std::vector<float>>("Mod Menu", "ACCENT COLOR")->value) : ImColor(102, 101, 102, 255);
	col.Value.w = 1.0f;

	fillRect(position, toggleSize, col, 1000);
	fillRect(value ? toggleIndicatorPosEnabled : toggleIndicatorPosDisabled, toggleIndicatorSize, IM_COL32_WHITE, 1000);

	if (Utils::IsMouseOverRectangle(Utils::mousepos, position, toggleSize) and ImGui::IsMouseClicked(0)) value = !value;

	return value;
}

int RenderUtils::ScoreControl(ImVec2 pos, int val, int min, int max) {
	ImVec2 pos1(pos.x, pos.y);
	ImVec2 pos2(pos1.x+Constraints::PercentageConstraint(.035), pos.y);
	ImVec2 pos3(pos2.x+Constraints::PercentageConstraint(.065), pos.y);

	ImVec2 SizeS(Constraints::PercentageConstraint(.03), Constraints::PercentageConstraint(.03));
	ImVec2 SizeB(Constraints::PercentageConstraint(.06), Constraints::PercentageConstraint(.06));

	ImColor accent = VecToImColour(Settings::getSettingByName<std::vector<float>>("Mod Menu", "ACCENT COLOR")->value);
	ImColor disable(120, 120, 120);

	fillRect(pos1, SizeS, val > min ? accent : disable, Constraints::RoundingConstraint(10));
	fillRect(pos2, SizeB, IM_COL32_BLACK_TRANS, Constraints::RoundingConstraint(10));
	fillRect(pos3, SizeS, val < max ? accent : disable, Constraints::RoundingConstraint(10));

	RenderText(pos1, SizeS, IM_COL32_WHITE, "-", 5 * .5 * .3, 2);
	RenderText(pos2, SizeS, IM_COL32_WHITE, std::to_string(val), 5 * .5 * .3, 2);
	RenderText(pos3, SizeS, IM_COL32_WHITE, "+", 5 * .5 * .3, 2);

	if (Utils::IsMouseOverRectangle(Utils::mousepos, pos1, SizeS) && ImGui::IsMouseClicked(0)) {
		CScoreN = true;
		clickTS = std::chrono::high_resolution_clock::now();
		if(val > min) val--;
	}
	if (Utils::IsMouseOverRectangle(Utils::mousepos, pos3, SizeS) && ImGui::IsMouseClicked(0)) {
		CScoreP = true;
		clickTS = std::chrono::high_resolution_clock::now();
		if(val < max) val++;
	}
	if (ImGui::IsMouseReleased(0)) {
		CScoreP = CScoreN = false;
		clickTS = std::chrono::high_resolution_clock::now();
	}
	if (ImGui::IsMouseDown(0)) {
		int s = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - clickTS).count();
		int ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - clickTS).count();
		if (s >= 1 && ms % 250 <= 25) {
			if (CScoreP && val < max && Utils::IsMouseOverRectangle(Utils::mousepos, pos3, SizeS)) val++;
			if (CScoreN && val > min && Utils::IsMouseOverRectangle(Utils::mousepos, pos1, SizeS)) val--;
		}
	}
	return val;
}

void RenderUtils::PushButton(ImVec2 pos, std::string modName, std::string name) {
	ImColor accent = VecToImColour(Settings::getSettingByName<std::vector<float>>("Mod Menu", "ACCENT COLOR")->value);
	ImVec2 size = ImVec2(Constraints::PercentageConstraint(.09), Constraints::PercentageConstraint(.03));
	fillRect(pos, size, accent, Constraints::RoundingConstraint(10));
	auto mod = moduleMgr.getModuleG(modName);
	std::string txt = mod->pbConfig[name].begin()->first;
	RenderText(pos, size, IM_COL32_WHITE, txt, 5 * .5 * .25, 2);
	if(ImGui::IsMouseClicked(0) && Utils::IsMouseOverRectangle(Utils::mousepos, pos, size)) return mod->pbConfig[name][txt]();
}

std::string rgbToHex(int r, int g, int b) {
	std::stringstream stream;
	stream << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << r
		<< std::setw(2) << std::setfill('0') << g
		<< std::setw(2) << std::setfill('0') << b;
	return stream.str();
}

ImColor HexToImColor(std::string hexString)
{
	if (hexString.length() < 6) {
		hexString = hexString + std::string(6 - hexString.length(), '0');
	}
	//
	try
	{
		// Convert the hex string to individual color components
		uint32_t hex = std::stoul(hexString, nullptr, 16);
		uint8_t red = (hex >> 16) & 0xFF;
		uint8_t green = (hex >> 8) & 0xFF;
		uint8_t blue = hex & 0xFF;

		// Normalize the color components to the range [0.0, 1.0]
		float normalizedRed = static_cast<float>(red) / 255.0f;
		float normalizedGreen = static_cast<float>(green) / 255.0f;
		float normalizedBlue = static_cast<float>(blue) / 255.0f;

		// Return the D2D1::ColorF struct
		return ImColor(normalizedRed, normalizedGreen, normalizedBlue);
	}
	catch (const std::exception&)
	{
		// Return black if the conversion fails
		return ImColor(0.0f, 0.0f, 0.0f);
	}
}

ImColor VecToImColour(std::vector<float> colour) {

	return ImColor(colour.at(0), colour.at(1), colour.at(2), colour.at(3));
}

std::vector<float> ImColourToVec(ImColor colour) {
	std::vector<float> color = { colour.Value.x, colour.Value.y, colour.Value.z, colour.Value.w };
	return color;
}

std::string hexCode;

void RenderUtils::ColourPicker(ImVec2 pos, SettingType<std::vector<float>>* setting, int index) {

	ImColor col = VecToImColour(setting->value);

	ImVec2 size = ImVec2(Constraints::PercentageConstraint(0.2), Constraints::PercentageConstraint(0.2));

	ImDrawList* draw_list = ImGui::GetForegroundDrawList();
	ImVec2 p0 = pos;
	ImVec2 p1 = ImVec2(pos.x + size.x, pos.y + size.y);

	if (ColourPickerColor[index] == NULL) {
		ColourPickerColor[index] = ImColor(255, 0, 0, 255);
		ColourPickerMarkerPos[index] = ImVec2(Constraints::PosToPercent(size.x, 1), 0);
		ColourPickerGradientMarkerPos[index] = 0;
		ColourPickerEnable[index] = false;
	}

	fillRect(pos, ImVec2(Constraints::PercentageConstraint(0.03), Constraints::PercentageConstraint(0.03)), col, Constraints::RoundingConstraint(10));

	if (ImGui::IsMouseClicked(0)) {
		bool cum = false;
		for (auto map : ColourPickerEnable) {
			if (ColourPickerEnable[map.first] && index != map.first) {
				cum = true;
			}
		}
		if (Utils::IsMouseOverRectangle(Utils::mousepos, pos, ImVec2(Constraints::PercentageConstraint(0.03), Constraints::PercentageConstraint(0.03))) and !ColourPickerEnable[index] and !cum) {
			ColourPickerEnable[index] = true;
			return;
		}
		else if(!Utils::IsMouseOverRectangle(Utils::mousepos, ImVec2(pos.x - Constraints::PercentageConstraint(0.03), pos.y - Constraints::PercentageConstraint(0.03)), ImVec2(size.x + Constraints::PercentageConstraint(0.07) + (size.x / 6), size.y + Constraints::PercentageConstraint(0.13))) and ColourPickerEnable[index]){
			TextIndex[1] = false;
			ColourPickerEnable[index] = false;
		}
	}
	if (!ColourPickerEnable[index]) return;
	colourPickerPos = pos;
	CurrentColourPickerIndex = index;
	CurrentColour = setting;
	DrawColourPicker = true;
	return;

}

void RenderUtils::drawTheColourPicker() {

	ImColor current = VecToImColour(CurrentColour->value);
	ImVec2 pos = colourPickerPos;
	ImVec2 size = ImVec2(Constraints::PercentageConstraint(0.2), Constraints::PercentageConstraint(0.2));

	if (ImGui::IsMouseClicked(0) && Utils::IsMouseOverRectangle(Utils::mousepos, pos, size)) {
		colourDragging = true;
	}
	if (ImGui::IsMouseReleased(0)) {
		colourDragging = false;
	}

	if (ImGui::IsMouseClicked(0) && Utils::IsMouseOverRectangle(Utils::mousepos, ImVec2(pos.x + size.x + Constraints::PercentageConstraint(0.01), pos.y), ImVec2(size.x / 6, size.x))) {
		hueDragging = true;
	}
	if (ImGui::IsMouseReleased(0)) {
		hueDragging = false;
	}

	fillRect(ImVec2(pos.x - Constraints::PercentageConstraint(0.03), pos.y - Constraints::PercentageConstraint(0.03)), ImVec2(size.x + Constraints::PercentageConstraint(0.07) + (size.x / 6), size.y + Constraints::PercentageConstraint(0.13)), ImColor(30, 30, 30, 255), Constraints::RoundingConstraint(30));

	textBox(&hexCode, ImVec2(pos.x, pos.y + size.x + Constraints::PercentageConstraint(0.013)), ImVec2(Constraints::PercentageConstraint(0.1), Constraints::PercentageConstraint(0.04)), ImColor(15, 15, 15, 255), 2);
	if (!TextIndex[2]) {

		std::tuple<int, int, int> rgbTuple = std::make_tuple(255 * current.Value.x, 255 * current.Value.y, 255 * current.Value.z);
		auto it = rgbToHexMap.find(rgbTuple);

		if (it != rgbToHexMap.end()) {
			// RGB to Hex mapping is found in the map, reuse it
			hexCode = it->second;
		}
		else {
			// RGB to Hex mapping is not found, calculate and store it
			hexCode = rgbToHex(255 * current.Value.x, 255 * current.Value.y, 255 * current.Value.z);
			rgbToHexMap[rgbTuple] = hexCode;
		}
	}

	else {
		current = HexToImColor(hexCode);
	}

	current.Value.w = Slider(ImVec2(pos.x, pos.y + size.x + Constraints::PercentageConstraint(0.07)), current.Value.w, 0, 1, 100);

	ImGui::ColorPicker4("yes", &current.Value.x, 0, 0, pos, size.x, Constraints::PercentageConstraint(0.01), colourDragging, hueDragging);
	std::vector<float> tempc = ImColourToVec(current);
	CurrentColour->value = tempc;
	return;
}

bool RenderUtils::anyKeybindActive() {
	for (auto map : keybindIndex) {
		if (keybindIndex[map.first]) return true;
	}
	return false;
}

int RenderUtils::KeybindSelector(ImVec2 pos, int Current, int index, bool mm) {
	ImColor col = VecToImColour(Settings::getSettingByName<std::vector<float>>("Mod Menu", "ACCENT COLOR")->value);
	col.Value.w = 1.0f;

	ImVec2 size = ImVec2(Constraints::PercentageConstraint(0.06), Constraints::PercentageConstraint(0.03));
	if (ImGui::IsMouseClicked(0)) {
		if (Utils::IsMouseOverRectangle(Utils::mousepos, pos, size)) {
			for (auto map : keybindIndex) {
				keybindIndex[map.first] = false;
			}
			keybindIndex[index] = true;
		}
		else
		{
			keybindIndex[index] = false;
		}
	}
	if (keybindIndex[index]) {
		fillRect(pos, size, col, Constraints::RoundingConstraint(10));
		std::string keybindselectedkey = Utils::GetKeyAsString(KeybindKey);
		if(keybindselectedkey != ""){
			Current = KeybindKey;
			keybindIndex[index] = false;
			if (mm)
				CloseMenu = false;
		}
        KeybindKey = -1;
		
	}
	else {
		fillRect(pos, size, ImColor(102, 101, 102, 255), Constraints::RoundingConstraint(10));
	}
	RenderText(pos, size, ImColor(255, 255, 255, 255), Utils::GetKeyAsString(Current, true), 0.015 * size.y, 2);

	return Current;

}

void RenderUtils::Blur(float intensity) {

	// Create Gaussian blur effect
	ID2D1Bitmap* bitmap = nullptr;
	CopyBitmap(D2D1Bitmap, &bitmap);

	ID2D1Effect* effect;
	D2D::context->CreateEffect(CLSID_D2D1GaussianBlur, &effect);

	effect->SetInput(0, bitmap);

	// Set blur intensity
	effect->SetValue(D2D1_GAUSSIANBLUR_PROP_BORDER_MODE, D2D1_BORDER_MODE_HARD);
	effect->SetValue(D2D1_GAUSSIANBLUR_PROP_OPTIMIZATION, D2D1_GAUSSIANBLUR_OPTIMIZATION_QUALITY);
	effect->SetValue(D2D1_GAUSSIANBLUR_PROP_STANDARD_DEVIATION, intensity);
	// Draw the image with the Gaussian blur effect
	D2D::context->DrawImage(effect);

	bitmap->Release();
	effect->Release();
}

void RenderUtils::CopyBitmap(ID2D1Bitmap1* from, ID2D1Bitmap** to)
{
	if (from == nullptr)
	{
		return;
	}

	if (*to == nullptr)
	{
		D2D1_BITMAP_PROPERTIES props = D2D1::BitmapProperties(from->GetPixelFormat());
		HRESULT hr = D2D::context->CreateBitmap(from->GetPixelSize(), props, to);
		if (FAILED(hr))
		{
			return;
		}
	}
	else if (from->GetPixelSize() != (*to)->GetPixelSize())
	{
		(*to)->Release();
		D2D1_BITMAP_PROPERTIES props = D2D1::BitmapProperties(from->GetPixelFormat());
		HRESULT hr = D2D::context->CreateBitmap(from->GetPixelSize(), props, to);
		if (FAILED(hr))
		{
			return;
		}
	}

	D2D1_POINT_2U destPoint = { 0, 0 };
	D2D1_SIZE_U size = from->GetPixelSize();
	D2D1_RECT_U rect = { 0, 0, size.width, size.height };
	(*to)->CopyFromBitmap(&destPoint, from, &rect);
}

ID2D1SolidColorBrush* RenderUtils::getBrush(D2D1_COLOR_F color) {
	std::string key = std::to_string(color.r) + std::to_string(color.g) + std::to_string(color.b) + std::to_string(color.a);


	auto it = brushCache.find(key);
	if (it != brushCache.end()) {

		return it->second;
	}
	else {

		ID2D1SolidColorBrush* brush;

		D2D::context->CreateSolidColorBrush(color, &brush);

		brushCache[key] = brush;

		return brush;
	}
}

void RenderUtils::BlurRect(D2D1_ROUNDED_RECT rect, float intensity) {
	ID2D1Bitmap1* newLayer = nullptr;
	D2D1_BITMAP_PROPERTIES1 newLayerProps = D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET,
		D2D::context->GetPixelFormat());
	D2D::context->CreateBitmap(D2D::context->GetPixelSize(), nullptr, 0, newLayerProps, &newLayer);

	if (blur == nullptr) {
		D2D::context->CreateEffect(CLSID_D2D1GaussianBlur, &blur);
		}

	if (newLayer != nullptr) {
		D2D::context->SetTarget(newLayer);
		//D2D::context->Clear(D2D1::ColorF(0, 0, 0, 0));

		ID2D1SolidColorBrush* colorBrush = nullptr;
		colorBrush = getBrush(D2D1::ColorF(0, 0, 0, 0));
		D2D::context->FillRoundedRectangle(rect, colorBrush);


		blur->SetInput(0, newLayer);
		blur->SetValue(D2D1_GAUSSIANBLUR_PROP_BORDER_MODE, D2D1_BORDER_MODE_HARD);
		blur->SetValue(D2D1_GAUSSIANBLUR_PROP_STANDARD_DEVIATION, intensity);

		ID2D1Image* out;
		blur->GetOutput(&out);

		D2D::context->SetTarget(D2D1Bitmap);


		D2D::context->DrawImage(out);

		out->Release();
	}

	newLayer->Release();
}

ImColor RenderUtils::VecToImColour(std::vector<float> colour) {

	return ImColor(colour.at(0), colour.at(1), colour.at(2), colour.at(3));
}

void RenderUtils::ShadowRectD2D(ImVec2 position, ImVec2 size, float rounding, ImColor color, float Width, float intensity) {
	if (Settings::getSettingByName<bool>("Mod Menu", "DISABLE SHADOWS")->value) return;
	ID2D1Bitmap1* newLayer = nullptr;
	D2D1_BITMAP_PROPERTIES1 newLayerProps = D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET,
		D2D::context->GetPixelFormat());
	D2D::context->CreateBitmap(D2D::context->GetPixelSize(), nullptr, 0, newLayerProps, &newLayer);

	if (blur == nullptr) {
		D2D::context->CreateEffect(CLSID_D2D1GaussianBlur, &blur);
		}

	if (newLayer != nullptr) {
		D2D::context->SetTarget(newLayer);
		D2D::context->Clear(D2D1::ColorF(0, 0, 0, 0));

		ID2D1SolidColorBrush* colorBrush = nullptr;
		colorBrush = getBrush(D2D1_COLOR_F(color.Value.x, color.Value.y, color.Value.z, color.Value.w));
		D2D::context->FillRoundedRectangle(D2D1::RoundedRect(D2D1::RectF(position.x - Width, position.y - Width, position.x + size.x + (Width), position.y + size.y + (Width)), rounding, rounding), colorBrush);


		blur->SetInput(0, newLayer);
		blur->SetValue(D2D1_GAUSSIANBLUR_PROP_BORDER_MODE, D2D1_BORDER_MODE_HARD);
		blur->SetValue(D2D1_GAUSSIANBLUR_PROP_STANDARD_DEVIATION, intensity);

		ID2D1Image* out;
		blur->GetOutput(&out);

		D2D::context->SetTarget(D2D1Bitmap);


		D2D::context->DrawImage(out);

		out->Release();
	}

	newLayer->Release();
}

int MC::countLeftCps() {
	int cps = 0;
	if (leftClick.size() == 0)return cps;
	for (Clickies clicks : leftClick) {
		std::chrono::duration<double> s = std::chrono::high_resolution_clock::now() - clicks.ClickTimeStamp;
		if (s.count() <= 1) {
			cps++;
		}
		else
		{
			break;
		}

	}
	return cps;
		
}

int MC::countRightCps() {
	int cps = 0;
	if (rightClick.size() == 0)return cps;
	for (Clickies clicks : rightClick) {
		std::chrono::duration<double> s = std::chrono::high_resolution_clock::now() - clicks.ClickTimeStamp;
		if (s.count() <= 1) {
			cps++;
		}
		else
		{
			break;
		}
	}
	return cps;

}

void MC::addLeftCps() {
	Clickies x;
	x.ClickTimeStamp = std::chrono::high_resolution_clock::now();
	leftClick.insert(leftClick.begin(), x);
}

void MC::addRightCps() {
	Clickies x;
	x.ClickTimeStamp = std::chrono::high_resolution_clock::now();
	rightClick.insert(rightClick.begin(), x);
}

auto X = [](){
	Clickies x;
	x.ClickTimeStamp = std::chrono::high_resolution_clock::now();
	return x;
}();

std::vector<Clickies> MC::leftClick = {X, X};
std::vector<Clickies> MC::rightClick = {X, X};
