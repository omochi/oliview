#pragma once

#include "./dependency.h"

namespace oliview {
    template <typename C>
    std::vector<typename C::value_type>
    ArrayRemove(const C & array,
                const std::function<bool(const typename C::value_type &)> & pred);
}

#include "./array_function_inline.h"

