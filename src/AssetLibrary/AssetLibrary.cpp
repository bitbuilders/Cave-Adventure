
#include "AssetLibrary.h"

#include <imgui-SFML.h>

#include "imgui.h"


#define ROBOTO_FONT(name) "assets/fonts/Roboto_Mono/static/RobotoMono-"#name".ttf"

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
}

bool AssetLibrary::LoadFonts()
{
    auto io = ImGui::GetIO();
    io.Fonts->Clear();
    io.Fonts->AddFontFromFileTTF(Fonts::Roboto::Light.c_str(), Fonts::DefaultSize);

    return ImGui::SFML::UpdateFontTexture();
}
