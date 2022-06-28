#pragma once
#include <vector>
#include <string>
#include <cassert>
#include "XValues.h"
#include "stl_templates.h"

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



std::string make_facet(std::vector<Point3f> v, Point3f nrml)
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
std::string make_facet(std::vector<Point3f> v)
{
    return make_facet(v, get_normal(v));
}

enum side
{
    top,
    left,
    right,
    frontback,
    bottom
};

std::string make_plane(
    const Point3f p
    , const long float len1
    , const long float len2
    , const int side
)
{
    std::string output{ "" };
    auto px{ p };
    auto py{ p };
    auto p2{ p };
    switch (side)
    {
    case side::top:
        px.x += len1;
        py.y += len2;
        p2.x += len1;
        p2.y += len2;
        output.append(make_facet({ p , px, py }, { 0,0,1 }));
        output.append(make_facet({ px, p2, py }, { 0,0,1 }));
        return output;
    case side::left:
        px.x += len1;
        py.z += len2;
        p2.x += len1;
        p2.z += len2;
        output.append(make_facet({ p , px, py }));
        output.append(make_facet({ px, p2, py }));
        return output;
    case side::right:
        px.x += len1;
        py.z += len2;
        p2.x += len1;
        p2.z += len2;
        output.append(make_facet({ p , py, px }));
        output.append(make_facet({ p2, px, py }));
        return output;
    case side::frontback:
        px.y += len1;
        py.z += len2;
        p2.y += len1;
        p2.z += len2;
        output.append(make_facet({ p , px, py }));
        output.append(make_facet({ px, p2, py }));
        return output;
    case side::bottom:
        px.x += len1;
        py.y += len2;
        p2.x += len1;
        p2.y += len2;
        output.append(make_facet({ py, p ,  px }));
        output.append(make_facet({ px, p2,  py }));
        return output;
    }
    
}

// To be specific: makes a 3D rectangular prism

std::string make_object(const Point3f& origin, const std::string& type)
{
    std::string output{ "" };
    auto templ { StlTemplates::template_map[type] };

    for (auto& fct : templ)
    {
        for (auto& point : fct.coordinates)
        {
            point.x += origin.x;
            point.y += origin.y;
            point.z += origin.z;
        }
        output += make_facet(fct.coordinates, fct.normal);
    }
    return output;
}

std::string make_rectangle(const Point3f& origin, const Point3f& size)
{
    std::string output{ "" };
    auto temp{ origin };

    temp.z += size.z;
    //output.append(make_plane(temp, size.x, size.y, side::bottom));
    output.append(make_facet({
            { origin.x, origin.y, origin.z }
        ,   { origin.x, origin.y + size.y, origin.z }
        ,   { origin.x + size.x, origin.y, origin.z }
        }));
    output.append(make_facet({
            { origin.x + size.x, origin.y + size.y, origin.z }
        ,   { origin.x + size.x, origin.y, origin.z }
        ,   { origin.x, origin.y + size.y, origin.z }
        }));
    //output.append(make_plane(origin, size.x, size.y, side::top));
    output.append(make_facet({
            { origin.x, origin.y, origin.z + size.z }
        ,   { origin.x, origin.y + size.y, origin.z + size.z }
        ,   { origin.x + size.x, origin.y, origin.z + size.z }
        }));
    output.append(make_facet({
            { origin.x + size.x, origin.y + size.y, origin.z + size.z }
        ,   { origin.x + size.x, origin.y, origin.z + size.z }
        ,   { origin.x, origin.y + size.y, origin.z + size.z }
        }));


    temp = origin;
    temp.y += size.y;
    output.append(make_plane(origin, size.x, size.z, side::left));
    output.append(make_plane(temp, size.x, size.z, side::right));

    temp = origin;
    temp.x += size.x;
    output.append(make_plane(temp, size.y, size.z, side::frontback));
    output.append(make_plane(origin, size.y, size.z, side::frontback));

    return output;
}

