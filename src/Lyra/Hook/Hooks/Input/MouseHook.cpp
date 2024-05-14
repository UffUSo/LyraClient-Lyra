#include "MouseHook.hpp"
#include "../../../Event/EventManager.hpp"
#include "../../../../Utils/RenderUtils.h"

MouseHook::MouseHook() : Hook("mouse_hook", "48 8B C4 48 89 58 ? 48 89 68 ? 48 89 70 ? 57 41 54 41 55 41 56 41 57 48 83 EC ? 44 0F B7 BC 24 ? ? ? ? 48 8B D9")
{
}

void MouseHook::enableHook()
{
	this->autoHook(mouseCallback, (void**)&func_original);
}

void MouseHook::mouseCallback(void* MouseDevice, char button, char action, short mouse_x, short mouse_y, short relativeMouseX,
	short relativeMouseY, bool isScrolling)
{
	
	MouseEvent event(button, action, mouse_x, mouse_y);
	Utils::mousepos.x = mouse_x;
	Utils::mousepos.y = mouse_y;

	if (event.action == MouseAction::PRESS) {
		if (event.button == Left)
			MC::addLeftCps();
		else if (event.button == Right)
			MC::addRightCps();
	}

	int KeyType;
	if (event.button == MouseButton::Left) KeyType = 0;
	else if (event.button == MouseButton::Right) KeyType = 1;
	else KeyType = 2;

	if (ImGui::GetCurrentContext() && button != MouseButton::None)
	ImGui::GetIO().MouseDown[KeyType] = event.action == MouseAction::PRESS;

	eventMgr.onMouse(event);
	if (!event.isCancelled())
		return func_original(MouseDevice, button, action, mouse_x, mouse_y, relativeMouseX, relativeMouseY, isScrolling);
}