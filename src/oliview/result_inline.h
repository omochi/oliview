#include "./result.h"

namespace oliview {

    template <typename T>
    Result<T>::Result(const Result<T> & other)
    {
        Print(Format("Result Copy Ctor: %p <= %p", this, &other));
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
        Print(Format("Result Failure Ctor: %p", this));

        OLIVIEW_ASSERT(failure.error != nullptr);
        error_ = failure.error;
    }

    template <typename T>
    template <typename U>
    Result<T>::Result(const Result<U> & other,
                      typename std::enable_if<std::is_convertible<U, T>::value>::type * enabler)
    {
        Print(Format("Result Cast Copy Ctor: %p <= %p", this, &other));
        value_ = static_cast<Optional<T>>(other.value_);
        error_ = other.error_;
    }

    template <typename T>
    Result<T>::~Result() {
        Print(Format("Result Dtor: %p", this));
    }

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
        Print(Format("operator* %d", (int)(*operator->()).size()));
        return *operator->();
    }


    template <typename T>
    Result<T>::Result(const Optional<T> & value,
                      const Ref<Error> & error):
    value_(value),
    error_(error)
    {
        Print(Format("Result Double Ctor: %p", this));
        Print(Format("size: %d", (int)value->size()));


    }

    template <typename T>
    Result<T> Success(const T & value) {
        return Result<T>(Some(value), nullptr);
    }

}
