#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>
#include <cmath>
#include <memory>

#include "stl generation.h"
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
                bool new_color{ true };
                for (auto& c : colors)
                    if (RGBA_2d_array[x][y] == c)
                    {
                        new_color = false;
                        break;
                    }
                if (new_color)
                    colors.push_back(RGBA_2d_array[x][y]);
            }
    }
};

//given 2 x/y coordinates, returns all pixel coordinates of a rectangle constructed between those 2 points
std::vector<std::pair<int, int>> all_coordinates(Values4 input)
{
    std::vector<std::pair<int, int>> output;
    for (long double x{ input[0] }; x < input[2]; x++)
        for (long double y{ input[1] }; y < input[3]; y++)
            output.push_back({ y, x });
    
    return output;
}


struct BodyPart
{
    const Point3f pos;
    const Skin& skin;
    
    std::map<std::string, Values4> coordinates{
           {"Back",   {}}
        ,  {"Bottom", {}}
        ,  {"Front",  {}}
        ,  {"Left",   {}}
        ,  {"Right",  {}}
        ,  {"Top",    {}}
    };
    std::map<std::string, Values4> outer_coordinates{
           {"Back",   {}}
        ,  {"Bottom", {}}
        ,  {"Front",  {}}
        ,  {"Left",   {}}
        ,  {"Right",  {}}
        ,  {"Top",    {}}
    };
    std::string output_text{ "" };

    BodyPart(std::string part_name, const Point3f& _pos, const Skin& skin_to_apply, const int& color_id) 
        : pos(_pos)
        , skin(skin_to_apply)
    {
        //core
        //output_text += make_rectangle(pos, size);
        //front
        
        auto Tpos{pos};

        //for (auto& a : skin_to_apply.skin_structure)
        //    for (auto& b : a.second)
        //    {
        //        std::cout << a.first << "|" << b.first << "|";
        //        for (int i{ 0 }; i < 4; i++)
        //            std::cout << b.second.at(i) << ' ';

        //        std::cout << '\n';
        //    }

        for (auto& pair : all_coordinates(skin_to_apply.skin_structure.at(part_name).at("Front")))
        {
            if (!(skin_to_apply.RGBA_2d_array[pair.first][pair.second] == skin_to_apply.colors[color_id]))
                continue;
            Tpos = pos;
            Tpos.z -= pair.first;
            Tpos.y -= pair.second;
            output_text += make_rectangle(Tpos, {1.f,1.f,1.f});
        }
        
    }
    std::string get_string() const
    {
        return output_text;
    }
};



int main()
{
    //std::ofstream outf{ "Input.stl" };

    //if (!outf){
    //    std::cerr << "Could not open the  STL file for writing!\n";
    //    return 1;
    //}
    

    std::vector<std::string> text_data{};
    text_data.reserve(constants::num_of_pixels * constants::num_of_pixels);
    std::string temp;
    std::ifstream skin_documentation_file("py_output.txt");
    int i{ -1 };
    while (getline(skin_documentation_file, temp))
        text_data.push_back(temp);//(*text_data)[i] = temp;
    skin_documentation_file.close();
    std::ifstream mc_skin_documentation("plane_coords.txt");
    Skin skin{ text_data, mc_skin_documentation };
    mc_skin_documentation.close();

    
    for (int i{ 0 }; i < skin.colors.size(); i++)
    {
        std::ofstream outf{ "EX" + std::to_string(i) + ".stl"};
        if (!outf){
            std::cerr << "Could not open  EX" + std::to_string(i) + ".stl for writing!\n";
            return 1;
        }


        BodyPart torso{ "Torso", {0.f,0.f,0.f}, skin, i };


        outf << "solid ASCII\n";
        outf << torso.get_string();
        //outf << make_rectangle({  0.f,  0.f,  0.f }, { 5.f, 10.f, 15.f });
        //outf << make_rectangle({  5.f,  0.f,  0.f }, { 5.f, 10.f,  2.f });
        //outf << make_rectangle({  0.f, 10.f,  5.f }, { 5.f, 10.f,  8.f });


        //stl_files.push_back(outf);
    }

    std::ofstream outf{ "test.stl" };
    if (!outf) {
        std::cerr << "Could not open  EX" + std::to_string(i) + ".stl for writing!\n";
        return 1;
    }
    outf << make_rectangle({ 0.f,  0.f,  0.f }, { 5.f, 10.f, 15.f });

    

    return 0;
}