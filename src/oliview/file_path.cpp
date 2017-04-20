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
        FilePath ret = *this;

        while (true) {
            if (ret.elements_.size() == 0) {
                if (type_ == Type::Relative) {
                    ret.elements_.push_back("..");
                }
                return ret;
            }

            auto last_iter = ret.elements_.end() - 1;
            auto elem = *last_iter;
            if (elem == "" || elem == ".") {
                ret.elements_.erase(last_iter);
                continue;
            }

            if (elem == "..") {
                ret.elements_.push_back("..");
                return ret;
            }

            ret.elements_.erase(last_iter);
            return ret;
        }
    }

    std::vector<FilePath> FilePath::GetChildren(Ref<Error> * error) const {
        std::string path_str = ToString();
        DIR * dir = opendir(path_str.c_str());
        if (!dir) {
            if (error) {
                *error = PosixError::Create(errno, "opendir(%s)", path_str.c_str());
            }
            return
        }
        return {};
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
        if (!cstr) {
            auto err = PosixError::Create(errno, "getcwd");
            Fatal(err->ToString());
        }
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

        if (elements.size() == 0) {
            type_ = Type::Relative;
            drive_letter_ = nullptr;
            elements_.clear();
            return;
        }

        std::string head_element = elements[0];

        if (head_element == "") {
            type_ = Type::Absolute;
            drive_letter_ = nullptr;
            elements_ = std::vector<std::string>(elements.begin() + 1,
                                                 elements.end());
            return;
        }

        if (EndWith(head_element, ":")) {
            type_ = Type::Absolute;
            drive_letter_ = Some(head_element.substr(0, head_element.size() - 1));
            elements_ = std::vector<std::string>(elements.begin() + 1,
                                                 elements.end());
            return;
        }

        type_ = Type::Relative;
        drive_letter_ = nullptr;
        elements_ = elements;
    }
}
