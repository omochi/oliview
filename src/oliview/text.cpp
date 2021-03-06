#include "./text.h"

namespace oliview {
    Text::Index::Index(): Index(0, 0){}
    
    Text::Index::Index(size_t line, size_t byte):
    line_(line),
    byte_(byte)
    {}
    
    bool Text::Index::operator==(const Index & other) const {
        return line_ == other.line_ &&
        byte_ == other.byte_;
    }
    
    bool Text::Index::operator<(const Index & other) const {
        if (line_ == other.line_) {
            if (byte_ < other.byte_) {
                return true;
            }
        }
        if (line_ < other.line_) {
            return true;
        }
        return false;
    }
    
    std::string Text::Index::ToString() const {
        return Format("Text::Index(%" PRIdS ", %" PRIdS ")", line_, byte_);
    }
    
    Text::Text() {
        FixLastLine();
    }
    
    Text::Text(const std::string & value) {
        set_string(value);
    }
    
    Text::Text(const Text & other) {
        *this = other;
    }
    
    Text & Text::operator=(const Text & other) {
        lines_.clear();
        
        for (auto & other_line : other.lines()) {
            Ptr<std::string> line = New<std::string>(*other_line);
            lines_.push_back(line);
        }

        return *this;
    }
    
    std::string Text::string() const {
        std::vector<std::string> ls = ArrayMap(lines_, [](auto x) { return *x; });
        return Join(ls, "");
    }
    
    void Text::set_string(const std::string & value) {
        std::vector<Ptr<std::string>> lines;
        TextLineReader reader(New<std::string>(value));
        while (true) {
            auto line = reader.Read();
            if (!line) {
                break;
            }
            lines.push_back(New<std::string>(line.value()));
        }
        
        lines_ = lines;
        FixLastLine();
    }
    
    std::vector<Ptr<const std::string>> Text::lines() const {
        return std::vector<Ptr<const std::string>>(lines_.begin(), lines_.end());
    }
    
    void Text::set_lines(const std::vector<Ptr<const std::string>> & value) {
        std::vector<Ptr<std::string>> lines;
        
        Ptr<std::string> current_line;
        for (auto target_line : value) {
            auto reader = TextLineReader(target_line);
            while (true) {
                auto new_line = reader.Read();
                if (!new_line) {
                    break;
                }
                
                if (!current_line) {
                    current_line = New<std::string>();
                }
                current_line->append(new_line.value());
                
                if (CheckEndWith(*current_line, current_line->size(), newline_strs())) {
                    lines.push_back(current_line);
                    current_line = nullptr;
                }
            }
        }
        
        if (current_line) {
            lines.push_back(current_line);
            current_line = nullptr;
        }
        
        lines_ = lines;
        FixLastLine();
    }
    
    size_t Text::line_num() const {
        return lines_.size();
    }
    
    Ptr<const std::string> Text::GetLineAt(size_t index) const {
        return lines_[index];
    }
    
    void Text::SetLineAt(size_t index, const Ptr<const std::string> & value) {
        lines_[index] = New<std::string>(*value);
    }
    
    std::string Text::GetCharAt(const Index & index) const {
        auto acc = AccessCharAt(index);
        return acc.string.AsString();
    }
    
    void Text::SetCharAt(const Index & index, const std::string & chr) {
        auto acc = AccessCharAt(index);
        auto str = acc.string;
        str.base()->replace(str.offset(), str.length(), chr);
    }
    
    Text::Index Text::begin_index() const {
        return Index(0, 0);
    }
    
    Text::Index Text::end_index() const {
        return Index(lines_.size() - 1, lines_.back()->size());
    }
    
    Text::Index Text::AdvanceIndex(const Index & index) const {
        if (index == end_index()) {
            return index;
        }
    
        Index next_index = index;
        while (true) {
            if (next_index.line() != index.line()) {
                break;
            }
            
            auto chr = GetCharAt(next_index);
            if (chr.length() == 0 ||
                ArrayFindIndexEq(newline_chars(), chr[0]))
            {
                next_index = AdvanceIndexNoSkip(next_index);
                continue;
            }
            
            next_index = AdvanceIndexNoSkip(next_index);
        
            break;
        }
        
        return next_index;
    }
    
    Text::Index Text::AdvanceIndexNoSkip(const Index & index_) const {
        Index index = index_;
        if (index == end_index()) {
            return index;
        }
        auto chr = GetCharAt(index);
        if (chr.length() == 0) {
            return Index(index.line() + 1, 0);
        }
        return Index(index.line(), index.byte() + chr.size());
    }
    
    Text::Index Text::BackIndex(const Index & index_) const {
        Index index = index_;
        
        if (index == begin_index()) {
            return index;
        }
        
        index = BackIndexNoSkip(index);
        while (true) {
            if (index.byte() == 0) {
                break;
            }
            
            Index back_index = BackIndexNoSkip(index);
            
            auto chr = GetCharAt(back_index);
            if (chr.length() == 0 ||
                ArrayFindIndexEq(newline_chars(), chr[0]))
            {
                index = back_index;
                continue;
            }
            
            break;
        }
        
        return index;
    }
    
