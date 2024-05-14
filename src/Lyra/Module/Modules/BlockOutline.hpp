#pragma once
#include "../Module.hpp"

class BlockOutline : public Module {
public:
    BlockOutline();
	std::string getModuleName() override;
    void onEnable() override;
    void onDisable() override;
    void onTick(const TickEvent& event) override;
    void InitModuleSettings() override;
private:
    static inline uintptr_t highlightColorRipRelAddr; // RIP REL 4BYTE FROM FUNC OFFSET ADDR
    static inline UINT32 highlightColorOrigRipRel;

    static inline uintptr_t outlineColorRipRelAddr;
    static inline UINT32 outlineColorOrigRipRel;

    static inline MCCColor* selectionColor; // allocate space for color
    static inline std::array<std::byte, 4> highlightColorNewRipRel;
    static inline std::array<std::byte, 4> outlineColorNewRipRel;
};