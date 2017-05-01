#include "./font.h"

namespace oliview {

    Font::Font(NVGcontext * context,
               int handle,
               std::string name,
               const Ref<const Data> & data):
    context_(context),
    handle_(handle),
    name_(name),
    data_(data)
    {        
    }

    Result<Ref<Font>> Font::Create(NVGcontext * context, const FilePath & path) {
        auto data_ret = path.Read();
        if (!data_ret) {
            return Failure(data_ret);
        }
        auto data = data_ret.value();
        auto split_ret = path.SplitExtension();
        auto name = split_ret.name;


        int offset = 0;
        int index = 0;
        for (int i = 0; ; i++) {
            int off = stbtt_GetFontOffsetForIndex((uint8_t *)data->bytes(), i);
            if (off < 0) {
                break;
            }

            stbtt_fontinfo fontinfo;
            int tt = stbtt_InitFont(&fontinfo, (uint8_t *)data->bytes(), offset);
            Print(Format("%d, %d, tt=%d", i, off, tt));
            
            offset = off;
            index = i;
        }


        int handle = nvgCreateFontMem(context,
                                      name.c_str(),
                                      (uint8_t *)data->bytes(),
                                      data->size(),
                                      false,
                                      offset);
        if (handle < 0) {
            return Failure(GenericError::Create("nvgCreateFontMem(path=%s, index=%d, offset=%d)",
                                                path.ToString().c_str(),
                                                index,
                                                offset));
        }
        
        return Success(oliview::Create<Font>(context,
                                             handle,
                                             name,
                                             data));
    }

    Result<Ref<Font>> Font::Find(NVGcontext * context, const std::string & name) {
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

    Ref<Font> Font::GetDefault(NVGcontext * context) {
        if (!default__) {
            auto ret = Find(context, "ヒラギノ角ゴシック W3");
            default__ = ret.value();
        }
        return default__;
    }

    std::vector<FilePath> Font::search_paths() {
        if (!search_paths_) {
            search_paths_ = Some<std::vector<FilePath>>({
                FilePath::home() + FilePath("Library/Fonts"),
                FilePath("/Library/Fonts"),
                FilePath("/System/Library/Fonts")
            });
        }
        return search_paths_.value();
    }

    void Font::set_search_paths(const std::vector<FilePath> & value) {
        search_paths_ = Some(value);
    }

    Ref<Font> Font::default__;
    Optional<std::vector<FilePath>> Font::search_paths_;
}