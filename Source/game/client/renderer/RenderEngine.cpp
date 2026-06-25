//
// Created by Keir Davis on 24/06/2026.
//

#include "RenderEngine.h"

#include <algorithm>
#include <cstdio>
#include <filesystem>
#include <stdexcept>

#include <glad/glad.h>

#include "ThreadDownloadImageData.h"
#include "TextureFX.h"
#include "game/client/options/GameSettings.h"

bool RenderEngine::useMipmaps = false;

namespace {
    std::string stripLeadingSlash(const std::string &path) {
        if (!path.empty() && (path[0] == '/' || path[0] == '\\')) {
            return path.substr(1);
        }
        return path;
    }

    std::vector<std::filesystem::path> resourceCandidates(const jstring &resource) {
        const std::string raw = String::toUtf8(resource);
        const std::string stripped = stripLeadingSlash(raw);
        return {
            std::filesystem::path(raw),
            std::filesystem::path(stripped),
            std::filesystem::path("Assets") / stripped,
            std::filesystem::path("..") / "Assets" / stripped,
            std::filesystem::path("..") / ".." / "Assets" / stripped,
        };
    }
}

RenderEngine::RenderEngine(GameSettings *options) : imageData(1048576), options(options) {
}

RenderEngine::~RenderEngine() {
    for (TextureFX *textureFX: textureList) {
        delete textureFX;
    }
}

int_t RenderEngine::getTexture(const jstring &path) {
    if (const auto found = textureMap.find(path); found != textureMap.end()) {
        return found->second;
    }

    GLuint textureName = 0;
    glGenTextures(1, &textureName);

    if (path.starts_with(u"##")) {
        setupTexture(unwrapImageByColumns(loadImageResource(path.substr(2))), static_cast<int_t>(textureName));
    } else if (path.starts_with(u"%%")) {
        clampTexture = true;
        setupTexture(loadImageResource(path.substr(2)), static_cast<int_t>(textureName));
        clampTexture = false;
    } else {
        setupTexture(loadImageResource(path), static_cast<int_t>(textureName));
    }

    textureMap[path] = static_cast<int_t>(textureName);
    return static_cast<int_t>(textureName);
}

int_t RenderEngine::allocateAndSetupTexture(const BufferedImage &image) {
    GLuint textureName = 0;
    glGenTextures(1, &textureName);
    setupTexture(image, static_cast<int_t>(textureName));
    textureContentsMap[static_cast<int_t>(textureName)] = image;
    return static_cast<int_t>(textureName);
}

void RenderEngine::setupTexture(const BufferedImage &image, const int_t textureName) {
    glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(textureName));
    if (useMipmaps) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    } else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    if (clampTexture) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    } else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    const int_t width = image.getWidth();
    const int_t height = image.getHeight();
    const auto &pixels = image.getRgbaPixels();
    if (pixels.size() != static_cast<std::size_t>(width) * static_cast<std::size_t>(height) * 4) {
        throw std::runtime_error("invalid texture pixel data");
    }

    const std::size_t byteCount = pixels.size();
    if (imageData.size() < byteCount) {
        imageData.resize(byteCount);
    }

    for (std::size_t pixel = 0; pixel < byteCount / 4; ++pixel) {
        int_t red = pixels[pixel * 4 + 0];
        int_t green = pixels[pixel * 4 + 1];
        int_t blue = pixels[pixel * 4 + 2];
        const int_t alpha = pixels[pixel * 4 + 3];
        if (options != nullptr && options->anaglyph) {
            const int_t anaglyphRed = (red * 30 + green * 59 + blue * 11) / 100;
            const int_t anaglyphGreen = (red * 30 + green * 70) / 100;
            const int_t anaglyphBlue = (red * 30 + blue * 70) / 100;
            red = anaglyphRed;
            green = anaglyphGreen;
            blue = anaglyphBlue;
        }

        imageData[pixel * 4 + 0] = static_cast<ubyte_t>(red);
        imageData[pixel * 4 + 1] = static_cast<ubyte_t>(green);
        imageData[pixel * 4 + 2] = static_cast<ubyte_t>(blue);
        imageData[pixel * 4 + 3] = static_cast<ubyte_t>(alpha);
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData.data());
    if (useMipmaps) {
        for (int_t level = 1; level <= 4; ++level) {
            const int_t previousWidth = width >> (level - 1);
            const int_t mipWidth = width >> level;
            const int_t mipHeight = height >> level;

            for (int_t x = 0; x < mipWidth; ++x) {
                for (int_t y = 0; y < mipHeight; ++y) {
                    const int_t first = readInt(imageData, (x * 2 + 0 + (y * 2 + 0) * previousWidth) * 4);
                    const int_t second = readInt(imageData, (x * 2 + 1 + (y * 2 + 0) * previousWidth) * 4);
                    const int_t third = readInt(imageData, (x * 2 + 1 + (y * 2 + 1) * previousWidth) * 4);
                    const int_t fourth = readInt(imageData, (x * 2 + 0 + (y * 2 + 1) * previousWidth) * 4);
                    writeInt(imageData, (x + y * mipWidth) * 4,
                             alphaBlend(alphaBlend(first, second), alphaBlend(third, fourth)));
                }
            }

            glTexImage2D(GL_TEXTURE_2D, level, GL_RGBA, mipWidth, mipHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                         imageData.data());
        }
    }
}

