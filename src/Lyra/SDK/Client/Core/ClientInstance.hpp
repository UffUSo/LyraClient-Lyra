#pragma once
#include "../../../../Utils/Memory/Memory.hpp"
#include "../Actor/LocalPlayer.hpp"
#include "../Block/BlockSource.hpp"
#include "../Render/GuiData.hpp"
#include "MCGame.hpp"
#include "../Level/LevelRender/LevelRender.hpp"
#include "../Network/LoopbackPacketSender.hpp"

class ClientInstance {
public:
	BUILD_ACCESS(this, GuiData*, guiData, 0x560);
	BUILD_ACCESS(this, float*, FovX, 0x6A0);
	BUILD_ACCESS(this, MCGame*, mcgame, 0xC8);
	BUILD_ACCESS(this, float*, FovY, 0x6B4);

	void grabMouse() {
		Memory::CallVFunc<331, void>(this);
	}

	void releaseMouse() {
		Memory::CallVFunc<332, void>(this);
	}

	BlockSource* getBlockSource() {
		return Memory::CallVFunc<28, BlockSource*>(this);
	}

	LocalPlayer* getLocalPlayer() {
		return Memory::CallVFunc<29, LocalPlayer*>(this);
	}


	LevelRender* getLevelRenderer() {
		return Memory::direct_access<LevelRender*>(this, 0xE0);
	}

    LoopbackPacketSender *getPacketSender() {
        return *reinterpret_cast<LoopbackPacketSender **>((uintptr_t) this + 0xF0);
    }

    RaknetConnector *getRakNetConnector() {
        if (getPacketSender() == nullptr)
            return nullptr;

        return getPacketSender()->networkSystem->remoteConnectorComposite->rakNetConnector;
    }
};