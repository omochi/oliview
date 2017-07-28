#pragma once

#include "./font.h"
#include "./char_event.h"
#include "./key_event.h"
#include "./math.h"
#include "./scroll_view.h"
#include "./text.h"
#include "./text_draw_info.h"
#include "./text_draw_layouter.h"
#include "./text_view_base.h"
#include "./view.h"

namespace oliview {
    class TextBoxTextView;
    
    class TextBox : public ScrollView {
    public:
        virtual void Init(const Ptr<Application> & application) override;
        virtual ~TextBox();
        
        RHETORIC_SUBCLASS_SHARED_FROM_THIS(TextBox, ScrollView)
        
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
        
        Option<size_t> max_line_num() const;
        void set_max_line_num(const Option<size_t> & value);
        
        bool word_wrap_enabled() const;
        void set_word_wrap_enabled(bool value);

        // cursor
        
        Text::Index cursor_index() const;
        void set_cursor_index(const Text::Index & value);
        
        bool MoveCursorLeft();
        bool MoveCursorRight();
        bool MoveCursorUp();
        bool MoveCursorDown();
        
        // view control
        
        virtual Size MeasureScrollContentView(NVGcontext * ctx,
                                              const Ptr<ScrollContentView> & view,
                                              const Size & visible_size) const override;
        virtual void LayoutScrollContentView(NVGcontext * ctx,
                                             const Ptr<ScrollContentView> & view) override;

        virtual bool OnKeyDownEvent(const KeyEvent & event) override;
        virtual bool OnKeyRepeatEvent(const KeyEvent & event) override;
        virtual void OnCharEvent(const CharEvent & event) override;
        
        virtual void OnFocus() override;
        virtual void OnUnfocus() override;
    private:
        Ptr<TextBoxTextView> text_view_;
    };
    
    class TextBoxTextView : public TextViewBase {
    public:
        virtual void Init(const Ptr<Application> &) override RHETORIC_UNAVAILABLE {}
        
        virtual void Init(const Ptr<Application> & application,
                          const Ptr<TextBox> & owner);
        
        RHETORIC_GETTER_WEAK(Ptr<TextBox>, owner)
        
        virtual bool _DoFocusByMouseDown() override;
    private:
        WeakPtr<TextBox> owner_;
    };
}
