#include "./text_line_reader.h"

namespace oliview {
    TextLineReader::TextLineReader(const Ptr<const std::string> & string):
    string_(string),
    index_(0)
    {}
    
    Option<std::string> TextLineReader::Read() {
        if (index_ == string_->size()) {
            return None();
        }
        
        size_t begin = index_;
        std::vector<std::string> newline_strs = rhetoric::newline_strs();
        while (true) {
            if (index_ == string_->size()) {
                break;
            }
            
            auto check_ret = CheckStartWith(*string_, index_, newline_strs);
            if (check_ret) {
                index_ += newline_strs[check_ret->target_index].size();
                break;
            }
            
            index_ += 1;
        }
        return Some(string_->substr(begin, index_ - begin));
    }

}
