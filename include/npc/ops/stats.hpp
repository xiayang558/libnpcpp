#pragma once

/// Aggregation and statistics functions.
/// Most functions accept optional `axis` (-1 = all elements) and `keepdims`
/// (0 = squeeze reduced dimensions) parameters, matching NumPy conventions.

#include "npc/array.hpp"

namespace npc {

// ── Basic aggregations ───────────────────────────────────────

inline Array sum(const Array& a, int axis = -1, bool keepdims = false) {
    return Array(::sum(a.get(), axis, keepdims ? 1 : 0));
}

inline Array mean(const Array& a, int axis = -1, bool keepdims = false) {
    return Array(::mean(a.get(), axis, keepdims ? 1 : 0));
}

inline Array min(const Array& a, int axis = -1, bool keepdims = false) {
    return Array(::min(a.get(), axis, keepdims ? 1 : 0));
}

inline Array max(const Array& a, int axis = -1, bool keepdims = false) {
    return Array(::max(a.get(), axis, keepdims ? 1 : 0));
}

inline Array prod(const Array& a, int axis = -1, bool keepdims = false) {
    return Array(::prod(a.get(), axis, keepdims ? 1 : 0));
}

// ── Standard deviation (C function was renamed to npc_std) ───

inline Array stddev(const Array& a, int axis = -1, bool keepdims = false) {
    return Array(::npc_std(a.get(), axis, keepdims ? 1 : 0));
}

// ── Variance (with ddof) ─────────────────────────────────────

inline Array var(const Array& a, int axis = -1, int ddof = 0, bool keepdims = false) {
    return Array(::var(a.get(), axis, ddof, keepdims ? 1 : 0));
}

// ── Cumulative operations ────────────────────────────────────

inline Array cumsum(const Array& a, int axis = -1) {
    return Array(::cumsum(a.get(), axis));
}

inline Array cumprod(const Array& a, int axis = -1) {
    return Array(::cumprod(a.get(), axis));
}

// ── Median / percentile / quantile ───────────────────────────

inline Array median(const Array& a, int axis = -1, bool keepdims = false) {
    return Array(::median(a.get(), axis, keepdims ? 1 : 0));
}

inline Array percentile(const Array& a, const Array& q, int axis = -1) {
    return Array(::percentile(a.get(), q.get(), axis));
}

inline Array quantile(const Array& a, const Array& q, int axis = -1) {
    return Array(::quantile(a.get(), q.get(), axis));
}

// ── NaN-ignoring aggregations ────────────────────────────────

inline Array nanmax(const Array& a, int axis = -1) {
    return Array(::nanmax(a.get(), axis));
}

inline Array nanmin(const Array& a, int axis = -1) {
    return Array(::nanmin(a.get(), axis));
}

inline Array nanmean(const Array& a, int axis = -1) {
    return Array(::nanmean(a.get(), axis));
}

inline Array nanmedian(const Array& a, int axis = -1) {
    return Array(::nanmedian(a.get(), axis));
}

inline Array nanpercentile(const Array& a, const Array& q, int axis = -1) {
    return Array(::nanpercentile(a.get(), q.get(), axis));
}

inline Array nanquantile(const Array& a, const Array& q, int axis = -1) {
    return Array(::nanquantile(a.get(), q.get(), axis));
}

inline Array nancumsum(const Array& a, int axis = -1) {
    return Array(::nancumsum(a.get(), axis));
}

inline Array nancumprod(const Array& a, int axis = -1) {
    return Array(::nancumprod(a.get(), axis));
}

// ── Argmin / argmax ──────────────────────────────────────────

inline Array argmax(const Array& a, int axis = -1, bool keepdims = false) {
    return Array(::argmax(a.get(), axis, keepdims ? 1 : 0));
}

inline Array argmin(const Array& a, int axis = -1, bool keepdims = false) {
    return Array(::argmin(a.get(), axis, keepdims ? 1 : 0));
}

// ── Count non-zero ───────────────────────────────────────────

inline Array count_nonzero(const Array& a, int axis = -1) {
    return Array(::count_nonzero(a.get(), axis));
}

// ── Peak-to-peak (max - min) ─────────────────────────────────

inline Array ptp(const Array& a, int axis = -1, bool keepdims = false) {
    return Array(::ptp(a.get(), axis, keepdims ? 1 : 0));
}

// ── Trapezoidal integration ──────────────────────────────────

inline Array trapz(const Array& y, double dx = 1.0, int axis = -1) {
    return Array(::trapz(y.get(), nullptr, dx, axis));
}

inline Array trapz(const Array& y, const Array& x,
                   double dx = 1.0, int axis = -1) {
    return Array(::trapz(y.get(), x.get(), dx, axis));
}

// ── Weighted average ─────────────────────────────────────────

inline Array average(const Array& a, int axis = -1, bool keepdims = false) {
    return Array(::average(a.get(), nullptr, axis, keepdims ? 1 : 0));
}

inline Array average(const Array& a, const Array& weights,
                     int axis = -1, bool keepdims = false) {
    return Array(::average(a.get(), weights.get(), axis, keepdims ? 1 : 0));
}

// ── Covariance / correlation ─────────────────────────────────

inline Array cov(const Array& m, bool rowvar = true, int ddof = 0,
                 DType dtype = DType::Float64) {
    return Array(::cov(m.get(), rowvar ? 1 : 0, ddof,
                       static_cast<DataType>(dtype)));
}

inline Array corrcoef(const Array& m, bool rowvar = true) {
    return Array(::corrcoef(m.get(), rowvar ? 1 : 0));
}

inline Array correlate(const Array& a, const Array& v,
                       const char* mode = "valid") {
    return Array(::correlate(a.get(), v.get(), mode));
}

// ── All / any ────────────────────────────────────────────────

inline Array all(const Array& a, int axis = -1, bool keepdims = false) {
    return Array(::all(a.get(), axis, keepdims ? 1 : 0));
}

inline Array any(const Array& a, int axis = -1, bool keepdims = false) {
    return Array(::any(a.get(), axis, keepdims ? 1 : 0));
}

// ── Conditional selection ────────────────────────────────────

inline Array where(const Array& condition, const Array& x, const Array& y) {
    return Array(::where(condition.get(), x.get(), y.get()));
}

} // namespace npc
