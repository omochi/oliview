#include "./font_manager.h"

#include "./application.h"

namespace oliview {
    FontManager::~FontManager() {}
    
    Result<None> FontManager::SetUp() {
        std::vector<FilePath> paths;
#if RHETORIC_MACOS
        paths.push_back(FilePath::home() + FilePath("Library/Fonts"));
        paths.push_back(FilePath("/Library/Fonts"));
        paths.push_back(FilePath("/System/Library/Fonts"));
#endif
        set_search_paths(paths);
        
        std::string default_font_name = "ヒラギノ角ゴシック W3";
        RHETORIC_TRY_ASSIGN(auto default_font, Find(default_font_name));
        if (!default_font) {
            return GenericError::Create("default font not found: %s",
                                        default_font_name.c_str());
        }
        set_default_font(default_font);
        
        return Ok(None());
    }
    
    Result<Ptr<Font>> FontManager::Open(const FilePath & path) {
        auto app = this->application();
        RHETORIC_ASSERT(app != nullptr);
        
        auto ctx = app->_nvg_context();
        RHETORIC_ASSERT(ctx != nullptr);
        
        RHETORIC_TRY_ASSIGN(auto data, path.Read())
        
        auto ext = path.SplitExtension();
        auto name = ext.name;
        
        int handle = nvgCreateFontMem(ctx,
                                      name.c_str(),
                                      (uint8_t *)data->bytes(),
                                      (int)data->size(),
                                      false);
        if (handle < 0) {
            return GenericError::Create("nvgCreateFontMem(%s)",
                                        path.ToString().c_str());
        }
        
        return Ok(New<Font>(ctx,
                            handle,
                            name,
                            data));
    }
    
    Result<Ptr<Font>> FontManager::Find(const std::string & name) {
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
                
                return Open(file);
            }
        }
        return Ok(nullptr);
    }
}

