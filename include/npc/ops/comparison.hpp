#pragma once

#include "npc/array.hpp"

namespace npc {

/// Named comparison functions (NumPy-style).
/// These return bool Arrays with element-wise comparison results.
/// C++ operator== is intentionally NOT overloaded for Array
/// because returning Array instead of bool would violate C++ conventions.

inline Array equal(const Array& a, const Array& b) {
    return Array(::equal(a.get(), b.get()));
}

inline Array not_equal(const Array& a, const Array& b) {
    return Array(logical_not(::equal(a.get(), b.get())));
}

inline Array greater(const Array& a, const Array& b) {
    return Array(::greater(a.get(), b.get()));
}

inline Array greater_equal(const Array& a, const Array& b) {
    return Array(::greater_equal(a.get(), b.get()));
}

inline Array less(const Array& a, const Array& b) {
    // Use libnpc's built-in less when available;
    // otherwise: a < b  ≡  b > a
    return Array(::greater(b.get(), a.get()));
}

inline Array less_equal(const Array& a, const Array& b) {
    // a <= b  ≡  b >= a
    return Array(::greater_equal(b.get(), a.get()));
}

} // namespace npc
