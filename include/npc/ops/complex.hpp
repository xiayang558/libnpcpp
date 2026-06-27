#pragma once

/// Complex number operations.

#include "npc/array.hpp"

namespace npc {

/// Conjugate transpose (Hermitian transpose).
/// For real arrays this is equivalent to a regular transpose.
inline Array conj_transpose(const Array& a) {
    return Array(::conj_transpose(a.get()));
}

} // namespace npc
