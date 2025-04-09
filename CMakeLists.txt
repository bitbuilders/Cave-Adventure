cmake_minimum_required(VERSION 3.28)
project(Adventure LANGUAGES CXX)

add_subdirectory(dependencies)
add_subdirectory(src)

target_include_directories(Adventure PRIVATE src assets)

#file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

add_custom_target(copy_assets
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/copy_assets.cmake
)
add_dependencies(Adventure copy_assets)