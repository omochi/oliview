#pragma once

#include "./color.h"
#include "./font.h"
#include "./text.h"
#include "./text_alignment.h"
#include "./text_draw_layouter.h"
#include "./text_draw_info.h"
#include "./view.h"

namespace oliview {
    class TextViewBase : public View {
    public:
        virtual void Init(const Ptr<Application> & application) override;
        
        RHETORIC_SUBCLASS_SHARED_FROM_THIS(TextViewBase, View)
        
        bool editable() const;
        void set_editable(bool value);
        
        // text
        
        Ptr<const Text> text() const;
        void set_text(const Ptr<const Text> & value);
        
        void InsertTextAt(const Text::Index & index,
                          const Ptr<const Text> & text,
                          Text::Index * end_index);
        void DeleteTextAt(const Text::Index & begin,
                          const Text::Index & end);
        
        // appearance
        
        Ptr<Font> font() const;
        void set_font(const Ptr<Font> & value);
        
        float font_size() const;
        void set_font_size(float value);
        
        Color font_color() const;
        void set_font_color(const Color & value);
        
        TextAlignment text_alignment() const;
        void set_text_alignment(TextAlignment value);
        
        // cursor
        
        Text::Index cursor_index() const;
        void set_cursor_index(const Text::Index & value);
        
        bool MoveCursorLeft();
        bool MoveCursorRight();
        bool MoveCursorUp();
        bool MoveCursorDown();
        
        // view control
        
        virtual Size MeasureOwnContent(NVGcontext * ctx, const MeasureQuery & query) const override;
        virtual void LayoutOwnContent(NVGcontext * ctx) override;
        virtual void DrawOwnContent(NVGcontext * ctx) override;
        
        virtual void OnUpdateAnimation(float delta_time) override;
        
        virtual bool OnMouseDownEvent(const MouseEvent & event) override;
        
        virtual bool OnKeyDownEvent(const KeyEvent & event) override;
        virtual bool OnKeyRepeatEvent(const KeyEvent & event) override;
        
        virtual void OnCharEvent(const CharEvent & event) override;
        
        virtual bool _DoFocusByMouseDown() = 0;
        
        bool _cursor_visible() const;
        void _set_cursor_visible(bool value);
    private:
        Text::Index GetTextIndexForLineIndexX(size_t line_index, float x);
        Rect GetCursorRect() const;
        
        bool editable_;
        
        Ptr<Text> text_;
        Color font_color_;
        
        Ptr<TextDrawLayouter> text_layouter_;
        Ptr<TextDrawInfo> text_draw_info_;
        
        bool cursor_visible_;
        Text::Index cursor_index_;
        float cursor_x_;
        float cursor_blink_time_;
    };
}