std::string make_half_rectangle(const Point3f& origin, const Point3f& size)
{
    std::string output{ "" };

    
    
    
    if (size.x != size.y)
    {
        //far 
        output.append(make_facet({
                { origin }
            ,   { origin.x, origin.y + size.y, origin.z }
            ,   { origin.x + size.x, origin.y, origin.z }
            }));
        //close
        output.append(make_facet({
            { origin.x, origin.y, origin.z + size.z }
        ,   { origin.x, origin.y + size.y, origin.z + size.z}
        ,   { origin.x + size.x, origin.y, origin.z + size.z}
            }));
    }
    else
    {
        //far 
        output.append(make_facet({
                { origin }
            ,   { origin.x + size.x, origin.y, origin.z }
            ,   { origin.x, origin.y + size.y, origin.z }
        }));
        //close
        output.append(make_facet({
            { origin.x, origin.y, origin.z + size.z }
        ,   { origin.x + size.x, origin.y, origin.z + size.z}
        ,   { origin.x, origin.y + size.y, origin.z + size.z}
        }));
    }

    //walls
    auto temp{ origin };
    if (size.x != size.z)
        output.append(make_plane(origin, size.x, size.z, side::right));
    else
        output.append(make_plane(origin, size.x, size.z, side::left));

    output.append(make_plane(origin, size.y, size.z, side::frontback));

    
    output.append(make_facet({
            { origin.x + size.x, origin.y, origin.z}
        ,   { origin.x + size.x, origin.y, origin.z + size.z}
        ,   { origin.x, origin.y + size.y, origin.z + size.z}
        }));
    output.append(make_facet({
            { origin.x + size.x, origin.y, origin.z}
        ,   { origin.x, origin.y + size.y, origin.z + size.z}
        ,   { origin.x, origin.y + size.y, origin.z}
        }));

    return output;
}

std::string make_side1_rectangle(const Point3f& origin, const Point3f& size)
{
    std::string output{ "" };
    auto temp{ origin };

    // rect walls
    if (size.x != size.y)
    {
        output.append(make_facet({
           origin
       ,   { origin.x, origin.y + size.y, origin.z}
       ,   { origin.x + size.x, origin.y, origin.z}
       }));
        output.append(make_facet({
           { origin.x + size.x, origin.y + size.y, origin.z}
       ,   { origin.x + size.x, origin.y, origin.z}
       ,   { origin.x, origin.y + size.y, origin.z}
       }));

    }//output.append(make_plane(origin, size.x, size.y, side::top));
    else
        output.append(make_plane(origin, size.x, size.y, side::bottom));

    temp = origin;
    temp.y += size.y;
    output.append(make_plane(origin, size.x, size.z, 1));

    // facet walls
    output.append(make_facet({
           origin
       ,   { origin.x, origin.y + size.y, origin.z}
       ,   { origin.x, origin.y, origin.z + size.z}
        }));

    output.append(make_facet({
           { origin.x + size.x, origin.y, origin.z}
       ,   { origin.x + size.x, origin.y + size.y, origin.z}
       ,   { origin.x + size.x, origin.y, origin.z + size.z}
        }));

    // angled wall
    if (size.x != size.y)
    { 
        output.append(make_facet({
               { origin.x, origin.y + size.y, origin.z}
           ,   { origin.x + size.x, origin.y + size.y, origin.z}
           ,   { origin.x + size.x, origin.y , origin.z + size.z}
            }));

        output.append(make_facet({
               { origin.x, origin.y, origin.z + size.z}
           ,   { origin.x, origin.y + size.y, origin.z}
           ,   { origin.x + size.x, origin.y , origin.z + size.z}
            }));
    }
    else
    {
        output.append(make_facet({
               { origin.x, origin.y + size.y, origin.z}
           ,   { origin.x + size.x, origin.y , origin.z + size.z}
           ,   { origin.x + size.x, origin.y + size.y, origin.z}

            }));

        output.append(make_facet({
               { origin.x, origin.y, origin.z + size.z}
           ,   { origin.x + size.x, origin.y , origin.z + size.z}
           ,   { origin.x, origin.y + size.y, origin.z}

            }));
    }
    return output;
}

