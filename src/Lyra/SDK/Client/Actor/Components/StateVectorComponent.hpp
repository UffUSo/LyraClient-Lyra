#pragma once

#include "../../../../../Utils/Utils.hpp"
#include "../../../../../Utils/Memory/Memory.hpp"

class StateVectorComponent {
public:
    BUILD_ACCESS(this, Vec3, pos, 0x0);
    BUILD_ACCESS(this, Vec3, posDelta, 0x18);
    BUILD_ACCESS(this, Vec3, posPrev, 0x0C);

    Vec3 Pos;
    Vec3 PrevPos;
    Vec3 velocity;
};