#pragma once

#include "npc/array.hpp"
#include <vector>

namespace npc {

// ── Factory functions ─────────────────────────────────────────

/// Create an array of zeros.
/// @param shape  Shape of the array (e.g., {3, 4} for a 3x4 matrix).
/// @param dtype  Element data type (default: Float64).
inline Array zeros(std::initializer_list<int> shape, DType dtype = DType::Float64) {
    std::vector<int> sh(shape);
    return Array(::zeros(sh.data(), static_cast<int>(sh.size()),
                         static_cast<DataType>(dtype)));
}

/// Create an array of ones.
inline Array ones(std::initializer_list<int> shape, DType dtype = DType::Float64) {
    std::vector<int> sh(shape);
    return Array(::ones(sh.data(), static_cast<int>(sh.size()),
                        static_cast<DataType>(dtype)));
}

/// Create an array filled with a constant value.
inline Array full(std::initializer_list<int> shape, double value,
                  DType dtype = DType::Float64) {
    std::vector<int> sh(shape);
    return Array(::full(sh.data(), static_cast<int>(sh.size()),
                        static_cast<DataType>(dtype), &value));
}

/// Create an uninitialized array.
inline Array empty(std::initializer_list<int> shape, DType dtype = DType::Float64) {
    std::vector<int> sh(shape);
    return Array(::empty(sh.data(), static_cast<int>(sh.size()),
                         static_cast<DataType>(dtype)));
}

/// Evenly spaced values within a given interval.
/// Like numpy.arange: [start, stop) with given step.
inline Array arange(double start, double stop, double step = 1.0,
                    DType dtype = DType::Float64) {
    return Array(::arange(start, stop, step, static_cast<DataType>(dtype)));
}

/// Evenly spaced numbers over a specified interval.
/// Like numpy.linspace: returns num evenly spaced samples from start to stop.
inline Array linspace(double start, double stop, int num, bool endpoint = true) {
    // libnpc provides linspace_float; for double precision we use arange-like logic.
    // Here we implement linspace directly:
    if (num <= 0) return Array(nullptr);
    if (num == 1) {
        auto result = full({1}, start);
        return result;
    }

    double step;
    if (endpoint) {
        step = (num > 1) ? (stop - start) / (num - 1) : 0.0;
    } else {
        step = (stop - start) / num;
    }

    auto result = arange(start, stop + (endpoint ? step * 0.5 : 0.0), step);
    // If endpoint, ensure last value is exactly stop
    if (endpoint && result.size() == num) {
        result.at<double>({num - 1}) = stop;
    }
    return result;
}

/// Create an identity matrix.
/// @param N  Number of rows.
/// @param M  Number of columns (default: N, for a square matrix).
/// @param k  Diagonal offset (default: 0 = main diagonal).
inline Array eye(int N, int M = -1, int k = 0, DType dtype = DType::Float64) {
    if (M <= 0) M = N;
    return Array(::eye(N, M, k, static_cast<DataType>(dtype)));
}

/// Create an NxN identity matrix.
inline Array identity(int N, DType dtype = DType::Float64) {
    return Array(::identity(N, static_cast<DataType>(dtype)));
}

// ── Convenience: from initializer_list of values ──────────────

/// Create a 1-D array from a list of values.
/// Usage: auto a = npc::array({1.0, 2.0, 3.0});
template <typename T>
Array array(std::initializer_list<T> data) {
    int shape[] = {static_cast<int>(data.size())};
    Array result = zeros({shape[0]}, dtype_of<T>::value);
    T* dst = result.data<T>();
    int i = 0;
    for (const auto& v : data) {
        dst[i++] = v;
    }
    return result;
}

/// Create a 2-D array from nested initializer lists.
/// Usage: auto a = npc::array({{1.0, 2.0}, {3.0, 4.0}});
template <typename T>
Array array(std::initializer_list<std::initializer_list<T>> data) {
    int rows = static_cast<int>(data.size());
    int cols = 0;
    for (const auto& row : data) {
        if (static_cast<int>(row.size()) > cols) cols = static_cast<int>(row.size());
    }

    int shape[] = {rows, cols};
    Array result = zeros({rows, cols}, dtype_of<T>::value);
    T* dst = result.data<T>();
    int r = 0;
    for (const auto& row : data) {
        int c = 0;
        for (const auto& v : row) {
            dst[r * cols + c] = v;
            ++c;
        }
        ++r;
    }
    return result;
}

// ── Copy ──────────────────────────────────────────────────────

/// Deep copy of an array.
inline Array copy(const Array& arr) {
    return Array(copy_array(arr.get()));
}

// ── Type conversion ───────────────────────────────────────────

/// Cast array to a new dtype.
inline Array astype(const Array& arr, DType new_dtype) {
    return Array(::astype(arr.get(), static_cast<DataType>(new_dtype)));
}

} // namespace npc
