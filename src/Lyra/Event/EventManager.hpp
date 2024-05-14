#pragma once

#include "Listener.hpp"
#include "Events/Network/PacketEvent.hpp"

class EventManager {
public:
    ~EventManager();

    void addListener(Listener* listener);
    void removeListener(const Listener* listener);

    void removeAll();

    void onKey(KeyEvent& event) const;
    void onMouse(MouseEvent& event) const;
    void onPreMCRender(const SetupAndRenderEvent& event) const;
    void onPostMCRender(const SetupAndRenderEvent& event) const;
    void onTick(const TickEvent& event) const;
    void onRender(const RenderEvent& event) const;
    void onAttack(AttackEvent& event) const;
    void onRenderLevel(const RenderLevelEvent& event) const;
    void onGetFogColorEvent(GetFogColorEvent& event) const;
    void onPacketSend(PacketEvent& event) const;
    void onPacketReceive(PacketEvent& event) const;

private:
    std::vector<Listener*> listeners;
};

extern EventManager eventMgr;
