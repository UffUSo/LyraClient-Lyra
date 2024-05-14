#pragma once
#include "../Module.hpp"
struct Messages {
    std::chrono::time_point<std::chrono::high_resolution_clock> timestamp;
    std::string message;
    ImVec2 Pos;
};
extern std::vector<Messages*> DisplayMessages;
class CustomChat : public Module {
public:
    CustomChat();
	std::string getModuleName() override;
    void onEnable() override;
    void onDisable() override;
    void InitModuleSettings() override;
    void onPreMCRender(const SetupAndRenderEvent &event) override;
    void onRender(const RenderEvent &event) override;
};