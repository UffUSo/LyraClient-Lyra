#include "../Module.hpp"

class ReachDisplay : public Module {
public:
    ReachDisplay();
    std::string getModuleName() override;
    void InitModuleSettings() override;
    void onEnable() override;
    void onDisable() override;
    void onAttack(AttackEvent& event) override;
    void onTick(const TickEvent& event) override;
    void onRender(const RenderEvent& event) override;
private:
    static inline float reach = 0.0f;
    static inline std::chrono::time_point<std::chrono::high_resolution_clock> last_hit;
};