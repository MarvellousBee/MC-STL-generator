#pragma once
#include "XValues.h"


Point3f get_normal(std::vector<Point3f> v)
{
    auto V{ v[1] - v[0] };
    auto W{ v[2] - v[0] };

    Point3f N{
        (V.y * W.z) - (V.z * W.y)
    ,   (V.z * W.x) - (V.x * W.z)
    ,   (V.x * W.y) - (V.y * W.x)
    };
    return N / N.sum();
}

std::string make_facet(std::vector<Point3f> v)
{
    assert(v.size() == 3);
    std::string output{ "  facet normal " };

    auto nrml = get_normal(v);
    output.append(std::to_string(nrml.x) + ' ' + std::to_string(nrml.y) + ' ' + std::to_string(-nrml.z));

    output.append("\n  outer loop\n");

    for (int i{ 0 }; i < 3; i++)
        output.append("    vertex   "
            + std::to_string(v[i].x) + ' '
            + std::to_string(v[i].y) + ' '
            + std::to_string(v[i].z) + '\n'
        );
    output.append("  endloop\n");
    output.append("endfacet\n");

    return output;
}

std::string make_plane(
    const Point3f p
    , const long float len1
    , const long float len2
    , const int side = 0
)
{
    std::string output{ "" };
    auto px{ p };
    auto py{ p };
    auto p2{ p };
    switch (side)
    {
    case 0:
        px.x += len1;
        py.y += len2;
        p2.x += len1;
        p2.y += len2;
        break;
    case 1:
        px.x += len1;
        py.z += len2;
        p2.x += len1;
        p2.z += len2;
        break;
    case 2:
        px.y += len1;
        py.z += len2;
        p2.y += len1;
        p2.z += len2;
        break;
    }
    output.append(make_facet({ p , px, py }));
    output.append(make_facet({ p2, px, py }));
    return output;
}

// To be specific: makes a 3D rectangular prism
std::string make_rectangle(const Point3f& origin, const Point3f& size)
{
    std::string output{ "" };
    auto temp{ origin };

    temp.z += size.z;
    output.append(make_plane(origin, size.x, size.y, 0));
    output.append(make_plane(temp, size.x, size.y, 0));

    temp = origin;
    temp.y += size.y;
    output.append(make_plane(origin, size.x, size.z, 1));
    output.append(make_plane(temp, size.x, size.z, 1));

    temp = origin;
    temp.x += size.x;
    output.append(make_plane(temp, size.y, size.z, 2));
    output.append(make_plane(origin, size.y, size.z, 2));

    return output;
}
std::string make_half_rectangle(const Point3f& origin, const Point3f& size)
{
    std::string output{ "" };
    auto temp{ origin };

    make_facet({
            { origin }
        ,   { origin.x + size.x, origin.y, origin.z }
        ,   { origin.x, origin.y + size.y, origin.z }
        });
    return output;
}