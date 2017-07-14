#pragma once

#include "./application_object.h"
#include "./font.h"

namespace oliview {
    class Application;
    
    class FontManager : public ApplicationObject {
    public:
        virtual ~FontManager();
        
        Result<None> SetUp();
        
        Result<Ptr<Font>> Open(const FilePath & path);
        Result<Ptr<Font>> Find(const std::string & name);
        
        RHETORIC_ACCESSOR(Ptr<Font>, default_font)
        RHETORIC_ACCESSOR(std::vector<FilePath>, search_paths)
        
        static Result<Ptr<FontManager>> Create(const Ptr<Application> & application);
    private:
        
        Ptr<Font> default_font_;
        std::vector<FilePath> search_paths_;
    };
}
