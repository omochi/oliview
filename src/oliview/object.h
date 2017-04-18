#pragma once

#include "./dependency.h"

namespace oliview {
    template <typename T> class Ref;

    template <typename T>
    class Object {
    public:
        Object();
        virtual ~Object();

        int retain_count() const;

        Ref<const T> this_ref() const;
        Ref<T> this_ref();

        void Retain();
        void Release();
    private:
        std::atomic<int> retain_count_;
    };
}

#include "./object_inline.h"


