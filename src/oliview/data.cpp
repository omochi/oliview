#include "./data.h"

namespace oliview {
    Data::Data():
    bytes_(nullptr),
    size_(0),
    capacity_(0),
    free_when_done_(true)
    {}
    
    Data::Data(const void * bytes,
               int size,
               bool copy,
               bool free_when_done)
    {
        if (copy) {
            OLIVIEW_ASSERT(free_when_done);

            if (size == 0) {
                bytes_ = nullptr;
                capacity_ = 0;
                size_ = 0;
            } else {
                bytes_ = malloc(size);
                OLIVIEW_ASSERT(bytes_ != nullptr);
                capacity_ = size;

                memcpy(bytes_, bytes, size);
                size_ = size;
            }
            free_when_done_ = true;
        } else {
            bytes_ = const_cast<void *>(bytes);
            capacity_ = size;
            size_ = size;
            free_when_done_ = free_when_done;
        }
    }

    Data::~Data() {
        if (free_when_done_ && bytes_) {
            free(bytes_);
        }
    }

    const void * Data::bytes() const {
        return bytes_;
    }
    
    void * Data::bytes() {
        return bytes_;
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

        memcpy((uint8_t *)bytes_ + size_, data->bytes_, data->size_);
        size_ += data->size_;
    }

    void Data::Reserve(int capacity) {
        OLIVIEW_ASSERT(capacity > 0);

        if (free_when_done_) {
            bytes_ = realloc(bytes_, capacity);
            OLIVIEW_ASSERT(bytes_ != nullptr);
            capacity_ = capacity;

            size_ = std::min(size_, capacity);
            free_when_done_ = true;
        } else {
            void * new_data = malloc(capacity);
            OLIVIEW_ASSERT(new_data != nullptr);

            int new_size = std::min(size_, capacity);
            memcpy(new_data, bytes_, new_size);

            bytes_ = new_data;
            capacity_ = capacity;
            size_ = new_size;
            free_when_done_ = true;
        }
    }
}
