#pragma once
#include <fstream>
#include <cmath>

template <class T>
void add_vectors(std::vector<T>& lhs, std::vector<T>& rhs)
{
    lhs.insert(lhs.end(), rhs.begin(), rhs.end());
}

template <class T>
void add_vectors(std::vector<T>& lhs, std::vector<T>&& rhs)
{
    lhs.insert(lhs.end(), rhs.begin(), rhs.end());
}

template <class T>// TODO : look up how to point to a function
std::string test_all(T function)
{
    std::string out{ "" };
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

void make_file(std::vector<StlTemplates::Facet>& color, std::string file_name)
{
    std::ofstream outf{ "STL_output/" + file_name + ".stl" };
    if (!outf) {
        std::cerr << "Could not make " + file_name + ".stl!\n";
        throw 1;
    }
    outf << "solid ASCII\n";
    for (auto& i : color)
        outf << make_facet_string(i);
}
// returns if there was enough data to create a file
bool make_file_with_id(std::vector<StlTemplates::Facet>& color, int& file_id)
{
    if (color.size() == 0)
        return false;

    std::ofstream outf{ "STL_output/EX" + std::to_string(file_id) + ".stl" };
    if (!outf) {
        std::cerr << "Could not make " + std::to_string(file_id) + ".stl!\n";
        throw 1;
    }
    outf << "solid ASCII\n";
    for (auto& i : color)
        outf << make_facet_string(i);
    return true;
}
void delete_all_files_in_STL_output()
{
    for (int i{ 0 }; i < 64 * 64; i++)
    {
        std::string file{ "STL_output/EX" + std::to_string(i) + ".stl" };
        std::filesystem::remove(file);
    }
    std::string file{ "STL_output/INFILL.stl" };
    std::filesystem::remove(file);
}

void copy_file(const std::string input, const std::string output)
{
    std::ifstream  src(input, std::ios::binary);
    std::ofstream  dst(output, std::ios::binary);

    dst << src.rdbuf();
}

namespace Python
{
    void print_color(const int a, const int b, const int c, const bool& show_rgb_brackets);
}

void make_color_files_and_print_colors(Skin& skin, std::vector<std::vector<StlTemplates::Facet>>& triangles, const bool& show_rgb_brackets)
{
    int file_id{ 1 };

    for (int i{ 0 }; i < skin.colors.size(); i++)
        if (skin.colors[i][3] != 0)
        {
            // On some skins, a color appears only under an outer layer.
            // This color is recognized, but not represented. make_file_with_id() returns false when it detects such a color.
            bool success = make_file_with_id(triangles[i], file_id);
            if (success) {
                std::cout << file_id << std::string(2 - trunc(log10(file_id)), ' ');
                Python::print_color(skin.colors[i][0], skin.colors[i][1], skin.colors[i][2], show_rgb_brackets);
                ++file_id;
            }
        }
}