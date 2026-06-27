add_executable(basic_usage basic_usage.cpp)
target_link_libraries(basic_usage PRIVATE libnpcpp)

# Run example as a test to verify it works
add_test(NAME example_basic_usage COMMAND basic_usage)
