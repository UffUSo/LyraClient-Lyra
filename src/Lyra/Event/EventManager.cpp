#include "EventManager.hpp"

EventManager eventMgr;

EventManager::~EventManager() {
    this->listeners.clear();
}

// Please optimize this hell
// this is so ass whoever wrote this die :)))))))

void EventManager::addListener(Listener *listener) {
    this->listeners.push_back(listener);
}

void EventManager::removeListener(const Listener *listener) {
    for (auto it = this->listeners.begin(); it < this->listeners.end(); it++) {
        if (*it == listener) {
            this->listeners.erase(it);
            return;
        }
    }
}

void EventManager::removeAll() {
    this->listeners.clear();
}

void EventManager::onKey(KeyEvent& event) const {
    for (const auto& listener : this->listeners) {
        if (listener != nullptr)
        listener->onKey(event);
    }
}

void EventManager::onMouse(MouseEvent& event) const {
    for (const auto& listener : this->listeners) {
        if (listener != nullptr)
        listener->onMouse(event);
    }
}

void EventManager::onPreMCRender(const SetupAndRenderEvent &event) const {
    for (const auto& listener : this->listeners) {
        if (listener != nullptr)
        listener->onPreMCRender(event);
    }
}

void EventManager::onPostMCRender(const SetupAndRenderEvent &event) const {
    for (const auto& listener : this->listeners) {
        if (listener != nullptr)
        listener->onPostMCRender(event);
    }
}

void EventManager::onTick(const TickEvent& event) const {
    for (const auto& listener : this->listeners) {
        if (listener != nullptr)
        listener->onTick(event);
    }
}

void EventManager::onRender(const RenderEvent& event) const {
    for (const auto& listener : this->listeners) {
        if (listener != nullptr)
        listener->onRender(event);
    }
}

void EventManager::onRenderLevel(const RenderLevelEvent &event) const {
    for (const auto& listener : this->listeners) {
        if (listener != nullptr)
        listener->onRenderLevel(event);
    }
}

void EventManager::onGetFogColorEvent(GetFogColorEvent& event) const{
    for (const auto& listener : this->listeners) {
        if (listener != nullptr)
            listener->onGetFogColorEvent(event);
    }
}

void EventManager::onAttack(AttackEvent &event) const {
    for (const auto& listener : this->listeners) {
        if (listener != nullptr)
            listener->onAttack(event);
    }
}

void EventManager::onPacketSend(PacketEvent &event) const {
    for (const auto& listener : this->listeners) {
        if (listener != nullptr)
            listener->onPacketSend(event);
    }
}

void EventManager::onPacketReceive(PacketEvent &event) const {
    for (const auto& listener : this->listeners) {
        if (listener != nullptr)
            listener->onPacketReceive(event);
    }
}
