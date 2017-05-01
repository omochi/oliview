#pragma once

#include "./dependency.h"
#include "./object.h"
#include "./optional.h"
#include "./result.h"
#include "./file_path.h"
#include "./string_format.h"

namespace oliview {
    class Font : public Object<Font> {
    public:
        Font(const FilePath & file);

        static Result<Ref<Font>> Create(NVGcontext * context, const FilePath & path);
        static Result<Ref<Font>> Find(const std::string & name);

        static Ref<Font> default_();

        static std::vector<FilePath> search_paths();
        static void set_search_paths(const std::vector<FilePath> & value);
    private:
        static Ref<Font> default__;
        static Optional<std::vector<FilePath>> search_paths_;


    };
}
