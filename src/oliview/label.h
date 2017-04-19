#pragma once

#include "./view.h"

namespace oliview {
    class Label : public View {
    public:
        Label();
        Label(const std::string & text);

        const std::string & text() const;
        void set_text(const std::string & value);
    private:
        std::string text_;
    };
}
