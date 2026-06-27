# BuildLibnpc.cmake — Integrate libnpc's GNU Make build into CMake
#
# libnpc uses GNU Make (not CMake). This module:
#   1. Runs `make` in third_party/libnpc to produce lib/libnpc.a
#   2. Creates an INTERFACE target `npc_libnpc` that exposes headers + library
#   3. Finds libzip and links it transitively
#
# Usage from top-level CMakeLists.txt:
#   include(cmake/BuildLibnpc.cmake)
#   target_link_libraries(your_target PRIVATE npc_libnpc)

set(LIBNPC_DIR ${CMAKE_SOURCE_DIR}/third_party/libnpc)
set(LIBNPC_STATIC_LIB ${LIBNPC_DIR}/lib/libnpc.a)

# Determine shared library extension for platform
if(APPLE)
    set(LIBNPC_SHARED_EXT dylib)
else()
    set(LIBNPC_SHARED_EXT so)
endif()

set(LIBNPC_SHARED_LIB ${LIBNPC_DIR}/lib/libnpc.${LIBNPC_SHARED_EXT})

# ── Custom target to build libnpc ──────────────────────────────────
# Runs `make` in the libnpc source directory.
# Make itself handles incremental builds — CMake just ensures it's been run.
add_custom_command(
    OUTPUT ${LIBNPC_STATIC_LIB}
    COMMAND make -C ${LIBNPC_DIR}
    COMMENT "Building libnpc (static library)..."
    WORKING_DIRECTORY ${LIBNPC_DIR}
)

add_custom_target(libnpc_build DEPENDS ${LIBNPC_STATIC_LIB})

# ── Find libzip (required by libnpc for NPZ I/O) ──────────────────
# libzip provides a CMake config; try that first, then fall back.
find_package(libzip QUIET CONFIG)
if(NOT libzip_FOUND)
    # Manual search — Homebrew paths on macOS
    if(APPLE)
        find_library(LIBZIP_LIBRARY libzip
            PATHS /opt/homebrew/lib /usr/local/lib
            NO_DEFAULT_PATH
        )
        find_path(LIBZIP_INCLUDE_DIR zip.h
            PATHS /opt/homebrew/include /usr/local/include
            NO_DEFAULT_PATH
        )
    endif()

    # System-wide fallback
    if(NOT LIBZIP_LIBRARY)
        find_library(LIBZIP_LIBRARY libzip)
        find_path(LIBZIP_INCLUDE_DIR zip.h)
    endif()
endif()

if(NOT libzip_FOUND AND NOT LIBZIP_LIBRARY)
    message(WARNING "libzip not found — NPZ I/O will not be available. "
                    "Install with: brew install libzip (macOS) or apt install libzip-dev (Linux)")
endif()

# ── Interface target: npc_libnpc ───────────────────────────────────
# Users link against this to get libnpc headers + static lib + transitive deps.
add_library(npc_libnpc INTERFACE)
target_include_directories(npc_libnpc INTERFACE ${LIBNPC_DIR}/include)
target_link_libraries(npc_libnpc INTERFACE ${LIBNPC_STATIC_LIB})

# Ensure libnpc is built before anything that links against it
add_dependencies(npc_libnpc libnpc_build)

# Transitively expose libzip and math library
if(libzip_FOUND)
    target_link_libraries(npc_libnpc INTERFACE libzip::zip)
elseif(LIBZIP_LIBRARY)
    target_link_libraries(npc_libnpc INTERFACE ${LIBZIP_LIBRARY})
    if(LIBZIP_INCLUDE_DIR)
        target_include_directories(npc_libnpc INTERFACE ${LIBZIP_INCLUDE_DIR})
    endif()
endif()

# Math library (-lm on Linux; automatic on macOS)
if(UNIX AND NOT APPLE)
    target_link_libraries(npc_libnpc INTERFACE m)
endif()
