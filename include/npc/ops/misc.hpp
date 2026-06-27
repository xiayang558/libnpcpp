#pragma once

/// Miscellaneous remaining libnpc functions.

#include "npc/array.hpp"
#include "npc/ops/shape.hpp"
#include <utility>
#include <functional>

namespace npc {

// ── Array generation ────────────────────────────────────────

inline Array logspace(double start, double stop, int num,
                      double base = 10.0, bool endpoint = true) {
    return Array(::logspace(start, stop, num, base, endpoint ? 1 : 0));
}

inline Array geomspace(double start, double stop, int num, bool endpoint = true) {
    return Array(::geomspace(start, stop, num, endpoint ? 1 : 0));
}

// ── Float decomposition ─────────────────────────────────────

inline std::pair<Array, Array> frexp(const Array& arr) {
    ::Array *mant = nullptr, *exp = nullptr;
    ::frexp_array(arr.get(), &mant, &exp);
    return {Array(mant), Array(exp)};
}

inline std::pair<Array, Array> modf(const Array& arr) {
    ::Array *frac = nullptr, *integ = nullptr;
    ::modf_array(arr.get(), &frac, &integ);
    return {Array(frac), Array(integ)};
}

// ── Trim zeros ──────────────────────────────────────────────

inline Array trim_zeros(const Array& arr, const char* trim = "fb") {
    return Array(::trim_zeros(arr.get(), trim));
}

// ── Signal processing ───────────────────────────────────────

inline Array convolve(const Array& a, const Array& v) {
    return Array(::convolve(a.get(), v.get()));
}

// ── Data I/O utilities ──────────────────────────────────────

inline Array frombuffer(const void* buffer, DType dtype, int count,
                        size_t offset = 0) {
    return Array(::frombuffer(buffer, static_cast<DataType>(dtype), count, offset));
}

inline Array fromfunction(double (*func)(int*, int),
                          std::initializer_list<int> shape, DType dtype = DType::Float64) {
    std::vector<int> sh(shape);
    return Array(::fromfunction(func, sh.data(), static_cast<int>(sh.size()),
                                static_cast<DataType>(dtype)));
}

// ── Array manipulation ──────────────────────────────────────

inline Array insert(const Array& arr, const Array& obj, const Array& values,
                    int axis) {
    return Array(::insert(arr.get(), obj.get(), values.get(), axis));
}

// ── array_delete (C function renamed from `delete` for C++ compat) ─

inline Array array_delete(const Array& arr, const Array& obj, int axis) {
    return Array(::npc_delete(arr.get(), obj.get(), axis));
}

// ── ix_ (open grid indices) ─────────────────────────────────

inline std::vector<Array> ix_(const std::vector<Array>& arrays) {
    auto ptrs = detail::to_c_ptrs(arrays);
    ::Array** out = nullptr;
    ::ix_(ptrs.data(), static_cast<int>(ptrs.size()), &out);
    std::vector<Array> results;
    if (out) {
        results.reserve(ptrs.size());
        for (size_t i = 0; i < ptrs.size(); ++i) results.emplace_back(out[i]);
        free(out);
    }
    return results;
}

// ── Convolve (signal processing) ────────────────────────────

inline Array extract(const Array& condition, const Array& arr) {
    return Array(::extract(condition.get(), arr.get()));
}

// ── Misc utilities ──────────────────────────────────────────

inline Array linspace_float(float start, float stop, int num) {
    return Array(::linspace_float(start, stop, num));
}

} // namespace npc
