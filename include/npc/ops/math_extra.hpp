#pragma once

/// Additional math functions from libnpc's function.h
/// GCD, LCM, bit shifts, bincount, ldexp.

#include "npc/array.hpp"

namespace npc {

// ── GCD / LCM ────────────────────────────────────────────────

inline Array gcd(const Array& x1, const Array& x2) {
    return Array(::gcd_array(x1.get(), x2.get()));
}

inline Array lcm(const Array& x1, const Array& x2) {
    return Array(::lcm(x1.get(), x2.get()));
}

// ── ldexp: x1 * 2^x2 ────────────────────────────────────────

inline Array ldexp(const Array& x1, const Array& x2) {
    return Array(::aldexp(x1.get(), x2.get()));
}

// ── Bit shifts ───────────────────────────────────────────────

inline Array left_shift(const Array& x1, const Array& x2) {
    return Array(::left_shift(x1.get(), x2.get()));
}

inline Array right_shift(const Array& x1, const Array& x2) {
    return Array(::right_shift(x1.get(), x2.get()));
}

// ── Bin count ────────────────────────────────────────────────

inline Array bincount(const Array& a, const Array& weights, int minlength) {
    return Array(::bincount(a.get(), weights.get(), minlength));
}

inline Array bincount(const Array& a, int minlength = 0) {
    return Array(::bincount(a.get(), nullptr, minlength));
}

} // namespace npc
