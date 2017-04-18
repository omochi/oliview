namespace oliview {

    template <typename T>
    Object<T>::Object():
    retain_count_(1)
    {}

    template <typename T>
    Object<T>::~Object()
    {}

    template <typename T>
    int Object<T>::retain_count() const {
        return retain_count_.load();
    }

    template <typename T>
    Ref<const T> Object<T>::this_ref() const {
        return Ref<const T>(static_cast<const T *>(this));
    }

    template <typename T>
    Ref<T> Object<T>::this_ref() {
        return Ref<T>(static_cast<T *>(this));
    }

    template <typename T>
    void Object<T>::Retain() {
        retain_count_++;
    }

    template <typename T>
    void Object<T>::Release() {
        int new_count = --retain_count_;
        if (new_count == 0) {
            delete this;
        }
    }

}
