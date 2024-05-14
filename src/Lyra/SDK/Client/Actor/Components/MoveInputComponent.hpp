#include "../../../../../Utils/Memory/Memory.hpp"

#pragma once

struct MoveInputComponent {
    bool sneakingPrev;

private:
    char pad0x1[0x5];

public:
    bool jumpingPrev;
    bool sprintingPrev;

private:
    char pad0x8[0x2];

public:
    bool forwardPrev;
    bool backwardPrev;
    bool leftPrev;
    bool rightPrev;

private:
    char pad0xE[0x12];

public:
    bool sneaking;

private:
    char pad0x21[0x5];

public:
    bool jumping;
    bool sprinting;

private:
    char pad0x28[0x2];

public:
    bool forward;
    bool backward;
    bool left;
    bool right;

private:
    char pad0x2E[0x1A];

public:
    float sideMovement;
    float forwardMovement;

private:
    char pad0x50[0x38];

public:
    bool isPressed() const {
        return forward || backward || left || right;
    };
};
