
#pragma once

#include <string>

namespace Fonts
{
    extern const float DefaultSize;

    namespace Roboto
    {
        extern const std::string Regular;
        extern const std::string Bold;
        extern const std::string SemiBold;
        extern const std::string Light;
        extern const std::string Thin;
        extern const std::string Italic;
    }
}

class AssetLibrary
{
public:
    /**
     * loads standard fonts to current ImGui-SFML window (needs to be done for each SFML window)
     * @return true if success
     */
    static bool LoadFonts();
};
