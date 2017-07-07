#include "./text_draw_layouter.h"

namespace oliview {
    
    Ptr<TextDrawInfo>
    TextDrawLayouter::Layout(NVGcontext * ctx,
                             const Ptr<Text> & text,
                             const Optional<float> & max_width)
    {
       
        std::vector<Ptr<TextDrawInfo::LineEntry>> result_lines;
        
        for (int line_index = 0; line_index < text->line_num(); line_index++) {
            auto line_result = LayoutLine(ctx,
                                          text,
                                          line_index,
                                          max_width);
            
            for (auto entry : line_result->lines()) {
                entry->set_text_position(Text::Position(line_index, 0));
                
                result_lines.push_back(entry);
            }
        }
    
        float line_height;
        nvgTextMetrics(ctx, nullptr, nullptr, &line_height);

        float draw_width = 0;
        float draw_y = 0;
        for (int i = 0; i < (int)result_lines.size(); i++) {
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
            
            for (auto & ch : line->chars()) {
                
                Print(Format("%s, %s",
                             ch->text_position().ToString().c_str(),
                             text->GetCharAt(ch->text_position()).AsString().c_str()));
            }
        }
    }
    
    Ptr<TextDrawInfo>
    TextDrawLayouter::LayoutLine(NVGcontext * ctx,
                                 const Ptr<Text> & text,
                                 int line_index,
                                 const Optional<float> & max_width)
    {
        Ptr<TextDrawInfo> result = New<TextDrawInfo>();
        auto result_lines = result->lines();
        
        Text::Position pos(line_index, 0);
        
        while (true) {
            auto line_entry = LayoutSingleLine(ctx,
                                               text,
                                               pos);
            
            auto chars = line_entry->chars();
            RHETORIC_ASSERT(chars.size() > 0);
            for (int i = 0; i < (int)chars.size(); i++) {
                auto & ch = chars[i];
                if (max_width) {
                    if (i > 0 && *max_width < ch->draw_right()) {
                        chars.erase(chars.begin() + i, chars.end());
                        break;
                    }
                }
            }
            line_entry->set_chars(chars);
            
            pos = text->AdvancePosition(chars.back()->text_position());
            
            if (result_lines.size() > 0) {
                line_entry->set_wrapped_line(true);
            }
            
            result_lines.push_back(line_entry);
            
            if (line_index < pos.line_index()) {
                break;
            }
        }
        
        result->set_lines(result_lines);
        return result;
    }

    Ptr<TextDrawInfo::LineEntry>
    TextDrawLayouter::LayoutSingleLine(NVGcontext * ctx,
                                       const Ptr<Text> & text,
                                       const Text::Position & pos_)
    {
        Text::Position pos = pos_;
        
        NVGSetFont(ctx, font());
        nvgFontSize(ctx, font_size());
        
        StringSlice start_char = text->GetCharAt(pos);
        const char * start = start_char.c_str();
        Ptr<std::string> line = text->GetLineAt(pos.line_index());
        const char * end = line->c_str() + line->size();
        RHETORIC_ASSERT(line == start_char.base());
        
        std::vector<NVGglyphPosition> glyphs((int)(end - start));
        int num = nvgTextGlyphPositions(ctx,
                                        0, 0,
                                        start,
                                        end,
                                        glyphs.data(),
                                        (int)glyphs.size());
        glyphs.resize(num);
        
        auto entry = New<TextDrawInfo::LineEntry>(pos, false);
        auto chars = entry->chars();
        
        for (int i = 0; i < num; i++) {
            auto & glyph = glyphs[i];
            
            auto kind = GetUtf8ByteKind(*glyph.str);
            RHETORIC_ASSERT(kind.tag() == Utf8ByteKind::HeadTag);
            
            Text::Position next_pos = text->AdvancePosition(pos);
            if (pos.line_index() == next_pos.line_index()) {
                const char * next_pos_char = text->GetCharAt(next_pos).c_str();
                if (next_pos_char <= glyph.str) {
                    pos = next_pos;
                }
            }
            
            chars.push_back(New<TextDrawInfo::CharPosition>(pos,
                                                            glyph.minx,
                                                            glyph.maxx));
        }
        entry->set_chars(chars);
        
        return entry;
    }
    
    
}
