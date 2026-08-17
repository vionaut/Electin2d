#include "el/core/io/filesystem/el_fstream.hpp"

#include <fstream>
#include <filesystem>
#include <iostream>

namespace el
{
    bool ElFstream::readTextFile(const el::ElString &filepath, el::ElString &out_text)
    {
        std::ifstream file(filepath, std::ios::in | std::ios::ate);
        if (!file.is_open())
        {
            std::cerr << "ElError: Could not open text file:" << filepath << '\n';
            return false;
        }

        std::streamsize size = file.tellg();
        out_text.reserve(size);

        file.seekg(0, std::ios::beg);

        out_text.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

        file.close();

        return true;
    }

    bool ElFstream::writeTextFile(const el::ElString &filepath, const el::ElString &in_text)
    {
        std::filesystem::path p(filepath);
        if (p.has_parent_path())
        {
            std::filesystem::create_directories(p.parent_path());
        }

        std::ofstream file(filepath);
        if (!file.is_open())
        {
            std::cerr << "ElError: Could not open text file:" << filepath << '\n';
            return false;
        }

        file << in_text;
        file.close();
        return true;
    }
}