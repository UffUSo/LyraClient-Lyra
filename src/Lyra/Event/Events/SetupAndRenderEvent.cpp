#include "SetupAndRenderEvent.hpp"

SetupAndRenderEvent::SetupAndRenderEvent(MinecraftUIRenderContext *renderCtx) {
    this->renderCtx = renderCtx;
}

MinecraftUIRenderContext *SetupAndRenderEvent::getRenderContext() const {
    return this->renderCtx;
}
