//
// Created by Keir Davis on 23/06/2026.
//

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Textures.h"

#include <filesystem>
#include <unordered_map>
#include <stdexcept>

#include <glad/glad.h>

namespace {
    std::unordered_map<std::string, int> idMap;

    std::filesystem::path resolveResourcePath(const std::string &resourceName) {
        std::string relative = resourceName;
        if (!relative.empty() && relative.front() == '/') {
            relative.erase(relative.begin());
        }

        const std::filesystem::path candidates[] = {
            relative,
            std::filesystem::path("Assets") / relative,
            std::filesystem::path("../Assets") / relative,
        };

        for (const auto &candidate : candidates) {
            if (std::filesystem::exists(candidate)) {
                return candidate;
            }
        }

        return candidates[0];
    }
}

namespace Textures {
    int loadTexture(const std::string &resourceName, int mode) {
        auto found = idMap.find(resourceName);
        if (found != idMap.end()) {
            return found->second;
        }

        const std::filesystem::path path = resolveResourcePath(resourceName);
        int width = 0;
        int height = 0;
        int channels = 0;
        stbi_set_flip_vertically_on_load(false);
        unsigned char *pixels = stbi_load(path.string().c_str(), &width, &height, &channels, STBI_rgb_alpha);
        if (pixels == nullptr) {
            throw std::runtime_error("failed to load texture: " + path.string());
        }

        GLuint id = 0;
        glGenTextures(1, &id);
        idMap[resourceName] = static_cast<int>(id);

        glBindTexture(GL_TEXTURE_2D, id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mode);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

        stbi_image_free(pixels);
        return static_cast<int>(id);
    }
}
