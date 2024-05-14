#pragma once

#include "../CancellableEvent.hpp"

class KeyEvent : public Cancellable {
public:
    KeyEvent(unsigned char key, bool pressed);

    [[nodiscard]] unsigned char getKey() const;
    [[nodiscard]] bool isPressed() const;

private:
    unsigned char key;
    bool pressed;
};