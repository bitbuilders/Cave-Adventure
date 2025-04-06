add_executable(Adventure WIN32 main.cpp)
target_link_libraries(Adventure PRIVATE ImGui-SFML::ImGui-SFML)

if (MSVC)
#   Remove console
    set_target_properties(Adventure PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif ()
