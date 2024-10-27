file(GLOB_RECURSE APP_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

add_executable(${CMAKE_PROJECT_NAME} ${APP_SRC})

set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY C_STANDARD 17)

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC include)

target_link_libraries(${CMAKE_PROJECT_NAME} c-liblogs)
