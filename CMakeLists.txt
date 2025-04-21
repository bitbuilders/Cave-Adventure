cmake_minimum_required(VERSION 3.31)
project(Adventure LANGUAGES CXX)

add_subdirectory(dependencies)
add_subdirectory(src)

set(GAME_INPUT_PATH staticlibs/Microsoft.GameInput.1.1.26100.4768)

target_include_directories(Adventure PRIVATE src assets ${GAME_INPUT_PATH}/native/include)
target_link_directories(Adventure PRIVATE ${GAME_INPUT_PATH}/native/lib/x64)
target_link_libraries(Adventure
        PRIVATE
        GameInput)

#file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

add_custom_target(copy_assets
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/copy_assets.cmake
)
add_dependencies(Adventure copy_assets)