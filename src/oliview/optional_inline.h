
namespace oliview {
    template <typename T>
    Optional<T>::Optional(): value_(nullptr)
    {}

    template <typename T>
    Optional<T>::Optional(const T & value): value_(new T(value))
    {}

    template <typename T>
    Optional<T>::Optional(std::nullptr_t null): value_(nullptr)
    {}

    template <typename T>
    Optional<T>::Optional(const Optional<T> & other)
    {
        if (other) {
            value_ = new T(*other);
        } else {
            value_ = nullptr;
        }
    }

    template <typename T>
    Optional<T> & Optional<T>::operator= (const Optional<T> & other)
    {
        Optional<T> temp;
        Swap(temp);
        if (other) {
            value_ = new T(*other);
        }
        return *this;
    }

    template <typename T>
    Optional<T>::~Optional()
    {
        if (value_) {
            delete value_;
            value_ = nullptr;
        }
    }

    template <typename T>
    void Optional<T>::Swap(Optional<T> & other) {
        std::swap(value_, other.value_);
    }

    template <typename T>
    Optional<T>::operator bool() const {
        return value_ != nullptr;
    }

    template <typename T>
    const T & Optional<T>::operator *() const {
        OLIVIEW_ASSERT(value_ != nullptr);
        return * value_;
    }

    template <typename T>
    const T * Optional<T>::operator ->() const {
        OLIVIEW_ASSERT(value_ != nullptr);
        return value_;
    }

    template <typename T>
    bool Optional<T>::operator == (const Optional<T> & other) const {
        if (*this) {
            if (other) {
                return *this == *other;
            } else {
                return false;
            }
        } else {
            if (other) {
                return false;
            } else {
                return true;
            }
        }
    }

    template <typename T>
    Optional<T> Some(const T & value) {
        return Optional<T>(value);
    }

}
