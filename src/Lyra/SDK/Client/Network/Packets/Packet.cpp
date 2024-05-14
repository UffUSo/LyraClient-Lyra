#include <string>
#include "Packet.hpp"
#include "../../../../../Utils/Memory/Memory.hpp"

MinecraftPacketIds Packet::getId() {
    return Memory::CallVFunc<1, MinecraftPacketIds>(this);
}