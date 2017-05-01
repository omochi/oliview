#pragma once

#include "./dependency.h"

namespace oliview {
    template <typename T> class Ref {
    public:
        Ref();
        explicit Ref(T * ptr);
        Ref(std::nullptr_t null);
        Ref(const Ref<T> & other);
        Ref<T> & operator=(const Ref<T> & other);
        template <typename U>
        Ref(const Ref<U> & other,
            typename std::enable_if<std::is_convertible<U*, T*>::value>::type * enabler = nullptr);
        ~Ref();

        T * get() const;

        void Swap(Ref<T> & other);

        void Attach(T * ptr);
        T * Dettach();

        explicit operator bool() const;

        T * operator->() const;
        T & operator*() const;

        bool operator==(const Ref<T> & other) const;
        bool operator!=(const Ref<T> & other) const;
    private:
        std::atomic<T *> ptr_;
    };

    template <typename T, typename... Args>
    Ref<T> Create(Args... args);
}

#include "./ref_inline.h"