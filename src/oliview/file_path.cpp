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
            
            ret.push_back(*this + FilePath(name));
        }

        return Success(ret);
    }

    FilePath FilePath::basename() const {
        if (elements_.size() == 0) {
            return FilePath();
        }
        return FilePath(elements_.back());
    }

    std::string FilePath::extension() const {
        if (elements_.size() == 0) {
            return "";
        }
        auto strs = SplitR(elements_.back(), std::string("."), { .limit = Some(2) });
        if (strs.size() < 2) {
            return "";
        }
        return std::string(".") + strs[1];
    }

    void FilePath::Append(const FilePath & path) {
        OLIVIEW_ASSERT(path.type() != Type::Absolute);

        elements_.insert(elements_.end(),
                         path.elements_.begin(),
                         path.elements_.end());
    }

    FilePath FilePath::operator+(const FilePath & path) const {
        FilePath ret = *this;
        ret.Append(path);
        return ret;
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
