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

int main()
{   
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
    int file_id{ 0 };

    //for (int i{ 0 }; i < skin.colors.size(); i++)
    //    if (skin.colors[i] == BodyPart::white) // white first, if exists
    //        make_file_with_id(triangles[i], ++file_id);

    for (int i{ 0 }; i < skin.colors.size(); i++)
        if (skin.colors[i][3] != 0) // other colors
            make_file_with_id(triangles[i], ++file_id);

    make_file(infill, "INFILL");

    return EXIT_SUCCESS;
}