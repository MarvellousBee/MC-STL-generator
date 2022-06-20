#pragma once

struct Values4
{
    long double val[4]{ 0,0,0,0 };
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
    friend bool operator ==(const Values4& arg1, const Values4& arg2);
};
bool operator ==(const Values4& arg1, const Values4& arg2)
{
    for (int i{ 0 }; i < 4; i++)
        if (arg1.val[i] != arg2.val[i])
            return false;
    return true;
}

struct Point3f
{
    long double x;
    long double y;
    long double z;
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