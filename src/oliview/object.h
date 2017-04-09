#pragma once

#include "./dependency.h"
#include "./ref.h"

namespace oliview {
    template <typename T> class Object {
    public:
        Object();
        virtual ~Object();

        int retain_count() const;
        Ref<T> this_ref();
        Ref<const T> this_ref() const;

        void Retain();
        void Release();
    private:
        std::atomic<int> retain_count_;
    };
}

#include "./object_inline.h"
