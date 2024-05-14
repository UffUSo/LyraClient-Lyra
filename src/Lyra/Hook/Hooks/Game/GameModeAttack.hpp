#include "../../../SDK/Client/Actor/Gamemode.hpp"
#include "../Hook.hpp"

class GameModeAttackHook : public Hook {

private:
    static void callback(Gamemode *gamemode, Actor *actor);

public:
    typedef void(__thiscall *original)(Gamemode *, Actor *);

    static inline original funcOriginal = nullptr;

    GameModeAttackHook();

    void enableHook() override;
};