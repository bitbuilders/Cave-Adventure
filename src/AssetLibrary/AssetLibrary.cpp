
#include "AssetLibrary.h"

#include <imgui-SFML.h>

#include "imgui.h"


#define ROBOTO_FONT(name) "assets/fonts/Roboto_Mono/static/RobotoMono-"#name".ttf"

#define FA_FONT(name, size) "assets/fonts/Font-Awesome-6.x/webfonts/fa-"#name"-"#size".ttf"

namespace Fonts
{
    const float DefaultSize = 20.0f;

    namespace Roboto
    {
        const std::string Regular = ROBOTO_FONT(Regular);
        const std::string Bold = ROBOTO_FONT(Bold);
        const std::string SemiBold = ROBOTO_FONT(SemiBold);
        const std::string Light = ROBOTO_FONT(Light);
        const std::string Thin = ROBOTO_FONT(Thin);
        const std::string Italic = ROBOTO_FONT(Italic);
    }

    namespace FontAwesome
    {
        const std::string Regular = FA_FONT(regular, 400);
        const std::string Solid = FA_FONT(solid, 900);
        const std::string Brands = FA_FONT(brands, 400);

        ImFont* LetterFont = nullptr;
    }
}

bool AssetLibrary::LoadFonts()
{
    auto io = ImGui::GetIO();
    io.Fonts->Clear();
    io.Fonts->AddFontFromFileTTF(Fonts::Roboto::Regular.c_str(), Fonts::DefaultSize);

    constexpr auto faFontSize = Fonts::DefaultSize * 0.8f;

    ImFontConfig config;
    config.MergeMode = true;
    config.GlyphMinAdvanceX = faFontSize;
    config.PixelSnapH = true;

    constexpr ImWchar iconRanges[] = { ICON_MIN_FA, ICON_MAX_FA, 0};
    io.Fonts->AddFontFromFileTTF(Fonts::FontAwesome::Solid.c_str(), faFontSize, &config, iconRanges);
    // io.Fonts->AddFontFromFileTTF(Fonts::FontAwesome::Regular.c_str(), Fonts::DefaultSize, &config, iconRanges);
    constexpr ImWchar iconRangesBrand[] = { ICON_MIN_FAB, ICON_MAX_FAB, 0};
    io.Fonts->AddFontFromFileTTF(Fonts::FontAwesome::Brands.c_str(), faFontSize, &config, iconRangesBrand);

    config.MergeMode = false;
    Fonts::FontAwesome::LetterFont = io.Fonts->AddFontFromFileTTF(Fonts::FontAwesome::Solid.c_str(), faFontSize, &config);

    return ImGui::SFML::UpdateFontTexture();
}
