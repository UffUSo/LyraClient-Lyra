#pragma once
#include "../../../../Utils/Memory/Memory.hpp"
#include "ScreenContext.hpp"
#include "../Core/ClientInstance.hpp"

FK(ItemRenderer)

class BaseActorRenderContext {
private:
    char filling[4096]{}; //padding so the compiler dosent accsess inproperly due us using memset in the constructor
public:
    BaseActorRenderContext(ScreenContext *screenCtx, ClientInstance *clientInstance, MCGame *game);

    BUILD_ACCESS(this, ItemRenderer*, itemRenderer, 0x58);
};