std::string make_side2_rectangle(Point3f origin, const Point3f& size)
{
    std::string output{ "" };
    auto temp{ origin };

    // rect walls
    if (size.x != size.y)
    {
        output.append(make_facet({
            { origin.x, origin.y, origin.z}
        ,   { origin.x, origin.y + size.y, origin.z }
        ,   { origin.x + size.x, origin.y, origin.z}
        
            }));
        output.append(make_facet({
           { origin.x + size.x, origin.y + size.y, origin.z}
       ,   { origin.x + size.x, origin.y, origin.z}
       ,   { origin.x, origin.y + size.y, origin.z }
       
            }));
    }
    else
        output.append(make_plane(origin, size.x, size.y, side::top));

    output.append(make_facet({
           origin
       ,   { origin.x, origin.y + size.y, origin.z }
       ,   { origin.x, origin.y, origin.z + size.z}
    }));

    output.append(make_facet({
           { origin.x, origin.y + size.y, origin.z + size.z }
       ,   { origin.x, origin.y + size.y, origin.z }
       ,   { origin.x, origin.y, origin.z + size.z}
        }));


    // facet walls
    output.append(make_facet({
           origin
       ,   { origin.x + size.x, origin.y , origin.z}
       ,   { origin.x, origin.y, origin.z + size.z}
    }));

    output.append(make_facet({
           { origin.x, origin.y + size.y, origin.z}
       ,   { origin.x + size.x, origin.y + size.y , origin.z}
       ,   { origin.x, origin.y + size.y, origin.z + size.z}
        }));

    //// angled wall
    if (size.x != size.y)
    {
        output.append(make_facet({
               { origin.x + size.x, origin.y + size.y, origin.z }
           ,   { origin.x, origin.y , origin.z + size.z}
           ,   { origin.x, origin.y + size.y, origin.z + size.z}
            }));
        output.append(make_facet({
               { origin.x + size.x, origin.y + size.y, origin.z }
           ,   { origin.x + size.x, origin.y , origin.z}
           ,   { origin.x, origin.y , origin.z + size.z}
            }));
    }
    else
    {
        output.append(make_facet({
               { origin.x + size.x, origin.y + size.y, origin.z }
           ,   { origin.x, origin.y + size.y, origin.z + size.z}
           ,   { origin.x, origin.y , origin.z + size.z}

            }));

        output.append(make_facet({
               { origin.x + size.x, origin.y + size.y, origin.z }
           ,   { origin.x, origin.y , origin.z + size.z}
           ,   { origin.x + size.x, origin.y , origin.z}
            }));
    }
    
    return output;
}

std::string make_one_third_of_rectangle(const Point3f& origin, const Point3f& size)
{
    std::string output{ "" };
    auto temp{ origin };
    
    // plane

    if (size.x != size.y)
    {
        output.append(make_facet({
                { origin }
            ,   { origin.x, origin.y + size.y, origin.z }
            ,   { origin.x + size.x, origin.y, origin.z }
            }));

        output.append(make_facet({
                { origin.x + size.x, origin.y + size.y, origin.z}
            ,   { origin.x + size.x, origin.y, origin.z }
            ,   { origin.x, origin.y + size.y, origin.z }
            }));
    }
    else
    {
        output.append(make_facet({
            { origin }
        ,   { origin.x + size.x, origin.y, origin.z }
        ,   { origin.x, origin.y + size.y, origin.z }
        }));

        output.append(make_facet({
                { origin.x + size.x, origin.y + size.y, origin.z}
            ,   { origin.x, origin.y + size.y, origin.z }
            ,   { origin.x + size.x, origin.y, origin.z }

        }));
    }

    //side walls

    output.append(make_facet({
            { origin }
        ,   { origin.x, origin.y + size.y, origin.z}
        ,   { origin.x, origin.y + size.y, origin.z + size.z }
    }));

    output.append(make_facet({
            { origin.x + size.x, origin.y + size.y, origin.z }
        ,   { origin.x, origin.y + size.y, origin.z}
        ,   { origin.x, origin.y + size.y, origin.z + size.z }
    }));

    //angled walls

    if (size.x != size.y)
    {
        output.append(make_facet({
                { origin.x + size.x, origin.y , origin.z }
            ,   { origin.x, origin.y , origin.z}
            ,   { origin.x, origin.y + size.y , origin.z + size.z }
            }));

        output.append(make_facet({
                { origin.x + size.x, origin.y , origin.z }
            ,   { origin.x, origin.y + size.y , origin.z + size.z }
            ,   { origin.x + size.x, origin.y + size.y , origin.z}

            }));
    }
    else
    {
        output.append(make_facet({
                { origin.x + size.x, origin.y , origin.z }
            ,   { origin.x, origin.y + size.y , origin.z + size.z }
            ,   { origin.x, origin.y , origin.z}
            }));

        output.append(make_facet({
                { origin.x + size.x, origin.y , origin.z }
            ,   { origin.x + size.x, origin.y + size.y , origin.z}
            ,   { origin.x, origin.y + size.y , origin.z + size.z }
            }));
    }

    return output;
}