void RenderEngine::deleteTexture(const int_t textureName) {
    textureContentsMap.erase(textureName);
    const GLuint glTextureName = static_cast<GLuint>(textureName);
    glDeleteTextures(1, &glTextureName);
}

int_t RenderEngine::getTextureForDownloadableImage(const jstring &url, const jstring &fallbackTexture) {
    const auto found = urlToImageDataMap.find(url);
    ThreadDownloadImageData *imageData = found == urlToImageDataMap.end() ? nullptr : found->second.get();
    if (imageData != nullptr && imageData->image != nullptr && !imageData->textureSetupComplete) {
        if (imageData->textureName < 0) {
            imageData->textureName = allocateAndSetupTexture(*imageData->image);
        } else {
            setupTexture(*imageData->image, imageData->textureName);
        }

        imageData->textureSetupComplete = true;
    }

    return imageData != nullptr && imageData->textureName >= 0 ? imageData->textureName : getTexture(fallbackTexture);
}

ThreadDownloadImageData *RenderEngine::obtainImageData(const jstring &url, ImageBuffer *buffer) {
    const auto found = urlToImageDataMap.find(url);
    if (found == urlToImageDataMap.end()) {
        auto imageData = std::make_unique<ThreadDownloadImageData>(url, buffer);
        ThreadDownloadImageData *imageDataPtr = imageData.get();
        urlToImageDataMap[url] = std::move(imageData);
        return imageDataPtr;
    }

    ++found->second->referenceCount;
    return found->second.get();
}

void RenderEngine::releaseImageData(const jstring &url) {
    const auto found = urlToImageDataMap.find(url);
    if (found == urlToImageDataMap.end()) {
        return;
    }

    --found->second->referenceCount;
    if (found->second->referenceCount == 0) {
        if (found->second->textureName >= 0) {
            deleteTexture(found->second->textureName);
        }

        urlToImageDataMap.erase(found);
    }
}

void RenderEngine::registerTextureFX(TextureFX *textureFX) {
    textureList.push_back(textureFX);
    textureFX->onTick();
}

