#include <iostream>

#include "Image.hpp"
#include "ResourcePackBuilder.hpp"

int main(int argc, char* argv[]) {
    try { 
        if (argc != 2) {
            throw std::invalid_argument(
                "На вход должно подаваться 1 аргумент - путь_к_изображению"
            );
        }

        Image panorama;
        panorama.load(argv[1]);

        std::filesystem::path projectRoot = std::filesystem::current_path(); 
        buildResourcePack(panorama, projectRoot);
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "OK: resourcepack generated successfully" << std::endl;
    return 0;
}
