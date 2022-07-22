#pragma once
#include <unordered_map>
#include <Python.h>

namespace Python
{
    namespace
    {
        static PyObject* Pymodule;
        PyObject* get_fun(const char* name)
        {
            return PyObject_GetAttrString(Pymodule, (char*)name);
        }
    }
    
    void add_script_path()
    {
        Pymodule = PyImport_ImportModule("shortcut");
        PyErr_Print();
    }

    void reduce_colors(int target_n_of_colors)
    {
        PyObject* kmeans{ get_fun("kmeans_improved_from_path") };
        PyObject_CallFunction(kmeans, "si", "skin.png", target_n_of_colors);
        PyErr_Print();
    }
    void convert_skin_to_txt()
    {
        PyObject* fun{ get_fun("convert_skin_to_txt") };
        PyObject_CallFunction(fun, NULL);
        PyErr_Print();
    }
    void print_color(const int a, const int b, const int c, const bool& show_rgb_brackets)
    {
        PyObject* fun{ get_fun("print_color_distinct") };
        PyObject_CallFunction(fun, "iii", a,b,c);
        if (show_rgb_brackets)
            std::cout << '\t' << "rgb(" << a << ", " << b << ", " << c << ")\n";
        else
            std::cout << '\t' << a << '\t' << b << '\t' << c << '\n';
            
        PyErr_Print();
    }
    void edit_skin(const bool& merge_colors, const bool& pause_to_edit, const bool& show_rgb_brackets, const int& max_colors)
    {
        if (merge_colors)
            Python::reduce_colors(max_colors);
        else
            copy_file("skin.png", "output.png");
        if (pause_to_edit)
        {
            int a{ 0 };
            std::cin >> a;
        }
    }
}