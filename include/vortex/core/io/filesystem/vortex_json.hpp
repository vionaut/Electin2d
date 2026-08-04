#pragma once

#include "vortex/core/containers/string.hpp"

#include "nlohmann/json_fwd.hpp"

namespace vortex::io
{
    class VxJsonParser
    {
    public:
        static bool load(const vortex::containers::VxString& filepath, nlohmann::json& out_json);
        static bool save(const vortex::containers::VxString& filepath, nlohmann::json& in_json);
    };
}