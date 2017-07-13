#pragma once

#include "./dependency.h"

namespace oliview {
    enum class KeyEventType {
        Down,
        Up,
        Repeat
    };
        
    class KeyEvent {
    public:
        RHETORIC_ACCESSOR(KeyEventType, type)
        RHETORIC_ACCESSOR(int, key)
        RHETORIC_ACCESSOR(int, scancode)
        RHETORIC_ACCESSOR(int, modifier)
    private:
        KeyEventType type_;
        int key_;
        int scancode_;
        int modifier_;
    };
}
