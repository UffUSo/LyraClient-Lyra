#include "../Module.hpp"

class HurtColor : public Module {
public:
    HurtColor();
    std::string getModuleName() override;
    void InitModuleSettings() override;
    void onEnable() override;
    void onDisable() override;
};