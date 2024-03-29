#pragma once
#include <vector>
#include <map>
#include <string>

#include "Skin.h"
#include "XValues.h"
#include "StlGeneration.h"
#include "Utils.h"

//given 2 x/y coordinates, returns all pixel coordinates of a rectangle constructed between those 2 points
std::vector<std::pair<int, int>> all_coordinates(Values4 input)
{
    std::vector<std::pair<int, int>> output;
    for (long double x{ input[0] }; x < input[2]; x++)
        for (long double y{ input[1] }; y < input[3]; y++)
            output.push_back({ y, x });

    return output;
}

bool is_pos_taken(const std::vector<Point3>& taken_coordinates, const Point3& cube_pos)
{
    return std::find(taken_coordinates.begin(), taken_coordinates.end(), cube_pos) != taken_coordinates.end();
}

class BodyPart
{
    std::vector<StlTemplates::Facet> triangles;
public:
    const Point3 pos;
    const Skin& skin;
    // add a non-colored box inside the part to make it sturdier
    constexpr static Values4 white{ 255, 255, 255, 255 };
    
    auto get_triangles()
    {
        return triangles;
    }

    BodyPart(std::string part_name
        , const Point3 _pos
        , const Skin& skin_to_apply
        , const bool& is_infill = false)
        : pos(_pos)
        , skin(skin_to_apply)
    {
        std::vector<StlTemplates::Facet> temp_triangles;
        std::map<std::string, std::vector<std::pair<int, int>>> coordinates{
            { "Front" , all_coordinates(skin_to_apply.skin_structure.at(part_name).at("Front")) }
        ,   { "Back"  , all_coordinates(skin_to_apply.skin_structure.at(part_name).at("Back")) }
        ,   { "Left"  , all_coordinates(skin_to_apply.skin_structure.at(part_name).at("Left")) }
        ,   { "Right" , all_coordinates(skin_to_apply.skin_structure.at(part_name).at("Right")) }
        ,   { "Bottom", all_coordinates(skin_to_apply.skin_structure.at(part_name).at("Bottom")) }
        ,   { "Top"   , all_coordinates(skin_to_apply.skin_structure.at(part_name).at("Top")) }
        };

        Point3 size{
            coordinates["Left"].back().second - coordinates["Left"][0].second + 1,
            coordinates["Front"].back().second - coordinates["Front"][0].second + 1,
            coordinates["Front"].back().first - coordinates["Front"][0].first + 1
        };

        auto inner_cube_pos{ pos };
        inner_cube_pos.x += 1;
        inner_cube_pos.y -= size.y - 2;
        inner_cube_pos.z -= size.z - 2;
        auto inner_cube_size{ size };
        inner_cube_size.x -= 2;
        inner_cube_size.y -= 2;
        inner_cube_size.z -= 2;
        auto obj{ make_rectangle(inner_cube_pos, inner_cube_size) };
        temp_triangles.insert(triangles.end(), obj.begin(), obj.end());

        for (int i{ 0 }; i < temp_triangles.size(); i++)
            triangles.push_back(temp_triangles[i]);
    }

