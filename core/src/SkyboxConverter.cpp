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

double lerp(double a, double b, double t) {
    return a * (1.0 - t) + b * t;
}

Pixel sampleBilinear(const Image& image, const Vec2& uv) {
    int width = image.getWidth();
    int height = image.getHeight();

    if (width <= 0 || height <= 0) {
        throw std::invalid_argument("Изображение должно быть не пустым");
    }

    double x = uv.u * width - 0.5;
    double y = uv.v * height - 0.5;

    y = std::clamp(y, 0.0, static_cast<double>(height - 1));
    
    int x0 = static_cast<int>(std::floor(x));
    int y0 = static_cast<int>(std::floor(y));
    int x1 = x0 + 1;
    int y1 = std::min(y0 + 1, height - 1);

    x0 = ((x0 % width) + width) % width;
    x1 = ((x1 % width) + width) % width;

    Pixel topLeft = image.getPixel(x0, y0);
    Pixel topRight = image.getPixel(x1, y0);
    Pixel bottomLeft = image.getPixel(x0, y1);
    Pixel bottomRight = image.getPixel(x1, y1);
    
    double dx = x - std::floor(x);
    double dy = y - std::floor(y);

    double res[4] = {};

    {
        double top = lerp(topLeft.r, topRight.r, dx);
        double bottom = lerp(bottomLeft.r, bottomRight.r, dx);
        res[0] = lerp(top, bottom, dy);
    }

    {
        double top = lerp(topLeft.g, topRight.g, dx);
        double bottom = lerp(bottomLeft.g, bottomRight.g, dx);
        res[1] = lerp(top, bottom, dy);
    }

    {
        double top = lerp(topLeft.b, topRight.b, dx);
        double bottom = lerp(bottomLeft.b, bottomRight.b, dx);
        res[2] = lerp(top, bottom, dy);
    }

    {
        double top = lerp(topLeft.a, topRight.a, dx);
        double bottom = lerp(bottomLeft.a, bottomRight.a, dx);
        res[3] = lerp(top, bottom, dy);
    }

    for (int i = 0; i < 4; ++i) {
        if (res[i] < 0) { res[i] = 0; }
        else if (res[i] > 255) { res[i] = 255; }
    }

    return Pixel {
        static_cast<uint8_t>(std::round(res[0])),
        static_cast<uint8_t>(std::round(res[1])),
        static_cast<uint8_t>(std::round(res[2])),
        static_cast<uint8_t>(std::round(res[3]))
    };
}

Image generateFace(const Image& panorama, CubeFace face, int faceSize) {
    if (faceSize <= 0) {
        throw std::invalid_argument("faceSize должен быть положительным");
    }

    if (!(panorama.getWidth() == 4096 && panorama.getHeight() == 2048)) {
        throw std::invalid_argument("Панорама должна иметь размер 4096×2048");
    }
    
    Image image(faceSize, faceSize);
    for (int y = 0; y < faceSize; ++y) {
        for (int x = 0; x < faceSize; ++x) {
            Vec3 vec3 = pixelDirection(face, x, y, faceSize);
            Vec2 vec2 = directionToUV(vec3);
            Pixel pixel = sampleBilinear(panorama, vec2);
            image.setPixel(x, y, pixel);
        }
    }
    return image;
}

enum class axes {
    X,
    Y
};

void flip(Image& image, axes axis) {
    if (axis == axes::X) {
        for (int y = 0; y < image.getHeight(); ++y) {
            for (int currX = 0; currX < image.getWidth() / 2; ++currX) {
                int swapX = image.getWidth() - 1 - currX;

                Pixel currPixel = image.getPixel(currX, y);
                Pixel swapPixel = image.getPixel(swapX, y);

                image.setPixel(currX, y, swapPixel);
                image.setPixel(swapX, y, currPixel);
            }
        }
    }
    else if (axis == axes::Y) {
        for (int currY = 0; currY < image.getHeight() / 2; ++currY) {
            for (int x = 0; x < image.getWidth(); ++x) {
                int swapY = image.getHeight() - 1 -currY;

                Pixel currPixel = image.getPixel(x, currY);
                Pixel swapPixel = image.getPixel(x, swapY);

                image.setPixel(x, currY, swapPixel);
                image.setPixel(x, swapY, currPixel);
            }
        }
    }
    else {
        throw std::invalid_argument("Поворот должен осуществляться либо по оси Ox либо по оси Oy");
    }
}

struct Transformation {
    CubeFace direction;
    int xMax;
    int yMax;
    axes axis;
};

Image generateAtlas(const Image& panorama, int faceSize) {
    if (faceSize <= 0) {
        throw std::invalid_argument("faceSize должен быть положительным");
    }

    Image image(faceSize * 3, faceSize * 2);

    std::vector<Transformation> faces = {
        Transformation{CubeFace::PositiveX, faceSize * 3, faceSize * 2, axes::X},
        Transformation{CubeFace::NegativeX, faceSize, faceSize * 2, axes::X},
        Transformation{CubeFace::PositiveY, faceSize * 2, faceSize, axes::Y},
        Transformation{CubeFace::NegativeY, faceSize, faceSize, axes::Y},
        Transformation{CubeFace::PositiveZ, faceSize * 3, faceSize, axes::X},
        Transformation{CubeFace::NegativeZ, faceSize * 2, faceSize * 2, axes::X}
    };

    for (auto face : faces) {
        Image newFace = generateFace(panorama, face.direction, faceSize);
        flip(newFace, face.axis);
        for (int y = face.yMax - faceSize; y < face.yMax; ++y) {
            for (int x = face.xMax - faceSize; x < face.xMax; ++x) {
                Pixel currPixel = newFace.getPixel(x % faceSize, y % faceSize);
                image.setPixel(x, y, currPixel);
            }
        }
    }

    return image;
}
