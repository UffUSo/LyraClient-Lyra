#include "Tessellator.hpp"
#include "../../../../Utils/Memory/Memory.hpp"

void Tessellator::begin(const VertexFormat vertexFormat) {
    static auto sig = Memory::findSig("48 89 5C 24 ? 56 48 83 EC ? 80 B9 ? ? ? ? ? 45 0F B6");
    Memory::CallThisFunc<void, VertexFormat>(sig, this, vertexFormat);
}

void Tessellator::vertex(const float x, const float y, const float z) {
    static auto sig = Memory::findSig("40 57 48 81 EC ? ? ? ? 0F 29 7C ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 8B 81");
    Memory::CallThisFunc<void, float, float, float>(sig, this, x, y, z);
}

void Tessellator::color(const float r, const float g, const float b, const float a) {
    static auto sig = Memory::findSig("80 B9 ? ? ? ? ? 4C 8B C1 75");
    Memory::CallThisFunc<void, float, float, float, float>(sig, this, r, g, b, a);
}

void Tessellator::color(const mce::Color &color) {
    static auto sig = Memory::findSig("80 B9 ? ? ? ? ? 4C 8B D2 4C 8B C1");
    Memory::CallThisFunc<void, const mce::Color&>(sig, this, color);
}

void Tessellator::renderMesh(ScreenContext *screenContext, mce::MaterialPtr *material) {
    static auto sig = Memory::findSig("48 89 5C 24 ? 48 89 74 24 ? 57 48 81 EC ? ? ? ? 49 8B F8 48 8B DA 48 8B F1 80 BA");
    Memory::CallThisFunc<void, Tessellator*, mce::MaterialPtr*>(sig, screenContext, this, material);
}