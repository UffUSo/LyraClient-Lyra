#pragma once
#include "Client/Core/ClientInstance.hpp"
#include "Client/Render/ScreenView/ScreenView.hpp"
#include "Client/Network/Packets/Packet.hpp"

class SDK {
public:
	static ClientInstance* clientInstance;
	static ScreenView* screenView;
	static std::string TopScreen;

    static inline std::string getServerIP();

    static std::shared_ptr<Packet> createPacket(int id);
};