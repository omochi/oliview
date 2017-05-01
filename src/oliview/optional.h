#pragma once

#include "./dependency.h"
#include "./assert.h"

namespace oliview {
    template <typename T>
    class Optional {
    public:
        Optional();
        explicit Optional(const T & value);
        Optional(std::nullptr_t null);
        Optional(const Optional<T> & other);
        Optional<T> & operator=(const Optional<T> & other);
        template <typename U>
        Optional(const Optional<U> & other,
                 typename std::enable_if<std::is_convertible<U, T>::value>::type * enabler = nullptr);

        ~Optional();

        void Swap(Optional<T> & other);

        bool present() const;
        explicit operator bool() const;

        const T & value() const;

        const T * operator->() const;
        const T & operator*() const;

        bool operator==(const Optional<T> & other) const;
        bool operator!=(const Optional<T> & other) const;
    private:
        T * value_;
    };

    template <typename T>
    Optional<T> Some(const T & value);
}

#include "./optional_inline.h"
