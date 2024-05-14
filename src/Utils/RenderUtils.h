#pragma once
#include <string>
#include <dxgi.h>
#include <d3d11.h>
#include <d3d12.h>
#include <d2d1_1.h>
#include <dxgi1_4.h>
#include <dwrite_1.h>
#include <initguid.h>

#include "../Lyra/ImGui/imgui.h"
#include "../Lyra/ImGui/imgui_impl_dx11.h"
#include "../Lyra/ImGui/imgui_impl_dx12.h"
#include "../Lyra/ImGui/imgui_impl_win32.h"
#include "../Lyra/ImGui/Fonts/ProductSans.h"
#include "../Lyra/Hook/Render.hpp"
#include "../Lyra/Config/ConfigManager.hpp"
#include <map>
#include <chrono>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dwrite.lib")

#include <dxgidebug.h>
#pragma comment(lib, "dxguid.lib")

#include "Constraints.hpp"

enum SwapChainDeviceType {
	INVALID = 0, Dx11 = 1, Dx12 = 2
};

class RenderUtils {
public:
	static SwapChainDeviceType type;

    static ID2D1Factory* factory;
    static ID2D1ImageBrush* blurbrush;
    static ID2D1Effect* blur;
    static std::map<std::string, ID2D1SolidColorBrush*> brushCache;

    static std::map<int, bool> TextIndex;

    static bool CloseMenu;

    static auto drawText(ImVec2, std::string, float, ImColor, ImDrawList* drawList = ImGui::GetForegroundDrawList()) -> void;
    static auto getTextSize(std::string, float) -> ImVec2;
    static auto fillRect(ImVec2, ImVec2, ImColor, float, ImDrawList* drawList = ImGui::GetForegroundDrawList()) -> void;
    static auto drawRect(ImVec2, ImVec2, ImColor, float, float, ImDrawList* drawList = ImGui::GetForegroundDrawList()) -> void;
    static void RenderImage(ImVec2 Pos, ImVec2 size, std::string imagePath, ImColor imgColour, ImDrawList* drawList = ImGui::GetForegroundDrawList());
    static ImVec4 RenderText(ImVec2 position, ImVec2 PaddingSize, ImU32 color, std::string text, float fontSize, int alignment = 1, ImDrawList* drawList = ImGui::GetForegroundDrawList(), bool mojangles = false);
    static void RenderTextWithOutline(ImVec2 pos, ImVec2 padSize, ImU32 color, std::string text, float fontSize, ImU32 outlineColor, float outlineWidth, int alignment = 1, ImDrawList* drawList = ImGui::GetForegroundDrawList());
    static void PushClippingArea(ImVec2 position, ImVec2 size, ImDrawList* drawList = ImGui::GetForegroundDrawList());
    static void PopClippingArea();
    static bool textBox(std::string* text, ImVec2 size, ImVec2 pos, ImColor color, int index, ImDrawList* drawList = ImGui::GetForegroundDrawList());
    static int key;
	static bool Capital;
	static bool BackSpace;
    static std::chrono::time_point<std::chrono::high_resolution_clock> last_deleted;
    static void DisableTextBoxes();
    static bool anyActivetextbox();
    static void EnableTextBox(int index);
    static float Slider(ImVec2 position, float value, float minVal, float maxVal, int index, float sizeX = Constraints::PercentageConstraint(0.18));
    static bool ToggleButton(ImVec2 position, bool value);
    static int ScoreControl(ImVec2 pos, int val, int min, int max);
    static void PushButton(ImVec2 pos, std::string modName, std::string name);
    static bool DrawColourPicker;
    static void ColourPicker(ImVec2 pos, SettingType<std::vector<float>>* setting, int index);
    static void drawTheColourPicker();
    static int KeybindSelector(ImVec2 pos, int Current, int index, bool mm);
    static bool anyKeybindActive();
    static int KeybindKey;
    static void Blur(float intensity);
    static void CopyBitmap(ID2D1Bitmap1* from, ID2D1Bitmap** to);
    static ImColor VecToImColour(std::vector<float> colour);
    static ID2D1SolidColorBrush* getBrush(D2D1_COLOR_F color);
    static void BlurRect(D2D1_ROUNDED_RECT rect, float intensity);
    static void ShadowRectD2D(ImVec2 position, ImVec2 size, float rounding, ImColor color, float Width, float intensity);
    static void SetFontConfig(ImFont* mojanglse, ImFont* notMojanglse);

private:
    static bool LoadTextureFromFileDX12(const char* filename, ID3D12Device* d3d_device, D3D12_CPU_DESCRIPTOR_HANDLE srv_cpu_handle, ID3D12Resource** out_tex_resource, int* out_width, int* out_height);
    static bool LoadTextureFromFile(const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height);
    static std::map<std::string, ID3D11ShaderResourceView*> images;
    static std::map<std::string, ImTextureID> imagesDX12;

    static std::map<int, bool> SliderDrag;

    static bool hueDragging;
    static bool colourDragging;

    static std::map<std::tuple<int, int, int>, std::string> rgbToHexMap;

    static std::map<int, bool> keybindIndex;
};

class Clickies {
public:
	std::chrono::time_point<std::chrono::high_resolution_clock> ClickTimeStamp;
};

class MC {
public:
	static inline int fps;
	static inline float Deltatime;
    static int countLeftCps();
    static int countRightCps();
    static void addLeftCps();
    static void addRightCps();
    static std::vector<Clickies> leftClick;
	static std::vector<Clickies> rightClick;
};
