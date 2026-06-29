#pragma once

/// Element-wise binary math functions.
/// Each takes two Arrays and returns a new Array with the operation applied
/// element-by-element, with broadcasting.

#include "npc/array.hpp"

namespace npc {

// ── Power / exponentiation ───────────────────────────────────

inline Array power(const Array& a, const Array& b) {
    return Array(::power(a.get(), b.get()));
}

// ── Hypotenuse: sqrt(x^2 + y^2) ──────────────────────────────

inline Array hypot(const Array& x, const Array& y) {
    return Array(::ahypot(x.get(), y.get()));
}

// ── Log-sum-exp (avoids overflow) ────────────────────────────

inline Array logaddexp(const Array& a, const Array& b) {
    return Array(::logaddexp(a.get(), b.get()));
}

inline Array logaddexp2(const Array& a, const Array& b) {
    return Array(::logaddexp2(a.get(), b.get()));
}

// ── Two-argument arctangent ──────────────────────────────────

inline Array arctan2(const Array& y, const Array& x) {
    return Array(::arctan2(y.get(), x.get()));
}

// ── Float manipulation ───────────────────────────────────────

inline Array copysign(const Array& x1, const Array& x2) {
    return Array(::copysign_array(x1.get(), x2.get()));
}

inline Array nextafter(const Array& x1, const Array& x2) {
    return Array(::nextafter_array(x1.get(), x2.get()));
}

// ── Modulo (remainder) ───────────────────────────────────────

inline Array mod(const Array& a, const Array& b) {
    return Array(::mod(a.get(), b.get()));
}

/// Floating-point modulo (like numpy.fmod)
inline Array fmod(const Array& x1, const Array& x2) {
    return Array(::fmod_arr(x1.get(), x2.get()));
}

// ── Element-wise min / max ───────────────────────────────────

inline Array maximum(const Array& x1, const Array& x2) {
    return Array(::maximum(x1.get(), x2.get()));
}

inline Array minimum(const Array& x1, const Array& x2) {
    return Array(::minimum(x1.get(), x2.get()));
}

// ── Clip ─────────────────────────────────────────────────────

inline Array clip(const Array& arr, const Array& lo, const Array& hi) {
    return Array(::clip(arr.get(), lo.get(), hi.get()));
}

} // namespace npc
