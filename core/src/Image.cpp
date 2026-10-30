#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stdexcept>

#include "Image.hpp"
#include "stb_image.h"
#include "stb_image_write.h"

int Image::getWidth() const {
   return width; 
}

int Image::getHeight() const {
   return height;
}

Pixel Image::getPixel(int x, int y) const {
   if (x < 0 || x >= width || y < 0 || y >= height) { 
      throw std::out_of_range("Указанные координаты пикселя находятся вне изображения");
   }
   return pixels[y * width + x];
}

void Image::setPixel(int x, int y, const Pixel& pixel) {
   if (x < 0 || x >= width || y < 0 || y >= height) { 
      throw std::out_of_range("Указанные координаты пикселя находятся вне изображения");
   }
   pixels[y * width + x] = pixel;
}

void Image::load(const std::string& path) {
   unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 4);
   
   if (data == nullptr) {
      throw std::runtime_error("Ошибка загрузки пикселей");
   }

   pixels.resize(static_cast<size_t>(width) * height);

   for (int i = 0; i < width * height * 4; i += 4) {
      int index = (i >> 2);
      pixels[index] = Pixel{data[i], data[i+1], data[i+2], data[i+3]};
   }

   stbi_image_free(data);
}

void Image::save(const std::string& path) const {
   int size = (width << 2);
   int res = stbi_write_png(path.c_str(), width, height, 4, pixels.data(), size);
   if (!res) {
      throw std::runtime_error("Ошибка сохранения изображения");
   }
}
