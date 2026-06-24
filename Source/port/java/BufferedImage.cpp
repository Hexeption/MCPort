//
// Created by Keir Davis on 24/06/2026.
//

#include "BufferedImage.h"

#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

BufferedImage::BufferedImage(const int_t width, const int_t height) : width(width), height(height),
                                                                      rgbaPixels(
                                                                          static_cast<std::size_t>(width) * static_cast<
                                                                              std::size_t>(height) * 4) {
}

BufferedImage::BufferedImage(const int_t width, const int_t height, std::vector<ubyte_t> rgbaPixels) : width(width),
    height(height), rgbaPixels(std::move(rgbaPixels)) {
}

int_t BufferedImage::getWidth() const {
    return width;
}

int_t BufferedImage::getHeight() const {
    return height;
}

const std::vector<ubyte_t> &BufferedImage::getRgbaPixels() const {
    return rgbaPixels;
}

std::vector<ubyte_t> &BufferedImage::getRgbaPixels() {
    return rgbaPixels;
}

bool BufferedImage::empty() const {
    return width <= 0 || height <= 0 || rgbaPixels.empty();
}

BufferedImage BufferedImage::readFile(const jstring &path) {
    int width = 0;
    int height = 0;
    int channels = 0;
    stbi_uc *pixels = stbi_load(String::toUtf8(path).c_str(), &width, &height, &channels, STBI_rgb_alpha);
    if (pixels == nullptr) {
        throw std::runtime_error("failed to load image: " + String::toUtf8(path));
    }

    const std::size_t byteCount = static_cast<std::size_t>(width) * static_cast<std::size_t>(height) * 4;
    std::vector<ubyte_t> rgbaPixels(pixels, pixels + byteCount);
    stbi_image_free(pixels);
    return BufferedImage(width, height, std::move(rgbaPixels));
}

BufferedImage BufferedImage::readMemory(const std::vector<ubyte_t> &encodedImage) {
    int width = 0;
    int height = 0;
    int channels = 0;
    stbi_uc *pixels = stbi_load_from_memory(encodedImage.data(), static_cast<int>(encodedImage.size()), &width, &height,
                                            &channels, STBI_rgb_alpha);
    if (pixels == nullptr) {
        throw std::runtime_error("failed to decode image from memory");
    }

    const std::size_t byteCount = static_cast<std::size_t>(width) * static_cast<std::size_t>(height) * 4;
    std::vector<ubyte_t> rgbaPixels(pixels, pixels + byteCount);
    stbi_image_free(pixels);
    return BufferedImage(width, height, std::move(rgbaPixels));
}
