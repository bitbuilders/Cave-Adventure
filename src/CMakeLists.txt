add_compile_definitions("DEBUG=$<CONFIG:Debug>")
add_executable(Adventure #[[WIN32]] # WIN32 will remove console, but needs to override WinMain otherwise linker error (see bottom of file)
        main.cpp
        Game/Game.cpp
        Game/Game.h)
target_link_libraries(Adventure PRIVATE ImGui-SFML::ImGui-SFML
    SFML::Audio
    SFML::Network)

if (MSVC)
#   Remove console, if commented out will throw linker error if WIN32 is passed as executable param
#    set_target_properties(Adventure PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif ()
