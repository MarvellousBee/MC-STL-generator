#pragma once
#include <map>
#include <string>
#include <fstream>
#include <vector>
#include "Constants.h"
#include "XValues.h"

std::map<std::string, std::map<std::string, Values4>> get_skin_structure(std::string raw_rgb_file_name = "text_output.txt")
{
    std::map<std::string, std::map<std::string, Values4>> skin_structure;
    std::string temp;
    std::ifstream raw_rgb_file(raw_rgb_file_name);

    while (getline(raw_rgb_file, temp))
    {
        static const std::string delimiter1 = "| 0 | ";
        static const std::string delimiter2 = "| 1.8 | ";

        static const std::string begin_coords_mark = "| `";
        static const std::string end_coords_mark = ")";
        static const std::string coordinate_delimiter = ", ";

        int pos{ 0 };
        std::string token;
        if (temp.find(delimiter1) != std::string::npos)
            pos = 5;
        else if (pos = temp.find(delimiter2) != std::string::npos)
            pos = 7;

        if (pos != 0)
        {
            auto coords_start{ temp.find(begin_coords_mark) - pos };
            auto coords_end{ temp.find(end_coords_mark) };

            token = temp.substr(pos + 1, coords_start - 1);

            std::string coords_str{ temp.substr(coords_start + pos + 4, coords_end - coords_start - pos - 4) };
            auto token_del{ token.find(" | ") };

            Values4 coords;
            int Cpos{ 0 };
            int Ci{ -1 };
            while ((Cpos = coords_str.find(coordinate_delimiter)) != std::string::npos)
            {
                long double ldbl = std::stod(coords_str.substr(0, Cpos));
                coords[++Ci] = ldbl;
                coords_str.erase(0, Cpos + 2);
            }
            long double ldbl = std::stod(coords_str.substr(0, Cpos));
            coords[++Ci] = ldbl;
            token.erase(token.size()-1, 1);
            skin_structure[token.substr(0, token_del)][token.substr(token_del + 3)] = coords;
        }
    }
    raw_rgb_file.close();
    return skin_structure;
}



std::vector<std::string> get_custom_skin_colors(std::string path = "text_output.txt")
{
    std::vector<std::string> mc_skin{};
    mc_skin.reserve(constants::num_of_pixels * constants::num_of_pixels);
    std::string temp;
    std::ifstream skin_file(path);
    int i{ -1 };
    while (getline(skin_file, temp))
        mc_skin.push_back(temp);//(*mc_skin_documentation)[i] = temp;
    skin_file.close();
    return mc_skin;
}

std::map<std::string, std::vector<Point3f>> init_taken_pixels_storage()
{
    std::map<std::string, std::vector<Point3f>> taken_pixels{
        {"Front", {}}
    ,   {"Back", {}}
    ,   {"Left", {}}
    ,   {"Right", {}}
    ,   {"Top", {}}
    ,   {"Bottom", {}}
    };
    for (auto& [side, vec] : taken_pixels)
        vec.reserve(64 * 64);
    return taken_pixels;
}



