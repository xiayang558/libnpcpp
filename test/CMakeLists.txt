# Simple test runner — each .cpp file becomes a test executable.
# All tests link against the libnpcpp header-only library.

# test_array — core Array creation, accessors, and operators
add_executable(test_array test_array.cpp)
target_link_libraries(test_array PRIVATE libnpcpp)
add_test(NAME test_array COMMAND test_array)

# test_math — element-wise unary and binary math operations
add_executable(test_math test_math.cpp)
target_link_libraries(test_math PRIVATE libnpcpp)
add_test(NAME test_math COMMAND test_math)

# test_stats — aggregation and statistics functions
add_executable(test_stats test_stats.cpp)
target_link_libraries(test_stats PRIVATE libnpcpp)
add_test(NAME test_stats COMMAND test_stats)

# test_linalg — linear algebra wrappers
add_executable(test_linalg test_linalg.cpp)
target_link_libraries(test_linalg PRIVATE libnpcpp)
add_test(NAME test_linalg COMMAND test_linalg)

# test_shape — shape manipulation wrappers
add_executable(test_shape test_shape.cpp)
target_link_libraries(test_shape PRIVATE libnpcpp)
add_test(NAME test_shape COMMAND test_shape)

# test_fft — FFT wrappers
add_executable(test_fft test_fft.cpp)
target_link_libraries(test_fft PRIVATE libnpcpp)
add_test(NAME test_fft COMMAND test_fft)

# test_random — random number generation wrappers
add_executable(test_random test_random.cpp)
target_link_libraries(test_random PRIVATE libnpcpp)
add_test(NAME test_random COMMAND test_random)

# test_io — I/O and window function wrappers
add_executable(test_io test_io.cpp)
target_link_libraries(test_io PRIVATE libnpcpp)
add_test(NAME test_io COMMAND test_io)

# test_polynomial — polynomial operations
add_executable(test_polynomial test_polynomial.cpp)
target_link_libraries(test_polynomial PRIVATE libnpcpp)
add_test(NAME test_polynomial COMMAND test_polynomial)

# test_setops — set and search operations
add_executable(test_setops test_setops.cpp)
target_link_libraries(test_setops PRIVATE libnpcpp)
add_test(NAME test_setops COMMAND test_setops)

# test_interp — interpolation and diff
add_executable(test_interp test_interp.cpp)
target_link_libraries(test_interp PRIVATE libnpcpp)
add_test(NAME test_interp COMMAND test_interp)

# test_histogram — histogram operations
add_executable(test_histogram test_histogram.cpp)
target_link_libraries(test_histogram PRIVATE libnpcpp)
add_test(NAME test_histogram COMMAND test_histogram)

# test_misc — miscellaneous remaining functions
add_executable(test_misc test_misc.cpp)
target_link_libraries(test_misc PRIVATE libnpcpp)
add_test(NAME test_misc COMMAND test_misc)

# test_cleanup — Phase 6 cleanup: operators, keepdims bool, nan stats, etc.
add_executable(test_cleanup test_cleanup.cpp)
target_link_libraries(test_cleanup PRIVATE libnpcpp)
add_test(NAME test_cleanup COMMAND test_cleanup)
