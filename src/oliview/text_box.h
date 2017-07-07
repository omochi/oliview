#pragma once

#include "./font.h"
#include "./text_draw_layouter.h"
#include "./view.h"

namespace oliview {
    class TextBox : public View {
    public:
        virtual void Init(const Ptr<Application> & application) override;
        
        std::string text() const;
        void set_text(const std::string & value);
        
        RHETORIC_ACCESSOR(Ptr<Font>, font)
        RHETORIC_ACCESSOR_TRIVIAL(float, font_size)
        RHETORIC_ACCESSOR(Color, font_color)
        
        virtual Size MeasureContent(NVGcontext * ctx, const MeasureQuery & query) const override;
        virtual void LayoutContent(NVGcontext * ctx) override;
        virtual void DrawContent(NVGcontext * ctx) override;
    private:
        TextDrawLayouter::Result LayoutText(NVGcontext * ctx,
                                            const Optional<float> & max_width) const;
        
        std::list<std::string> lines_;
        Ptr<Font> font_;
        float font_size_;
        Color font_color_;
        
        TextDrawLayouter::Result text_layout_;
    };
}
