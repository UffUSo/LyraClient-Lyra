#include "ActorMovementProxy.hpp"

void ActorMovementProxy::SetRotation(Vec2* rot) {
	return Memory::CallVFunc<57, void>(this, rot);
}
