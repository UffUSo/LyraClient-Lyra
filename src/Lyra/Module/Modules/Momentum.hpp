#include "../Module.hpp"

class Momentum : public Module {
public:
    Momentum();
    std::string getModuleName() override;
    void InitModuleSettings() override;
    void onEnable() override;
    void onDisable() override;
    void onRender(const RenderEvent& event) override;
};