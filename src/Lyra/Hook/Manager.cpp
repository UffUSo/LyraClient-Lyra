#pragma once
#include "Manager.hpp"

//Hooks
#include "Hooks/Input/KeyHook.hpp"
#include "Hooks/Visual/GetFovHook.hpp"
#include "Hooks/Render/DirecXHook.hpp"
#include "Hooks/Render/CommandListHook.hpp"
#include "Hooks/Input/MouseHook.hpp"
#include "Hooks/Render/ResizeHook.hpp"
#include "Hooks/Game/SetupAndRender.hpp"
#include "Hooks/Game/ActorNormalTick.hpp"
#include "Hooks/Game/GetViewPerspective.hpp"
#include "Hooks/Game/GameModeAttack.hpp"
#include "Hooks/Visual/RenderHighlightSelectionHook.hpp"
#include "Hooks/Visual/OverworldGetFogColorHook.hpp"
#include "Hooks/Visual/HurtColorHook.hpp"
#include "Hooks/Visual/getGammaHook.hpp"
#include "Hooks/Game/RenderLevel.hpp"
#include "Hooks/Game/PacketHooks.hpp"
#include "Hooks/Game/FontDrawTransformedHook.hpp"
#include "Hooks/Game/AddGuiMessage.hpp"

std::vector<Hook*> HookManager::hooks;

void HookManager::initialize() {
    MH_Initialize();

    //hooks.push_back(new CommandListHook());
    hooks.push_back(new KeyHook());
    hooks.push_back(new GetFovHook());
    if (kiero::init(kiero::RenderType::D3D12) == kiero::Status::Success) {
        Logger::info("DX 12 Device");
	}
	else if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success) {
		Logger::info("DX 11 Device");
	}
    hooks.push_back(new DirectXHook());
    hooks.push_back(new ResizeHook());
    hooks.push_back(new MouseHook());
    hooks.push_back(new SetupAndRenderHook());
    hooks.push_back(new ActorNormalTickHook());
    hooks.push_back(new GetViewPerspective());
    hooks.push_back(new OverworldGetFogColorHook());
    hooks.push_back(new HurtColorHook());
    hooks.push_back(new getGammaHook());
    hooks.push_back(new GameModeAttackHook());
    hooks.push_back(new RenderLevel());
    hooks.push_back(new SendPacketHook());
    hooks.push_back(new FontDrawTransformedHook());
    hooks.push_back(new AddGuiMessage());
    //hooks.push_back(new RenderHighlightSelectionHook());

    for (auto hook : hooks)
        hook->enableHook();

}

void HookManager::terminate()
{
    for (auto hook : hooks)
        delete hook;

    hooks.clear();
}

