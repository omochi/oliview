#include "./text_draw_layouter.h"

namespace oliview {
    
    Ptr<TextDrawInfo>
    TextDrawLayouter::Layout(NVGcontext * ctx,
                             const Ptr<Text> & text,
                             const Optional<float> & max_width)
    {
        float ascent, descent, line_height;
        nvgTextMetrics(ctx, &ascent, &descent, &line_height);
        float font_height = ascent - descent;
        float line_gap = line_height - font_height;
        line_gap *= 0.5f;
        line_height = font_height + line_gap;
        
        std::vector<Ptr<TextDrawInfo::LineEntry>> result_lines;
        
        for (size_t line_index = 0; line_index < text->line_num(); line_index++) {
            auto line_result = LayoutLine(ctx,
                                          text,
                                          line_index,
                                          max_width);
            
            for (auto entry : line_result->lines()) {
                entry->set_text_index(Text::Index(line_index, 0));
                
                result_lines.push_back(entry);
            }
        }

        Ptr<TextDrawInfo> result = New<TextDrawInfo>();
        result->set_font_ascent(ascent);
        result->set_font_descent(descent);
        result->set_line_height(line_height);
        result->set_line_gap(line_gap);
        
        float top = 0;
        float bottom = 0;
        float right = 0;
        float draw_y = 0;
        
        top = result->GetLineTop(draw_y);
        
        for (size_t i = 0; i < result_lines.size(); i++) {
            auto line = result_lines[i];
            line->set_draw_y(draw_y);
            
            right = std::max(right, line->draw_width());
            
            top = std::min(top, result->GetLineTop(draw_y));
            bottom = std::max(bottom, result->GetLineBottom(draw_y));
            
            draw_y += result->line_height();
        }
        
        result->set_lines(result_lines);
        result->set_frame(Rect(Vector2(0, top), Size(right, bottom - top)));

        return result;
    }
        
    void TextDrawLayouter::Draw(NVGcontext * ctx,
                                const Ptr<Text> & text,
                                const Ptr<TextDrawInfo> & draw_info)
    {
        NVGSetFont(ctx, font());
        nvgFontSize(ctx, font_size());
        
        for (auto & line : draw_info->lines()) {
            if (line->char_position_num() == 0) {
                continue;
            }
            
            auto str = line->GetLine(text);
            
            NVGDrawText(ctx,
                        draw_info->draw_offset().x(),
                        line->draw_y() + draw_info->draw_offset().y(),
                        str);
        }
    }
    
    void TextDrawLayouter::DrawCursor(NVGcontext * ctx,
                                      const Text::Index & cursor_index,
                                      const Ptr<TextDrawInfo> & draw_info)
    {
        auto rect = GetCursorRect(cursor_index, draw_info);
        DrawCursor(ctx, rect, draw_info);
    }
    
    void TextDrawLayouter::DrawCursor(NVGcontext * ctx,
                                      const Rect & rect_,
                                      const Ptr<TextDrawInfo> & draw_info)
    {
        auto rect = rect_;
        
        rect.set_origin(rect.origin() + draw_info->draw_offset());
        
        nvgBeginPath(ctx);
        NVGAddRectPath(ctx, rect);
        nvgFill(ctx);
    }
    
    Rect TextDrawLayouter::GetCursorRect(const Text::Index & index,
                                         const Ptr<TextDrawInfo> & draw_info)
    {
        auto position_index = draw_info->GetIndexFor(index);
        auto pos = draw_info->GetDrawPointFor(position_index);
        
        float top = draw_info->GetLineTop(pos.y());
        float bottom = draw_info->GetLineBottom(pos.y());

        return Rect(Vector2(pos.x(), top), Size(1.0f, bottom - top));
    }
    
    Ptr<TextDrawInfo>
    TextDrawLayouter::LayoutLine(NVGcontext * ctx,
                                 const Ptr<Text> & text,
                                 size_t line_index,
                                 const Optional<float> & max_width)
    {
        Ptr<TextDrawInfo> result = New<TextDrawInfo>();
        auto result_lines = result->lines();
        
        Text::Index index(line_index, 0);
        
        while (true) {
            if (index == text->end_index()) {
                break;
            }
            
            Ptr<TextDrawInfo::LineEntry> line_entry = LayoutSingleLine(ctx, text, index);

            auto char_positions = line_entry->char_positions();

            bool wrapped = false;
            
            for (size_t i = 0; i < char_positions.size(); i++) {
                auto & ch = char_positions[i];
                if (max_width) {
                    if (i > 0 && *max_width < ch->draw_right()) {
                        ArrayRemoveRange(&char_positions, MakeIndexRange(i, char_positions.size()));
                        
                        auto back_char = char_positions[i - 1];
                        index = text->AdvanceIndex(back_char->text_index());
                        wrapped = true;
                        break;
                    }
                }
            }
            line_entry->set_char_positions(char_positions);
            
            if (wrapped) {
                line_entry->set_wrapped_line(wrapped);
            }
            
            result_lines.push_back(line_entry);
            
            if (!wrapped) {
                break;
            }
        }
        
        result->set_lines(result_lines);
        return result;
    }

    Ptr<TextDrawInfo::LineEntry>
    TextDrawLayouter::LayoutSingleLine(NVGcontext * ctx,
                                       const Ptr<Text> & text,
                                       const Text::Index & index_)
    {
        Text::Index index = index_;
        
        auto ret = New<TextDrawInfo::LineEntry>(index, false);
        
        NVGSetFont(ctx, font());
        nvgFontSize(ctx, font_size());
        
        Ptr<const std::string> line = text->GetLineAt(index.line());
        size_t line_size = line->size();
        auto newline_chars = rhetoric::newline_chars();
        auto check_newline_ret = CheckEndWith(*line, line_size, newline_chars);
        if (check_newline_ret) {
            auto newline = newline_chars[check_newline_ret->target_index];
            line_size -= newline.size();
            ret->set_newline(Some(newline));
        }
        
        const char * start = line->c_str() + index.byte();
        const char * end = line->c_str() + line_size;
        
        std::vector<NVGglyphPosition> glyphs(ToUnsigned(end - start));
        size_t num = (size_t)nvgTextGlyphPositions(ctx,
                                                   0, 0,
                                                   start,
                                                   end,
                                                   glyphs.data(),
                                                   (int)glyphs.size());
        glyphs.resize((size_t)num);
        
        auto char_positions = ret->char_positions();
        
        for (size_t i = 0; i < num; i++) {
            auto & glyph = glyphs[i];
            
            auto kind = GetUtf8ByteKind((uint8_t)(glyph.str[0]));
            RHETORIC_ASSERT(kind.tag() == Utf8ByteKind::HeadTag);
            
            while (true) {
                Text::Index next_index = text->AdvanceIndex(index);
                if (index.line() < next_index.line()) {
                    break;
                }
                const char * next_index_char = line->c_str() + next_index.byte();
                if (glyph.str < next_index_char) {
                    break;
                }
                index = next_index;
            }
            
            char_positions.push_back(New<TextDrawInfo::CharPosition>(index,
                                                                     glyph.x,
                                                                     glyph.minx,
                                                                     glyph.maxx));
        }
        
        ret->set_char_positions(char_positions);
        
        return ret;
    }
    
    
}
