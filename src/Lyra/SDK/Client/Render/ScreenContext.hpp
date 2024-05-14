#pragma once
#include "../../../../Utils/Utils.hpp"
#include "Tessellator.hpp"

class ScreenContext {
public:
    inline MCCColor *getColorHolder() {
        return *reinterpret_cast<MCCColor **>((uintptr_t) (this) + 0x30);

    }

    inline Tessellator *getTessellator() {
        return *reinterpret_cast<Tessellator **>((uintptr_t) (this) + 0xC0);

    }
};