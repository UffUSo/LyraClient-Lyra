#pragma once
#include "../../../../Utils/Utils.hpp"
#include "../../../Event/EventManager.hpp"

class MenuSelector : public Listener {
public:
    bool enabled;

    void onEnable();
    void onRender(const RenderEvent &event) override;
    void onKey(KeyEvent &event) override;
    void onMouse(MouseEvent &event) override;
    void toggle();
    bool hideUI = false;
};

extern MenuSelector menuSelector;