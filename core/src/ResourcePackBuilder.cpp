#include <system_error>

#include "ResourcePackBuilder.hpp"
#include "SkyboxConverter.hpp"

void buildResourcePack(const Image& panorama, const std::filesystem::path& projectRoot) {
    // определим путь к папке templates
    std::filesystem::path templatePath = projectRoot/"templates"/"Custom-Sky";

    // если папка template отсутствует выкинем исключение
    if (!std::filesystem::is_directory(templatePath)) {
        throw std::runtime_error("Отсутствует папка templates");
    }

    // создадим папку output
    std::filesystem::path outputPath = projectRoot/"output";
    std::filesystem::create_directories(outputPath);

    // получим путь к временной папке и проверим что такой папки еще нет
    std::filesystem::path tempPath = outputPath / "Custom-Sky-tmp";
    if (std::filesystem::exists(tempPath)) {
        throw std::runtime_error("Временная папка Custom-Sky-tmp уже создана");
    }

    // получим путь к финальной папке и проверим что такой папки еще нет
    std::filesystem::path finalPath = outputPath / "Custom-Sky";
    if (std::filesystem::exists(finalPath)) {
        throw std::runtime_error("Ресурс-пак с таким именем уже создан");
    }
    
    // подготовим путь
    std::filesystem::path packPath = tempPath / "assets" / "minecraft" / "optifine" / "sky" / "world0" / "sky0.png";

    // создадим атлас
    Image sky = generateAtlas(panorama, 1024);
   
    try {
        // скопируем шаблон в папку output
        std::filesystem::copy(
            templatePath, 
            tempPath,
            std::filesystem::copy_options::recursive
        );

        // сохраним небо в ресурс-пак
        sky.save(packPath.string());

        // переименуем временную папку
        std::filesystem::rename(tempPath, finalPath);
    }
    catch (...) {
        std::error_code ec;
        // удалим временную папку и передадим исключение наверх
        std::filesystem::remove_all(tempPath, ec);
        throw;
    }
}