void RenderEngine::updateDynamicTextures() {
    for (TextureFX *textureFX: textureList) {
        textureFX->anaglyphEnabled = options != nullptr && options->anaglyph;
        textureFX->onTick();

        if (imageData.size() < textureFX->imageData.size()) {
            imageData.resize(textureFX->imageData.size());
        }
        std::copy(textureFX->imageData.begin(), textureFX->imageData.end(), imageData.begin());

        textureFX->bindImage(*this);
        for (int_t tileX = 0; tileX < textureFX->tileSize; ++tileX) {
            for (int_t tileY = 0; tileY < textureFX->tileSize; ++tileY) {
                glTexSubImage2D(GL_TEXTURE_2D, 0, textureFX->iconIndex % 16 * 16 + tileX * 16,
                                textureFX->iconIndex / 16 * 16 + tileY * 16, 16, 16, GL_RGBA, GL_UNSIGNED_BYTE,
                                imageData.data());

                if (useMipmaps) {
                    for (int_t level = 1; level <= 4; ++level) {
                        const int_t previousWidth = 16 >> (level - 1);
                        const int_t mipWidth = 16 >> level;
                        for (int_t x = 0; x < mipWidth; ++x) {
                            for (int_t y = 0; y < mipWidth; ++y) {
                                const int_t first = readInt(imageData, (x * 2 + 0 + (y * 2 + 0) * previousWidth) * 4);
                                const int_t second = readInt(imageData, (x * 2 + 1 + (y * 2 + 0) * previousWidth) * 4);
                                const int_t third = readInt(imageData, (x * 2 + 1 + (y * 2 + 1) * previousWidth) * 4);
                                const int_t fourth = readInt(imageData, (x * 2 + 0 + (y * 2 + 1) * previousWidth) * 4);
                                writeInt(imageData, (x + y * mipWidth) * 4,
                                         averageColor(averageColor(first, second), averageColor(third, fourth)));
                            }
                        }

                        glTexSubImage2D(GL_TEXTURE_2D, level, textureFX->iconIndex % 16 * mipWidth,
                                        textureFX->iconIndex / 16 * mipWidth, mipWidth, mipWidth, GL_RGBA,
                                        GL_UNSIGNED_BYTE, imageData.data());
                    }
                }
            }
        }
    }

    for (TextureFX *textureFX: textureList) {
        if (textureFX->textureId > 0) {
            if (imageData.size() < textureFX->imageData.size()) {
                imageData.resize(textureFX->imageData.size());
            }
            std::copy(textureFX->imageData.begin(), textureFX->imageData.end(), imageData.begin());
            glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(textureFX->textureId));
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 16, 16, GL_RGBA, GL_UNSIGNED_BYTE, imageData.data());

            if (useMipmaps) {
                for (int_t level = 1; level <= 4; ++level) {
                    const int_t previousWidth = 16 >> (level - 1);
                    const int_t mipWidth = 16 >> level;
                    for (int_t x = 0; x < mipWidth; ++x) {
                        for (int_t y = 0; y < mipWidth; ++y) {
                            const int_t first = readInt(imageData, (x * 2 + 0 + (y * 2 + 0) * previousWidth) * 4);
                            const int_t second = readInt(imageData, (x * 2 + 1 + (y * 2 + 0) * previousWidth) * 4);
                            const int_t third = readInt(imageData, (x * 2 + 1 + (y * 2 + 1) * previousWidth) * 4);
                            const int_t fourth = readInt(imageData, (x * 2 + 0 + (y * 2 + 1) * previousWidth) * 4);
                            writeInt(imageData, (x + y * mipWidth) * 4,
                                     averageColor(averageColor(first, second), averageColor(third, fourth)));
                        }
                    }

                    glTexSubImage2D(GL_TEXTURE_2D, level, 0, 0, mipWidth, mipWidth, GL_RGBA, GL_UNSIGNED_BYTE,
                                    imageData.data());
                }
            }
        }
    }
}

void RenderEngine::refreshTextures() {
    for (const auto &[textureName, image]: textureContentsMap) {
        setupTexture(image, textureName);
    }

    for (const auto &[url, imageData]: urlToImageDataMap) {
        imageData->textureSetupComplete = false;
    }

    for (const auto &[path, textureName]: textureMap) {
        try {
            BufferedImage image;
            if (path.starts_with(u"##")) {
                image = unwrapImageByColumns(loadImageResource(path.substr(2)));
            } else if (path.starts_with(u"%%")) {
                clampTexture = true;
                image = loadImageResource(path.substr(2));
                clampTexture = false;
            } else {
                image = loadImageResource(path);
            }

            setupTexture(image, textureName);
        } catch (const std::exception &e) {
            printf("Failed to refresh texture: %s\n", e.what());
        }
    }

}

