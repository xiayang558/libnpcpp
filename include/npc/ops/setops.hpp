#pragma once

/// Set operations and search functions.

#include "npc/array.hpp"

namespace npc {

// ── UniqueResult ─────────────────────────────────────────────

struct UniqueResult {
    Array values;
    Array indices;
    Array inverse;
    Array counts;
};

inline UniqueResult unique(const Array& arr,
                           bool return_index = false,
                           bool return_inverse = false,
                           bool return_counts = false) {
    auto r = ::unique(arr.get(), return_index ? 1 : 0,
                      return_inverse ? 1 : 0, return_counts ? 1 : 0);
    return UniqueResult{Array(r.values), Array(r.indices),
                        Array(r.inverse), Array(r.counts)};
}

// ── Set operations ──────────────────────────────────────────

inline Array intersect1d(const Array& a, const Array& b,
                         bool assume_unique = false) {
    return Array(::intersect1d(a.get(), b.get(), assume_unique ? 1 : 0));
}

inline Array setdiff1d(const Array& a, const Array& b) {
    return Array(::setdiff1d(a.get(), b.get()));
}

// ── Search ──────────────────────────────────────────────────

inline Array searchsorted(const Array& a, const Array& v,
                          const char* side = "left") {
    return Array(::searchsorted(a.get(), v.get(), side));
}

inline Array digitize(const Array& arr, const Array& bins, bool right = false) {
    return Array(::digitize(arr.get(), bins.get(), right ? 1 : 0));
}

inline Array argwhere(const Array& arr) {
    return Array(::argwhere(arr.get()));
}

} // namespace npc
