#pragma once

/// File I/O wrappers for libnpc.
/// Supports .txt, .npy, and .npz file formats.

#include "npc/array.hpp"
#include <string>
#include <vector>

namespace npc {

// ── Text I/O ─────────────────────────────────────────────────

inline Array readtxt(const char* filename, const char* delimiter = " ",
                     char comment = '#', int skiplines = 0, int max_rows = -1,
                     int ndmin = 0, bool unpack = false,
                     DType dtype = DType::Float64) {
    return Array(::readtxt(filename, delimiter, comment, skiplines,
                           max_rows, nullptr, ndmin, unpack ? 1 : 0,
                           static_cast<DataType>(dtype)));
}

inline Array genfromtxt(const char* filename, const char* delimiter = ",",
                        int skip_header = 0) {
    return Array(::genfromtxt(filename, delimiter, skip_header));
}

inline int savetxt(const char* filename, const Array& arr,
                   const char* fmt = "%g", const char* delimiter = " ",
                   const char* newline = "\n", const char* header = nullptr,
                   const char* footer = nullptr, const char* comments = "# ") {
    return ::savetxt(filename, arr.get(), fmt, delimiter, newline,
                     header, footer, comments);
}

// ── NPY I/O ──────────────────────────────────────────────────

inline int save_npy(const char* filename, const Array& arr) {
    return ::save_npy(filename, arr.get());
}

inline Array load_npy(const char* filename) {
    return Array(::load_npy(filename));
}

// ── NPZ I/O ──────────────────────────────────────────────────
// NOTE: save_npz is an advanced API requiring raw C pointer arrays.
// For convenience use savez() which accepts std::vector<Array>.

inline int savez(const char* filename, const std::vector<Array>& arrays) {
    std::vector<::Array*> ptrs;
    for (auto& a : arrays) ptrs.push_back(a.get());
    return ::savez(filename, static_cast<int>(ptrs.size()), ptrs.data(),
                   0, nullptr, nullptr);
}

// ── Convenience: save/load with std::string ──────────────────

inline int savetxt(const std::string& filename, const Array& arr,
                   const char* fmt = "%g", const char* delimiter = " ") {
    return savetxt(filename.c_str(), arr, fmt, delimiter);
}

inline int save_npy(const std::string& filename, const Array& arr) {
    return save_npy(filename.c_str(), arr);
}

inline Array load_npy(const std::string& filename) {
    return load_npy(filename.c_str());
}

inline Array readtxt(const std::string& filename, const char* delimiter = " ") {
    return readtxt(filename.c_str(), delimiter);
}

} // namespace npc
