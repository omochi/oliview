#include "./file_path.h"

namespace oliview {
    FilePath::FilePath():
    type_(Type::Relative)
    {}

    FilePath::FilePath(const std::string & string) {
        Parse(string);
    }

    FilePath::FilePath(const FilePath & other) {
        *this = other;
    }

    FilePath & FilePath::operator= (const FilePath & other) {
        type_ = other.type_;
        drive_letter_ = other.drive_letter_;
        elements_ = other.elements_;
        return *this;
    }

    FilePath::Type FilePath::type() const {
        return type_;
    }

    const Optional<std::string> & FilePath::drive_letter() const {
        return drive_letter_;
    }

    const std::vector<std::string> & FilePath::elements() const {
        return elements_;
    }

    FilePath FilePath::parent() const {
        auto elements = elements_;
        int index = (int)elements.size() - 1;
        while (true) {
            if (index == -1) {
                FilePath ret = *this;
                ret.elements_.clear();
                if (type_ == Type::Relative) {
                    ret.elements_.push_back("..");
                }
                return ret;
            }

            auto elem = elements[index];
            if (elem == "" || elem == ".") {
                index -= 1;
                continue;
            }

            if (elem == "..") {
                FilePath ret = *this;
                ret.elements_.erase(ret.elements_.cbegin() + index + 1,
                                    ret.elements_.cend());
                ret.elements_.push_back("..");
                return ret;
            }

            FilePath ret = *this;
            ret.elements_.erase(ret.elements_.cbegin() + index,
                                ret.elements_.cend());
            return ret;
        }
    }

    void FilePath::Append(const FilePath & path) {
        OLIVIEW_ASSERT(path.type() != Type::Absolute);

        elements_.insert(elements_.end(),
                         path.elements_.begin(),
                         path.elements_.end());
    }

    void FilePath::Expand() {
        FilePath ret = *this;
        ret.elements_.clear();

        for (auto & elem : elements_) {
            if (elem == "" || elem == ".") {
                continue;
            }
            if (elem == "..") {
                ret = ret.parent();
                continue;
            }
            ret.Append(FilePath(elem));
        }

        *this = ret;
    }

    std::string FilePath::ToString() const {
#ifdef OLIVIEW_MACOS
        std::string str;

        if (type_ == Type::Absolute) {
            str += separator();
        }

        str += Join(elements_, separator());
        return str;
#else
#   warning TODO
        return "";
#endif
    }

    std::string FilePath::separator() {
#ifdef OLIVIEW_MACOS
        return "/";
#else
#   warning TODO
#endif
    }

    FilePath FilePath::current() {
#ifdef OLIVIEW_MACOS
        char * cstr = getcwd(nullptr, 0);
        std::string str = std::string(cstr);
        FilePath ret(str);
        free(cstr);
        return ret;
#else
#   warning TODO
#endif
    }

    void FilePath::Parse(const std::string & string) {
        auto elements = Split(string, separator());

#ifdef OLIVIEW_MACOS
        if (elements.size() >= 2 && elements[0] == "") {
            type_ = Type::Absolute;
            drive_letter_ = nullptr;
            elements_ = std::vector<std::string>(elements.cbegin() + 1,
                                                 elements.cend());
        } else {
            type_ = Type::Relative;
            drive_letter_ = nullptr;
            elements_ = elements;
        }

#else
#   warning TODO
#endif
    }
}
