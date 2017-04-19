#pragma once

#include "./dependency.h"
#include "./optional.h"
#include "./string_function.h"

namespace oliview {
    class FilePath {
    public:
        enum class Type {
            Relative,
            Absolute
        };

        FilePath();
        FilePath(const std::string & string);
        FilePath(const FilePath & other);
        FilePath & operator= (const FilePath & other);

        Type type() const;
        const Optional<std::string> & drive_letter() const;
        const std::vector<std::string> & elements() const;

        FilePath parent() const;

        void Append(const FilePath & path);
        void Expand();

        std::string ToString() const;

        static std::string separator();
        static FilePath current();
    private:
        void Parse(const std::string & string);

        Type type_;
        Optional<std::string> drive_letter_;
        std::vector<std::string> elements_;
    };
}
