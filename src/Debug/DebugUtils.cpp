
#include "DebugUtils.h"

#include <format>


namespace DebugUtils
{
    std::string DebugUtils::GetCallLine(std::source_location call)
    {
        std::string file = call.file_name();
        int nameStart = static_cast<int>(file.rfind('\\')) + 1;
        file = file.substr(nameStart, file.length() - nameStart);

        return std::format("{} ({}:{}) \"{}\"", file, call.line(), call.column(), call.function_name());
    }
}
