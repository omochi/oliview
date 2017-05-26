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

    Result<Ptr<Font>> Font::Create(NVGcontext * context, const FilePath & path) {
        auto data_ret = path.Read();
        if (!data_ret) {
            return Failure(data_ret);
        }
        auto data = data_ret.value();

        auto ext = path.SplitExtension();
        auto name = ext.name;

        int handle = nvgCreateFontMem(context,
                                      name.c_str(),
                                      (uint8_t *)data->bytes(),
                                      data->size(),
                                      false,
                                      0);
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
            auto files = dir.GetChildren();
            if (!files) {
                return Failure(files);
            }
            for (auto file : files.value()) {
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

                auto font = Create(context, file);
                return font;
            }
        }
        return Failure(GenericError::Create("not found (%s)", name.c_str()));
    }

    Ptr<Font> Font::GetDefault(NVGcontext * context) {
        if (!default__) {
            auto ret = Find(context, "ヒラギノ角ゴシック W3");
            default__ = ret.value();
        }
        return default__;
    }

    std::vector<FilePath> Font::search_paths() {
        if (!search_paths_) {
            search_paths_ = Some(default_search_paths());
        }
        return search_paths_.value();
    }

    void Font::set_search_paths(const std::vector<FilePath> & value) {
        search_paths_ = Some(value);
    }

#if RHETORIC_MACOS
    std::vector<FilePath> Font::default_search_paths() {
        std::vector<FilePath> ret;
        ret.push_back(FilePath::home() + FilePath("Library/Fonts"));
        ret.push_back(FilePath("/Library/Fonts"));
        ret.push_back(FilePath("/System/Library/Fonts"));
        return ret;
    }
#endif

    Ptr<Font> Font::default__;
    Optional<std::vector<FilePath>> Font::search_paths_;
}
