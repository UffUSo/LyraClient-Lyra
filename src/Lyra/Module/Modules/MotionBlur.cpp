#include "MotionBlur.hpp"
#include "../../../Utils/RenderUtils.h"
#include "../../../Utils/Memory/Memory.hpp"
#include "../../SDK/SDK.hpp"

MotionBlur::MotionBlur() : ModuleCtor(0, "MotionBlur", "MotionBlur") {
	this->InitModuleSettings();
}

std::string MotionBlur::getModuleName() {
	return "Motion Blur";
}

void MotionBlur::InitModuleSettings() {
	Settings::addSetting<bool>("Motion Blur", "enabled", false, true);
	Settings::addSetting<float>("Motion Blur", "intensity", 2.5, true);
    Settings::addSetting<float>("Motion Blur", "blur amount", 0.7, true);

	this->SettingsList["SCALE"]["intensity"] = "slider-0-10";
    this->SettingsList["SCALE"]["blur amount"] = "slider-0-1";
}

void MotionBlur::onEnable() {
	Settings::getSettingByName<bool>("Motion Blur", "enabled")->value = true;
	eventMgr.addListener(this);
}

void MotionBlur::onDisable() {
	Settings::getSettingByName<bool>("Motion Blur", "enabled")->value = false;
	eventMgr.removeListener(this);
}

static inline std::vector<ID2D1Bitmap *> previousFrames;

void MotionBlur::onRender(const RenderEvent &event) {
    if(!SDK::clientInstance || !SDK::clientInstance->getLocalPlayer()) return;
    if (!(SDK::TopScreen.rfind("hud_screen") != std::string::npos)) return;

    int maxFrames = (int)round((MC::fps/120) * Settings::getSettingByName<float>("Motion Blur", "intensity")->value);

        // TODO: Clear frames if not in HUD !!!

        if (SDK::TopScreen == "hud_screen" and maxFrames > 0) {

            if (previousFrames.size() >= static_cast<size_t>(maxFrames)) {
                // Remove excess frames if maxFrames is reduced
                size_t framesToRemove = previousFrames.size() - static_cast<size_t>(maxFrames);
                for (size_t i = 0; i < framesToRemove; ++i) {
                    previousFrames[i]->Release();
                }
                previousFrames.erase(previousFrames.begin(), previousFrames.begin() + framesToRemove);
            }

            ID2D1Bitmap *currentFrame = nullptr;

            RenderUtils::CopyBitmap(D2D1Bitmap, &currentFrame);

            previousFrames.push_back(currentFrame);

            float alpha = 0.3f;
            for (ID2D1Bitmap *frame: previousFrames) {
                D2D::context->DrawBitmap(frame, D2D1::RectF(0.f, 0.f,D2D::context->GetSize().width, D2D::context->GetSize().height), alpha, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);

                alpha *= Settings::getSettingByName<float>("Motion Blur", "blur amount")->value;
            }
        } else {

            for (ID2D1Bitmap *bitmap: previousFrames) {
                Memory::SafeRelease(bitmap);
            }

            previousFrames.clear();

        }

        D2D::context->Flush();
}