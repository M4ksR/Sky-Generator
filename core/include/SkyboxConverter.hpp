#pragma once

#include "Image.hpp"

enum class CubeFace {
    PositiveX,
    NegativeX,
    PositiveY,
    NegativeY,
    PositiveZ,
    NegativeZ
};

struct Vec3 {
    double x;
    double y;
    double z;
};

struct Vec2 {
    double u;
    double v;
};

Vec3 pixelDirection(CubeFace face, int x, int y, int faceSize);

Vec2 directionToUV(const Vec3& direction);

Pixel sampleBilinear(const Image& image, const Vec2& uv);

Image generateFace(const Image& panorama, CubeFace face, int faceSize);
