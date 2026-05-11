#pragma once

#include <iostream>
#include <iomanip>
#include<glm/glm.hpp>

// dvec2
inline std::ostream& operator<<(std::ostream& os, const glm::dvec2& v)
{
    os << "(" << v.x << ", " << v.y << ")";
    return os;
}

// dvec3
inline std::ostream& operator<<(std::ostream& os, const glm::dvec3& v)
{
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}

// dvec4
inline std::ostream& operator<<(std::ostream& os, const glm::dvec4& v)
{
    os << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
    return os;
}

// mat4
inline std::ostream& operator<<(std::ostream& os, const glm::mat4& m)
{
    os << std::fixed << std::setprecision(2);
    os << "Matrix4x4:" << std::endl;
    for (int row = 0; row < 4; row++)
    {
        os << "  [";
        for (int col = 0; col < 4; col++)
        {
            os << std::setw(8) << m[col][row];  // 注意：glm是列优先
            if (col < 3) os << ", ";
        }
        os << "]" << std::endl;
    }
    return os;
}