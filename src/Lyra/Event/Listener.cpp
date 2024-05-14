#include "Listener.hpp"

void Listener::onKey(KeyEvent& event) {}

void Listener::onMouse(MouseEvent& event) {}

void Listener::onPreMCRender(const SetupAndRenderEvent &event) {}

void Listener::onPostMCRender(const SetupAndRenderEvent &event) {}

void Listener::onTick(const TickEvent& event) {}

void Listener::onAttack(AttackEvent &event) {}

void Listener::onRender(const RenderEvent& event) {}

void Listener::onRenderLevel(const RenderLevelEvent &event) {}

void Listener::onGetFogColorEvent(GetFogColorEvent &event) {}

void Listener::onPacketSend(PacketEvent &event) {}

void Listener::onPacketReceive(PacketEvent &event) {}
