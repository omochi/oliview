#pragma once

#include "./font.h"
#include "./text.h"
#include "./text_draw_info.h"
#include "./text_draw_layouter.h"
#include "./view.h"

namespace oliview {
    class Application;
    
    class Label : public View {
    public:
        virtual void Init(const Ptr<Application> & application) override;
        
        RHETORIC_SUBCLASS_SHARED_FROM_THIS(Label, View)

        Ptr<const Text> text() const;
        void set_text(const Ptr<const Text> & value);
        
        Ptr<Font> font() const;
        void set_font(const Ptr<Font> & value);
        
        float font_size() const;
        void set_font_size(float value);
        
        RHETORIC_ACCESSOR(Color, font_color)

        virtual Size MeasureOwnContent(NVGcontext * ctx, const MeasureQuery & query) const override;
        virtual void LayoutOwnContent(NVGcontext * ctx) override;
        virtual void DrawOwnContent(NVGcontext * ctx) override;
    private:
        Ptr<Text> text_;
        Color font_color_;
        
        Ptr<TextDrawLayouter> text_layouter_;
        Ptr<TextDrawInfo> text_draw_info_;
    };
}
