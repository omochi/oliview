#pragma once

#include "./rect.h"
#include "./color.h"

namespace oliview {
    class View {
    public:
        Rect frame() const;
        void set_frame(const Rect & value);

        Color background_color() const;
        void set_background_color(const Color & value);
    private:
        Rect frame_;
        Color background_color_;
    };
}
