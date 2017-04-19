#pragma once

#include "./dependency.h"
#include "./optional.h"

namespace oliview {
    class FilePath {
    public:
        enum class Type {
            Relative,
            Absolute
        };

        FilePath();
        FilePath(const std::string & string);

        std::string ToString() const;
    private:
        void Parse(const std::string & string);

        Type type_;
        Optional<std::string> drive_letter_;
        std::vector<std::string> elements_;
    };
}