    BodyPart(std::string part_name
        , const Point3 _pos
        , const Skin& skin_to_apply
        , const int& color_id
        , std::map<std::string
        , std::vector<Point3>>& taken_coordinates
        , const bool& is_outer = false)
        : pos(_pos)
        , skin(skin_to_apply)
    {

        std::map<std::string, std::vector<std::pair<int, int>>> coordinates{
            { "Front" , all_coordinates(skin_to_apply.skin_structure.at(part_name).at("Front")) }
        ,   { "Back"  , all_coordinates(skin_to_apply.skin_structure.at(part_name).at("Back")) }
        ,   { "Left"  , all_coordinates(skin_to_apply.skin_structure.at(part_name).at("Left")) }
        ,   { "Right" , all_coordinates(skin_to_apply.skin_structure.at(part_name).at("Right")) }
        ,   { "Bottom", all_coordinates(skin_to_apply.skin_structure.at(part_name).at("Bottom")) }
        ,   { "Top"   , all_coordinates(skin_to_apply.skin_structure.at(part_name).at("Top")) }
        };

        Point3 size{
            coordinates["Left"].back().second - coordinates["Left"][0].second + 1,
            coordinates["Front"].back().second - coordinates["Front"][0].second + 1,
            coordinates["Front"].back().first - coordinates["Front"][0].first + 1
        };
        std::vector<StlTemplates::Facet> temp_triangles;

        //std::cout << skin_to_apply.colors[color_id];
        auto starting_pos{ pos };
        auto cube_pos{ pos };

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
            

            if (is_pos_taken(taken_coordinates["Front"], cube_pos))
                continue;
            else
                taken_coordinates["Front"].push_back(cube_pos);
  
            if (iter == 1) 
                add_vectors(temp_triangles, make_object(cube_pos, "ld quarter cube 1"));
            else if (iter == size.z)
                add_vectors(temp_triangles, make_object(cube_pos, "lu quarter cube 1"));
            else if (iter == size.z * (size.y - 1) + 1)
                add_vectors(temp_triangles, make_object(cube_pos, "rd quarter cube 1"));
            else if (iter == size.z * size.y)
                add_vectors(temp_triangles, make_object(cube_pos, "ru quarter cube 1"));
            else if (iter <= size.z)
                add_vectors(temp_triangles, make_object(cube_pos, "side half cube 1"));
            else if (iter > (size.y - 1) * size.z)
                add_vectors(temp_triangles, make_object(cube_pos, "side half cube 2"));
            else if (iter % static_cast<int>(size.z) == 1)
                add_vectors(temp_triangles, make_object(cube_pos, "down half cube 1"));
            else if (iter % int(size.z) == 0) 
                add_vectors(temp_triangles, make_object(cube_pos, "top half cube 1"));
            else
                add_vectors(temp_triangles, make_object(cube_pos, "cube"));
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

            if (is_pos_taken(taken_coordinates["Back"], cube_pos))
                continue;
            else
                taken_coordinates["Back"].push_back(cube_pos);

            if (iter == 1)
                add_vectors(temp_triangles, make_object(cube_pos, "ld quarter cube 3"));
            else if (iter == size.z)
                add_vectors(temp_triangles, make_object(cube_pos, "lu quarter cube 3"));
            else if (iter == size.z * (size.y - 1) + 1)
                add_vectors(temp_triangles, make_object(cube_pos, "rd quarter cube 3"));
            else if (iter == size.z * size.y)
                add_vectors(temp_triangles, make_object(cube_pos, "ru quarter cube 3"));
            else if (iter <= size.z) 
                add_vectors(temp_triangles, make_object(cube_pos, "side half cube 3"));
            else if (iter > (size.y - 1) * size.z) 
                add_vectors(temp_triangles, make_object(cube_pos, "side half cube 4"));
            else if (iter % static_cast<int>(size.z) == 1) 
                add_vectors(temp_triangles, make_object(cube_pos, "down half cube 3"));
            else if (iter % int(size.z) == 0) 
                add_vectors(temp_triangles, make_object(cube_pos, "top half cube 3"));
            else
                add_vectors(temp_triangles, make_object(cube_pos, "cube"));
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

            if (is_pos_taken(taken_coordinates["Left"], cube_pos))
                continue;
            else
                taken_coordinates["Left"].push_back(cube_pos);

            if (iter == 1)
                add_vectors(temp_triangles, make_object(cube_pos, "ld quarter cube 2"));
            else if (iter == size.z)
                add_vectors(temp_triangles, make_object(cube_pos, "lu quarter cube 2"));
            else if (iter == size.z * (size.x - 1) + 1)
                add_vectors(temp_triangles, make_object(cube_pos, "rd quarter cube 2"));
            else if (iter == size.z * size.x)
                add_vectors(temp_triangles, make_object(cube_pos, "ru quarter cube 2"));
            else if (iter <= size.z) 
                add_vectors(temp_triangles, make_object(cube_pos, "side half cube 4"));
            else if (iter > (size.x - 1) * size.z)
                add_vectors(temp_triangles, make_object(cube_pos, "side half cube 1"));
            else if (iter % static_cast<int>(size.z) == 1)
                add_vectors(temp_triangles, make_object(cube_pos, "down half cube 4"));
            else if (iter % int(size.z) == 0)
                add_vectors(temp_triangles, make_object(cube_pos, "top half cube 4"));
            else
                add_vectors(temp_triangles, make_object(cube_pos, "cube"));
            
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

            if (is_pos_taken(taken_coordinates["Right"], cube_pos))
                continue;
            else
                taken_coordinates["Right"].push_back(cube_pos);

            if (iter == 1)
                add_vectors(temp_triangles, make_object(cube_pos, "ld quarter cube 4"));
            else if (iter == size.z)
                add_vectors(temp_triangles, make_object(cube_pos, "lu quarter cube 4"));
            else if (iter == size.z * (size.x - 1) + 1)
                add_vectors(temp_triangles, make_object(cube_pos, "rd quarter cube 4"));
            else if (iter == size.z * size.x)
                add_vectors(temp_triangles, make_object(cube_pos, "ru quarter cube 4"));
            else if (iter <= size.z) 
                add_vectors(temp_triangles, make_object(cube_pos, "side half cube 2"));
            else if (iter > (size.x - 1) * size.z)
                add_vectors(temp_triangles, make_object(cube_pos, "side half cube 3"));
            else if (iter % static_cast<int>(size.z) == 1)
                add_vectors(temp_triangles, make_object(cube_pos, "down half cube 2"));
            else if (iter % static_cast<int>(size.z) == 0)
                add_vectors(temp_triangles, make_object(cube_pos, "top half cube 2"));
            else
                add_vectors(temp_triangles, make_object(cube_pos, "cube"));


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
            
            if (is_pos_taken(taken_coordinates["Top"], cube_pos))
                continue;
            else
                taken_coordinates["Top"].push_back(cube_pos);

            if (iter == 1) 
                add_vectors(temp_triangles, make_object(cube_pos, "close down quarter cube"));
            else if (iter == size.x)
                add_vectors(temp_triangles, make_object(cube_pos, "mid1 down quarter cube"));
            else if (iter == size.x * (size.y - 1) + 1)
                add_vectors(temp_triangles, make_object(cube_pos, "mid2 down quarter cube"));
            else if (iter == size.x * size.y)
                add_vectors(temp_triangles, make_object(cube_pos, "far down quarter cube"));
            else if (iter < size.x)
                add_vectors(temp_triangles, make_object(cube_pos, "top half cube 4"));
            else if (iter % static_cast<int>(size.x) == 0) 
                add_vectors(temp_triangles, make_object(cube_pos, "top half cube 3"));
            else if (iter % static_cast<int>(size.x) == 1) 
                add_vectors(temp_triangles, make_object(cube_pos, "top half cube 1"));
            else if (iter > (size.y - 1) * size.x) 
                add_vectors(temp_triangles, make_object(cube_pos, "top half cube 2"));
            else
                add_vectors(temp_triangles, make_object(cube_pos, "cube"));
            
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
            
            if (is_pos_taken(taken_coordinates["Bottom"], cube_pos))
                continue;
            else
                taken_coordinates["Bottom"].push_back(cube_pos);

            if (iter == 1)
                add_vectors(temp_triangles, make_object(cube_pos, "close up quarter cube"));
            else if (iter == size.x)
                add_vectors(temp_triangles, make_object(cube_pos, "mid1 up quarter cube"));
            else if (iter == size.x * (size.y - 1) + 1)
                add_vectors(temp_triangles, make_object(cube_pos, "mid2 up quarter cube"));
            else if (iter == size.x * size.y)
                add_vectors(temp_triangles, make_object(cube_pos, "far up quarter cube"));
            else if (iter < size.x) 
                add_vectors(temp_triangles, make_object(cube_pos, "down half cube 4"));
            else if (iter % static_cast<int>(size.x) == 0) 
                add_vectors(temp_triangles, make_object(cube_pos, "down half cube 3"));
            else if (iter % static_cast<int>(size.x) == 1)
                add_vectors(temp_triangles, make_object(cube_pos, "down half cube 1"));
            else if (iter > (size.y - 1) * size.x) 
                add_vectors(temp_triangles, make_object(cube_pos, "down half cube 2"));
            else
                add_vectors(temp_triangles, make_object(cube_pos, "cube"));
        }
        
