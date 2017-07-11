#include "./text_draw_layouter.h"

namespace oliview {
    
    Ptr<TextDrawInfo>
    TextDrawLayouter::Layout(NVGcontext * ctx,
                             const Ptr<Text> & text,
                             const Optional<float> & max_width)
    {
       
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
    
        float line_height;
        nvgTextMetrics(ctx, nullptr, nullptr, &line_height);

        float draw_width = 0;
        float draw_y = 0;
        for (size_t i = 0; i < result_lines.size(); i++) {
            auto line = result_lines[i];
            line->set_draw_y(draw_y);
            
            draw_width = std::max(draw_width, line->draw_width());
            
            draw_y += line_height;
        }
        
        Ptr<TextDrawInfo> result = New<TextDrawInfo>();
        result->set_lines(result_lines);
        result->set_size(Size(draw_width, draw_y));
        return result;
    }
        
    void TextDrawLayouter::Draw(NVGcontext * ctx,
                                const Ptr<Text> & text,
                                const Ptr<TextDrawInfo> & draw_info)
    {
        NVGSetFont(ctx, font());
        nvgFontSize(ctx, font_size());
        
        float ascender;
        nvgTextMetrics(ctx, &ascender, nullptr, nullptr);
        
        for (auto & line : draw_info->lines()) {
            if (line->chars_num() == 0) {
                continue;
            }
            
            auto str = line->GetLine(text);
            
            NVGDrawText(ctx,
                        0,
                        line->draw_y() + ascender,
                        str);
        }
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

            auto chars = line_entry->chars();
            RHETORIC_ASSERT(chars.size() > 0);
            for (size_t i = 0; i < chars.size(); i++) {
                auto & ch = chars[i];
                if (max_width) {
                    if (i > 0 && *max_width < ch->draw_right()) {
                        ArrayRemoveRange(&chars, MakeIndexRange(i, chars.size()));
                        break;
                    }
                }
            }
            line_entry->set_chars(chars);
            
            //  TODO: skip newline char
            index = text->AdvanceIndex(chars.back()->text_index());
            
            if (result_lines.size() > 0) {
                line_entry->set_wrapped_line(true);
            }
            result_lines.push_back(line_entry);
            
            if (line_index < index.line()) {
                break;
            }
        }
        
        result->set_lines(result_lines);
        return result;
    }

    //  TODO: return newline status
    Ptr<TextDrawInfo::LineEntry>
    TextDrawLayouter::LayoutSingleLine(NVGcontext * ctx,
                                       const Ptr<Text> & text,
                                       const Text::Index & index_)
    {
        Text::Index index = index_;
        
        NVGSetFont(ctx, font());
        nvgFontSize(ctx, font_size());
        
        StringSlice start_char = text->GetCharAt(index);
        RHETORIC_ASSERT(start_char.base() != nullptr);
        
        const char * start = start_char.c_str();
        
        Ptr<std::string> line = text->GetLineAt(index.line());
        size_t line_size = line->size();
        auto newline_chars = rhetoric::newline_chars();
        auto check_newline_ret = CheckEndWith(*line, line_size, newline_chars);
        if (check_newline_ret) {
            line_size -= newline_chars[check_newline_ret->target_index].size();
        }
    
        const char * end = line->c_str() + line_size;
        RHETORIC_ASSERT(line == start_char.base());
        
        std::vector<NVGglyphPosition> glyphs(ToUnsigned(end - start));
        size_t num = (size_t)nvgTextGlyphPositions(ctx,
                                                   0, 0,
                                                   start,
                                                   end,
                                                   glyphs.data(),
                                                   (int)glyphs.size());
        glyphs.resize((size_t)num);
        
        auto entry = New<TextDrawInfo::LineEntry>(index, false);
        auto chars = entry->chars();
        
        for (size_t i = 0; i < num; i++) {
            auto & glyph = glyphs[i];
            
            auto kind = GetUtf8ByteKind((uint8_t)(glyph.str[0]));
            RHETORIC_ASSERT(kind.tag() == Utf8ByteKind::HeadTag);
            
            Text::Index next_index = text->AdvanceIndex(index);
            if (index.line() == next_index.line()) {
                const char * next_index_char = text->GetCharAt(next_index).c_str();
                if (next_index_char <= glyph.str) {
                    index = next_index;
                }
            }
            
            chars.push_back(New<TextDrawInfo::CharPosition>(index,
                                                            glyph.minx,
                                                            glyph.maxx));
        }
        entry->set_chars(chars);
        
        return entry;
    }
    
    
}
