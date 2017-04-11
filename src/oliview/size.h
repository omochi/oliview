#pragma once

#include "./dependency.h"

namespace oliview {
    class Size {
    public:
        Size();
        Size(float width, float height);

        float width() const;
        void set_width(float value);

        float height() const;
        void set_height(float value);
    private:
        float width_;
        float height_;
    };
}
