#pragma once

#include "./font.h"

namespace oliview {
    class Application;
    
    class FontManager : public std::enable_shared_from_this<FontManager> {
    public:
        virtual ~FontManager();
        
        Result<Ptr<Font>> Open(const FilePath & path);
        Result<Ptr<Font>> Find(const std::string & name);
        
        RHETORIC_ACCESSOR(Ptr<Font>, default_font)
        RHETORIC_ACCESSOR(std::vector<FilePath>, search_paths)
        
        static Result<Ptr<FontManager>> Create(const Ptr<Application> & application);
    private:
        FontManager();
        Result<None> Init(const Ptr<Application> & application);
        NVGcontext * nvg_context() const;
        
        WeakPtr<Application> application_;
        
        Ptr<Font> default_font_;
        std::vector<FilePath> search_paths_;
    };
}
