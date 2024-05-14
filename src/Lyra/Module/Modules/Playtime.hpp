#include "../Module.hpp"

class Playtime : public Module {
public:
    Playtime();
    std::string getModuleName() override;
    void onEnable() override;
    void onDisable() override;
    void InitModuleSettings() override;
    void onRender(const RenderEvent &event) override;
    void RenderHUD(std::string text) override;
};