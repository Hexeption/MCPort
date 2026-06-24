//
// Created by Keir Davis on 24/06/2026.
//

#include "ThreadDownloadImage.h"

#include <cstdio>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "ThreadDownloadImageData.h"
#include "utils/Networking.h"

namespace {
    std::vector<ubyte_t> readLocationBytes(const jstring &location) {
        std::string locationUtf8 = String::toUtf8(location);
        if (Networking::startsWith(Networking::lowerCase(locationUtf8), "file://")) {
            return Networking::readFileBytes(locationUtf8.substr(7));
        }

        if (Networking::startsWith(Networking::lowerCase(locationUtf8), "http://")) {
            return Networking::downloadHttpBytes(locationUtf8);
        }

        if (std::filesystem::is_regular_file(locationUtf8)) {
            return Networking::readFileBytes(locationUtf8);
        }

        throw std::runtime_error("unsupported image location: " + locationUtf8);
    }
}

ThreadDownloadImage::ThreadDownloadImage(ThreadDownloadImageData &imageData, jstring location,
                                         ImageBuffer *buffer) : imageData(imageData), location(std::move(location)),
                                                                buffer(buffer) {
}

ThreadDownloadImage::~ThreadDownloadImage() {
    if (thread.joinable()) {
        thread.join();
    }
}

void ThreadDownloadImage::start() {
    thread = std::thread(&ThreadDownloadImage::run, this);
}

void ThreadDownloadImage::run() {
    try {
        const std::vector<ubyte_t> encodedImage = readLocationBytes(location);
        if (encodedImage.empty()) {
            return;
        }

        BufferedImage decodedImage = BufferedImage::readMemory(encodedImage);
        if (buffer == nullptr) {
            imageData.image = std::make_unique<BufferedImage>(std::move(decodedImage));
        } else {
            imageData.image = std::make_unique<BufferedImage>(buffer->parseUserSkin(decodedImage));
        }
    } catch (const std::exception &e) {
        printf("Failed to download image: %s\n", e.what());
    }
}
