#include "./unicode_util.h"

namespace oliview {
    std::vector<UnicodeScalarIndex> GetUnicodeScalarIndices(const std::string & string) {
        std::vector<UnicodeScalarIndex> ret;
        
        int group_begin_index = 0;
        int group_length = 0;
        int group_expected_length = 0;

        for (int i = 0; i < (int)string.size(); i++) {
            int byte = string[i] & 0xFF;
            
            int char_length = 0;
            if ((byte & 0x80) == 0x00) {
                char_length = 1;
            } else if ((byte & 0xC0) == 0x80) {
                if (group_expected_length > 0) {
                    group_length += 1;
                    if (group_length > group_expected_length) {
                        group_expected_length = 0;
                    }
                }
            } else if ((byte & 0xE0) == 0xC0) {
                char_length = 2;
            } else if ((byte & 0xF0) == 0xE0) {
                char_length = 3;
            } else if ((byte & 0xF8) == 0xF0) {
                char_length = 4;
            } else if ((byte & 0xFC) == 0xF8) {
                char_length = 5;
            } else if ((byte & 0xFE) == 0xFC) {
                char_length = 6;
            } else {
                group_expected_length = 0;
            }
            
            if (char_length > 0) {
                if ((group_expected_length > 0) &&
                    (group_length == group_expected_length))
                {
                    UnicodeScalarIndex usi;
                    usi.index_range = MakeRange(group_begin_index, group_begin_index + group_length);
                    ret.push_back(usi);
                }
                
                group_begin_index = i;
                group_length = 1;
                group_expected_length = char_length;
            }
        }
        
        if ((group_expected_length > 0) &&
            (group_length == group_expected_length))
        {
            UnicodeScalarIndex usi;
            usi.index_range = MakeRange(group_begin_index, group_begin_index + group_length);
            ret.push_back(usi);
        }
        
        return ret;
    }
}
