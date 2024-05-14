#pragma once

#include "Events/KeyEvent.hpp"
#include "Events/RenderEvent.hpp"
#include "Events/RenderLevelEvent.hpp"
#include "Events/SetupAndRenderEvent.hpp"
#include "Events/TickEvent.hpp"
#include "Events/MouseEvent.hpp"
#include "Events/GetFogColorEvent.hpp"
#include "Events/AttackEvent.hpp"
#include "Events/Network/PacketEvent.hpp"

class Listener {
public:
    virtual void onKey(KeyEvent& event);
    virtual void onMouse(MouseEvent& event);
    virtual void onPreMCRender(const SetupAndRenderEvent& event);
    virtual void onPostMCRender(const SetupAndRenderEvent& event);
    virtual void onTick(const TickEvent& event);
    virtual void onRender(const RenderEvent& event);
    virtual void onAttack(AttackEvent& event);
    virtual void onRenderLevel(const RenderLevelEvent& event);
    virtual void onGetFogColorEvent(GetFogColorEvent& event);
    virtual void onPacketSend(PacketEvent& event);
    virtual void onPacketReceive(PacketEvent& event);
};