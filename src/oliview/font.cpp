#include "./font.h"

namespace oliview {

    Font::Font(const FilePath & file) {
        auto data = file.Read().value();
        
    }

    Result<Ref<Font>> Font::Create(NVGcontext * context, const FilePath & path) {
        auto data_ret = path.Read();
        if (!data_ret) {
            return Failure(data_ret);
        }
        auto data = data_ret.value();

//        nvgCreateFontMem(context, <#const char *name#>, <#unsigned char *data#>, <#int ndata#>, <#int freeData#>)
        return Failure(nullptr);
    }

    Result<Ref<Font>> Font::Find(const std::string & name) {
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

                if (ext == "ttf" || ext == "ttc") {
                    if (split_ret.name == name) {
                        Print(file.ToString());
                    }
                }
            }
        }
        return Failure(GenericError::Create("todo"));
    }

    Ref<Font> Font::default_() {
        if (!default__) {
            auto ret = Find("ヒラギノ角ゴシック W3");
            if (!ret) {
                Fatal(ret.error());
            }
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
