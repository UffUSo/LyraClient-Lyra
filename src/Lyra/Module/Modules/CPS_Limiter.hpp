#include "../Module.hpp"
class CPS_Limiter : public Module {
public:
    CPS_Limiter();
    void InitModuleSettings() override;
    std::string getModuleName() override;
    void onEnable() override;
    void onDisable() override;
    void onMouse(MouseEvent &event) override;
};