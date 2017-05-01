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
        Font(NVGcontext * context,
             int handle,
             std::string name,
             const Ref<const Data> & data);

        static Result<Ref<Font>> Create(NVGcontext * context, const FilePath & path);
        static Result<Ref<Font>> Find(NVGcontext * context, const std::string & name);

        static Ref<Font> GetDefault(NVGcontext * context);

        static std::vector<FilePath> search_paths();
        static void set_search_paths(const std::vector<FilePath> & value);
    private:
        NVGcontext * context_;
        int handle_;
        std::string name_;
        Ref<const Data> data_;

        static Ref<Font> default__;
        static Optional<std::vector<FilePath>> search_paths_;


    };
}
