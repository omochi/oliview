#pragma once

#include "./assert.h"
#include "./optional.h"
#include "./error.h"
#include "./ref.h"

namespace oliview {
    struct ResultFailureValue {
        Ref<Error> error;
    };

    template <typename T>
    class Result {
    public:
        Result(const Result<T> & other);
        Result<T> & operator=(const Result<T> & other);
        Result(const ResultFailureValue & failure);
        template <typename U>
        Result(const Result<U> & other,
               typename std::enable_if<std::is_convertible<U, T>::value>::type * enabler = nullptr);
        
        bool is_success() const;

        const Ref<Error> & error() const;

        const T * operator->() const;
        const T & operator*() const;

        template <typename U>
        friend Result<U> Success(const U & value);
    private:
        Result(const Optional<T> & value,
               const Ref<Error> & error);

        Optional<T> value_;
        Ref<Error> error_;
    };

    template <typename T>
    Result<T> Success(const T & value);

    ResultFailureValue Failure(const Ref<Error> & error);
}

#include "./result_inline.h"
