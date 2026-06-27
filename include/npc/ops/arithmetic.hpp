#pragma once

#include "npc/array.hpp"
#include <type_traits>

namespace npc {

// ── Array + Array (must have identical shapes) ──────────────────

inline Array operator+(const Array& a, const Array& b) {
    return Array(add(a.get(), b.get()));
}

inline Array operator-(const Array& a, const Array& b) {
    return Array(subtract(a.get(), b.get()));
}

inline Array operator*(const Array& a, const Array& b) {
    return Array(multiply(a.get(), b.get()));
}

inline Array operator/(const Array& a, const Array& b) {
    return Array(divide(a.get(), b.get()));
}

// ── Unary negation ────────────────────────────────────────────

inline Array operator-(const Array& a) {
    // -a = a * (-1)
    double neg_one = -1.0;
    return Array(multiply_scalar(a.get(), &neg_one));
}

// ── Array + Scalar ────────────────────────────────────────────
// Uses libnpc's add_scalar / multiply_scalar which broadcast
// the scalar across all array elements.

inline Array operator+(const Array& a, double scalar) {
    return Array(add_scalar(a.get(), &scalar));
}

inline Array operator-(const Array& a, double scalar) {
    // a - s = a + (-s)
    double neg = -scalar;
    return Array(add_scalar(a.get(), &neg));
}

inline Array operator*(const Array& a, double scalar) {
    return Array(multiply_scalar(a.get(), &scalar));
}

inline Array operator/(const Array& a, double scalar) {
    // a / s = a * (1/s)
    double inv = 1.0 / scalar;
    return Array(multiply_scalar(a.get(), &inv));
}

// ── Scalar + Array ────────────────────────────────────────────

inline Array operator+(double scalar, const Array& a) {
    return a + scalar;
}

inline Array operator-(double scalar, const Array& a) {
    // s - a = -(a - s) = -(a + (-s))
    double neg = -scalar;
    Array neg_a = Array(add_scalar(a.get(), &neg));
    return -neg_a;
}

inline Array operator*(double scalar, const Array& a) {
    return a * scalar;
}

inline Array operator/(double scalar, const Array& a) {
    // s / a — create a copy of `a`, then fill with scalar/a[i] values
    auto result = copy(a);
    for (int i = 0; i < result.size(); ++i) {
        result[i] = scalar / result[i];
    }
    return result;
}

// ── Compound assignment ───────────────────────────────────────

inline Array& operator+=(Array& a, const Array& b) { a = a + b; return a; }
inline Array& operator-=(Array& a, const Array& b) { a = a - b; return a; }
inline Array& operator*=(Array& a, const Array& b) { a = a * b; return a; }
inline Array& operator/=(Array& a, const Array& b) { a = a / b; return a; }

inline Array& operator+=(Array& a, double s) { a = a + s; return a; }
inline Array& operator-=(Array& a, double s) { a = a - s; return a; }
inline Array& operator*=(Array& a, double s) { a = a * s; return a; }
inline Array& operator/=(Array& a, double s) { a = a / s; return a; }

} // namespace npc
