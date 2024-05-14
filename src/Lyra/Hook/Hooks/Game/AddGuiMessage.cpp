#include <iostream>
#include "AddGuiMessage.hpp"
#include "../../../Client.hpp"
#include "../../../Module/Modules/CustomChat.hpp"


AddGuiMessage::AddGuiMessage() : Hook("AddGuiMessage Hook", "40 53 55 41 56 48 83 EC ? 48 8B 81")
{
}

void AddGuiMessage::enableHook()
{
	this->autoHook((void*)AddGuiMessageCallback, (void**)&func_original);
}

__int64 AddGuiMessage::AddGuiMessageCallback(GuiData* _this, GuiMessage* message, int profanityFilterContext)
{
	Messages* mes = new Messages();
	mes->message = message->displayedMessage;
	mes->timestamp = std::chrono::high_resolution_clock::now();
	DisplayMessages.push_back(mes);

	return func_original(_this, message, profanityFilterContext);

}