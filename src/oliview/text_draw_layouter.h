#pragma once

#include "./nanovg_util.h"
#include "./rect.h"
#include "./size.h"
#include "./string_slice.h"
#include "./text.h"
#include "./text_draw_info.h"
#include "./utf8.h"

namespace oliview {
    class TextDrawLayouter {
    public:        
        RHETORIC_ACCESSOR(Ptr<Font>, font)
        RHETORIC_ACCESSOR_TRIVIAL(float, font_size)
        
        Ptr<TextDrawInfo> Layout(NVGcontext * ctx,
                                 const Ptr<Text> & text,
                                 const Optional<float> & max_width);
                
        void Draw(NVGcontext * ctx,
                  const Ptr<Text> & text,
                  const Ptr<TextDrawInfo> & draw_info);

        void DrawCursor(NVGcontext * ctx,
                        const Text::Index & cursor_index,
                        const Ptr<TextDrawInfo> & draw_info);
        void DrawCursor(NVGcontext * ctx,
                        const Rect & rect,
                        const Ptr<TextDrawInfo> & draw_info);
        
        Rect GetCursorRect(const Text::Index & index,
                           const Ptr<TextDrawInfo> & draw_info);
        
    private:
        Ptr<TextDrawInfo> LayoutLine(NVGcontext * ctx,
                                     const Ptr<Text> & text,
                                     size_t line_index,
                                     const Optional<float> & max_width);
        
        Ptr<TextDrawInfo::LineEntry> LayoutSingleLine(NVGcontext * ctx,
                                                      const Ptr<Text> & text,
                                                      const Text::Index & index);
        
        Ptr<Font> font_;
        float font_size_;
    };
}
