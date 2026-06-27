#pragma once

/// In-place mutation operations on arrays.

#include "npc/array.hpp"

namespace npc {

// ── Place / Put / Putmask ────────────────────────────────────

inline int place(Array& arr, const Array& mask, const Array& vals) {
    return ::place(arr.get(), mask.get(), vals.get());
}

inline int put(Array& arr, const Array& indices, const Array& values,
               const char* mode = "raise") {
    return ::put(arr.get(), indices.get(), values.get(), mode);
}

inline void putmask(Array& arr, const Array& mask, const Array& values) {
    ::putmask(arr.get(), mask.get(), values.get());
}

inline void fill_diagonal(Array& arr, double val) {
    // fill_diagonal takes void* — cast the double
    ::fill_diagonal(arr.get(), &val);
}

} // namespace npc
