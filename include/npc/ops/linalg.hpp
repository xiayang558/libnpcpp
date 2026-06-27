#pragma once

/// Linear algebra wrappers for libnpc.
///
/// Includes matrix multiplication, decompositions (SVD, QR, LU, Cholesky, Eig),
/// inverses, determinant, trace, and various products.

#include "npc/array.hpp"

namespace npc {

// ── In-place row/column swaps (from linalg.h) ────────────────

inline void swap_rows(Array& arr, int row1, int row2) {
    ::swap_rows(arr.get(), row1, row2);
}

inline void swap_cols(Array& arr, int col1, int col2) {
    ::swap_cols(arr.get(), col1, col2);
}

// ── C++ result structs for decomposition functions ────────────

struct SVDResult {
    Array U;
    Array s;
    Array Vt;
};

struct QRResult {
    Array Q;
    Array R;
};

struct LUResult {
    Array P;
    Array L;
    Array U;
};

struct EigResult {
    Array eigenvalues;
    Array eigenvectors;
};

// ── Matrix multiplication ────────────────────────────────────

inline Array dot(const Array& a, const Array& b) {
    return Array(::dot(a.get(), b.get()));
}

inline Array matmul(const Array& a, const Array& b) {
    return Array(::matmul(a.get(), b.get()));
}

inline Array vdot(const Array& a, const Array& b) {
    return Array(::vdot(a.get(), b.get()));
}

inline Array inner(const Array& a, const Array& b) {
    return Array(::inner(a.get(), b.get()));
}

inline Array outer(const Array& a, const Array& b) {
    return Array(::outer(a.get(), b.get()));
}

inline Array cross(const Array& a, const Array& b) {
    return Array(::cross(a.get(), b.get()));
}

inline Array kron(const Array& a, const Array& b) {
    return Array(::kron(a.get(), b.get()));
}

// ── Tensor dot product ───────────────────────────────────────

inline Array tensordot(const Array& a, const Array& b,
                       const Array& axes_a, const Array& axes_b) {
    return Array(::tensordot(a.get(), b.get(), axes_a.get(), axes_b.get()));
}

inline Array tensordot(const Array& a, const Array& b, int axes) {
    return Array(::tensordot_scalar(a.get(), b.get(), axes));
}

// ── Matrix properties ────────────────────────────────────────

inline Array inv(const Array& a) {
    return Array(::inv(a.get()));
}

inline Array pinv(const Array& a, double rcond = 1e-15) {
    return Array(::pinv(a.get(), rcond));
}

inline Array det(const Array& a) {
    return Array(::det(a.get()));
}

inline Array trace(const Array& a) {
    return Array(::trace(a.get()));
}

// ── Decompositions ───────────────────────────────────────────

inline Array cholesky(const Array& a) {
    return Array(::cholesky(a.get()));
}

inline SVDResult svd(const Array& a, bool full_matrices = true) {
    auto r = ::svd(a.get(), full_matrices ? 1 : 0);
    return SVDResult{Array(r.U), Array(r.s), Array(r.Vt)};
}

inline QRResult qr(const Array& a) {
    auto r = ::qr(a.get());
    return QRResult{Array(r.Q), Array(r.R)};
}

inline LUResult lu(const Array& a) {
    auto r = ::lu(a.get());
    return LUResult{Array(r.P), Array(r.L), Array(r.U)};
}

inline EigResult eig(const Array& a) {
    auto r = ::eig(a.get());
    return EigResult{Array(r.eigenvalues), Array(r.eigenvectors)};
}

} // namespace npc
