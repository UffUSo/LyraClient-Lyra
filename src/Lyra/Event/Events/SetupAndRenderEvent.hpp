#pragma once

#include "../Event.hpp"
#include "../../SDK/Client/Render/MinecraftUIRenderContext.hpp"

class SetupAndRenderEvent : public Event {
public:
    SetupAndRenderEvent() = default;
    explicit SetupAndRenderEvent(MinecraftUIRenderContext* renderCtx);

    [[nodiscard]] MinecraftUIRenderContext* getRenderContext() const;

private:
    MinecraftUIRenderContext* renderCtx;
};
