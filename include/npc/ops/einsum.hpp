#pragma once

/// Einstein summation (einsum) wrappers for libnpc.

#include "npc/array.hpp"
#include "npc/ops/shape.hpp"
#include <string>
#include <utility>

namespace npc {

inline Array einsum(const char* subscripts,
                    const std::vector<Array>& operands,
                    const Array& out = Array(),
                    const char* optimize = "greedy") {
    auto ptrs = detail::to_c_ptrs(operands);
    return Array(::einsum(subscripts, ptrs.data(),
                          static_cast<int>(ptrs.size()),
                          out.is_null() ? nullptr : out.get(),
                          optimize));
}

inline std::pair<std::vector<std::vector<int>>, std::string> einsum_path(
        const char* subscripts,
        const std::vector<Array>& operands,
        const char* optimize = "greedy") {
    auto ptrs = detail::to_c_ptrs(operands);
    int** out_path = nullptr;
    int out_path_len = 0;
    char* out_info = nullptr;
    ::einsum_path(subscripts, ptrs.data(), static_cast<int>(ptrs.size()),
                  optimize, &out_path, &out_path_len, &out_info);
    std::vector<std::vector<int>> path;
    if (out_path && out_path_len > 0) {
        path.reserve(out_path_len);
        for (int i = 0; i < out_path_len; ++i) {
            path.emplace_back(out_path[i], out_path[i] + 2);
            free(out_path[i]);
        }
        free(out_path);
    }
    std::string info(out_info ? out_info : "");
    if (out_info) free(out_info);
    return {std::move(path), std::move(info)};
}

} // namespace npc
