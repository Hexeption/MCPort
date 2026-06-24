//
// Created by Keir Davis on 24/06/2026.
//

#ifndef MCPORT_THREADDOWNLOADIMAGEDATA_H
#define MCPORT_THREADDOWNLOADIMAGEDATA_H

#include <memory>

#include "java/BufferedImage.h"
#include "ImageBuffer.h"
#include "java/String.h"

class ThreadDownloadImage;

class ThreadDownloadImageData {
public:
    std::unique_ptr<BufferedImage> image;
    int_t referenceCount = 1;
    int_t textureName = -1;
    bool textureSetupComplete = false;

    ThreadDownloadImageData(const jstring &url, ImageBuffer *buffer);

    ~ThreadDownloadImageData();

private:
    std::unique_ptr<ThreadDownloadImage> downloadThread;
};

#endif //MCPORT_THREADDOWNLOADIMAGEDATA_H
