#include "./data.h"

namespace oliview {
    Data::Data():
    data_(nullptr),
    size_(0),
    capacity_(0),
    free_when_done_(true)
    {}
    
    Data::Data(const void * data,
               int size,
               bool copy,
               bool free_when_done)
    {
        if (size == 0) {
            data_ = nullptr;
            size_ = 0;
            capacity_ = 0;
            free_when_done_ = true;
        } else if (copy) {
            OLIVIEW_ASSERT(free_when_done);

            CopyData(data, size);
        } else {
            data_ = const_cast<void *>(data);
            capacity_ = size;
            free_when_done_ = free_when_done;
        }
    }

    Data::~Data() {
        Clear();
    }

    const void * Data::data() const {
        return data_;
    }
    
    void * Data::data() {
        return data_;
    }

    int Data::size() const {
        return size_;
    }

    void Data::Append(const Ref<const Data> & data) {
        if (data->size_ == 0) {
            return;
        }

        int new_size = size_ + data->size_;
        if (new_size > capacity_) {
            if (new_size <= capacity_ * 2) {
                Reserve(capacity_ * 2);
            } else {
                Reserve(new_size);
            }
        }

        memcpy((uint8_t *)data_ + size_, data->data_, data->size_);
        size_ += data->size_;
    }

    void Data::Reserve(int capacity) {
        OLIVIEW_ASSERT(capacity > 0);

        void * new_data = realloc(data_, capacity);
        int size = std::min(size_, capacity);

        Clear();

        data_ = new_data;
        size_ = size;
        capacity_ = capacity;
        free_when_done_ = true;
    }

    void Data::CopyData(const void * data, int size) {
        data_ = malloc(size);
        memcpy(data_, data, size);
        size_ = size;
        capacity_ = size;
        free_when_done_ = true;
    }

    void Data::Clear() {
        if (free_when_done_ && data_) {
            free(data_);
        }

        data_ = nullptr;
        size_ = 0;
        capacity_ = 0;
        free_when_done_ = true;
    }
}
