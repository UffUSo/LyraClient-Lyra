#include "PacketHooks.hpp"
#include "../../../Event/Events/Network/PacketEvent.hpp"
#include "../../../SDK/SDK.hpp"
#include "../../../Event/EventManager.hpp"

void SendPacketHook::callback(LoopbackPacketSender *pSender, Packet *pPacket) {
    PacketEvent event(pPacket);
    eventMgr.onPacketSend(event);

    if (!event.isCancelled()) {
        sendPacketOriginal(pSender, pPacket);
    }
}

void SendPacketHook::receiveCallbackResourcePacksInfo(const float *a1, const float *networkIdentifier, const float *netEventCallback,
                                        const std::shared_ptr<Packet>& packet) {

    PacketEvent event(packet.get());
    eventMgr.onPacketReceive(event);
    if (!event.isCancelled())
        receiveResourcePacksInfoPacketOriginal(a1, networkIdentifier, netEventCallback, packet);
}

void SendPacketHook::receiveCallbackText(const float *a1, const float *networkIdentifier, const float *netEventCallback,
                                         const std::shared_ptr<Packet>& packet) {
        PacketEvent event(packet.get());
        eventMgr.onPacketReceive(event);
        if (!event.isCancelled())
            receiveTextPacketOriginal(a1, networkIdentifier, netEventCallback, packet);

}

void
SendPacketHook::receiveCallbackSetTitle(const float *a1, const float *networkIdentifier, const float *netEventCallback,
                                        const std::shared_ptr<Packet>& packet) {

    PacketEvent event(packet.get());
    eventMgr.onPacketReceive(event);
    if (!event.isCancelled())
        receiveSetTitlePacketOriginal(a1, networkIdentifier, netEventCallback, packet);
}

void
SendPacketHook::receiveCallbackPlaySound(const float *a1, const float *networkIdentifier, const float *netEventCallback,
                                         const std::shared_ptr<Packet>& packet) {

    PacketEvent event(packet.get());
    eventMgr.onPacketReceive(event);
    if (!event.isCancelled())
        receivePacketPlaySounOriginal(a1, networkIdentifier, netEventCallback, packet);
}

void
SendPacketHook::receiveCallbackChangeDimension(const float *a1, const float *networkIdentifier, const float *netEventCallback,
                                               const std::shared_ptr<Packet>& packet) {

    PacketEvent event(packet.get());
    eventMgr.onPacketReceive(event);
    if (!event.isCancelled())
        receivePacketChangeDimensionOriginal(a1, networkIdentifier, netEventCallback, packet);
}

void SendPacketHook::enableHook() {

    std::shared_ptr<Packet> resourcePacksInfoPacket = SDK::createPacket((int) MinecraftPacketIds::ResourcePacksInfo);
    Memory::hookFunc((void *) resourcePacksInfoPacket->packetHandler->vTable[1], (void*)receiveCallbackResourcePacksInfo,
                     (void **) &receiveResourcePacksInfoPacketOriginal, "ReceivePacketHook");
    std::shared_ptr<Packet> textPacket = SDK::createPacket((int) MinecraftPacketIds::Text);
    Memory::hookFunc((void *) textPacket->packetHandler->vTable[1], receiveCallbackText,
                     (void **) &receiveTextPacketOriginal, "ReceivePacketHook");

    std::shared_ptr<Packet> setTitlePacket = SDK::createPacket((int) MinecraftPacketIds::SetTitle);
    Memory::hookFunc((void *) setTitlePacket->packetHandler->vTable[1], receiveCallbackSetTitle,
                     (void **) &receiveSetTitlePacketOriginal, "ReceivePacketHook");

    std::shared_ptr<Packet> playSoundPacket = SDK::createPacket((int) MinecraftPacketIds::PlaySoundA);
    Memory::hookFunc((void *) playSoundPacket->packetHandler->vTable[1], receiveCallbackPlaySound,
                     (void **) &receivePacketPlaySounOriginal, "ReceivePacketHook");

    std::shared_ptr<Packet> changeDimensionPacket = SDK::createPacket((int) MinecraftPacketIds::ChangeDimension);
    Memory::hookFunc((void *) changeDimensionPacket->packetHandler->vTable[1], receiveCallbackChangeDimension,
                     (void **) &receivePacketChangeDimensionOriginal, "ReceivePacketHook");

    this->autoHook((void *) callback, (void **) &sendPacketOriginal);
}
