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
#include "Skin.h"


//given 2 x/y coordinates, returns all pixel coordinates of a rectangle constructed between those 2 points
std::vector<std::pair<int, int>> all_coordinates(Values4 input)
{
    std::vector<std::pair<int, int>> output;
    for (long double x{ input[0] }; x < input[2]; x++)
        for (long double y{ input[1] }; y < input[3]; y++)
            output.push_back({ y, x});
    
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

    BodyPart(std::string part_name, const Point3f _pos, const Point3f size, const Skin& skin_to_apply, const int& color_id)
        : pos(_pos)
        , skin(skin_to_apply)
    {
        //core
        //output_text += make_rectangle(pos, size);
        //front
        
        std::map<std::string, std::vector<std::pair<int, int>>> coordinates{
            { "Front" , all_coordinates(skin_to_apply.skin_structure.at(part_name).at("Front")) }
        ,   { "Back"  , all_coordinates(skin_to_apply.skin_structure.at(part_name).at("Back")) }
        ,   { "Left"  , all_coordinates(skin_to_apply.skin_structure.at(part_name).at("Left")) }
        ,   { "Right" , all_coordinates(skin_to_apply.skin_structure.at(part_name).at("Right")) }
        ,   { "Bottom", all_coordinates(skin_to_apply.skin_structure.at(part_name).at("Bottom")) }
        ,   { "Top"    , all_coordinates(skin_to_apply.skin_structure.at(part_name).at("Top")) }
        };

        auto starting_pos{ pos };
        auto cube_pos{pos};


        auto center_pos{ _pos };
        center_pos.y -= size.y - 1;
        center_pos.x += 1;
        center_pos.z -= size.z - 1;
        output_text += make_rectangle(center_pos, size);
        

        auto tex_coords{ coordinates["Front"] };
        auto offset_a = tex_coords[0].first;
        auto offset_b = tex_coords[0].second;

        for (auto& pair : tex_coords)
        {
            if (!(skin_to_apply.RGBA_2d_array[pair.first][pair.second] == skin_to_apply.colors[color_id]))
                continue;

            cube_pos = starting_pos;
            cube_pos.z -= pair.first - offset_a;
            cube_pos.y -= pair.second - offset_b;
            output_text += make_rectangle(cube_pos, {1.f,1.f,1.f});
        }

        tex_coords = coordinates["Back"];
        offset_a = tex_coords[0].first;
        offset_b = tex_coords[0].second;
        

        for (auto& pair : tex_coords)
        {
            if (!(skin_to_apply.RGBA_2d_array[pair.first][pair.second] == skin_to_apply.colors[color_id]))
                continue;

            cube_pos = starting_pos;
            cube_pos.z -= pair.first - offset_a;
            cube_pos.y += pair.second - offset_b - size.y + 1;
            cube_pos.x += size.x + 1;
            output_text += make_rectangle(cube_pos, { 1.f, 1.f, 1.f });
        }

        tex_coords = coordinates["Left"];
        offset_a = tex_coords[0].first;
        offset_b = tex_coords[0].second;


        for (auto& pair : tex_coords)
        {
            if (!(skin_to_apply.RGBA_2d_array[pair.first][pair.second] == skin_to_apply.colors[color_id]))
                continue;

            cube_pos = starting_pos;
            cube_pos.z -= pair.first - offset_a;
            cube_pos.y -= size.y;
            cube_pos.x += pair.second - offset_b + 1;
            output_text += make_rectangle(cube_pos, { 1.f, 1.f, 1.f });
        }

        tex_coords = coordinates["Right"];
        offset_a = tex_coords[0].first;
        offset_b = tex_coords[0].second;


        for (auto& pair : tex_coords)
        {
            if (!(skin_to_apply.RGBA_2d_array[pair.first][pair.second] == skin_to_apply.colors[color_id]))
                continue;

            cube_pos = starting_pos;
            cube_pos.z -= pair.first - offset_a;
            cube_pos.y += 1;
            cube_pos.x -= pair.second - offset_b - size.x;
            output_text += make_rectangle(cube_pos, { 1.f, 1.f, 1.f });
        }

    //    for (auto& pair : tex_coords)
    //    {
    //        if (!(skin_to_apply.RGBA_2d_array[pair.first][pair.second] == skin_to_apply.colors[color_id]))
    //            continue;

    //        cube_pos = starting_pos;
    //        cube_pos.z += 1;// offset_a;
    //        cube_pos.y -= pair.second;
    //        //cube_pos.x += offset_x - pair.first + 4;
    //        output_text += make_rectangle(cube_pos, { 1.f, 1.f, 1.f });
    //    }

    //    tex_coords = coordinates["Bottom"];
    //    //offset_x = tex_coords[0].first;
    //    //offset_b = tex_coords[0].second;


    //    for (auto& pair : tex_coords)
    //    {
    //        if (!(skin_to_apply.RGBA_2d_array[pair.first][pair.second] == skin_to_apply.colors[color_id]))
    //            continue;

    //        cube_pos = starting_pos;
    //        cube_pos.z -= 12.f;
    //        cube_pos.y -= pair.second;
    //        //cube_pos.x += offset_x - pair.first + 4;
    //        output_text += make_rectangle(cube_pos, { 1.f, 1.f, 1.f });
    //    }
    //    
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


        outf << "solid ASCII\n";
        outf << BodyPart{ "Torso",  { 0.f,0.f,0.f  }, {  4.f,  8.f, 12.f  }, skin, i }.get_string();
        outf << BodyPart{ "Head" ,  { 0.f,0.f,50.f }, {  8.f,  8.f, 8.f  }, skin, i }.get_string();
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
    //outf << make_rectangle({ 0.f,  0.f,  0.f }, { 5.f, 10.f, 15.f });

    

    return 0;
}