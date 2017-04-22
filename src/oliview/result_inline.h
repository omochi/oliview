#include "./result.h"

namespace oliview {

    template <typename T>
    Result<T>::Result(const Result<T> & other)
    {
        *this = other;
    }

    template <typename T>
    Result<T> & Result<T>::operator=(const Result<T> & other) {
        value_ = other.value_;
        error_ = other.error_;
        return *this;
    }

    template <typename T>
    Result<T>::Result(const ResultFailureValue & failure) {
        OLIVIEW_ASSERT(failure.error != nullptr);
        error_ = failure.error;
    }

    template <typename T>
    template <typename U>
    Result<T>::Result(const Result<U> & other,
                      typename std::enable_if<std::is_convertible<U, T>::value>::type * enabler)
    {
        value_ = static_cast<Optional<T>>(other.value_);
        error_ = other.error_;
    }

    template <typename T>
    Result<T>::~Result()
    {}

    template <typename T>
    bool Result<T>::is_success() const {
        return value_.present();
    }

    template <typename T>
    const Ref<Error> & Result<T>::error() const {
        OLIVIEW_ASSERT(error_ != nullptr);
        return error_;
    }

    template <typename T>
    const T * Result<T>::operator->() const {
        if (error_) {
            Fatal(error_);
        }
        return value_.operator->();
    }

    template <typename T>
    const T & Result<T>::operator*() const {
        return *operator->();
    }


    template <typename T>
    Result<T>::Result(const Optional<T> & value,
                      const Ref<Error> & error):
    value_(value),
    error_(error)
    {}

    template <typename T>
    Result<T> Success(const T & value) {
        return Result<T>(Some(value), nullptr);
    }

}
