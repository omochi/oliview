#pragma once

#include "./dependency.h"
#include "./ref.h"

namespace oliview {
    template <typename T>
    class Object {
    public:
        Object();
        Object(const Object<T> & other) = delete;
        Object & operator=(const Object<T> & other) = delete;
        virtual ~Object();

        int retain_count() const;

        Ref<const T> this_ref() const;
        Ref<T> this_ref();

        void Retain() const;
        void Release() const;
    private:
        mutable std::atomic<int> retain_count_;
    };
}

#include "./object_inline.h"


