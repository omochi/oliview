#include "./file_path.h"

namespace oliview {
    FilePath::FilePath():
    type_(Type::Relative)
    {}

    FilePath::FilePath(const std::string & string) {
        Parse(string);
    }

    std::string FilePath::ToString() const {
        //TODO
        return "";
    }

    void FilePath::Parse(const std::string & string) {
        
    }
}
