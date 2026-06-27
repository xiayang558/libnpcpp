#pragma once

/// Shared shape conversion utility for the libnpc++ library.
/// Replaces the NPCPP_SHAPE macro and repetitive std::vector<int> sh(...)
/// patterns found throughout the ops headers.

#include <vector>
#include <initializer_list>

namespace npc {
namespace detail {

/// Convert an initializer_list<int> to a (data, ndim) pair for the C API.
/// Returns a vector containing the shape values, with data() and size()
/// directly usable for libnpc's int* shape, int ndim parameters.
inline std::vector<int> make_shape(std::initializer_list<int> shape) {
    return std::vector<int>(shape);
}

} // namespace detail
} // namespace npc
