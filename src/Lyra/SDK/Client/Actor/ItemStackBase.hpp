#pragma once
#include "Item.hpp"
#include "../Core/WeakPtr.hpp"

class ItemStackBase {
public:
	BUILD_ACCESS(this, WeakPtr<Item>, mItem, 0x8);
	BUILD_ACCESS(this, INT8, count, 0x22);
};