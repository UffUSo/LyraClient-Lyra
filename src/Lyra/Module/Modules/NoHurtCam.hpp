#pragma once
#include "../Module.hpp"

class NoHurtCam : public Module {
public:
    NoHurtCam();
	std::string getModuleName() override;
    void onEnable() override;
    void onDisable() override;
    void InitModuleSettings() override;
private:
    const static inline uintptr_t sigOffset = Memory::findSig("8B 44 24 48 89 43 08 C6") + 4;
    static inline std::vector<uint8_t> originalCameraAngle;
    static inline uint8_t newCameraAngle[4] = {0x90, 0x90, 0x90};
};