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

    std::string FilePath::ToString() const {
        std::string str;

        if (type_ == Type::Absolute) {
            if (drive_letter_) {
                str = *drive_letter_ + ":" + separator();
            } else {
                str = separator();
            }
        }

        str += Join(elements_, separator());
        return str;
    }

    bool FilePath::operator==(const FilePath & other) const {
        return ToString() == other.ToString();
    }

    bool FilePath::operator!=(const FilePath & other) const {
        return !(*this == other);
    }


    Optional<FilePath> FilePath::parent() const {
        FilePath ret = *this;

        while (true) {
            if (ret.elements_.size() == 0) {
                if (type_ == Type::Absolute) {
                    return nullptr;
                }

                ret.elements_.push_back("..");

                return Some(ret);
            }

            auto iter = ret.elements_.end() - 1;
            auto elem = *iter;
            if (elem == "" || elem == ".") {
                ret.elements_.erase(iter);
                continue;
            }

            if (elem == "..") {
                ret.elements_.push_back("..");
                return Some(ret);
            }

            break;
        }

        ret.elements_.erase(ret.elements_.end() - 1);
        return Some(ret);
    }

    Result<std::vector<FilePath>> FilePath::GetChildren() const {
        std::string str = ToString();
        DIR * handle = opendir(str.c_str());
        if (!handle) {
            return Failure(PosixError::Create(errno, "opendir(%s)", str.c_str()));
        }

        OLIVIEW_DEFER([=] {
            if (closedir(handle)) {
                Fatal(PosixError::Create(errno, "closedir(%s)", str.c_str()));
            }
        })

        std::vector<FilePath> ret;

        dirent * entry;
        errno = 0;
        while (true) {
            entry = readdir(handle);
            if (!entry) {
                if(errno) {
                    return Failure(PosixError::Create(errno, "readdir(%s)", str.c_str()));
                }
                break;
            }

            auto name = std::string(entry->d_name);
            if (name == "." || name == "..") {
                continue;
            }

            auto child = *this;
            child.Append(FilePath(name)).value();
            ret.push_back(child);
        }

        return Success(ret);
    }

    FilePath FilePath::basename() const {
        if (elements_.size() == 0) {
            return FilePath("");
        }
        return FilePath(elements_.back());
    }

    Result<None> FilePath::SetBasename(const FilePath & value) {
        if (value.elements_.size() != 1) {
            return Failure(GenericError::Create("value must has 1 element (%s)",
                                                value.ToString().c_str()));
        }
        if (value.type_ != Type::Relative) {
            return Failure(GenericError::Create("value must be relative (%s)",
                                                value.ToString().c_str()));
        }

        if (elements_.size() == 0) {
            Append(value).value();
        } else {
            elements_.back() = value.ToString();
        }

        return Success(None());
    }

    SplitExtensionResult FilePath::SplitExtension() const {
        std::string basename = this->basename().ToString();
        auto strs = SplitR(basename, std::string("."), { .limit = Some(2) });

        Optional<std::string> ext;
        if (strs.size() == 2) {
            ext = Some(strs[1]);
        }

        return { strs[0], ext };
    }

    Optional<std::string> FilePath::extension() const {
        return SplitExtension().extension;
    }

    void FilePath::set_extension(const Optional<std::string> & value) {
        auto split_ret = SplitExtension();
        std::string name = split_ret.name;
        if (value) {
            name = name + "." + *value;
        }
        SetBasename(FilePath(name)).value();
    }

    Result<None> FilePath::Append(const FilePath & path) {
        if (path.type_ != Type::Relative) {
            return Failure(GenericError::Create("path must be relative (%s)",
                                                path.ToString().c_str()));
        }

        elements_.insert(elements_.end(),
                         path.elements_.begin(),
                         path.elements_.end());

        return Success(None());
    }

    FilePath FilePath::operator+(const FilePath & other) {
        auto ret = *this;
        ret.Append(other).value();
        return ret;
    }

    Result<None> FilePath::Expand() {
        FilePath ret = *this;
        ret.elements_.clear();

        for (auto & elem : elements_) {
            if (elem == "" || elem == ".") {
                continue;
            }
            if (elem == "..") {
                auto parent = ret.parent();
                if (!parent) {
                    return Failure(GenericError::Create("can not get parent (%s)",
                                                        ToString().c_str()));
                }
                ret = *parent;
                continue;
            }

            ret.Append(FilePath(elem)).value();
        }

        *this = ret;

        return Success(None());
    }

    Result<Ref<Data>> FilePath::Read() const {
        std::string str = ToString();

        int fd = open(str.c_str(), O_RDONLY | O_CLOEXEC);
        if (fd == -1) {
            return Failure(PosixError::Create(errno, "open(%s)", str.c_str()));
        }

        OLIVIEW_DEFER([=] {
            if (close(fd)) {
                Fatal(PosixError::Create(errno, "close(%s)", str.c_str()));
            }
        })

        struct stat st;
        if (fstat(fd, &st)) {
            return Failure(PosixError::Create(errno, "fstat(%s)", str.c_str()));
        }
        int size = (int)st.st_size;

        Ref<Data> data = Create<Data>();
        data->Reserve(size);

        uint8_t chunk[1024];
        while (true) {
            int chunk_size = (int)read(fd, chunk, OLIVIEW_ARRAY_SIZE(chunk));
            if (chunk_size < 0) {
                return Failure(PosixError::Create(errno, "read(%s)", str.c_str()));
            }
            if (chunk_size == 0) {
                break;
            }

            data->Append(Create<Data>(chunk, chunk_size, false, false));
        }

        return Success(data);
    }

    Result<None> FilePath::Write(const Ref<const Data> & data) {
        std::string str = ToString();

        int fd = open(str.c_str(),
                      O_WRONLY | O_TRUNC | O_CREAT | O_CLOEXEC,
                      S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
        if (fd == -1) {
            return Failure(PosixError::Create(errno, "open(%s)", str.c_str()));
        }

        OLIVIEW_DEFER([=] {
            if (close(fd)) {
                Fatal(PosixError::Create(errno, "close(%s)", str.c_str()));
            }
        })

        int total_wrote_size = 0;
        while (true) {
            int write_size = std::min(data->size() - total_wrote_size, 1024);
            if (write_size == 0) {
                break;
            }

            int wrote_size = (int)write(fd,
                                        (const uint8_t *)data->data() + total_wrote_size,
                                        write_size);
            if (wrote_size < 0) {
                return Failure(PosixError::Create(errno, "write(%s)", str.c_str()));
            }
            total_wrote_size += wrote_size;
        }

        return Success(None());
    }

    std::string FilePath::separator() {
#ifdef OLIVIEW_MACOS
        return "/";
#elif defined OLIVIEW_WINDOWS
        return "\\";
#endif
    }

    FilePath FilePath::current() {
#ifdef OLIVIEW_MACOS
        char * cstr = getcwd(nullptr, 0);
        if (!cstr) {
            Fatal(PosixError::Create(errno, "getcwd"));
        }
        std::string str(cstr);
        FilePath ret(str);
        free(cstr);
        return ret;
#else
#   warning TODO
#endif
    }

    FilePath FilePath::home() {
        char * cstr = getenv("HOME");
        if (!cstr) {
            Fatal("HOME is null");
        }
        std::string str(cstr);
        return FilePath(str);
    }

    void FilePath::Parse(const std::string & string) {
        std::vector<std::string> separators = { "/" };
#ifdef OLIVIEW_WINDOWS
        separators.push_back("\\");
#endif

        auto elements = Split(string, separators);

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
