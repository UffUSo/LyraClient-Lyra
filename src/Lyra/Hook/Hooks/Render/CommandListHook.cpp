#include "CommandListHook.hpp"
#include "DirecXHook.hpp"
#include "../../../Client.hpp"

typedef void(__thiscall* commando)(ID3D12CommandQueue* queue, UINT numCommandLists, const ID3D12CommandList** ppCommandLists);
commando commandori = nullptr;

void CommandListHook::enableHook() {

    if (kiero::getRenderType() == kiero::RenderType::D3D12)
        this->manualHook((void*)kiero::getMethodsTable()[54], CommandListHook::listCallback, (void**)&commandori);
    else if (kiero::getRenderType() == 0) {
        kiero::init(kiero::RenderType::D3D11);
        d3d12CommandQueue = nullptr;
        }

}

CommandListHook::CommandListHook() : Hook("CommandListHook", "") {}

void CommandListHook::listCallback(ID3D12CommandQueue* queue, UINT numCommandLists, const ID3D12CommandList** ppCommandLists)
{
    if(!d3d12CommandQueue) d3d12CommandQueue = queue;

    return commandori(queue, numCommandLists, ppCommandLists);

}
