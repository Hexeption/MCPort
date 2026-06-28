//
// Created by Keir Davis on 24/06/2026.
//

#ifndef MCPORT_THREADDOWNLOADIMAGE_H
#define MCPORT_THREADDOWNLOADIMAGE_H

#include <thread>

#include "ImageBuffer.h"
#include "java/String.h"

#include "game/client/renderer/ThreadDownloadImageData.h"

class ThreadDownloadImage {
public:
    ThreadDownloadImage(ThreadDownloadImageData &imageData, jstring location, ImageBuffer *buffer);

    ~ThreadDownloadImage();

    void start();

    void run();

private:
    ThreadDownloadImageData &imageData;
    jstring location;
    ImageBuffer *buffer;
    std::thread thread;
};

#endif //MCPORT_THREADDOWNLOADIMAGE_H
