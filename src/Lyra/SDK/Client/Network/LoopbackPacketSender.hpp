#pragma once
#include "../../../../Utils/Memory/Memory.hpp"
#include "RaknetConnector.hpp"
#include "Packets/Packet.hpp"

class LoopbackPacketSender {
public:
    BUILD_ACCESS(this, class NetworkSystem*, networkSystem, 0x20);
    virtual ~LoopbackPacketSender() = 0;

    virtual void send(Packet *packet);

    virtual void sendToServer(Packet *packet);
};

class NetworkSystem {
public:
    BUILD_ACCESS(this, class RemoteConnectorComposite*, remoteConnectorComposite, 0x80);
};

class RemoteConnectorComposite {
public:
    BUILD_ACCESS(this, RaknetConnector*, rakNetConnector, 0x60);
};