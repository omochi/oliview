#include "./color.h"

namespace oliview {
    Color::Color():
    Color(0, 0, 0, 1)
    {}

    Color::Color(float red,
                 float green,
                 float blue,
                 float alpha):
    red_(red),
    green_(green),
    blue_(blue),
    alpha_(alpha)
    {}

    float Color::red() const {
        return red_;
    }

    void Color::set_red(float value) {
        red_ = value;
    }

    float Color::green() const {
        return green_;
    }

    void Color::set_green(float value) {
        green_ = value;
    }

    float Color::blue() const {
        return blue_;
    }

    void Color::set_blue(float value) {
        blue_ = value;
    }

    float Color::alpha() const {
        return alpha_;
    }

    void Color::set_alpha(float value) {
        alpha_ = value;
    }

    NVGcolor Color::ToNanoVG() const {
        return nvgRGBAf(red_, green_, blue_, alpha_);
    }
}

