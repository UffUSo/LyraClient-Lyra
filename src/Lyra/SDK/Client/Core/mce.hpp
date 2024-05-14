#pragma once
#include "HashedString.hpp"
#include "../../../../Utils/Memory/Memory.hpp"

namespace mce {
	class MaterialPtr {
    public:
		static MaterialPtr* createMaterial(const HashedString& name) {
			static void* materialCreator = nullptr;

			if (materialCreator == nullptr)
				materialCreator = Memory::getFromOffset<void*>(Memory::findSig("48 8B 05 ? ? ? ? 48 8D 55 ? 48 8D 0D ? ? ? ? 48 8B 40 ? FF 15 ? ? ? ? 48 8B F8"), 3);

			return Memory::CallVFunc<1, MaterialPtr*>(materialCreator, name);
		}
    };
	struct Color {
		float r;
		float g;
		float b;
		float a;

		Color() : r(0), g(0), b(0) {};
		Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {};
	};
}