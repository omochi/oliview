#pragma once

#include "./assert.h"
#include "./optional.h"
#include "./error.h"
#include "./ref.h"

#include "./print.h"
#include "./string_format.h"

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
        ~Result();


        bool success() const;
        explicit operator bool() const;

        const T & value() const;
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

    template <typename T>
    ResultFailureValue Failure(const Result<T> & result);
}

#include "./result_inline.h"
