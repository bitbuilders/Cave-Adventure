message(STATUS "Fetching SFML...")

set(SFML_BUILD_AUDIO TRUE)
set(SFML_BUILD_NETWORK TRUE)

FetchContent_MakeAvailable(sfml)