//
// Created by Keir Davis on 24/06/2026.
//

#include "ThreadDownloadImageData.h"

#include "ThreadDownloadImage.h"

ThreadDownloadImageData::ThreadDownloadImageData(const jstring &url, ImageBuffer *buffer) {
    downloadThread = std::make_unique<ThreadDownloadImage>(*this, url, buffer);
    downloadThread->start();
}

ThreadDownloadImageData::~ThreadDownloadImageData() = default;
