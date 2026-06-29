cmake_minimum_required(VERSION 3.16)
project(libnpcpp VERSION 1.0.1
    DESCRIPTION "libnpc++ — C++17 wrapper for libnpc — NumPy-compatible numerical computing in C++"
    LANGUAGES C CXX
)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ── Build libnpc (third-party C library) ──────────────────────
include(cmake/BuildLibnpc.cmake)

# ── libnpc++ header-only wrapper library ──────────────────────
add_library(libnpcpp INTERFACE)
target_include_directories(libnpcpp INTERFACE
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(libnpcpp INTERFACE npc_libnpc)

# Alias for modern CMake usage: libnpc::npcpp
add_library(libnpc::npcpp ALIAS libnpcpp)

# ── Options ───────────────────────────────────────────────────
option(NPCPP_BUILD_TESTS "Build test suite" ON)
option(NPCPP_BUILD_EXAMPLES "Build examples" ON)

# ── Tests ─────────────────────────────────────────────────────
if(NPCPP_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

# ── Examples ──────────────────────────────────────────────────
if(NPCPP_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
