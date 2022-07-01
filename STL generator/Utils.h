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

template <class T>//too lazy to remember/look up how to point to a function
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