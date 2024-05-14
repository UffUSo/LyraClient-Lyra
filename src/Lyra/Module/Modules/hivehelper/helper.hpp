#pragma once
#include "../../Module.hpp"

class helper : public Module {
public:
    helper();
    std::string getModuleName() override;
    void onEnable() override;
    void onDisable() override;
    void InitModuleSettings() override;

    void onPacketReceive(PacketEvent &event) override;
    void onRender(const RenderEvent &event) override;


};
