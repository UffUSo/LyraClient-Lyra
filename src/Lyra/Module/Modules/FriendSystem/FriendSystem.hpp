#pragma once
#include "../../../../Utils/Utils.hpp"
#include "../../../Event/EventManager.hpp"

class FriendSystem : public Listener {
public:
	std::vector<std::string> pendings;
	bool enabled;

	void HeartBeat();
	void AddFriend(std::string name);
	void RemoveFriend(std::string name);
	void AcceptFriend(std::string name);
	void onEnable();
	void onDisable();
	void onRender(const RenderEvent& event) override;
	void onKey(KeyEvent& event) override;
    void onMouse(MouseEvent& event) override;
	void toggle();

    static inline std::string token;
private:
	std::vector<std::string> friends;
	std::string LocalPlayerName;
};

extern FriendSystem FriendSys;