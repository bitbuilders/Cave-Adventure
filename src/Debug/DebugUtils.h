
#pragma once

#include <string>
#include <source_location>
#include <cassert>

#define CHECK(Exp, Msg) assert((Msg, Exp));
#define CHECKV(Exp) assert(Exp);

namespace DebugUtils
{
    std::string GetCallLine(std::source_location call = std::source_location::current());
}
