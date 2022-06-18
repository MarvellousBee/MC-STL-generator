#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>
#include <cmath>

#include "stl generation.h"



int main()
{
    std::ofstream outf{ "Input.stl" };

    if (!outf)
    {
        std::cerr << "Could not open the file!\n";
        return 1;
    }
    outf << "solid ASCII\n";

    outf << make_rectangle({  0.f,  0.f,  0.f }, { 5.f, 10.f, 15.f });
    outf << make_rectangle({ -5.f,  0.f,  0.f }, { 5.f, 10.f,  2.f });
    outf << make_rectangle({  0.f, 10.f,  5.f }, { 5.f, 10.f,  8.f });

    return 0;
}