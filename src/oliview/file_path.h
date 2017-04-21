#pragma once

#include "./dependency.h"
#include "./optional.h"
#include "./result.h"
#include "./string_function.h"
#include "./posix_error.h"
#include "./ref.h"
#include "./defer.h"
#include "./print.h"

namespace oliview {
    class FilePath {
    public:
        enum class Type {
            Relative,
            Absolute
        };

        FilePath();
        explicit FilePath(const std::string & string);
        FilePath(const FilePath & other);
        FilePath & operator= (const FilePath & other);

        Type type() const;
        const Optional<std::string> & drive_letter() const;
        const std::vector<std::string> & elements() const;

        std::string ToString() const;

        FilePath parent() const;
        Result<std::vector<FilePath>> GetChildren() const;

        FilePath basename() const;
        std::string extension() const;


        void Append(const FilePath & path);
        void Expand();

        FilePath operator+(const FilePath & path) const;

        static std::string separator();
        static FilePath current();
        static FilePath home();
    private:
        void Parse(const std::string & string);

        Type type_;
        Optional<std::string> drive_letter_;
        std::vector<std::string> elements_;
    };
}
