//
// Created by Keir Davis on 25/06/2026.
//

#ifndef MCPORT_COMPRESSEDSTREAMTOOLS_H
#define MCPORT_COMPRESSEDSTREAMTOOLS_H

#include <memory>

#include "NBTTagCompound.h"
#include "java/File.h"

class CompressedStreamTools {
public:
    static std::unique_ptr<NBTTagCompound> readCompressed(const File &file);

    static void writeCompressed(NBTTagCompound &compound, const File &file);
};

#endif //MCPORT_COMPRESSEDSTREAMTOOLS_H
