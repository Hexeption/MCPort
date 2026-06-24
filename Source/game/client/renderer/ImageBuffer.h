//
// Created by Keir Davis on 24/06/2026.
//

#ifndef MCPORT_IMAGEBUFFER_H
#define MCPORT_IMAGEBUFFER_H

#include "port/java/BufferedImage.h"

class ImageBuffer {
public:
    virtual ~ImageBuffer() = default;

    virtual BufferedImage parseUserSkin(const BufferedImage &image) = 0;
};

#endif //MCPORT_IMAGEBUFFER_H
