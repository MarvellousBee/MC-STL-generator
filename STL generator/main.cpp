#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>
#include <cmath>
#include <memory>
#include <filesystem>

#include "stl generation.h"
#include "XValues.h"
#include "SkinStructure.h"
#include "Skin.h"
#include "SkinStructure.h"
#include "BodyPart.h"
#include "StlTemplates.h"
#include "Utils.h"
#include "PythonRunner.h"

int main()
{   
    bool merge_colors{ true };
    bool pause_to_edit{ false };
    bool show_rgb_brackets{ true };

    // If an outer part of the skin covers EVERY pixel of a given color,
    // then we ignore that color.
    int max_colors{ 15 };

    Py_Initialize();
    Python::add_script_path();
    if (merge_colors)
        Python::reduce_colors(max_colors);
    else
        copy_file("skin.png", "output.png");
    if (pause_to_edit)
    {
        int a{ 0 };
        std::cin >> a;
    }
    //Python::get_and_store_unique_colors();
    Python::convert_skin_to_txt();

    // Initialize the custom skin and its official documentation
    auto skin_documentation{ get_skin_structure("plane_coords.txt") };
    auto custom_skin_colors{ get_custom_skin_colors("text_output.txt") };
    auto taken_pixels{ init_taken_pixels_storage() };
    Skin skin{ custom_skin_colors, skin_documentation };
    auto triangles{ init_facets_storage(skin)};
    
    //Construct the skin

    // Layer 2
    // is dealt with first, so it covers up whatever it needs to cover first.
    for (int i{ 0 }; i < skin.colors.size(); i++)
        if (skin.colors[i][3] != 0)
            add_outer_layer(triangles[i], skin, i, taken_pixels);
    
    // Layer 1
    for (int i{ 0 }; i < skin.colors.size(); i++)
        if (skin.colors[i][3] != 0)
            add_inner_layer(triangles[i], skin, i, taken_pixels);

    std::vector<StlTemplates::STLtriangle> infill;
    add_infill(infill, skin);

    //create output STL files
    delete_all_files_in_STL_output();

    int file_id{ 1 };

    for (int i{ 0 }; i < skin.colors.size(); i++)
        if (skin.colors[i][3] != 0)
        {
            // On some skins, a color appears only under an outer layer.
            // This color is recognized, but not represented. make_file_with_id() returns false when it detects such a color.
            bool success = make_file_with_id(triangles[i], file_id);
            if (success) {
                std::cout << file_id;
                Python::print_color(skin.colors[i][0], skin.colors[i][1], skin.colors[i][2], show_rgb_brackets);
                ++file_id;
            }
        }
    make_file(infill, "INFILL");

    
    Py_Finalize();
    return EXIT_SUCCESS;
}