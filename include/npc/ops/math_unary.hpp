#pragma once

/// Element-wise unary math functions.
/// Each takes an Array and returns a new Array with the operation applied
/// element-by-element.  Broadcasting is automatic.

#include "npc/array.hpp"

namespace npc {

// ── Basic math ───────────────────────────────────────────────

inline Array abs(const Array& a)    { return Array(::abs_array(a.get())); }
inline Array sqrt(const Array& a)   { return Array(::sqrt_array(a.get())); }
inline Array exp(const Array& a)    { return Array(::exp_array(a.get())); }
inline Array log(const Array& a)    { return Array(::log_array(a.get())); }
inline Array log10(const Array& a)  { return Array(::log10_array(a.get())); }
inline Array log2(const Array& a)   { return Array(::log2_array(a.get())); }

// ── Trigonometry ─────────────────────────────────────────────

inline Array sin(const Array& a)    { return Array(::sin_array(a.get())); }
inline Array cos(const Array& a)    { return Array(::cos_array(a.get())); }
inline Array tan(const Array& a)    { return Array(::tan_array(a.get())); }

inline Array sinh(const Array& a)   { return Array(::sinh_array(a.get())); }
inline Array cosh(const Array& a)   { return Array(::cosh_array(a.get())); }
inline Array tanh(const Array& a)   { return Array(::tanh_array(a.get())); }

// ── Inverse trigonometry ─────────────────────────────────────

inline Array arccos(const Array& a)   { return Array(::arccos(a.get())); }
inline Array arccosh(const Array& a)  { return Array(::arccosh(a.get())); }
inline Array arcsin(const Array& a)   { return Array(::arcsin(a.get())); }
inline Array arcsinh(const Array& a)  { return Array(::arcsinh(a.get())); }
inline Array arctan(const Array& a)   { return Array(::arctan(a.get())); }
inline Array arctanh(const Array& a)  { return Array(::arctanh(a.get())); }

// ── Angle conversion ─────────────────────────────────────────

inline Array deg2rad(const Array& a) { return Array(::deg2rad(a.get())); }
inline Array rad2deg(const Array& a) { return Array(::rad2deg(a.get())); }
inline Array degrees(const Array& a) { return Array(::degrees(a.get())); }
inline Array radians(const Array& a) { return Array(::radians(a.get())); }

// ── Rounding ─────────────────────────────────────────────────

inline Array floor(const Array& a) { return Array(::floor_array(a.get())); }
inline Array ceil(const Array& a)  { return Array(::ceil_array(a.get())); }
inline Array rint(const Array& a)  { return Array(::arint(a.get())); }
inline Array trunc(const Array& a) { return Array(::atrunc(a.get())); }
inline Array fix(const Array& a)   { return Array(::fix(a.get())); }

// ── Round to given decimals ──────────────────────────────────

inline Array around(const Array& a, int decimals = 0) {
    return Array(::around(a.get(), decimals));
}

// ── Sign / special ───────────────────────────────────────────

inline Array sign(const Array& a)     { return Array(::sign(a.get())); }
inline Array sinc(const Array& a)     { return Array(::sinc(a.get())); }
inline Array fabs(const Array& a)     { return Array(::fabs_array(a.get())); }
inline Array absolute(const Array& a) { return Array(::absolute(a.get())); }
inline Array reciprocal(const Array& x) { return Array(::reciprocal(x.get())); }
inline Array positive(const Array& x)   { return Array(::positive(x.get())); }
inline Array negative(const Array& x)   { return Array(::negative(x.get())); }
inline Array cbrt(const Array& x)       { return Array(::npc_cbrt(x.get())); }
inline Array square(const Array& x)     { return Array(::square(x.get())); }

// ── Heaviside step function ──────────────────────────────────

inline Array heaviside(const Array& x, double h = 0.5) {
    return Array(::heaviside(x.get(), h));
}

// ── Bessel function ──────────────────────────────────────────

inline Array i0(const Array& a) { return Array(::i0(a.get())); }

// ── Complex number parts ─────────────────────────────────────

inline Array real(const Array& a) { return Array(::real_array(a.get())); }
inline Array imag(const Array& a) { return Array(::imag_array(a.get())); }

// ── Phase angle (complex) ────────────────────────────────────

inline Array angle(const Array& z, bool deg = false) {
    return Array(::angle(z.get(), deg ? 1 : 0));
}

// ── Float spacing ────────────────────────────────────────────

inline Array spacing(const Array& a) { return Array(::spacing(a.get())); }

// ── Utility ──────────────────────────────────────────────────

/// Return the number of dimensions of an array.
inline int ndim(const Array& a) { return ::ndim(a.get()); }

} // namespace npc
