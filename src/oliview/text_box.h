#pragma once

#include "./font.h"
#include "./char_event.h"
#include "./key_event.h"
#include "./math.h"
#include "./scroll_view.h"
#include "./text.h"
#include "./text_box_content_view.h"
#include "./text_draw_info.h"
#include "./text_draw_layouter.h"
#include "./view.h"

namespace oliview {
    class TextBox : public ScrollView {
    public:
        virtual void Init(const Ptr<Application> & application) override;
        
        RHETORIC_SUBCLASS_SHARED_FROM_THIS(TextBox, View)
        
        Ptr<const Text> text() const;
        void set_text(const Ptr<const Text> & value);
        
        Ptr<Font> font() const;
        void set_font(const Ptr<Font> & value);
        
        float font_size() const;
        void set_font_size(float value);
        
        RHETORIC_ACCESSOR(Color, font_color)
        
        void InsertTextAt(const Text::Index & index,
                          const Ptr<const Text> & string,
                          Text::Index * end_index);
        void DeleteTextAt(const Text::Index & begin,
                          const Text::Index & end);
        
        RHETORIC_GETTER(Text::Index, cursor_index)
        void set_cursor_index(const Text::Index & value);
        
        bool MoveCursorLeft();
        bool MoveCursorRight();
        bool MoveCursorUp();
        bool MoveCursorDown();
        
        virtual Size MeasureScrollContentView(NVGcontext * ctx,
                                              const Ptr<ScrollContentView> & view,
                                              const Size & visible_size) const override;
        virtual void LayoutScrollContentView(NVGcontext * ctx,
                                             const Ptr<ScrollContentView> & view) override;

        virtual bool OnKeyDownEvent(const KeyEvent & event) override;
        virtual void OnCharEvent(const CharEvent & event) override;
        virtual void OnUpdateAnimation(float delta_time) override;
        
        bool _OnTextViewMouseDownEvent(const MouseEvent & event);
        void _DrawTextViewContent(NVGcontext * ctx,
                                  const Ptr<TextBoxContentView> & view);
    private:
        Text::Index GetTextIndexForLineIndexX(size_t line_index, float x);
        Rect GetCursorRect() const;
        
        Ptr<Text> text_;
        Color font_color_;
        
        Ptr<TextBoxContentView> text_view_;
        Ptr<TextDrawLayouter> text_layouter_;
        Ptr<TextDrawInfo> text_draw_info_;
        
        Text::Index cursor_index_;
        float cursor_x_;
        float cursor_blink_time_;
    };
}
