add_compile_definitions("DEBUG=$<CONFIG:Debug>")
add_executable(Adventure WIN32
        main.cpp
        Game/Game.cpp
        Game/Game.h
        Debug/Console.cpp
        Debug/Console.h
        AssetLibrary/AssetLibrary.cpp
        AssetLibrary/AssetLibrary.h
        Debug/DebugUtils.cpp
        Debug/DebugUtils.h
        Debug/Stats.cpp
        Debug/Stats.h
        Debug/DebugDraw.cpp
        Debug/DebugDraw.h
        Game/CaveChrono.cpp
        Game/CaveChrono.h
        Calc/Calc.cpp
        Calc/Calc.h
        Game/Module.cpp
        Game/Module.h
        Game/Tickable.cpp
        Game/Tickable.h
        Input/Controls.cpp
        Input/Controls.h
        Input/CaveGameInput.cpp
        Input/CaveGameInput.h)
target_include_directories(Adventure PRIVATE Game Debug AssetLibrary Calc Input)
#set_target_properties(Adventure PROPERTIES CXX_STANDARD 23)
target_compile_features(Adventure PUBLIC cxx_std_23)
target_link_libraries(Adventure
        PRIVATE
        ImGui-SFML::ImGui-SFML
        SFML::Audio
        SFML::Network)
#set_target_properties(Adventure PROPERTIES WIN32_EXECUTABLE $<CONFIG:Release>) # will remove console, but needs to override WinMain otherwise linker error (see bottom of file)

#[[if (MSVC)

    #   Ignores the need to create a WinApp entry point if the console is removed
    set_target_properties(Adventure PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif ()]]
