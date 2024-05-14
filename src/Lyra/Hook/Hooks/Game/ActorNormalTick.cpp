#include <iostream>
#include "ActorNormalTick.hpp"
#include "../../../Client.hpp"
#include "../../../Event/EventManager.hpp"

ActorNormalTickHook::ActorNormalTickHook() : Hook("Actor Normal Tick Hook", "48 8B 01 48 8B 80 ? ? ? ? 48 FF 25 ? ? ? ? CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC 48 89 5C 24 ? 55")
{
}

void ActorNormalTickHook::enableHook()
{

	auto vft = Memory::findSig("48 8D 05 ? ? ? ? 48 89 01 B8 ? ? ? ? 8D 50 FA 44 8D 48 ? 44 8D 40 ? 66 89 44 ? ? E8 ? ? ? ? 48 8B 8B");
	int xd = *reinterpret_cast<int*>(vft + 3);
	uintptr_t** vftREAL = reinterpret_cast<uintptr_t**>(vft + xd + 7);

	this->manualHook(vftREAL[202], TickCallback, (void**)&func_original);


}

void ActorNormalTickHook::TickCallback(Actor* _this, uintptr_t entt)
{

	func_original(_this, entt);

	TickEvent event(_this);

	eventMgr.onTick(event);
}