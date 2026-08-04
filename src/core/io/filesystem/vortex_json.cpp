#include "vortex/core/io/filesystem/vortex_json.hpp"
#include "vortex/core/io/filesystem/vortex_fstream.hpp"

#include "nlohmann/json.hpp"
#include <iostream>

namespace vortex::io
{
    bool VxJsonParser::load(const vortex::containers::VxString& filepath, nlohmann::json& out_json)
    {
        vortex::containers::VxString json_string;
        if(!vortex::io::VxFstream::readTextFile(filepath, json_string))
            return false;

        try
        {
            out_json = nlohmann::json::parse(json_string);
        }
        catch (const nlohmann::json::parse_error& e)
        {
            std::cerr << "VxError: JSON Parse error in " << filepath << '\n';
            std::cerr << e.what() << '\n';
            return false;
        }

        return true;
    }
}