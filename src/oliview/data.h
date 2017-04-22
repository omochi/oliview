#pragma once

#include "./object.h"
#include "./assert.h"

namespace oliview {
    class Data : public Object<Data> {
    public:
        Data();
        Data(const void * data,
             int size,
             bool copy = true,
             bool free_when_done = true);
        virtual ~Data();

        const void * data() const;
        void * data();

        int size() const;

        void Append(const Ref<const Data> & data);
        void Reserve(int capacity);
    private:
        void CopyData(const void * data, int size);
        void Clear();

        void * data_;
        int size_;
        int capacity_;
        bool free_when_done_;
    };
}
