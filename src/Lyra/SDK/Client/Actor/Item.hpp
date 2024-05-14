#pragma once
#include "../../../../Utils/Memory/Memory.hpp"
#include "../Core/HashedString.hpp"

class Item {
public:
	BUILD_ACCESS(this, std::string, AtlasTextureFile, 0x8);
    BUILD_ACCESS(this, std::string, Namespace, 0xF8);
    BUILD_ACCESS(this, std::string, name, 0xD0);
    BUILD_ACCESS(this, bool, Glint, 0x14A);
};