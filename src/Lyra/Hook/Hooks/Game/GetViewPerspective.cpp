#include <iostream>
#include "GetViewPerspective.hpp"
#include "../../../Client.hpp"
#include "../../../Module/Modules/FreeLook.hpp"
#include "../../../Module/Modules/SnapLook.hpp"

GetViewPerspective::GetViewPerspective() : Hook("GetViewPerspective", "48 83 EC ? 48 8B 01 48 8D 54 ? ? 41 B8 03 00 00 00")
{
}

void GetViewPerspective::enableHook()
{

	this->autoHook((void*)TickCallback, (void**)&func_original);
}

int GetViewPerspective::TickCallback(uintptr_t* a1)
{
	if (ShouldSnapLook)
		return 2;
	if (shouldFreelook)
		return 1;
	return func_original(a1);

}