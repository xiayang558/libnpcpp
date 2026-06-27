#pragma once

/// Floating-point comparison with tolerance.
/// Returns a BOOL array where elements are true if they differ by at most
/// (atol + rtol * abs(b)).

#include "npc/array.hpp"

namespace npc {

inline Array isclose(const Array& a, const Array& b,
                     double rtol = 1e-5, double atol = 1e-8,
                     bool equal_nan = false) {
    return Array(::isclose(a.get(), b.get(), rtol, atol, equal_nan ? 1 : 0));
}

} // namespace npc
