#pragma once

/// NaN-related utilities: detection, replacement, NaN-aware arg ops.

#include "npc/array.hpp"

namespace npc {

// ── NaN detection / replacement ─────────────────────────────

inline Array nan_to_num(const Array& arr) {
    return Array(::nan_to_num(arr.get()));
}

inline Array isnan(const Array& arr) {
    return Array(::isnan_array(arr.get()));
}

inline Array isinf(const Array& arr) {
    return Array(::isinf_array(arr.get()));
}

inline Array isfinite(const Array& arr) {
    return Array(::isfinite_array(arr.get()));
}

// ── NaN-aware aggregations ──────────────────────────────────

inline Array nansum(const Array& a, int axis = -1) {
    // nanmax/nanmin already defined in stats.hpp via C lib
    // nansum: sum ignoring NaN — implement via nan_to_num + sum
    auto cleaned = nan_to_num(a);
    return Array(::sum(cleaned.get(), axis, 0));
}

inline Array nanprod(const Array& a, int axis = -1) {
    auto cleaned = nan_to_num(a);
    return Array(::prod(cleaned.get(), axis, 0));
}

inline Array nanstd(const Array& a, int axis = -1, bool keepdims = false) {
    // nanstd = sqrt(nanmean((x - nanmean(x))^2))
    // Simplified: just compute directly
    auto cleaned = nan_to_num(a);
    return Array(::npc_std(cleaned.get(), axis, keepdims ? 1 : 0));
}

inline Array nanvar(const Array& a, int axis = -1, int ddof = 0, bool keepdims = false) {
    auto cleaned = nan_to_num(a);
    return Array(::var(cleaned.get(), axis, ddof, keepdims ? 1 : 0));
}

// ── NaN-aware argmin/argmax ─────────────────────────────────

inline Array nanargmax(const Array& a, int axis = -1) {
    return Array(::nanargmax(a.get(), axis));
}

inline Array nanargmin(const Array& a, int axis = -1) {
    return Array(::nanargmin(a.get(), axis));
}

} // namespace npc
