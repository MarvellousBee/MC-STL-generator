#pragma once

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

void make_file(std::vector<StlTemplates::STLtriangle>& color, std::string file_name)
{
    std::ofstream outf{ "STL_output/" + file_name + ".stl" };
    if (!outf) {
        std::cerr << "Could not make " + file_name + ".stl!\n";
        throw 1;
    }
    outf << "solid ASCII\n";
    for (auto& i : color)
        outf << make_facet(i);
}

void make_file_with_id(std::vector<StlTemplates::STLtriangle>& color, int& file_id)
{
    std::ofstream outf{ "STL_output/EX" + std::to_string(file_id) + ".stl" };
    if (!outf) {
        std::cerr << "Could not make " + std::to_string(file_id) + ".stl!\n";
        throw 1;
    }
    outf << "solid ASCII\n";
    for (auto& i : color)
        outf << make_facet(i);
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