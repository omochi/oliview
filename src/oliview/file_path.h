#pragma once

#include "./dependency.h"
#include "./array_size.h"
#include "./optional.h"
#include "./result.h"
#include "./string_function.h"
#include "./posix_error.h"
#include "./ref.h"
#include "./defer.h"
#include "./print.h"
#include "./none.h"
#include "./data.h"
#include "./generic_error.h"

namespace oliview {
    struct SplitExtensionResult {
        std::string name;
        Optional<std::string> extension;
    };

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

        bool operator==(const FilePath & other) const;
        bool operator!=(const FilePath & other) const;

        Optional<FilePath> parent() const;
        Result<std::vector<FilePath>> GetChildren() const;

        FilePath basename() const;
        OLIVIEW_NO_DISCARD Result<None> SetBasename(const FilePath & value);

        SplitExtensionResult SplitExtension() const;

        Optional<std::string> extension() const;
        void set_extension(const Optional<std::string> & value);

        OLIVIEW_NO_DISCARD Result<None> Append(const FilePath & path);
        FilePath operator+(const FilePath & other);

        OLIVIEW_NO_DISCARD Result<None> Expand();

        Result<Ref<Data>> Read() const;
        OLIVIEW_NO_DISCARD Result<None> Write(const Ref<const Data> & data);


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
