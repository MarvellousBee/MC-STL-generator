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

int main()
{
    //std::ofstream outf{ "Input.stl" };

    //if (!outf){
    //    std::cerr << "Could not open the  STL file for writing!\n";
    //    return 1;
    //}
    

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

    int file_id{ -1 };
    for (int i{ 0 }; i < skin.colors.size(); i++)
    {
        if (skin.colors[i][3] == 0)
            continue;

       

        std::ofstream outf{ "EX" + std::to_string(++file_id) + ".stl"};
        if (!outf){
            std::cerr << "Could not open  EX" + std::to_string(file_id) + ".stl for writing!\n";
            return 1;
        }

        outf << "solid ASCII\n";
        outf << BodyPart{ "Torso",  { 0.f,0.f,26.f  }, skin, i }.get_string(); // size:  {  4.f,  8.f, 12.f  }
        outf << BodyPart{ "Head" ,  { -2.f,0.f,34.f }, skin, i }.get_string(); // size:  {  8.f,  8.f,  8.f  }
        
        outf << BodyPart{ "Right Leg",  { 0.f, 0.f, 14.f }, skin, i }.get_string();// size:  {  4.f,  4.f,  12.f  }
        outf << BodyPart{ "Left Leg" ,  { 0.f,-4.f, 14.f }, skin, i }.get_string();// size:  {  4.f,  4.f,  12.f  }
       
        outf << BodyPart{ "Right Arm" ,  { 0.f,  4.f, 26.f }, skin, i }.get_string(); // size:  {  4.f,  4.f,  12.f  }
        outf << BodyPart{ "Left Arm" ,   { 0.f,-8.f, 26.f }, skin, i }.get_string(); // size:  {  4.f,  4.f,  12.f  }

        
    }

    
    std::ofstream outf{ "test.stl" };
    if (!outf) {
        std::cerr << "test.stl for writing!\n";
        return 1;
    }
    outf << "solid ASCII\n";
    outf << make_rectangle({ 0.f,  0.f,  0.f }, { 1.f, 1.f, 1.f });
    outf << make_one_third_of_rectangle({ 0.f,2.f,0.f }, { 1.f,1.f,1.f });

    

    return 0;
}