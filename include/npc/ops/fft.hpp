#pragma once

/// FFT (Fast Fourier Transform) wrappers for libnpc.
/// Supports 1-D, 2-D, N-D transforms, real-valued FFT variants,
/// and frequency utilities.

#include "npc/array.hpp"
#include <array>

namespace npc {

// ── Basic 1-D FFT ───────────────────────────────────────────

inline Array fft(const Array& a)  { return Array(::fft(a.get())); }
inline Array ifft(const Array& a) { return Array(::ifft(a.get())); }

// ── Real FFT (returns only non-negative frequencies) ─────────

inline Array rfft(const Array& a)  { return Array(::rfft(a.get())); }
inline Array rfft2(const Array& a) { return Array(::rfft2(a.get())); }
inline Array rfftn(const Array& a) { return Array(::rfftn(a.get())); }

// ── Hermitian FFT ───────────────────────────────────────────

inline Array hfft(const Array& a, int n)  { return Array(::hfft(a.get(), n)); }
inline Array ihfft(const Array& a, int n) { return Array(::ihfft(a.get(), n)); }

// ── Inverse real FFT ────────────────────────────────────────

inline Array irfft(const Array& a, int n = -1) {
    return Array(::irfft(a.get(), n));
}

inline Array irfft2(const Array& a, std::array<int, 2> s = {}) {
    int sz[2] = {s[0], s[1]};
    return Array(::irfft2(a.get(), sz));
}

inline Array irfftn(const Array& a, std::initializer_list<int> s = {}) {
    std::vector<int> sh(s);
    return Array(::irfftn(a.get(), sh.empty() ? nullptr : sh.data()));
}

// ── 2-D / N-D FFT ───────────────────────────────────────────

inline Array fft2(const Array& a)  { return Array(::fft2(a.get())); }
inline Array ifft2(const Array& a) { return Array(::ifft2(a.get())); }
inline Array fftn(const Array& a)  { return Array(::fftn(a.get())); }
inline Array ifftn(const Array& a) { return Array(::ifftn(a.get())); }

// ── FFT shift (zero frequency to center) ────────────────────

inline Array fftshift(const Array& a)  { return Array(::fftshift(a.get())); }
inline Array ifftshift(const Array& a) { return Array(::ifftshift(a.get())); }

// ── Frequency arrays ────────────────────────────────────────

inline Array fftfreq(int n, double d = 1.0) {
    return Array(::fftfreq(n, d));
}

inline Array rfftfreq(int n, double d = 1.0) {
    return Array(::rfftfreq(n, d));
}

} // namespace npc
