add_subdirectory(tests)

# Add the necessary configuration for building and running the tests using Google Test
target_link_libraries(${PROJECT_NAME} gtest gtest_main)
add_test(NAME test_hello_world COMMAND test_hello_world)