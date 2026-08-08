#pragma once

#include "vortex/core/containers/string.hpp"

namespace vortex::io
{
    class VxFstream
    {
    public:
        static bool readTextFile(const vortex::containers::VxString &filepath, vortex::containers::VxString &out_text);

        // static bool readBinaryFile(const vortex::containers::VxString& filepath, & out_text);

        static bool writeTextFile(const vortex::containers::VxString &filepath, const vortex::containers::VxString &in_text);
    };
}