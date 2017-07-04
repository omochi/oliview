#pragma once

#include "./font.h"
#include "./text_draw_layouter.h"
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

        virtual void Layout() override;
        virtual void Draw() override;
    private:
        std::list<std::string> lines_;
        Ptr<Font> font_;
        float font_size_;
        
        TextDrawLayouter::Result text_layout_;
    };
}
