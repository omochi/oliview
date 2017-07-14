#pragma once

#include "./font.h"
#include "./key_event.h"
#include "./math.h"
#include "./text.h"
#include "./text_draw_info.h"
#include "./text_draw_layouter.h"
#include "./view.h"

namespace oliview {
    class TextBox : public View {
    public:
        virtual void Init(const Ptr<Application> & application) override;
        
        RHETORIC_SUBCLASS_SHARED_FROM_THIS(TextBox, View)
        
        std::string string() const;
        void set_string(const std::string & value);
        Ptr<Font> font() const;
        void set_font(const Ptr<Font> & value);
        float font_size() const;
        void set_font_size(float value);
        RHETORIC_ACCESSOR(Color, font_color)
        
        Ptr<const Text> text() const;
        
        void InsertStringAt(const Text::Index & index,
                            const std::string & string,
                            Text::Index * end_index);
        
        RHETORIC_GETTER(Text::Index, cursor_index)
        void set_cursor_index(const Text::Index & value);
        
        bool MoveCursorLeft();
        bool MoveCursorRight();
        bool MoveCursorUp();
        bool MoveCursorDown();
        
        virtual Size MeasureContent(NVGcontext * ctx, const MeasureQuery & query) const override;
        virtual void LayoutContent(NVGcontext * ctx) override;
        virtual void DrawContent(NVGcontext * ctx) override;
        
        virtual Ptr<View> HitTest(const MouseEvent & event) override;
        
        virtual void OnMouseDownEvent(const MouseEvent & event) override;
        virtual void OnMouseMoveEvent(const MouseEvent & event) override;
        virtual void OnMouseUpEvent(const MouseEvent & event) override;
        virtual void OnMouseCancelEvent() override;
        
        virtual bool OnKeyEvent(const KeyEvent & event) override;
        
        virtual void OnUpdateAnimation(float delta_time) override;
    private:
        Text::Index GetTextIndexForLineIndexX(size_t line_index, float x);
        Rect GetCursorRect() const;
        
        Ptr<Text> text_;
        Color font_color_;
        
        Ptr<TextDrawLayouter> text_layouter_;
        Ptr<TextDrawInfo> text_draw_info_;
        
        Text::Index cursor_index_;
        float cursor_x_;
        float cursor_blink_time_;
    };
}
