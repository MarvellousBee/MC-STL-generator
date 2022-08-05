#pragma once
#include <vector>
#include <string>
#include <cassert>
#include "XValues.h"
#include "StlTemplates.h"

std::string make_facet_string(std::vector<Point3f> v, Point3f nrml)
{
    assert(v.size() == 3);
    std::string output{ "  facet normal " };
    output.append(std::to_string(nrml.x) + ' ' + std::to_string(nrml.y) + ' ' + std::to_string(-nrml.z));

    output.append("\n  outer loop\n");

    constexpr static float size_multiplier{ 3.25f };

    for (int i{ 0 }; i < 3; i++)
        output.append("    vertex   "
            + std::to_string(v[i].x * size_multiplier) + ' '
            + std::to_string(v[i].y * size_multiplier) + ' '
            + std::to_string(v[i].z * size_multiplier) + '\n'
        );
    output.append("  endloop\n");
    output.append("endfacet\n");

    return output;
}

std::string make_facet_string(StlTemplates::Facet t)
{
    return make_facet_string(t.coordinates, t.normal);
}

StlTemplates::Facet make_facet_object(std::vector<Point3f> v, Point3f nrml)
{
    assert(v.size() == 3);
    StlTemplates::Facet output;

    constexpr static float size_multiplier{ 3.25f };

    output.normal = nrml;

    for (int i{ 0 }; i < 3; i++)
        output.coordinates.push_back({
              v[i].x * size_multiplier
            , v[i].y * size_multiplier
            , v[i].z * size_multiplier
            }
        );

    return output;
}

// To be specific: makes a 3D rectangular prism
std::vector<StlTemplates::Facet> make_object(const Point3f& origin, const std::string& type)
{
    //assert(StlTemplates::template_map.find(type) != StlTemplates::template_map.end());

    std::vector<StlTemplates::Facet> output;
    auto templ { StlTemplates::template_map[type] };

    for (auto& fct : templ)
    {
        for (auto& point : fct.coordinates)
        {
            point.x += origin.x;
            point.y += origin.y;
            point.z += origin.z;
        }
        output.push_back(make_facet_object(fct.coordinates, fct.normal));
    }
    assert(output.size() > 0);
    return output;
}

std::vector<StlTemplates::Facet> make_rectangle(const Point3f& origin, const Point3f& size)
{
    std::vector<StlTemplates::Facet> output;

    for (int x{0}; x < size.x;x++)
        for (int y{ 0 }; y < size.y; y++)
            for (int z{ 0 }; z < size.z; z++)
            {
                auto temp{ origin };
                temp.x += x;
                temp.y += y;
                temp.z += z;
                auto obj{ make_object(temp, "cube") };
                output.insert(output.end(), obj.begin(), obj.end());
            }
    return output;
}