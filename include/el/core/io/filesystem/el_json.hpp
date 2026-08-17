#pragma once

#include "el/core/containers/string.hpp"

#include "nlohmann/json_fwd.hpp"

namespace el
{
    class ElJsonParser
    {
    public:
        static bool load(const el::ElString &filepath, nlohmann::json &out_json);
        static bool save(const el::ElString &filepath, nlohmann::json &in_json);
    };
}