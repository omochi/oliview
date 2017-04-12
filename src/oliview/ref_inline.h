namespace oliview {
    template <typename T>
    Ref<T>::Ref():
    ptr_(nullptr)
    {}

    template <typename T>
    Ref<T>::Ref(T * ptr):
    ptr_(ptr)
    {
        if (ptr) {
            ptr->Retain();
        }
    }

    template <typename T>
    Ref<T>::Ref(std::nullptr_t null): Ref()
    {}

    template <typename T>
    Ref<T>::Ref(const Ref<T> & other):
    Ref(other.ptr_.load())
    {}

    template <typename T>
    Ref<T> & Ref<T>::operator= (const Ref<T> & other)
    {
        Ref<T> swap(other);
        Swap(swap);
        return *this;
    }

    template <typename T>
    Ref<T>::~Ref()
    {
        T * ptr = ptr_.load();
        if (ptr) {
            ptr->Release();
        }
    }

    template <typename T>
    T * Ref<T>::get() const {
        return ptr_.load();
    }

    template <typename T>
    void Ref<T>::Swap(Ref<T> & other) {
        T * temp;
        temp = ptr_.exchange(nullptr);
        temp = other.ptr_.exchange(temp);
        ptr_.exchange(temp);
    }

    template <typename T>
    void Ref<T>::Attach(T * ptr) {
        Ref<T> other;
        other.ptr_.store(ptr);
        Swap(other);
    }

    template <typename T>
    T * Ref<T>::Dettach() {
        Ref<T> other;
        Swap(other);
        return other.ptr_.load();
    }

    template <typename T>
    Ref<T>::operator bool() const {
        return ptr_ != nullptr;
    }

    template <typename T>
    T * Ref<T>::operator-> () const {
        return ptr_.load();
    }

    template <typename T>
    T & Ref<T>::operator* () const {
        return * ptr_.load();
    }

    template <typename T>
    bool Ref<T>::operator== (const Ref<T> & other) const {
        return ptr_.load() == other.ptr_.load();
    }

    template <typename T>
    bool Ref<T>::operator!= (const Ref<T> & other) const {
        return !(*this == other);
    }
}
