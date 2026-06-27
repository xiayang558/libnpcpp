#pragma once

/// Logical and bitwise operations.
/// These return BOOL-type Arrays for logical ops, and integer-type
/// Arrays for bitwise ops.

#include "npc/array.hpp"

namespace npc {

// ── Logical operations ───────────────────────────────────────

inline Array logical_and(const Array& a, const Array& b) {
    return Array(::logical_and(a.get(), b.get()));
}

inline Array logical_or(const Array& a, const Array& b) {
    return Array(::logical_or(a.get(), b.get()));
}

inline Array logical_xor(const Array& a, const Array& b) {
    return Array(::logical_xor(a.get(), b.get()));
}

inline Array logical_not(const Array& a) {
    return Array(::logical_not(a.get()));
}

// ── Bitwise operations ───────────────────────────────────────

inline Array bitwise_and(const Array& a, const Array& b) {
    return Array(::bitwise_and(a.get(), b.get()));
}

inline Array bitwise_or(const Array& a, const Array& b) {
    return Array(::bitwise_or(a.get(), b.get()));
}

inline Array bitwise_xor(const Array& a, const Array& b) {
    return Array(::bitwise_xor(a.get(), b.get()));
}

inline Array bitwise_not(const Array& a) {
    return Array(::bitwise_not(a.get()));
}

inline Array invert(const Array& a) {
    return Array(::invert(a.get()));
}

} // namespace npc
