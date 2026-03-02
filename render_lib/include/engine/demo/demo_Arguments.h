#pragma once
#include <engine/eng_HandleGraphicsArgs.h>
#include <utility>

namespace eng::demo {

inline std::pair<int, char**> GetArguments(int argc, char** argv) {
#ifdef __SWITCH__
    /* Use hardcoded args on NX. */
    static char* argstr[] {
        "winwidth", "1280",
        "winHeight", "720",
        "width", "1280",
        "height", "720",
        "numcpus", "3",
        "rpp", "10",            // change me
        "recursionDepth", "4"   // change me
    };
    return {sizeof(argstr) / sizeof(argstr[0]), argstr};
#else
    return {argc, argv};
#endif
}

} // namespace eng::demo
