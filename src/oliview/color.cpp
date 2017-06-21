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

    NVGcolor Color::ToNanoVG() const {
        return nvgRGBAf(red_, green_, blue_, alpha_);
    }
}

