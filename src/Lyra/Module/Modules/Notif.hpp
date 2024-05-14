#include "../Module.hpp"
extern std::vector<std::string> notifs;
class Notif : public Module {
public:
    Notif();
    void InitModuleSettings() override;
    std::string getModuleName() override;
    void onEnable() override;
    void onRender(const RenderEvent &event) override;
    void onTick(const TickEvent& event) override;
    void onPacketReceive(PacketEvent& event) override;
};