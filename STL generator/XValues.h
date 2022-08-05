#pragma once
#include <ostream>

typedef long double ld;

class Values4
{
    ld val[4]{ 0,0,0,0 };
public:
    Values4() {};
    constexpr Values4(ld&& a, ld&& b, ld&& c, ld&& d){
        val[0] = a;
        val[1] = b;
        val[2] = c;
        val[3] = d;
    }

    ld& operator[] (const int& i) 
    {
        return val[i];
    }
    const ld&  operator[] (const int& i) const
    {
        return val[i];
    }
    bool operator ==(const Values4& arg) const
    {
        for (int i{ 0 }; i < 4; i++)
            if (val[i] != arg[i])
                return false;
        return true;
    }

    friend std::ostream& operator<<(std::ostream& os, const Values4& rhs);
};

std::ostream& operator<<(std::ostream& os, const Values4& rhs)
{
    for (int i{ 0 }; i < 4; i++)
        os << rhs.val[i] << ' ';
    return os;
}

struct Point3
{
    ld x;
    ld y;
    ld z;

    Point3 operator -(const Point3& a2)
    {
        Point3 output{*this};
        output.x -= a2.x;
        output.y -= a2.y;
        output.z -= a2.z;
        return output;
    }
    Point3 operator *(const Point3& a2)
    {
        Point3 output{ *this };
        output.x *= a2.x;
        output.y *= a2.y;
        output.z *= a2.z;
        return output;
    }
    Point3& operator *=(const Point3& a2)
    {
        *this = *this * a2;
        return *this;
    }
    Point3 operator /(const ld& a2)
    {
        Point3 output{ *this };
        output.x /= a2;
        output.y /= a2;
        output.z /= a2;
        return output;
    }
    Point3 operator +(const Point3& a2)
    {
        Point3 output{ *this };
        output.x += a2.x;
        output.y += a2.y;
        output.z += a2.z;
        return output;
    }
    ld sum()
    {
        return x + y + z;
    }
    bool operator ==( const Point3& arg2) const
    {
        return x == arg2.x
            && y == arg2.y
            && z == arg2.z;
    }
};



