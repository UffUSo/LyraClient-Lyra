#pragma once
#include "../Module.hpp"

extern bool shouldFreelook;

class FreeLook : public Module {
public:
    FreeLook();
	std::string getModuleName() override;
    void onEnable() override;
    void onDisable() override;
    void InitModuleSettings() override;
    void onKey(KeyEvent& event) override;
    void onTick(const TickEvent& event) override;
    void patch() const;
    void unpatch() const;

private:
    Vec2 oldRotations;
    static inline uintptr_t yaw1 = Memory::findSig("F3 0F 11 ? F3 0F 11 ? ? 48 8B CE");
    static inline uintptr_t yaw2 = Memory::findSig("F3 0F 11 ? F3 0F 11 ? ? 48 8B 4D");
    static inline uintptr_t pitch = Memory::findSig("F3 0F 11 0E 48 89 9C");
    static inline uintptr_t rot = Memory::findSig("F3 0F 11 38 ? ? 7C 24 ? 48 81 C4");
    static inline uintptr_t movement = Memory::findSig("F3 0F 11 01 48 8D 56");

    static inline std::vector<uint8_t> OriginalYaw1;
    static inline std::vector<uint8_t> PatchedYaw1;
    static inline std::vector<uint8_t> OriginalYaw2;
    static inline std::vector<uint8_t> PatchedYaw2;
    static inline std::vector<uint8_t> OriginalPitch;
    static inline std::vector<uint8_t> PatchedPitch;

    static inline std::vector<uint8_t> Originalmovement;
    static inline std::vector<uint8_t> Patchedmovement;
};