    Text::Index Text::BackIndexNoSkip(const Index & index_) const {
        Index index = index_;
        if (index == begin_index()) {
            return index;
        }
        if (index.byte() == 0) {
            size_t line_index = index.line() - 1;
            auto line = GetLineAt(line_index);
            return Index(line_index, line->size());
        }
        index = Index(index.line(), index.byte() - 1);
        while (true) {
            if (index.byte() == 0) {
                return index;
            }
            auto acc = AccessCharAt(index);
            if (acc.kind->tag() == Utf8ByteKind::HeadTag) {
                return index;
            }
            index = Index(index.line(), index.byte() - 1);
        }
    }
    
    bool Text::CheckIndex(const Index & index) const {
        if (!(index.line() < line_num())) {
            return false;
        }
        auto line = lines_[index.line()];
        if (!(index.byte() <= line->size())) {
            return false;
        }
        return true;
    }
    
    void Text::InsertAt(const Text::Index & index_,
                        const Ptr<const Text> & text,
                        Text::Index * end_index)
    {
        auto index = index_;
        
        RHETORIC_ASSERT(CheckIndex(index));
        index = MayLineWrapIndex(index);
        
        Ptr<std::string> dest_line = lines_[index.line()];
        std::string head_str = dest_line->substr(0, index.byte() - 0);
        std::string tail_str = dest_line->substr(index.byte(), dest_line->size() - index.byte());

        for (size_t i = 0; i < text->line_num(); i++) {
            Ptr<const std::string> insert_line = text->GetLineAt(i);
            
            if (i == 0) {
                dest_line->erase(index.byte(), dest_line->size() - index.byte());
                dest_line->insert(index.byte(), *insert_line);
            } else {
                dest_line = New<std::string>();
                lines_.insert(lines_.begin() + ToSigned(index.line() + i), dest_line);
                *dest_line = *insert_line;
            }

            if (i + 1 == text->line_num()) {
                dest_line->append(tail_str);
            }
        }
        
        size_t last_line_index = index.line() + text->line_num() - 1;
        Ptr<std::string> last_line = lines_[last_line_index];
        
        index = Index(last_line_index, last_line->size() - tail_str.size());
        index = MayLineWrapIndex(index);

        if (end_index) {
            *end_index = index;
        }
    }
    
    void Text::DeleteAt(const Index & begin_,
                        const Index & end_)
    {
        auto begin = begin_;
        auto end = end_;
        
        RHETORIC_ASSERT(CheckIndex(begin));
        RHETORIC_ASSERT(CheckIndex(end));
        RHETORIC_ASSERT(begin <= end);
        
        begin = MayLineWrapIndex(begin);
        end = MayLineWrapIndex(end);
        
        auto end_line = lines_[end.line()];
        auto end_tail_str = end_line->substr(end.byte(),
                                              end_line->size() - end.byte());
        auto begin_line = lines_[begin.line()];
        
        if (begin.line() < end.line()) {
            ArrayRemoveRange(&lines_, MakeRange(begin.line() + 1,
                                                end.line() + 1));
        }
        
        begin_line->replace(begin.byte(),
                            begin_line->size() - begin.byte(),
                            end_tail_str);
    }
    
    Text::Index Text::GetNewlineIndex(size_t line_index) const {
        Ptr<std::string> line = lines_[line_index];
        size_t i = 0;
        for (; i < line->size(); i++) {
            char ch = (*line)[i];
            if (ArrayFindIndexEq(newline_chars(), ch)) {
                break;
            }
        }
        return Text::Index(line_index, i);
    }
    
    void Text::FixLastLine() {
        if (lines_.size() == 0) {
            lines_.push_back(New<std::string>());
        } else {
            auto last_line = lines_.back();
            if (CheckEndWith(*last_line, last_line->size(), newline_strs())) {
                lines_.push_back(New<std::string>());
            }
        }
    }
    
    Text::StringAccess<const std::string>
    Text::AccessCharAt(const Index & index_) const {
        Index index = index_;
        
        RHETORIC_ASSERT(CheckIndex(index));
        
        Ptr<std::string> line = lines_[index.line()];

        if (index.byte() == line->size()) {
            return StringAccess<const std::string>(ConstStringSlice(line, index.byte(), 0), None());
        }
        
        char c = (*line)[index.byte()];
        auto kind = GetUtf8ByteKind(c);
        switch (kind.tag()) {
            case Utf8ByteKind::HeadTag: {
                size_t end = std::min(index.byte() + kind.AsHead().length, line->size());
                return StringAccess<const std::string>(ConstStringSlice(line, index.byte(), end - index.byte()), Some(kind));
            }
            case Utf8ByteKind::BodyTag: {
                return StringAccess<const std::string>(ConstStringSlice(line, index.byte(), 1), Some(kind));
            }
        }
        RHETORIC_FATAL("never");
    }
    
    Text::StringAccess<std::string>
    Text::AccessCharAt(const Index & index) {
        auto cret = const_cast<const Text *>(this)->AccessCharAt(index);
        return StringAccess<std::string>(StringSlice(), cret.kind);
    }
    
    Text::Index Text::MayLineWrapIndex(const Index & index) const {
        if (index.line() + 1 < lines_.size()) {
            if (index.byte() == lines_[index.line()]->size()) {
                return Index(index.line() + 1, 0);
            }
        }
        return index;
    }
    
}
