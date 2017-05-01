#pragma once

#include "./object.h"
#include "./assert.h"

namespace oliview {
    class Data : public Object<Data> {
    public:
        Data();
        Data(const void * bytes,
             int size,
             bool copy = true,
             bool free_when_done = true);
        virtual ~Data();

        const void * bytes() const;
        void * bytes();

        int size() const;

        void Append(const Ref<const Data> & data);
        void Reserve(int capacity);
    private:
        void * bytes_;
        int size_;
        int capacity_;
        bool free_when_done_;
    };
}
