#include "./text_line_reader.h"

namespace oliview {
    TextLineReader::TextLineReader(const Ptr<std::string> & string):
    string_(string),
    index_(0)
    {}
    
    Optional<StringSlice> TextLineReader::Read() {
        std::vector<std::string> newline_chars = rhetoric::newline_chars();
        size_t begin = index_;
        while (true) {
            if (index_ == string_->size()) {
                if (index_ == begin) {
                    return None();
                }
                break;
            }
            
            auto check_ret = CheckStartWith(*string_, index_, newline_chars);
            if (check_ret) {
                index_ += newline_chars[check_ret->target_index].size();
                break;
            }
            
            index_ += 1;
        }
        return Some(StringSlice(string_, begin, index_ - begin));
    }

}