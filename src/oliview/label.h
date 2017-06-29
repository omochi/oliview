#pragma once

#include "./font.h"
#include "./view.h"

namespace oliview {
    class Application;
    
    class Label : public View {
    public:
        Label(const Ptr<Application> & application,
              const std::string & text = "");

        std::string text() const;
        void set_text(const std::string & value);

        RHETORIC_ACCESSOR(Ptr<Font>, font)
        RHETORIC_ACCESSOR_TRIVIAL(float, font_size)

        virtual void DrawContent(NVGcontext * ctx);
    private:
        std::list<std::string> lines_;
        Ptr<Font> font_;
        float font_size_;
    };
}
