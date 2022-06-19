#pragma once
#include <map>
#include <string>
#include <fstream>

#include "XValues.h"


std::map<std::string, std::map<std::string, Values4>> get_skin_structure(std::ifstream& raw_rgb_file)
{
    std::map<std::string, std::map<std::string, Values4>> skin_structure;
    std::string temp;

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
    return skin_structure;
}