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
    // If an outer part of the skin covers EVERY pixel of a given color,
    // then we ignore that color.
    const int max_colors{ 15 };
    const bool merge_colors{ false };
    const bool pause_to_edit{ false };
    const bool show_rgb_brackets{ true };

    Py_Initialize();
    Python::add_script_path();
    Python::edit_skin(merge_colors
                    , pause_to_edit
                    , show_rgb_brackets
                    , max_colors);
    Python::convert_skin_to_txt();

    auto skin_documentation{ get_skin_structure("plane_coords.txt") };
    auto custom_skin_colors{ get_custom_skin_colors("text_output.txt") };
    auto taken_pixels{ init_taken_pixels_storage() };
    Skin skin{ custom_skin_colors, skin_documentation };
    auto triangles{ init_facets_storage(skin)};
    construct_skin(skin, triangles, taken_pixels);
    auto infill{ get_infill(skin) };

    delete_all_files_in_STL_output();
    make_color_files_and_print_colors(skin, triangles, show_rgb_brackets);
    make_file(infill, "INFILL");

    Py_Finalize();

    return EXIT_SUCCESS;
}