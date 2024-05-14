#pragma once

#include "Mob.hpp"
#include "PlayerInventory.hpp"

class Player : public Mob {
public:
	BUILD_ACCESS(this, PlayerInventory*, Supplies, 0x788);
};