void RenderEngine::bindTexture(const int_t textureName) {
    if (textureName >= 0) {
        glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(textureName));
    }
}

BufferedImage RenderEngine::loadImageResource(const jstring &path) const {
    for (const auto &candidate: resourceCandidates(path)) {
        if (!std::filesystem::is_regular_file(candidate)) {
            continue;
        }

        return BufferedImage::readFile(String::fromUtf8(candidate.string()));
    }

    throw std::runtime_error("image resource not found: " + String::toUtf8(path));
}

BufferedImage RenderEngine::unwrapImageByColumns(const BufferedImage &image) const {
    const int_t columns = image.getWidth() / 16;
    BufferedImage unwrapped(16, image.getHeight() * columns);
    const auto &sourcePixels = image.getRgbaPixels();
    auto &destPixels = unwrapped.getRgbaPixels();

    for (int_t column = 0; column < columns; ++column) {
        for (int_t y = 0; y < image.getHeight(); ++y) {
            const std::size_t sourceOffset = (static_cast<std::size_t>(y) * image.getWidth() + column * 16) * 4;
            const std::size_t destOffset = (static_cast<std::size_t>(column * image.getHeight() + y) * 16) * 4;
            std::copy_n(sourcePixels.begin() + static_cast<long_t>(sourceOffset), 16 * 4,
                        destPixels.begin() + static_cast<long_t>(destOffset));
        }
    }

    return unwrapped;
}

int_t RenderEngine::averageColor(const int_t first, const int_t second) {
    const int_t firstAlpha = (first & static_cast<int_t>(0xFF000000)) >> 24 & 255;
    const int_t secondAlpha = (second & static_cast<int_t>(0xFF000000)) >> 24 & 255;
    return ((firstAlpha + secondAlpha) >> 1 << 24) + (((first & 0x00FEFEFE) + (second & 0x00FEFEFE)) >> 1);
}

int_t RenderEngine::alphaBlend(int_t first, int_t second) {
    int_t firstAlpha = (first & static_cast<int_t>(0xFF000000)) >> 24 & 255;
    int_t secondAlpha = (second & static_cast<int_t>(0xFF000000)) >> 24 & 255;
    short_t alpha = 255;
    if (firstAlpha + secondAlpha == 0) {
        firstAlpha = 1;
        secondAlpha = 1;
        alpha = 0;
    }

    const int_t firstRed = (first >> 16 & 255) * firstAlpha;
    const int_t firstGreen = (first >> 8 & 255) * firstAlpha;
    const int_t firstBlue = (first & 255) * firstAlpha;
    const int_t secondRed = (second >> 16 & 255) * secondAlpha;
    const int_t secondGreen = (second >> 8 & 255) * secondAlpha;
    const int_t secondBlue = (second & 255) * secondAlpha;
    const int_t red = (firstRed + secondRed) / (firstAlpha + secondAlpha);
    const int_t green = (firstGreen + secondGreen) / (firstAlpha + secondAlpha);
    const int_t blue = (firstBlue + secondBlue) / (firstAlpha + secondAlpha);
    return alpha << 24 | red << 16 | green << 8 | blue;
}

int_t RenderEngine::readInt(const std::vector<ubyte_t> &data, const std::size_t offset) {
    return static_cast<int_t>(data[offset]) << 24
           | static_cast<int_t>(data[offset + 1]) << 16
           | static_cast<int_t>(data[offset + 2]) << 8
           | static_cast<int_t>(data[offset + 3]);
}

void RenderEngine::writeInt(std::vector<ubyte_t> &data, const std::size_t offset, const int_t value) {
    data[offset] = static_cast<ubyte_t>(value >> 24 & 255);
    data[offset + 1] = static_cast<ubyte_t>(value >> 16 & 255);
    data[offset + 2] = static_cast<ubyte_t>(value >> 8 & 255);
    data[offset + 3] = static_cast<ubyte_t>(value & 255);
}
