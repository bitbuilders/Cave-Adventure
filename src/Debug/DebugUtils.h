
#pragma once

#include <string>
#include <source_location>
#include <cassert>

#if WIN32
#include <intrin.h>
#endif

#define CHECK(Exp, Msg) assert((Msg, Exp));
#define CHECKV(Exp) assert(Exp);

#define ISTRUE(Expr) if (!(Expr)) BREAK

// Linux debug break
#ifdef __has_builtin
    #if __has_builtin(__builtin_debugtrap)
        #define BREAK __builtin_trap();
    #endif
#endif

#ifndef BREAK
#if WIN32
    #define BREAK __debugbreak(); int __stophere = 0;
    #define BREAK_IF(cond) if (cond) BREAK
#else
    #define BREAK CHECKV(false)
#endif
#endif


namespace DebugUtils
{
    std::string GetCallLine(std::source_location call = std::source_location::current());
}
