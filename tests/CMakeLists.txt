enable_testing()

add_executable(${CMAKE_PROJECT_NAME}_tests ${CMAKE_CURRENT_SOURCE_DIR}/test_main.cpp)

target_link_libraries(${CMAKE_PROJECT_NAME}_tests gtest gtest_main)

add_test(NAME ${CMAKE_PROJECT_NAME}_tests COMMAND ${CMAKE_PROJECT_NAME}_tests)