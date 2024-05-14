#pragma once

#include "HitResultType.hpp"
#include "FacingID.hpp"
#include "../../../../Utils/Utils.hpp"

class HitResult {
public:
    Vec3 startPos;
    Vec3 rayDir;
    HitResultType type;
    FacingID facing;
    BlockPos blockPos;
    Vec3 pos;

    class Actor* getEntity();
    [[nodiscard]] float distance() const;
};
