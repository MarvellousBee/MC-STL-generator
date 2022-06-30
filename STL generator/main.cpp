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
#include "SkinStructure.h"
#include "BodyPart.h"
#include "stl_templates.h"

template <class T>//too lazy to remember/look up how to point to a function
std::string test_all(T function)
{
    std::string out{""};
    out.append(function({ 0.f,0.f,0.f }, { 1.f,1.f,1.f }));
    out.append(function({ 0.f,2.f,0.f }, { -1.f,1.f,1.f }));
    out.append(function({ 0.f,4.f,0.f }, { 1.f,-1.f,1.f }));
    out.append(function({ 0.f,6.f,0.f }, { -1.f,-1.f,1.f }));

    out.append(function({ 5.f,2.f,0.f }, { 1.f,1.f,-1.f }));
    out.append(function({ 5.f,4.f,0.f }, { -1.f,1.f,-1.f }));
    out.append(function({ 5.f,6.f,0.f }, { 1.f,-1.f,-1.f }));
    out.append(function({ 5.f,8.f,0.f }, { -1.f,-1.f,-1.f }));
    return out;
}

int main()
{
    std::vector<std::string> mc_skin_documentation{};
    mc_skin_documentation.reserve(constants::num_of_pixels * constants::num_of_pixels);
    std::string temp;
    std::ifstream skin_documentation_file("text_output.txt");
    int i{ -1 };
    while (getline(skin_documentation_file, temp))
        mc_skin_documentation.push_back(temp);//(*mc_skin_documentation)[i] = temp;
    skin_documentation_file.close();
    std::ifstream text_data("plane_coords.txt");
    Skin skin{ mc_skin_documentation, text_data };
    text_data.close();

    
    std::map<std::string, std::vector<Point3f>> taken_pixels{
        {"Front", {}}
    ,   {"Back", {}}
    ,   {"Left", {}}
    ,   {"Right", {}}
    ,   {"Top", {}}
    ,   {"Bottom", {}}
    };

    // excessive, but does not matter
    for (auto& [side, vec] : taken_pixels)
        vec.reserve(64 * 64);

    std::vector<std::vector<StlTemplates::STLtriangle>> triangles;

    triangles.resize(skin.colors.size());
    for (auto& vec : triangles)
        vec.reserve(64 * 64);

    //int file_id{ -1 };
    for (int i{ 0 }; i < skin.colors.size(); i++)
    {
        if (skin.colors[i][3] == 0)
            continue;
        // Layer 2
        // is dealt with first, so it covers up whatever it needs to cover first.
        add_vectors(triangles[i], BodyPart{"Helm", {-2.f,0.f,34.f}, skin, i, taken_pixels, true}.get_triangles());
        
        add_vectors(triangles[i], BodyPart{ "Torso Layer 2",  { 0.f,0.f,26.f  }, skin, i, taken_pixels, true }.get_triangles()); // size:  {  4.f,  8.f, 12.f  }
        add_vectors(triangles[i], BodyPart{ "Right Leg Layer 2",  { 0.f, 0.f, 14.f }, skin, i, taken_pixels, true }.get_triangles());// size:  {  4.f,  4.f,  12.f  }
        add_vectors(triangles[i], BodyPart{ "Left Leg Layer 2" ,  { 0.f,-4.f, 14.f }, skin, i, taken_pixels, true }.get_triangles());// size:  {  4.f,  4.f,  12.f  }
        add_vectors(triangles[i], BodyPart{ "Right Arm Layer 2" ,  { 0.f,  4.f, 26.f }, skin, i, taken_pixels, true }.get_triangles()); // size:  {  4.f,  4.f,  12.f  }
        add_vectors(triangles[i], BodyPart{ "Left Arm Layer 2" ,   { 0.f,-8.f, 26.f }, skin, i, taken_pixels, true }.get_triangles()); // size:  {  4.f,  4.f,  12.f  }
    }
    //file_id = -1;
    for (int i{ 0 }; i < skin.colors.size(); i++)
    {
        if (skin.colors[i][3] == 0)
            continue;
        // Layer 1
        add_vectors(triangles[i], BodyPart{ "Head", { -2.f,0.f,34.f }, skin, i, taken_pixels }.get_triangles()); // size:  {  8.f,  8.f,  8.f  }
        add_vectors(triangles[i], BodyPart{ "Torso",  { 0.f,0.f,26.f  }, skin, i, taken_pixels }.get_triangles()); // size:  {  4.f,  8.f, 12.f  }
        add_vectors(triangles[i], BodyPart{ "Right Leg",  { 0.f, 0.f, 14.f }, skin, i, taken_pixels }.get_triangles());// size:  {  4.f,  4.f,  12.f  }
        add_vectors(triangles[i], BodyPart{ "Left Leg" ,  { 0.f,-4.f, 14.f }, skin, i, taken_pixels }.get_triangles());// size:  {  4.f,  4.f,  12.f  }
        add_vectors(triangles[i], BodyPart{ "Right Arm" ,  { 0.f,  4.f, 26.f }, skin, i, taken_pixels }.get_triangles()); // size:  {  4.f,  4.f,  12.f  }
        add_vectors(triangles[i], BodyPart{ "Left Arm" ,   { 0.f,-8.f, 26.f }, skin, i, taken_pixels }.get_triangles()); // size:  {  4.f,  4.f,  12.f  }

    }

    int file_id{ -1 };
    for (int i{ 0 }; i < skin.colors.size(); i++)
    {
        if (skin.colors[i][3] == 0)
            continue;
        std::ofstream outf{ "STL_output/EX" + std::to_string(++file_id) + ".stl" };
        if (!outf) {
            std::cerr << "Could not open EX" + std::to_string(file_id) + ".stl for WRITING!\n";
            return 1;
        }
        outf << "solid ASCII\n";
        for (auto& i : triangles[i])
            outf << make_facet(i);
    }
    
    
    //std::ofstream outf{ "STL_output/test.stl" };
    //if (!outf) {
    //    std::cerr << "test.stl for writing!\n";
    //    return 1;
    //}
    //outf << "solid ASCII\n";
    //outf << make_rectangle({ 0.f,  0.f,  0.f }, { 1.f, 1.f, 1.f });
    //outf << test_all(make_rectangle);
    //outf << make_object({ 0,0,0 }, "ru quarter cube 1");
    return 0;
}