        for (int i{ 0 }; i < temp_triangles.size(); i++)
            //if (active_triangles[i] == '1')
                triangles.push_back(temp_triangles[i]);

    }
};

void add_outer_layer(std::vector<StlTemplates::Facet>& color, Skin& skin, int& iter, std::map<std::string, std::vector<Point3>>& taken_pixels)
{
    add_vectors(color, BodyPart{ "Helm", {-2.f,0.f,34.f}, skin, iter, taken_pixels, true }.get_triangles());
    add_vectors(color, BodyPart{ "Torso Layer 2",  { 0.f,0.f,26.f  }, skin, iter, taken_pixels, true }.get_triangles()); // size:  {  4.f,  8.f, 12.f  }
    add_vectors(color, BodyPart{ "Right Leg Layer 2",  { 0.f, 0.f, 14.f }, skin, iter, taken_pixels, true }.get_triangles());// size:  {  4.f,  4.f,  12.f  }
    add_vectors(color, BodyPart{ "Left Leg Layer 2" ,  { 0.f,-4.f, 14.f }, skin, iter, taken_pixels, true }.get_triangles());// size:  {  4.f,  4.f,  12.f  }
    add_vectors(color, BodyPart{ "Right Arm Layer 2" ,  { 0.f,  4.f, 26.f }, skin, iter, taken_pixels, true }.get_triangles()); // size:  {  4.f,  4.f,  12.f  }
    add_vectors(color, BodyPart{ "Left Arm Layer 2" ,   { 0.f,-8.f, 26.f }, skin, iter, taken_pixels, true }.get_triangles()); // size:  {  4.f,  4.f,  12.f  }
}

