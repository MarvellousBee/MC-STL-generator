#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>
#include <cmath>
#include <memory>
#include <filesystem>

#include "StlGeneration.h"
#include "SkinStructure.h"
#include "Skin.h"
#include "BodyPart.h"
#include "StlTemplates.h"
#include "Utils.h"
#include "PythonRunner.h"
#include "3mf.h"
#include "Settings.h"

int main()
{   
    Py_Initialize();
    Python::add_script_path();
    Python::edit_skin(Settings::merge_colors
                    , Settings::pause_to_edit
                    , Settings::show_rgb_brackets
                    , Settings::max_colors);
    Python::convert_skin_to_txt();

    auto skin_documentation{ get_skin_structure("plane_coords.txt") };
    auto custom_skin_colors{ get_custom_skin_colors("text_output.txt") };
    auto taken_pixels{ init_taken_pixels_storage() };
    Skin skin{ custom_skin_colors, skin_documentation };
    auto triangles{ init_facets_storage(skin) };
    construct_skin(skin, triangles, taken_pixels);

    delete_all_files_in_STL_output();
    delete_3mf_output();
    if (strcmp (Settings::desired_file_type, "stl") == 0)
    {
        auto infill{ get_infill(skin) };
        make_color_files_and_print_colors(skin, triangles, Settings::show_rgb_brackets);
        make_file(infill, "INFILL");
    }
    else if (strcmp(Settings::desired_file_type, "3mf") == 0)
        make_3mf_file(skin, triangles);
    else {
        std::cerr << "[ERROR] Invalid output file type: " << Settings::desired_file_type;
        return EXIT_FAILURE;
    }

    Py_Finalize();

    return EXIT_SUCCESS;
}