#pragma once

#include "./dependency.h"

namespace oliview {
    class Color {
    public:
        Color();
        Color(float red,
              float green,
              float blue,
              float alpha);
        
        RHETORIC_ACCESSOR_TRIVIAL(float, red)
        RHETORIC_ACCESSOR_TRIVIAL(float, green)
        RHETORIC_ACCESSOR_TRIVIAL(float, blue)
        RHETORIC_ACCESSOR_TRIVIAL(float, alpha)

        NVGcolor ToNanoVG() const;
    private:
        float red_;
        float green_;
        float blue_;
        float alpha_;
    };
}
