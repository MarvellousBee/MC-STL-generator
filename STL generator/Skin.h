#pragma once
#include <vector>
#include <map>
#include <fstream>
#include <string>
#include "XValues.h"
#include "SkinStructure.h"

namespace constants
{
    static const int num_of_pixels{ 64 };
}
struct Skin
{
    std::vector<std::vector<Values4>> RGBA_2d_array;
    std::map<std::string, std::map<std::string, Values4>> skin_structure;

    // checking num of colors again to be 100% certain
    std::vector<Values4> colors;

    Skin(std::vector<std::string>& raw_skin_data, std::ifstream& skin_documentation_file)
    {
        skin_structure = get_skin_structure(skin_documentation_file);

        RGBA_2d_array.resize(constants::num_of_pixels);
        for (int i{ 0 }; i < constants::num_of_pixels; i++)
            RGBA_2d_array[i].resize(constants::num_of_pixels);

        for (int x{ 0 }; x < constants::num_of_pixels; x++)
            for (int y{ 0 }; y < constants::num_of_pixels; y++)
            {
                auto& str{ raw_skin_data[x * constants::num_of_pixels + y] };
                static const char delimiter = ' ';
                int pos{ 0 };
                int id_of_number_in_row{ -1 };
                std::string token;
                while ((pos = str.find(delimiter)) != std::string::npos)
                {
                    token = str.substr(0, pos);
                    long double temp{ std::stod(token) };
                    RGBA_2d_array[x][y][++id_of_number_in_row] = temp;
                    str.erase(0, pos + 1);
                }
                RGBA_2d_array[x][y][++id_of_number_in_row] = std::stod(str.substr(0, pos));
                bool new_color{ true };
                for (auto& c : colors)
                    if (RGBA_2d_array[x][y] == c)
                    {
                        new_color = false;
                        break;
                    }
                //std::cout << RGBA_2d_array[x][y][0] << ' ' << RGBA_2d_array[x][y][1] << ' ' << RGBA_2d_array[x][y][2] << ' ' << '\n';
                if (new_color)
                    colors.push_back(RGBA_2d_array[x][y]);
            }
    }
};