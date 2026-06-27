#pragma once

/// Window function wrappers for libnpc.
/// These generate 1-D window arrays of length M, commonly used in
/// signal processing and spectral analysis.

#include "npc/array.hpp"

namespace npc {

inline Array bartlett(int M) { return Array(::bartlett(M)); }
inline Array blackman(int M) { return Array(::blackman(M)); }
inline Array kaiser(int M, double beta) { return Array(::kaiser(M, beta)); }
inline Array hanning(int M) { return Array(::hanning(M)); }
inline Array hamming(int M) { return Array(::hamming(M)); }

} // namespace npc
