#include "el/core/io/filesystem/el_json.hpp"
#include "el/core/io/filesystem/el_fstream.hpp"

#include "nlohmann/json.hpp"
#include <iostream>

namespace el
{
    bool ElJsonParser::load(const el::ElString &filepath, nlohmann::json &out_json)
    {
        el::ElString json_string;
        if (!el::ElFstream::readTextFile(filepath, json_string))
            return false;

        try
        {
            out_json = nlohmann::json::parse(json_string);
        }
        catch (const nlohmann::json::parse_error &e)
        {
            std::cerr << "ElError: JSON Parse error in " << filepath << '\n';
            std::cerr << e.what() << '\n';
            return false;
        }

        return true;
    }
}