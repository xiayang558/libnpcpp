#pragma once

/// Interpolation, difference, and gradient functions.

#include "npc/array.hpp"
#include "npc/ops/shape.hpp"

namespace npc {

// ── 1-D linear interpolation ────────────────────────────────

inline Array interp(const Array& arr, const Array& xp, const Array& fp,
                    const Array& left = Array(), const Array& right = Array()) {
    return Array(::interp(arr.get(), xp.get(), fp.get(),
                          left.is_null() ? nullptr : left.get(),
                          right.is_null() ? nullptr : right.get()));
}

// ── Diff: n-th order differences ────────────────────────────

inline Array diff(const Array& arr, int n = 1, int axis = -1) {
    return Array(::diff(arr.get(), n, axis));
}

// ── Differences between adjacent elements ───────────────────

inline Array ediff1d(const Array& y, const Array& to_begin = Array(),
                     const Array& to_end = Array()) {
    return Array(::ediff1d(y.get(),
                           to_begin.is_null() ? nullptr : to_begin.get(),
                           to_end.is_null() ? nullptr : to_end.get()));
}

// ── Gradient ────────────────────────────────────────────────

inline std::vector<Array> gradient(const Array& arr, const Array& spacing = Array()) {
    int num_out = 0;
    ::Array** raw = nullptr;
    int ok = ::gradient(arr.get(),
                        spacing.is_null() ? nullptr : spacing.get(),
                        &raw, &num_out);
    std::vector<Array> results;
    if (ok == 0 && raw) {
        results.reserve(num_out);
        for (int i = 0; i < num_out; ++i) results.emplace_back(raw[i]);
        free(raw);
    }
    return results;
}

} // namespace npc
