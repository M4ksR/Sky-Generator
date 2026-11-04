#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <memory>
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
   int w = 0;
   int h = 0;
   int chan = 0;

   unsigned char* data = stbi_load(path.c_str(), &w, &h, &chan, 4);
   
   if (data == nullptr) {
      throw std::runtime_error("Ошибка загрузки пикселей");
   }

   std::unique_ptr<unsigned char, decltype(&stbi_image_free)> guard(data, &stbi_image_free);
   
   size_t size = static_cast<size_t>(w) * h;
   std::vector<Pixel> tmpPixels; 
   tmpPixels.resize(size);

   for (size_t i = 0; i < size; ++i) {
      size_t index = i << 2;
      tmpPixels[i] = Pixel{
         data[index], 
         data[index+1], 
         data[index+2], 
         data[index+3]
      };
   }

   pixels.swap(tmpPixels);
   width = w;
   height = h;
   channels = chan;
}

void Image::save(const std::string& path) const {
   int size = (width << 2);
   int res = stbi_write_png(path.c_str(), width, height, 4, pixels.data(), size);
   if (!res) {
      throw std::runtime_error("Ошибка сохранения изображения");
   }
}
