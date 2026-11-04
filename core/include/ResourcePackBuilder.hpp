#pragma once

#include <filesystem>

#include "Image.hpp"

void buildResourcePack(const Image& panorama, const std::filesystem::path& projectRoot);
