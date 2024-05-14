#pragma once

#include "BaseActorRenderContext.hpp"
#include "../Actor/ItemStack.hpp"

class ItemRenderer {
public:
    void renderGuiItemNew(BaseActorRenderContext *BaseActorRenderCtx, ItemStack *item, int mode, float x, float y,
                          float opacity, float scale, bool isEnchanted);
};