#pragma once

#include <cstdint>
#include <string>
#include <vector>

struct Pixel {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

static_assert(sizeof(Pixel) == 4, "Пиксель должен занимать ровно 4 байта");

class Image {
private:
    int width = 0;
    int height = 0;
    int channels = 0;
    std::vector<Pixel> pixels;
public:
    // конструкторы
    Image(int w = 0, int h = 0) 
        : width(w), height(h), 
        pixels(static_cast<size_t>(w) * h) {}

    // геттеры
    int getWidth() const;
    int getHeight() const;
    Pixel getPixel(int x, int y) const;

    // сеттеры
    void setPixel(int x, int y, const Pixel& pixel);

    // методы
    void load(const std::string& path);
    void save(const std::string& path) const;
};
