#pragma once

#include "Packet.hpp"
#include "../../../../../Utils/Vector.hpp"
#include <string>

class NetworkBlockPosition {
public:
    Vec3 position;
};

class PlaySoundPacket : public ::Packet {
public:
    PlaySoundPacket();

    std::string mName;   // this+0x30
    Vec3 mPos;    // this+0x50
    float mVolume; // this+0x5C
    float mPitch;  // this+0x60
};