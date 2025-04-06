include(FetchContent)

FetchContent_Declare(
  sfml
  GIT_REPOSITORY "https://github.com/SFML/SFML"
  GIT_TAG 3.0.0
)

add_subdirectory(sfml)

FetchContent_Declare(
        imgui
        URL "https://github.com/ocornut/imgui/archive/v1.91.1.zip"
)

FetchContent_MakeAvailable(imgui)

FetchContent_Declare(
  imgui-sfml
  GIT_REPOSITORY https://github.com/SFML/imgui-sfml
  GIT_TAG v3.0
)
add_subdirectory(imgui-sfml)