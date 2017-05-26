#pragma once

#include "./dependency.h"

namespace oliview {
    class Font : public std::enable_shared_from_this<Font> {
    public:
        Font(NVGcontext * context,
             int handle,
             std::string name,
             const Ptr<const Data> & data);

        static Result<Ptr<Font>> Create(NVGcontext * context, const FilePath & path);
        static Result<Ptr<Font>> Find(NVGcontext * context, const std::string & name);

        static Ptr<Font> GetDefault(NVGcontext * context);

        static std::vector<FilePath> search_paths();
        static void set_search_paths(const std::vector<FilePath> & value);
    private:
        NVGcontext * context_;
        int handle_;
        std::string name_;
        Ptr<const Data> data_;

        static std::vector<FilePath> default_search_paths();

        static Ptr<Font> default__;
        static Optional<std::vector<FilePath>> search_paths_;


    };
}