std::string make_side1_one_third_of_rectangle(const Point3f& origin, const Point3f& size)
{
    std::string output{ "" };
    auto temp{ origin };

    //plane
    output.append(make_facet({
            { origin }
        ,   { origin.x + size.x, origin.y, origin.z + size.z }
        ,   { origin.x + size.x, origin.y, origin.z }
    }));
    output.append(make_facet({
            { origin.x, origin.y, origin.z + size.z }
        ,   { origin.x + size.x, origin.y, origin.z + size.z }
        ,   { origin.x, origin.y, origin.z }
    }));

    // aligned side
    if (size.x != size.y)
        output.append(make_facet({
                { origin.x, origin.y, origin.z + size.z}
            ,   { origin.x + size.x, origin.y + size.y, origin.z + size.z}
            ,   { origin.x + size.x, origin.y, origin.z + size.z}
            
            }));
    else
        output.append(make_facet({
                { origin.x, origin.y, origin.z + size.z}
            ,   { origin.x + size.x, origin.y, origin.z + size.z}
            ,   { origin.x + size.x, origin.y + size.y, origin.z + size.z}
            }));

    // bottom
    if (size.x != size.y)
        output.append(make_facet({
                { origin.x, origin.y, origin.z}
            ,   { origin.x + size.x, origin.y + size.y, origin.z + size.z }
            ,   { origin.x + size.x, origin.y, origin.z}
       
            }));
    else
        output.append(make_facet({
                { origin.x, origin.y, origin.z}
            ,   { origin.x + size.x, origin.y, origin.z}
            ,   { origin.x + size.x, origin.y + size.y, origin.z + size.z }
            

            }));
    // side
    output.append(make_facet({
            { origin.x, origin.y, origin.z}
        ,   { origin.x + size.x, origin.y + size.y, origin.z + size.z }
        ,   { origin.x, origin.y, origin.z + size.z }
    }));

    // flat side

    output.append(make_facet({
            { origin.x + size.x, origin.y, origin.z + size.z }
        ,   { origin.x + size.x, origin.y, origin.z }
        ,   { origin.x + size.x, origin.y + size.y, origin.z + size.z}
        }));
    

    return output;
}

std::string make_side2_one_third_of_rectangle(const Point3f& origin, const Point3f& size)
{
    std::string output{ "" };
    auto temp{ origin };

    //plane
    output.append(make_facet({
            { origin.x + size.x, origin.y + size.y, origin.z }
        ,   { origin.x + size.x, origin.y, origin.z + size.z }
        ,   { origin.x + size.x, origin.y, origin.z }
        }));

    output.append(make_facet({
            { origin.x + size.x, origin.y + size.y, origin.z }
        ,   { origin.x + size.x, origin.y, origin.z + size.z }
        ,   { origin.x + size.x, origin.y + size.y, origin.z + size.z}
        }));
    //Close facet
    if (size.x != size.y)
    output.append(make_facet({
            { origin.x , origin.y + size.y, origin.z + size.z }
        ,   { origin.x + size.x, origin.y + size.y, origin.z + size.z}
        ,   { origin.x + size.x, origin.y, origin.z + size.z }
        }));

    else
        output.append(make_facet({
                { origin.x , origin.y + size.y, origin.z + size.z }
            ,   { origin.x + size.x, origin.y, origin.z + size.z }
            ,   { origin.x + size.x, origin.y + size.y, origin.z + size.z}
            }));
    
    //side 1
    output.append(make_facet({
            { origin.x, origin.y + size.y, origin.z + size.z }
        ,   { origin.x + size.x, origin.y + size.y, origin.z }
        ,   { origin.x + size.x, origin.y + size.y, origin.z + size.z}
        }));

    //top/bottom
    if (size.x != size.y)
        output.append(make_facet({
            { origin.x, origin.y + size.y, origin.z + size.z }
        ,   { origin.x + size.x, origin.y + size.y, origin.z }
        ,   { origin.x + size.x, origin.y, origin.z}
        }));
    else
    output.append(make_facet({
            { origin.x, origin.y + size.y, origin.z + size.z }
        ,   { origin.x + size.x, origin.y, origin.z}
        ,   { origin.x + size.x, origin.y + size.y, origin.z }
        }));

    //side 2
    output.append(make_facet({
            { origin.x, origin.y + size.y, origin.z + size.z }
        ,   { origin.x + size.x, origin.y, origin.z + size.z }
        ,   { origin.x + size.x, origin.y, origin.z}
        }));

    return output;
}