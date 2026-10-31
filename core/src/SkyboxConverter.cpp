#include <algorithm>
#include <cmath>
#include <stdexcept>

#include "SkyboxConverter.hpp"

Vec3 pixelDirection(CubeFace face, int x, int y, int faceSize) {
    if (faceSize <= 0) {
        throw std::invalid_argument("faceSize должен быть положительным");
    }

    double s = 2 * (x + 0.5) / faceSize - 1;
    double t = 1 - 2 * (y + 0.5) / faceSize;

    Vec3 vec;
    if (face == CubeFace::PositiveX) { vec = Vec3{1, t, -s}; }
    else if (face == CubeFace::PositiveY) { vec = Vec3{s, 1, -t}; } 
    else if (face == CubeFace::PositiveZ) { vec = Vec3{s, t, 1}; } 
    else if (face == CubeFace::NegativeX) { vec = Vec3{-1, t, s}; } 
    else if (face == CubeFace::NegativeY) { vec = Vec3{s, -1, t}; } 
    else if (face == CubeFace::NegativeZ) { vec = Vec3{-s, t, -1}; } 
    else { throw std::invalid_argument("Недопустимая грань куба"); }
    
    double leng = std::sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
    
    vec.x /= leng;
    vec.y /= leng;
    vec.z /= leng;

    return vec;
}

Vec2 directionToUV(const Vec3& direction) {
    constexpr double pi = 3.14159265358979323846;
    double theta = std::atan2(direction.z, direction.x);
    double phi = std::asin(std::clamp(direction.y, -1.0, 1.0));
    
    double u = theta / (2 * pi) + 0.5;
    double v = 0.5 - phi / pi;

    return Vec2{u, v};
}

// реализовать
Pixel sampleBilinear(const Image& image, const Vec2& uv) {}
