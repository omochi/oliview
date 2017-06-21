#pragma once

#include "./font.h"
#include "./view.h"

namespace oliview {
    class Label : public View {
    public:
        Label(const std::string & text = "");

        RHETORIC_ACCESSOR(std::string, text)
        RHETORIC_ACCESSOR(Ptr<Font>, font)
        RHETORIC_ACCESSOR_TRIVIAL(float, font_size)

        virtual void DrawContent(NVGcontext * ctx);
    private:
        std::string text_;
        Ptr<Font> font_;
        float font_size_;
    };
}
