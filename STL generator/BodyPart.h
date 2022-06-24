#pragma once
#include <vector>
#include <map>
#include <string>

#include "Skin.h"
#include "XValues.h"
#include "stl generation.h"


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
    
    std::string output_text{ "" };

    BodyPart(std::string part_name, const Point3f _pos, const Skin& skin_to_apply, const int& color_id)
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
        ,   { "Top"   , all_coordinates(skin_to_apply.skin_structure.at(part_name).at("Top")) }
        };

        Point3f size{
            coordinates["Left"].back().second - coordinates["Left"][0].second + 1,
            coordinates["Front"].back().second - coordinates["Front"][0].second + 1,
            coordinates["Front"].back().first - coordinates["Front"][0].first + 1
        };

        auto starting_pos{ pos };
        auto cube_pos{ pos };


        //auto center_pos{ _pos };
        //center_pos.y -= size.y - 1;
        //center_pos.x += 1;
        //center_pos.z -= size.z - 1;
        //output_text += make_rectangle(center_pos, size);


        auto tex_coords{ coordinates["Front"] };
        auto offset_a = tex_coords[0].first;
        auto offset_b = tex_coords[0].second;
        int iter{ 0 };

        for (auto& pair : tex_coords)
        {
            iter++;
            if (!(skin_to_apply.RGBA_2d_array[pair.first][pair.second] == skin_to_apply.colors[color_id]))
                continue;

            cube_pos = starting_pos;
            cube_pos.z -= pair.first - offset_a;
            cube_pos.y -= pair.second - offset_b;
            

  
            if (iter == 1){
                cube_pos.x += 1;
                cube_pos.y += 1;
                cube_pos.z += 1;
                output_text += make_side2_one_third_of_rectangle(cube_pos, {-1.f,-1.f,-1.f });
            }
            else if (iter == size.z){
                cube_pos.y += 1;
                cube_pos.x += 1;
                output_text += make_side2_one_third_of_rectangle(cube_pos, { -1.f,-1.f,1.f });
            }
            else if (iter == size.z * (size.y - 1) + 1){
                cube_pos.x += 1;
                cube_pos.z += 1;
                output_text += make_side2_one_third_of_rectangle(cube_pos, { -1.f,1.f,-1.f });
            }
            else if (iter == size.z * size.y){
                cube_pos.x += 1;
                output_text += make_side2_one_third_of_rectangle(cube_pos, { -1.f,1.f,1.f });
            }
            else if (iter <= size.z)
                output_text += make_half_rectangle(cube_pos, { 1.f,1.f,1.f });
            else if (iter > (size.y - 1) * size.z){
                cube_pos.y += 1;
                output_text += make_half_rectangle(cube_pos, { 1.f,-1.f,1.f });
            }
            else if (iter % static_cast<int>(size.z) == 1) {
                output_text += make_side2_rectangle(cube_pos, { 1.f,1.f,1.f });
            }
            else if (iter % int(size.z) == 0) {
                cube_pos.z += 1;
                output_text += make_side2_rectangle(cube_pos, { 1.f,1.f,-1.f });
            }
            else
                output_text += make_rectangle(cube_pos, { 1.f,1.f,1.f });
        }

        tex_coords = coordinates["Back"];
        offset_a = tex_coords[0].first;
        offset_b = tex_coords[0].second;
        iter = 0;

        for (auto& pair : tex_coords)
        {
            iter++;
            if (!(skin_to_apply.RGBA_2d_array[pair.first][pair.second] == skin_to_apply.colors[color_id]))
                continue;

            cube_pos = starting_pos;
            cube_pos.z -= pair.first - offset_a;
            cube_pos.y += pair.second - offset_b - size.y + 1;
            cube_pos.x += size.x - 1;

            if (iter == 1) {
                cube_pos.z += 1;
                output_text += make_side2_one_third_of_rectangle(cube_pos, { 1.f,1.f,-1.f });
            }
            else if (iter == size.z) {
                output_text += make_side2_one_third_of_rectangle(cube_pos, { 1.f,1.f,1.f });
            }
            else if (iter == size.z * (size.y - 1) + 1) {
                cube_pos.y += 1;
                cube_pos.z += 1;
                output_text += make_side2_one_third_of_rectangle(cube_pos, { 1.f,-1.f,-1.f });
            }
            else if (iter == size.z * size.y) {
                cube_pos.y += 1;
                output_text += make_side2_one_third_of_rectangle(cube_pos, { 1.f,-1.f,1.f });
            }
            else if (iter <= size.z) {
                cube_pos.y += 1;
                cube_pos.x += 1;
                output_text += make_half_rectangle(cube_pos, { -1.f,-1.f,1.f });
            }
            else if (iter > (size.y - 1) * size.z) {
                cube_pos.x += 1;
                output_text += make_half_rectangle(cube_pos, { -1.f,1.f,1.f });
            }
            else if (iter % static_cast<int>(size.z) == 1) {
                cube_pos.x += 1;
                output_text += make_side2_rectangle(cube_pos, { -1.f,1.f,1.f });
            }
            else if (iter % int(size.z) == 0) {
                cube_pos.x += 1;
                cube_pos.z += 1;
                output_text += make_side2_rectangle(cube_pos, { -1.f,1.f,-1.f });
            }
            else
                output_text += make_rectangle(cube_pos, { 1.f,1.f,1.f });
        }

        tex_coords = coordinates["Left"];
        offset_a = tex_coords[0].first;
        offset_b = tex_coords[0].second;
        iter = 0;

        for (auto& pair : tex_coords)
        {
            iter++;
            if (!(skin_to_apply.RGBA_2d_array[pair.first][pair.second] == skin_to_apply.colors[color_id]))
                continue;

            cube_pos = starting_pos;
            cube_pos.z -= pair.first - offset_a;
            cube_pos.y -= size.y - 1;
            cube_pos.x += pair.second - offset_b;


            if (iter == 1) {
                cube_pos.z += 1;
                output_text += make_side1_one_third_of_rectangle(cube_pos, { 1.f,1.f,-1.f });
            }
            else if (iter == size.z) {
                output_text += make_side1_one_third_of_rectangle(cube_pos, { 1.f,1.f,1.f });
            }
            else if (iter == size.z * (size.x - 1) + 1) {
                cube_pos.x += 1;
                cube_pos.z += 1;
                output_text += make_side1_one_third_of_rectangle(cube_pos, { -1.f,1.f,-1.f });
            }
            else if (iter == size.z * size.x) {
                cube_pos.x += 1;
                output_text += make_side1_one_third_of_rectangle(cube_pos, { -1.f,1.f,1.f });
            }
            else if (iter <= size.z) {
                cube_pos.x += 1;
                output_text += make_half_rectangle(cube_pos, { -1.f,1.f,1.f });
            }
            else if (iter > (size.x - 1) * size.z) {
                output_text += make_half_rectangle(cube_pos, { 1.f,1.f,1.f });
            }
            else if (iter % static_cast<int>(size.z) == 1) {
                //cube_pos.x += 1;
                output_text += make_side1_rectangle(cube_pos, { 1.f,1.f,1.f });
            }
            else if (iter % int(size.z) == 0) {
                //cube_pos.x += 1;
                cube_pos.z += 1;
                output_text += make_side1_rectangle(cube_pos, {1.f,1.f,-1.f });
            }
            else{
                output_text += make_rectangle(cube_pos, { 1.f,1.f,1.f });
            }
        }

        tex_coords = coordinates["Right"];
        offset_a = tex_coords[0].first;
        offset_b = tex_coords[0].second;
        iter = 0;

        for (auto& pair : tex_coords)
        {
            iter++;
            if (!(skin_to_apply.RGBA_2d_array[pair.first][pair.second] == skin_to_apply.colors[color_id]))
                continue;

            cube_pos = starting_pos;
            cube_pos.z -= pair.first - offset_a;
            cube_pos.x -= pair.second - offset_b - size.x + 1;

            if (iter == 1) {
                cube_pos.x += 1;
                cube_pos.y += 1;
                cube_pos.z += 1;
                output_text += make_side1_one_third_of_rectangle(cube_pos, { -1.f,-1.f,-1.f });
            }
            else if (iter == size.z) {
                cube_pos.x += 1;
                cube_pos.y += 1;
                output_text += make_side1_one_third_of_rectangle(cube_pos, { -1.f,-1.f,1.f });
            }
            else if (iter == size.z * (size.x - 1) + 1) {
                cube_pos.y += 1;
                cube_pos.z += 1;
                output_text += make_side1_one_third_of_rectangle(cube_pos, { 1.f,-1.f,-1.f });
            }
            else if (iter == size.z * size.x) {
                cube_pos.y += 1;
                output_text += make_side1_one_third_of_rectangle(cube_pos, { 1.f,-1.f,1.f });
            }
            else if (iter <= size.z) {
                cube_pos.y += 1;
                output_text += make_half_rectangle(cube_pos, { 1.f,-1.f,1.f });
            }
            else if (iter > (size.x - 1) * size.z) {
                cube_pos.y += 1;
                cube_pos.x += 1;
                output_text += make_half_rectangle(cube_pos, { -1.f,-1.f,1.f });
            }
            else if (iter % static_cast<int>(size.z) == 1) {
                cube_pos.x += 1;
                cube_pos.y += 1;
                output_text += make_side1_rectangle(cube_pos, { -1.f,-1.f,1.f });
            }
            else if (iter % static_cast<int>(size.z) == 0) {
                cube_pos.x += 1;
                cube_pos.y += 1;
                cube_pos.z += 1;
                output_text += make_side1_rectangle(cube_pos, { -1.f,-1.f,-1.f });
            }
            else
                output_text += make_rectangle(cube_pos, { 1.f,1.f,1.f });


        }

        tex_coords = coordinates["Top"];
        offset_a = tex_coords[0].first;
        offset_b = tex_coords[0].second;
        iter = 0;

        for (auto& pair : tex_coords)
        {
            iter++;
            if (!(skin_to_apply.RGBA_2d_array[pair.first][pair.second] == skin_to_apply.colors[color_id]))
                continue;

            cube_pos = starting_pos;
            cube_pos.x -= pair.first - offset_a - size.x + 1;
            cube_pos.y -= pair.second - offset_b;


            if (iter == 1) {
                cube_pos.y += 1;
                cube_pos.z += 1;
                output_text += make_one_third_of_rectangle(cube_pos, { 1.f,-1.f,-1.f });
            }
            else if (iter == size.x) {
                cube_pos.x += 1;
                cube_pos.y += 1;
                cube_pos.z += 1;
                output_text += make_one_third_of_rectangle(cube_pos, { -1.f,-1.f,-1.f });
                
            }
            else if (iter == size.x * (size.y - 1) + 1) {
                cube_pos.z += 1;
                output_text += make_one_third_of_rectangle(cube_pos, { 1.f,1.f,-1.f });
            }
            else if (iter == size.x * size.y) {
                cube_pos.x += 1;
                cube_pos.z += 1;
                output_text += make_one_third_of_rectangle(cube_pos, { -1.f,1.f,-1.f });
            }
            else if (iter < size.x) {
                cube_pos.z += 1;
                output_text += make_side1_rectangle(cube_pos, { 1.f,1.f,-1.f });
            }
            else if (iter % static_cast<int>(size.x) == 0) {
                cube_pos.x += 1;
                cube_pos.y += 1;
                cube_pos.z += 1;
                output_text += make_side2_rectangle(cube_pos, {-1.f,-1.f,-1.f });
            }
            else if (iter % static_cast<int>(size.x) == 1) {
                cube_pos.z += 1;
                output_text += make_side2_rectangle(cube_pos, { 1.f,1.f,-1.f });
            }
            else if (iter > (size.x - 1) * size.y) {
                cube_pos.x += 1;
                cube_pos.y += 1;
                cube_pos.z += 1;
                output_text += make_side1_rectangle(cube_pos, { -1.f,-1.f,-1.f });
            }
            else
                output_text += make_rectangle(cube_pos, { 1.f,1.f,1.f });
            
        }

        tex_coords = coordinates["Bottom"];
        offset_a = tex_coords[0].first;
        offset_b = tex_coords[0].second;
        iter = 0;

        for (auto& pair : tex_coords)
        {
            iter++;
            if (!(skin_to_apply.RGBA_2d_array[pair.first][pair.second] == skin_to_apply.colors[color_id]))
                continue;

            cube_pos = starting_pos;
            cube_pos.x -= pair.first - offset_a - size.x + 1;
            cube_pos.y -= pair.second - offset_b;
            cube_pos.z -= size.z - 1;
            

            if (iter == 1) {
                cube_pos.y += 1;
                output_text += make_one_third_of_rectangle(cube_pos, { 1.f,-1.f,1.f });
            }
            else if (iter == size.x) {
                cube_pos.x += 1;
                cube_pos.y += 1;
                output_text += make_one_third_of_rectangle(cube_pos, { -1.f,-1.f,1.f });
            }
            else if (iter == size.x * (size.y - 1) + 1) {
                output_text += make_one_third_of_rectangle(cube_pos, { 1.f,1.f,1.f });
            }
            else if (iter == size.x * size.y) {
                cube_pos.x += 1;
                output_text += make_one_third_of_rectangle(cube_pos, { -1.f,1.f,1.f });
            }
            else if (iter < size.x) {
                output_text += make_side1_rectangle(cube_pos, { 1.f,1.f,1.f });
            }
            else if (iter % static_cast<int>(size.x) == 0) {
                cube_pos.y += 1;
                cube_pos.x += 1;
                output_text += make_side2_rectangle(cube_pos, { -1.f,-1.f,1.f });
            }
            else if (iter % static_cast<int>(size.x) == 1) {
                output_text += make_side2_rectangle(cube_pos, { 1.f,1.f,1.f });
            }
            else if (iter > (size.x - 1) * size.y) {
                cube_pos.y += 1;
                cube_pos.x += 1;
                output_text += make_side1_rectangle(cube_pos, { -1.f,-1.f,1.f });
            }
            else
                output_text += make_rectangle(cube_pos, { 1.f,1.f,1.f });
        }
    }

    std::string get_string() const
    {
        return output_text;
    }
};
