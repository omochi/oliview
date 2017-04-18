#pragma once

#include "./dependency.h"
#include "./optional.h"

namespace oliview {
    template <typename C>
    Optional<int>
    ArrayFind(const C & array,
              const std::function<bool(const typename C::value_type &)> & pred);

    template <typename C>
    Optional<int>
    ArrayFindR(const C & array,
               const std::function<bool(const typename C::value_type &)> & pred);

    template <typename C>
    void
    ArrayRemove(C & array,
                const std::function<bool(const typename C::value_type &)> & pred);

    template <typename C>
    void
    ArrayRemoveAt(C & array, int index);
}

#include "./array_function_inline.h"

