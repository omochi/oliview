#include "./label.h"

namespace oliview {
    Label::Label()
    {}

    Label::Label(const std::string & text) {
        set_text(text);
    }

    const std::string & Label::text() const {
        return text_;
    }

    void Label::set_text(const std::string & value) {
        text_ = value;
    }
}
