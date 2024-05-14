#include <iostream>
#include "RenderLevel.hpp"
#include "../../../Client.hpp"
#include "../../../../Utils/TessUtils/TessUtils.hpp"


RenderLevel::RenderLevel() : Hook("Render Level Hook", "48 89 5C 24 ? 55 56 57 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 84 24 ? ? ? ? 49 8B F8 48 8B DA 48 8B E9")
{
}

void RenderLevel::enableHook()
{
	this->autoHook((void*)RenderLevelCallback, (void**)&func_original);
}

__int64 RenderLevel::RenderLevelCallback(LevelRender* _this, ScreenContext* context, const __int64& renderObj)
{
	TessUtils::updateLevel(context);

	eventMgr.onRenderLevel(RenderLevelEvent());

	return func_original(_this, context, renderObj);

}