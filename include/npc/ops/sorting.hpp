#pragma once

/// Sorting, partitioning, and special functions.

#include "npc/array.hpp"
#include "npc/ops/shape.hpp"
#include <functional>
#include <cmath>

namespace npc {

// ── Sort ────────────────────────────────────────────────────

inline Array sort(const Array& a) {
    return Array(::msort(a.get()));
}

/// Deprecated alias — use sort() instead.
inline Array msort(const Array& a) { return sort(a); }

inline Array argsort(const Array& arr, int axis = -1, const char* kind = "quicksort") {
    return Array(::argsort(arr.get(), axis, kind));
}

// ── Partition ───────────────────────────────────────────────

inline Array argpartition(const Array& arr, int kth, int axis = -1) {
    return Array(::argpartition(arr.get(), kth, axis));
}

inline Array partition(const Array& arr, int kth, int axis = -1) {
    return Array(::partition(arr.get(), kth, axis));
}

// ── Piecewise ───────────────────────────────────────────────

inline Array piecewise(const Array& x,
                       const std::vector<Array>& condlist,
                       const std::vector<double(*)(double)>& funclist) {
    auto c_ptrs = detail::to_c_ptrs(condlist);
    return Array(::piecewise(x.get(), c_ptrs.data(),
                             static_cast<int>(c_ptrs.size()),
                             const_cast<double(**)(double)>(funclist.data())));
}

// ── Unwrap ──────────────────────────────────────────────────

inline Array unwrap(const Array& arr, double discont = M_PI, int axis = -1) {
    return Array(::unwrap(arr.get(), discont, axis));
}

} // namespace npc
