#pragma once

#include "./dependency.h"

namespace oliview {
    class Font : public std::enable_shared_from_this<Font> {
    public:
        Font(NVGcontext * context,
             int handle,
             std::string name,
             const Ptr<const Data> & data);

        int handle() const;

        static Result<Ptr<Font>> Open(NVGcontext * context, const FilePath & path);
        static Result<Ptr<Font>> Find(NVGcontext * context, const std::string & name);

        static Ptr<Font> default_font();
        static void set_default_font(const Ptr<Font> & value);

        static std::vector<FilePath> search_paths();
        static void set_search_paths(const std::vector<FilePath> & value);
    private:
        NVGcontext * context_;
        int handle_;
        std::string name_;
        Ptr<const Data> data_;

        static Ptr<Font> default_font_;
        static std::vector<FilePath> search_paths_;
    };
}
