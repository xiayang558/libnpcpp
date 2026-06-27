#pragma once

/// libnpc++ — C++ wrapper for libnpc (NumPy C Library)
///
/// Include this single header to get all libnpc++ functionality:
///
/// ```cpp
/// #include <npc/npc.hpp>
///
/// int main() {
///     auto a = npc::ones({3, 3});
///     auto b = npc::full({3, 3}, 2.0);
///     auto c = a + b;
///     std::cout << c << std::endl;
///     for (const double* p = c.begin<double>(); p != c.end<double>(); ++p) printf("%f ", *p);
///     return 0;
/// }
/// ```

// Core types
#include "dtype.hpp"
#include "array.hpp"

// Shared utilities
#include "detail/shape_util.hpp"

// Factory functions
#include "creation.hpp"

// Operations — arithmetic
#include "ops/arithmetic.hpp"

// Operations — comparison
#include "ops/comparison.hpp"

// Operations — math (unary, binary, extra)
#include "ops/math_unary.hpp"
#include "ops/math_binary.hpp"
#include "ops/math_extra.hpp"

// Operations — logical & bitwise
#include "ops/logical.hpp"

// Operations — floating-point comparison
#include "ops/isclose.hpp"

// Operations — complex numbers
#include "ops/complex.hpp"

// Operations — statistics
#include "ops/stats.hpp"

// Operations — linear algebra
#include "ops/linalg.hpp"

// Operations — shape manipulation
#include "ops/shape.hpp"

// Operations — FFT
#include "ops/fft.hpp"

// Operations — random number generation
#include "ops/random.hpp"

// Operations — file I/O
#include "ops/io.hpp"

// Operations — window functions
#include "ops/window.hpp"

// Operations — polynomial
#include "ops/polynomial.hpp"

// Operations — set & search
#include "ops/setops.hpp"

// Operations — interpolation & diff
#include "ops/interp.hpp"

// Operations — sorting & special
#include "ops/sorting.hpp"

// Operations — in-place mutation
#include "ops/mutation.hpp"

// Operations — NaN utilities
#include "ops/nan_utils.hpp"

// Operations — histograms
#include "ops/histogram.hpp"

// Operations — einsum
#include "ops/einsum.hpp"

// Operations — datetime64
#include "ops/datetime64.hpp"

// Operations — miscellaneous
#include "ops/misc.hpp"
