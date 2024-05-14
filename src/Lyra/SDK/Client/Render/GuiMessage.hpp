#pragma once

class GuiMessage {
    char pad_0x0[0x8];  // 0x0
public:
    std::string sentMessage;       // 0x8
    std::string ttsMessage;        // 0x28
    std::string userName;          // 0x48
    std::string displayedMessage;  // 0x68
private:
    char pad_0x88[0x24];  // 0x88
public:
    float timeDisplayed;  // 0xAC
private:
    char pad_0xB0[0x8];  // 0xB0
};
