#pragma once
#include <unordered_map>
#include <Python.h>

namespace Python
{
    namespace
    {
        static PyObject* Pymodule;
        PyObject* get_fun(char* name)
        {
            return PyObject_GetAttrString(Pymodule, name);
        }
    }
    
    void add_script_path()
    {
        Pymodule = PyImport_ImportModule("shortcut");
        PyErr_Print();
    }

    void reduce_colors(int target_n_of_colors)
    {
        PyObject* kmeans = PyObject_GetAttrString(Pymodule, (char*)"kmeans_improved_from_path");
        PyObject_CallFunction(kmeans, "si", "skin.png", target_n_of_colors);
        PyErr_Print();
    }

    //void get_and_store_unique_colors()
    //{
    //    PyObject* uc = PyObject_GetAttrString(Pymodule, (char*)"get_and_store_unique_colors");
    //    PyObject* N_of_colors{ PyObject_CallFunction(uc, NULL) };
    //    PyErr_Print();
    //}
    void convert_skin_to_txt()
    {
        PyObject* uc = PyObject_GetAttrString(Pymodule, (char*)"convert_skin_to_txt");
        PyObject* N_of_colors{ PyObject_CallFunction(uc, NULL) };
        PyErr_Print();
    }
    void print_color(const int a, const int b, const int c, const bool& show_rgb_brackets)
    {
        PyObject* uc = PyObject_GetAttrString(Pymodule, (char*)"print_color_distinct");
        std::cout << ' ';
        PyObject* N_of_colors{ PyObject_CallFunction(uc, "iii", a,b,c) };
        if (show_rgb_brackets)
            std::cout << '\t' << "rgb(" << a << ", " << b << ", " << c << ")\n";
        else
            std::cout << '\t' << a << '\t' << b << '\t' << c << '\n';
            
        PyErr_Print();
    }
}