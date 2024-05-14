#include "HitResult.hpp"

Actor *HitResult::getEntity() {
    static auto sig = "E8 ? ? ? ? EB 1A 48 8B CE";
    static auto getEntity = *(decltype(&HitResult::getEntity)*)&sig;
    return (this->*getEntity)();
}

float HitResult::distance() const {
    return startPos.dist(pos);
}
