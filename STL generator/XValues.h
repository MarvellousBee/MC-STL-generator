#pragma once
#include <ostream>
struct Values4
{
    long double val[4]{ 0,0,0,0 };

    Values4() {};
    constexpr Values4(int&& a, int&& b, int&& c, int&& d){
        val[0] = a;
        val[1] = b;
        val[2] = c;
        val[3] = d;
    }

    long double& operator[] (const int& i)
    {
        return val[i];
    }
    long double at (int i) const
    {
        return val[i];
    }
    bool operator ==(const Values4& arg)
    {
        for (int i{ 0 }; i < 4; i++)
            if (val[i] != arg.val[i])
                return false;
        return true;
    }
    template <class T>
    bool operator ==(const T& arg)
    {
        for (int i{ 0 }; i < 4; i++)
            if (val[i] != arg[i])
                return false;
        return true;
    }
    friend bool operator ==(const Values4& arg1, const Values4& arg2);
};
bool operator ==(const Values4& arg1, const Values4& arg2)
{
    for (int i{ 0 }; i < 4; i++)
        if (arg1.val[i] != arg2.val[i])
            return false;
    return true;
}
std::ostream& operator<<(std::ostream& os, const Values4& rhs)
{
    for (int i{ 0 }; i < 4; i++)
        os << rhs.val[i] << ' ';
    return os;
}

struct Point3f
{
    long double x;
    long double y;
    long double z;
    //Point3f(int _x, int _y, int _z) : x(static_cast<long double>(_x))
    //                                , y(static_cast<long double>(_y))
    //                                , z(static_cast<long double>(_z))
    //{};
    //Point3f() {};

    Point3f& operator -(const Point3f& a2)
    {
        x -= a2.x;
        y -= a2.y;
        z -= a2.z;
        return *this;
    }
    Point3f& operator *(const Point3f& a2)
    {
        x *= a2.x;
        y *= a2.y;
        z *= a2.z;
        return *this;
    }
    Point3f& operator *=(const Point3f& a2)
    {
        *this = *this * a2;
        return *this;
    }
    Point3f& operator /(const long double& a2)
    {
        x /= a2;
        y /= a2;
        z /= a2;
        return *this;
    }
    Point3f& operator +(const Point3f& a2)
    {
        x += a2.x;
        y += a2.y;
        z += a2.z;
        return *this;
    }
    long double sum()
    {
        return x + y + z;
    }
    friend Point3f operator -(const Point3f& arg1, const Point3f& arg2);
    friend bool operator ==(const Point3f& arg1, const Point3f& arg2);
};

Point3f operator -(const Point3f& arg1, const Point3f& arg2)
{
    Point3f ret{ 0,0,0 };

    ret.x -= arg1.x;
    ret.y -= arg1.y;
    ret.z -= arg1.z;

    ret.x -= arg2.x;
    ret.y -= arg2.y;
    ret.z -= arg2.z;

    return ret;
}

bool operator ==(const Point3f& arg1, const Point3f& arg2)
{
    return arg1.x == arg2.x 
        && arg1.y == arg2.y
        && arg1.z == arg2.z;
}