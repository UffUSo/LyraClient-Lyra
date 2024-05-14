#include "../Module.hpp"

class Ping : public Module {
public:
    Ping();
    std::string getModuleName() override;
    void InitModuleSettings() override;
    void onEnable() override;
    void onDisable() override;
    void onRender(const RenderEvent& event) override;
};