#pragma once

#include "el/core/containers/string.hpp"

namespace el
{
    class ElFstream
    {
    public:
        static bool readTextFile(const el::ElString &filepath, el::ElString &out_text);

        // static bool readBinaryFile(const el::ElString& filepath, & out_text);

        static bool writeTextFile(const el::ElString &filepath, const el::ElString &in_text);
    };
}