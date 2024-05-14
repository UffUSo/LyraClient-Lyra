#pragma once

#include <memory>
#include "../../Event.hpp"
#include "../../CancellableEvent.hpp"
#include "../../../SDK/Client/Network/Packets/Packet.hpp"

class PacketEvent : public Event, public Cancellable {

    Packet *packet;

public:

    Packet *getPacket() {
        return this->packet;
    }

    explicit PacketEvent(Packet *e) {
        this->packet = e;
    }
};