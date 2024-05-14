#include "../Module.hpp"

class ArrowCounter : public Module {
public:
    ArrowCounter();
    std::string getModuleName() override;
    void onEnable() override;
    void onDisable() override;
    void InitModuleSettings() override;
    void onRender(const RenderEvent &event) override;
};