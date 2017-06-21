#include "./font.h"

namespace oliview {

    Font::Font(NVGcontext * context,
               int handle,
               std::string name,
               const Ptr<const Data> & data):
    context_(context),
    handle_(handle),
    name_(name),
    data_(data)
    {        
    }

    int Font::handle() const {
        return handle_;
    }

    Result<Ptr<Font>> Font::Open(NVGcontext * context, const FilePath & path) {
        RHETORIC_TRY_ASSIGN(auto data, path.Read())

        auto ext = path.SplitExtension();
        auto name = ext.name;

        int handle = nvgCreateFontMem(context,
                                      name.c_str(),
                                      (uint8_t *)data->bytes(),
                                      data->size(),
                                      false);
        if (handle < 0) {
            return Failure(GenericError::Create("nvgCreateFontMem(%s)",
                                                path.ToString().c_str()));
        }
        
        return Success(New<Font>(context,
                                 handle,
                                 name,
                                 data));
    }

    Result<Ptr<Font>> Font::Find(NVGcontext * context, const std::string & name) {
        for (auto dir : search_paths()) {
            RHETORIC_TRY_ASSIGN(auto files, dir.GetChildren())

            for (auto file : files) {
                auto split_ret = file.SplitExtension();
                if (!split_ret.extension) {
                    continue;
                }
                auto ext = *split_ret.extension;

                if (!(ext == "ttf" || ext == "ttc")) {
                    continue;
                }

                if (!(split_ret.name == name)) {
                    continue;
                }

                return Open(context, file);
            }
        }
        return Failure(GenericError::Create("not found (%s)", name.c_str()));
    }

    Ptr<Font> Font::default_font() {
        return default_font_;
    }

    void Font::set_default_font(const Ptr<Font> & value) {
        default_font_ = value;
    }

    std::vector<FilePath> Font::search_paths() {
        return search_paths_;
    }

    void Font::set_search_paths(const std::vector<FilePath> & value) {
        search_paths_ = value;
    }

    Ptr<Font> Font::default_font_;
    std::vector<FilePath> Font::search_paths_;
}