void add_inner_layer(std::vector<StlTemplates::Facet>& color, Skin& skin, int& iter, std::map<std::string, std::vector<Point3>>& taken_pixels)
{
    add_vectors(color, BodyPart{ "Head", { -2.f,0.f,34.f }, skin, iter, taken_pixels }.get_triangles()); // size:  {  8.f,  8.f,  8.f  }
    add_vectors(color, BodyPart{ "Torso",  { 0.f,0.f,26.f  }, skin, iter, taken_pixels }.get_triangles()); // size:  {  4.f,  8.f, 12.f  }
    add_vectors(color, BodyPart{ "Right Leg",  { 0.f, 0.f, 14.f }, skin, iter, taken_pixels }.get_triangles());// size:  {  4.f,  4.f,  12.f  }
    add_vectors(color, BodyPart{ "Left Leg" ,  { 0.f,-4.f, 14.f }, skin, iter, taken_pixels }.get_triangles());// size:  {  4.f,  4.f,  12.f  }
    add_vectors(color, BodyPart{ "Right Arm" ,  { 0.f,  4.f, 26.f }, skin, iter, taken_pixels }.get_triangles()); // size:  {  4.f,  4.f,  12.f  }
    add_vectors(color, BodyPart{ "Left Arm" ,   { 0.f,-8.f, 26.f }, skin, iter, taken_pixels }.get_triangles()); // size:  {  4.f,  4.f,  12.f  }
}

std::vector<StlTemplates::Facet> get_infill(Skin& skin)
{
    std::vector<StlTemplates::Facet> output;
    add_vectors(output, BodyPart{ "Head", { -2.f,0.f,34.f }, skin, true }.get_triangles()); // size:  {  8.f,  8.f,  8.f  }
    add_vectors(output, BodyPart{ "Torso",  { 0.f,0.f,26.f  }, skin, true }.get_triangles()); // size:  {  4.f,  8.f, 12.f  }
    add_vectors(output, BodyPart{ "Right Leg",  { 0.f, 0.f, 14.f }, skin, true }.get_triangles());// size:  {  4.f,  4.f,  12.f  }
    add_vectors(output, BodyPart{ "Left Leg" ,  { 0.f,-4.f, 14.f }, skin, true }.get_triangles());// size:  {  4.f,  4.f,  12.f  }
    add_vectors(output, BodyPart{ "Right Arm" ,  { 0.f,  4.f, 26.f}, skin, true }.get_triangles()); // size:  {  4.f,  4.f,  12.f  }
    add_vectors(output, BodyPart{ "Left Arm" ,   { 0.f,-8.f, 26.f }, skin, true }.get_triangles()); // size:  {  4.f,  4.f,  12.f  }
    return output;
}
void construct_skin(Skin& skin,
    std::vector<std::vector<StlTemplates::Facet>>& triangles,
    std::map<std::string, std::vector<Point3>>& taken_pixels
)
{
    // Layer 2
    // is dealt with first, so it covers up whatever it needs to cover first.
    for (int i{ 0 }; i < skin.colors.size(); i++)
        if (skin.colors[i][3] != 0)
            add_outer_layer(triangles[i], skin, i, taken_pixels);

    // Layer 1
    for (int i{ 0 }; i < skin.colors.size(); i++)
        if (skin.colors[i][3] != 0)
            add_inner_layer(triangles[i], skin, i, taken_pixels);
}