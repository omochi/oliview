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
        float red() const;
        void set_red(float value);
        float green() const;
        void set_green(float value);
        float blue() const;
        void set_blue(float value);
        float alpha() const;
        void set_alpha(float value);
    private:
        float red_;
        float green_;
        float blue_;
        float alpha_;
    };
}
