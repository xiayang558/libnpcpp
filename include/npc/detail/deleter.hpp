#pragma once

// Include libnpc's C header.  The vendored array.h is patched with proper
// `extern "C"` guards (added after the system includes, before libnpc's own
// declarations) and the `delete` function is renamed to `npc_delete` when
// compiled as C++ (to avoid clashing with the C++ `delete` keyword).
#include <array.h>
#include <function.h>
#include <shape.h>
#include <linalg.h>
#include <eig.h>
#include <utils.h>
#include <random.h>
#include <io.h>
#include <window.h>
#include <datetime64.h>
#include <histogram.h>
#include <einsum.h>

namespace npc {
namespace detail {

/// Custom deleter for ::Array* — calls libnpc's free_array().
/// Used with std::unique_ptr to provide RAII for C arrays.
struct ArrayDeleter {
    void operator()(::Array* arr) const noexcept {
        if (arr) {
            free_array(arr);
        }
    }
};

} // namespace detail
} // namespace npc
