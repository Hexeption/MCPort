//
// Created by Keir Davis on 24/06/2026.
//

#ifndef MCPORT_BUFFEREDIMAGE_H
#define MCPORT_BUFFEREDIMAGE_H

#include <vector>

#include "String.h"
#include "Type.h"

class BufferedImage {
public:
    BufferedImage() = default;

    BufferedImage(int_t width, int_t height);

    BufferedImage(int_t width, int_t height, std::vector<ubyte_t> rgbaPixels);

    int_t getWidth() const;

    int_t getHeight() const;

    const std::vector<ubyte_t> &getRgbaPixels() const;

    std::vector<ubyte_t> &getRgbaPixels();

    bool empty() const;

    static BufferedImage readFile(const jstring &path);

    static BufferedImage readMemory(const std::vector<ubyte_t> &encodedImage);

private:
    int_t width = 0;
    int_t height = 0;
    std::vector<ubyte_t> rgbaPixels;
};

#endif //MCPORT_BUFFEREDIMAGE_H
