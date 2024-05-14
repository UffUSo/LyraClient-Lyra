#include "KeyEvent.hpp"

KeyEvent::KeyEvent(const unsigned char key, const bool pressed) {
    this->key = key;
    this->pressed = pressed;
}

unsigned char KeyEvent::getKey() const {
    return this->key;
}

bool KeyEvent::isPressed() const {
    return this->pressed;
}