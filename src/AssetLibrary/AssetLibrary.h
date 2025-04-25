
#pragma once

#include <string>

// Include font helper files so you don't need to include them everywhere else
#include "IconsFontAwesome6.h"
#include "IconsFontAwesome6Brands.h"

struct ImFont;

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

    namespace FontAwesome
    {
        extern const std::string Regular;
        extern const std::string Solid;
        extern const std::string Brands;

        /* For solid letters */
        extern ImFont* LetterFont;
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
