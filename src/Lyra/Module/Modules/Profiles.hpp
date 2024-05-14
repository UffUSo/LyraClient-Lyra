#include "../Module.hpp"

class Profiles : public Module {
public:
    Profiles();
    std::string getModuleName() override;
    void onEnable() override;
    void onDisable() override;
    void InitModuleSettings() override;
    void onRender(const RenderEvent &event) override;
    void onMouse(MouseEvent &event);
};
extern bool SaveUI, LoadUI, DelUI, ReplaceUI;