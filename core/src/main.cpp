#include <iostream>

#include "Image.hpp"
#include "SkyboxConverter.hpp"

int main(int argc, char* argv[]) {
    {
        Image img;
        img.load("C:\\Users\\Maksr\\Desktop\\img.jpg");
        
        std::cout << img.getWidth() << " " << img.getHeight() << std::endl;
        
        Pixel pix = img.getPixel(0,0);

        std::cout << static_cast<int>(pix.r) << " " 
                    << static_cast<int>(pix.g) << " " 
                    << static_cast<int>(pix.b) << " " 
                    << static_cast<int>(pix.a) << std::endl;
    }

    {
        Image img(16, 16);
        // пустое изображение
        img.save("C:\\Users\\Maksr\\Desktop\\imgOutputFirst.png");
        for (int y = 0; y < 16; ++y) {
            for (int x = 0; x < 16; ++x) {
                Pixel pixel;

                if (x < 4) { pixel = Pixel{255,0,0,255}; }
                else if (4 <= x && x < 8) { pixel = Pixel{0,255,0,255}; }
                else if (8 <= x && x < 12) { pixel = Pixel{0,0,255,255}; }
                else if (12 <= x && x < 16) { pixel = Pixel{255,255,255,255}; }

                img.setPixel(x,y,pixel);
            }
        }
        // разноцветное изображение
        img.save("C:\\Users\\Maksr\\Desktop\\imgOutputSecond.png");
    }

    {
        Vec3 direction{0, 0, 1};
        Vec2 directionUV = directionToUV(direction);
        std::cout << directionUV.u << " " << directionUV.v << std::endl;
    }

    return 0;
}
