#pragma once

/// Shape manipulation wrappers for libnpc.
///
/// Includes reshape, transpose, flatten, stack, split, flip, roll, tile, etc.

#include "npc/array.hpp"
#include <vector>

namespace npc {

namespace detail {
    // Build a raw C pointer array from a vector of npc::Array references.
    inline std::vector<::Array*> to_c_ptrs(const std::vector<Array>& vec) {
        std::vector<::Array*> out;
        out.reserve(vec.size());
        for (auto& a : vec) out.push_back(a.get());
        return out;
    }
}

// ── Reshape / flatten ────────────────────────────────────────

inline Array reshape(const Array& a, std::initializer_list<int> new_shape) {
    std::vector<int> sh(new_shape);
    return Array(::reshape(a.get(), sh.data(), static_cast<int>(sh.size())));
}

inline Array resize(const Array& a, std::initializer_list<int> new_shape) {
    std::vector<int> sh(new_shape);
    return Array(::resize(a.get(), sh.data(), static_cast<int>(sh.size())));
}

inline Array transpose(const Array& a) {
    return Array(::transpose(a.get()));
}

inline Array flatten(const Array& a) {
    return Array(::flatten(a.get()));
}

inline Array ravel(const Array& a, const char* order = "C") {
    return Array(::ravel(a.get(), order));
}

inline Array squeeze(const Array& a, int axis = -1) {
    return Array(::squeeze(a.get(), axis));
}

inline Array expand_dims(const Array& a, int axis) {
    return Array(::expand_dims(a.get(), axis));
}

// ── Repeat / tile ────────────────────────────────────────────

inline Array tile(const Array& a, std::initializer_list<int> reps) {
    std::vector<int> r(reps);
    return Array(::tile(a.get(), r.data(), static_cast<int>(r.size())));
}

inline Array repeat(const Array& a, const Array& repeats, int axis) {
    return Array(::repeat(a.get(), repeats.get(), axis));
}

// ── Roll / flip ──────────────────────────────────────────────

inline Array roll(const Array& a, int shift, int axis = -1) {
    return Array(::roll(a.get(), shift, axis));
}

inline Array flip(const Array& a, int axis = -1) {
    return Array(::flip(a.get(), axis));
}

inline Array fliplr(const Array& a) {
    return Array(::fliplr(a.get()));
}

inline Array flipud(const Array& a) {
    return Array(::flipud(a.get()));
}

inline Array rot90(const Array& m, int k = 1, int axis1 = 0, int axis2 = 1) {
    return Array(::rot90(m.get(), k, axis1, axis2));
}

// ── Axis movement ────────────────────────────────────────────

inline Array moveaxis(const Array& a, int source, int destination) {
    return Array(::moveaxis(a.get(), source, destination));
}

inline Array rollaxis(const Array& a, int axis, int start) {
    return Array(::rollaxis(a.get(), axis, start));
}

/// Swap two axes (convenience), like numpy.swapaxes.
inline Array swapaxes(const Array& a, int axis1, int axis2) {
    // swapaxes(a, 0, 1) = moveaxis(moveaxis(a, 0, 1), 1, 0)... no.
    // Use transpose-like logic. For simplicity, implement via moveaxis:
    // moveaxis(a, axis1, axis2) then moveaxis(..., axis2 adjusted, axis1)
    if (axis1 == axis2) return Array(copy_array(a.get()));
    auto tmp = moveaxis(a, axis1, axis2);
    int adjusted = (axis1 < axis2) ? axis1 : axis1 - 1;
    return moveaxis(tmp, axis2 + (axis2 < axis1 ? 0 : -1), axis1);
}

// ── atleast helpers ──────────────────────────────────────────

inline Array atleast_1d(const Array& a) {
    return Array(::atleast_1d(a.get()));
}

inline Array atleast_2d(const Array& a) {
    return Array(::atleast_2d(a.get()));
}

inline Array atleast_3d(const Array& a) {
    return Array(::atleast_3d(a.get()));
}

// ── Broadcast ────────────────────────────────────────────────

inline Array broadcast_to(const Array& a, std::initializer_list<int> shape) {
    std::vector<int> sh(shape);
    return Array(::broadcast_to(a.get(), sh.data(), static_cast<int>(sh.size())));
}

inline std::vector<Array> broadcast_arrays(const std::vector<Array>& arrays) {
    auto c_ptrs = detail::to_c_ptrs(arrays);
    int out_ndim = 0;
    int* out_shape = nullptr;
    ::Array** raw = ::broadcast_arrays(c_ptrs.data(), static_cast<int>(c_ptrs.size()),
                                       &out_ndim, &out_shape);
    std::vector<Array> results;
    if (raw) {
        results.reserve(c_ptrs.size());
        for (size_t i = 0; i < c_ptrs.size(); ++i) results.emplace_back(raw[i]);
        free(raw);
    }
    if (out_shape) free(out_shape);
    return results;
}

// ── Concat / append ──────────────────────────────────────────

inline Array concatenate(const Array& a, const Array& b, int axis = 0) {
    return Array(::concatenate(a.get(), b.get(), axis));
}

inline Array append(const Array& a, const Array& values, int axis) {
    return Array(::append(a.get(), values.get(), axis));
}

// ── Stack / hstack / vstack / dstack ─────────────────────────

inline Array stack(const std::vector<Array>& arrays, int axis = 0) {
    auto ptrs = detail::to_c_ptrs(arrays);
    return Array(::stack(ptrs.data(), static_cast<int>(ptrs.size()), axis));
}

inline Array hstack(const std::vector<Array>& arrays) {
    auto ptrs = detail::to_c_ptrs(arrays);
    return Array(::hstack(ptrs.data(), static_cast<int>(ptrs.size())));
}

inline Array vstack(const std::vector<Array>& arrays) {
    auto ptrs = detail::to_c_ptrs(arrays);
    return Array(::vstack(ptrs.data(), static_cast<int>(ptrs.size())));
}

inline Array dstack(const std::vector<Array>& arrays) {
    auto ptrs = detail::to_c_ptrs(arrays);
    return Array(::dstack(ptrs.data(), static_cast<int>(ptrs.size())));
}

inline Array column_stack(const std::vector<Array>& arrays) {
    auto ptrs = detail::to_c_ptrs(arrays);
    return Array(::column_stack(ptrs.data(), static_cast<int>(ptrs.size())));
}

// ── Split ────────────────────────────────────────────────────

inline std::vector<Array> split(const Array& ary,
                                const Array& indices_or_sections,
                                int axis = 0) {
    int num = 0;
    ::Array** raw = ::split(ary.get(), indices_or_sections.get(), axis, &num);
    std::vector<Array> results;
    if (raw) {
        results.reserve(num);
        for (int i = 0; i < num; ++i) results.emplace_back(raw[i]);
        free(raw); // free the pointer array, not the individual arrays
    }
    return results;
}

inline std::vector<Array> hsplit(const Array& ary,
                                 const Array& indices_or_sections) {
    int num = 0;
    ::Array** raw = ::hsplit(ary.get(), indices_or_sections.get(), &num);
    std::vector<Array> results;
    if (raw) {
        results.reserve(num);
        for (int i = 0; i < num; ++i) results.emplace_back(raw[i]);
        free(raw);
    }
    return results;
}

inline std::vector<Array> vsplit(const Array& ary,
                                 const Array& indices_or_sections) {
    int num = 0;
    ::Array** raw = ::vsplit(ary.get(), indices_or_sections.get(), &num);
    std::vector<Array> results;
    if (raw) {
        results.reserve(num);
        for (int i = 0; i < num; ++i) results.emplace_back(raw[i]);
        free(raw);
    }
    return results;
}

inline std::vector<Array> dsplit(const Array& ary,
                                 const Array& indices_or_sections) {
    int num = 0;
    ::Array** raw = ::dsplit(ary.get(), indices_or_sections.get(), &num);
    std::vector<Array> results;
    if (raw) {
        results.reserve(num);
        for (int i = 0; i < num; ++i) results.emplace_back(raw[i]);
        free(raw);
    }
    return results;
}

// ── Diagonal extraction ──────────────────────────────────────

inline Array diag(const Array& v, int k = 0) {
    return Array(::diag(v.get(), k));
}

inline Array diagflat(const Array& a, int k = 0) {
    return Array(::diagflat(a.get(), k));
}

inline Array diagonal(const Array& a, int offset = 0,
                      int axis1 = 0, int axis2 = 1) {
    return Array(::diagonal(a.get(), offset, axis1, axis2));
}

// ── Take / compress / choose ─────────────────────────────────

inline Array take(const Array& a, const Array& indices, int axis) {
    return Array(::take(a.get(), indices.get(), axis));
}

inline Array compress(const Array& condition, const Array& a, int axis) {
    return Array(::compress(condition.get(), a.get(), axis));
}

inline Array choose(const Array& a, const std::vector<Array>& choices, int mode) {
    auto c_ptrs = detail::to_c_ptrs(choices);
    return Array(::choose(a.get(), static_cast<int>(c_ptrs.size()),
                          c_ptrs.data(), mode));
}

// ── Pad ──────────────────────────────────────────────────────

inline Array pad(const Array& a, const Array& pad_width,
                 const char* mode = "constant", double constant_value = 0.0) {
    return Array(::pad(a.get(), pad_width.get(), mode, constant_value));
}

// ── Flat nonzero ─────────────────────────────────────────────

inline Array flatnonzero(const Array& a) {
    return Array(::flatnonzero(a.get()));
}

// ── Bmat / block ─────────────────────────────────────────────

inline Array block(const std::vector<Array>& blocks, int rows, int cols) {
    auto ptrs = detail::to_c_ptrs(blocks);
    return Array(::block(ptrs.data(), rows, cols));
}

inline Array bmat(const std::vector<Array>& blocks, int rows, int cols) {
    auto ptrs = detail::to_c_ptrs(blocks);
    return Array(::bmat(ptrs.data(), rows, cols));
}

// ── Meshgrid ─────────────────────────────────────────────────

inline std::vector<Array> meshgrid(const std::vector<Array>& arrays,
                                   const char* indexing = "xy") {
    auto ptrs = detail::to_c_ptrs(arrays);
    ::Array** out = nullptr;
    // libnpc meshgrid returns 0 on success, allocates `out`
    if (::meshgrid(ptrs.data(), static_cast<int>(ptrs.size()), indexing, &out) != 0) {
        return {};
    }
    std::vector<Array> results;
    results.reserve(ptrs.size());
    for (size_t i = 0; i < ptrs.size(); ++i) {
        results.emplace_back(out[i]);
    }
    free(out); // free the pointer array, not individual arrays
    return results;
}

} // namespace npc
