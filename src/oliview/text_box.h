#pragma once

#include "./font.h"
#include "./math.h"
#include "./text.h"
#include "./text_draw_info.h"
#include "./text_draw_layouter.h"
#include "./view.h"

namespace oliview {
    class TextBox : public View {
    public:
        virtual void Init(const Ptr<Application> & application) override;
        
        std::string text() const;
        void set_text(const std::string & value);
        Ptr<Font> font() const;
        void set_font(const Ptr<Font> & value);
        float font_size() const;
        void set_font_size(float value);
        RHETORIC_ACCESSOR(Color, font_color)
        
        RHETORIC_GETTER(Text::Position, cursor_position)
        void set_cursor_position(const Text::Position & value);
        
        virtual Size MeasureContent(NVGcontext * ctx, const MeasureQuery & query) const override;
        virtual void LayoutContent(NVGcontext * ctx) override;
        virtual void DrawContent(NVGcontext * ctx) override;
    private:
        Ptr<Text> text_;
        Color font_color_;
        
        Ptr<TextDrawLayouter> text_layouter_;
        Ptr<TextDrawInfo> text_draw_info_;
        
        Text::Position cursor_position_;
    };
}
