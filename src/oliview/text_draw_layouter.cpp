#include "./text_draw_layouter.h"

namespace oliview {
    TextDrawLayouter::CharPosition::CharPosition(const char * chr,
                                                 int index,
                                                 int length,
                                                 float left,
                                                 float right):
    chr(chr),
    index(index),
    length(length),
    left(left),
    right(right)
    {}
    
    TextDrawLayouter::LineEntry::LineEntry(int original_line_index,
                                           bool wrapped_line):
    original_line_index_(original_line_index),
    wrapped_line_(wrapped_line)
    {}
    
    int TextDrawLayouter::LineEntry::chars_num() const {
        return (int)chars_.size();
    }
    
    const char * TextDrawLayouter::LineEntry::str() const {
        if (chars_.size() == 0) {
            return nullptr;
        }
        return chars_.front()->chr;
    }
    
    int TextDrawLayouter::LineEntry::length() const {
        if (chars_.size() == 0) {
            return 0;
        }
        auto ch = chars_.back();
        return ch->index + ch->length;
    }
    
    float TextDrawLayouter::LineEntry::draw_width() const {
        if (chars_.size() == 0) {
            return 0;
        }
        return chars_.back()->right;
    }
    
    TextDrawLayouter::Result
    TextDrawLayouter::Layout(NVGcontext * ctx,
                             const Ptr<Text> & text,
                             const Optional<float> & max_width)
    {
        Result result;
        
        for (int i = 0; i < text->line_num(); i++) {
            auto line = text->GetLinePtrAt(i);
            auto line_result = LayoutLine(ctx,
                                          line->c_str(),
                                          (int)line->size(),
                                          max_width);
            
            for (auto entry : line_result.lines) {
                entry.set_original_line_index(i);
                
                result.lines.push_back(entry);
            }
        }
        
        return result;
    }
    
    
    
    TextDrawLayouter::Result
    TextDrawLayouter::LayoutLine(NVGcontext * ctx,
                                 const char * line,
                                 int line_length,
                                 const Optional<float> & max_width)
    {
        TextDrawLayouter::Result result;
        
        int start = 0;
        
        while (true) {
            auto line_entry = LayoutSingleLine(ctx,
                                               line + start,
                                               line_length - start);
            
            auto chars = line_entry.chars();
            for (int i = 0; i < (int)chars.size(); i++) {
                auto & ch = chars[i];
                if (max_width) {
                    if (i > 0 && *max_width < ch->right) {
                        chars.erase(chars.begin() + i, chars.end());
                        break;
                    }
                }
            }
            line_entry.set_chars(chars);
            
            start += line_entry.length();
            
            if (result.lines.size() > 0) {
                line_entry.set_wrapped_line(true);
            }
            
            result.lines.push_back(line_entry);
            
            if (start == line_length) {
                break;
            }
        }
        
        return result;
    }
    
    
    Size TextDrawLayouter::GetResultSize(NVGcontext * ctx,
                                         const Result & result)
    {
        NVGSetFont(ctx, font());
        nvgFontSize(ctx, font_size());
        
        float width = 0.0f;
        for (auto line : result.lines) {
            width = std::max(width, line.draw_width());
        }
        
        float line_height;
        nvgTextMetrics(ctx, nullptr, nullptr, &line_height);
        
        float height = line_height * (float)result.lines.size();
        
        return Size(width, height);
    }
    
    void TextDrawLayouter::DrawResult(NVGcontext * ctx,
                                      const Result & result)
    {
        NVGSetFont(ctx, font());
        nvgFontSize(ctx, font_size());
        
        float ascender;
        float line_height;
        nvgTextMetrics(ctx, &ascender, nullptr, &line_height);
        
        float y = 0;
        for (auto & line : result.lines) {
            if (line.chars_num() > 0) {
                nvgText(ctx,
                        0,
                        y + ascender,
                        line.str(),
                        line.str() + line.length());
            }
            y += line_height;
        }
    }
    
    TextDrawLayouter::LineEntry
    TextDrawLayouter::LayoutSingleLine(NVGcontext * ctx,
                                       const char * line,
                                       int line_length)
    {
        NVGSetFont(ctx, font());
        nvgFontSize(ctx, font_size());
        
        std::vector<NVGglyphPosition> positions(line_length);
        int num = nvgTextGlyphPositions(ctx,
                                        0, 0,
                                        line,
                                        line + line_length,
                                        positions.data(),
                                        (int)positions.size());
        positions.resize(num);
        
        LineEntry entry(0, false);
        auto chars = entry.chars();
        
        for (int i = 0; i < num; i++) {
            auto & pos = positions[i];
            
            auto kind = GetUtf8ByteKind(*pos.str);
            RHETORIC_ASSERT(kind.tag() == Utf8ByteKind::HeadTag);
            
            int len = kind.AsHead().length;
            
            chars.push_back(New<CharPosition>(pos.str,
                                              (int)(pos.str - line),
                                              len,
                                              pos.minx,
                                              pos.maxx));
        }
        entry.set_chars(chars);
        
        return entry;
    }
    
    
}
