//
// Created by Keir Davis on 25/06/2026.
//

#include "CompressedStreamTools.h"

#include <sstream>
#include <stdexcept>
#include <vector>

#include <zlib.h>

#include "NBTBase.h"
#include "java/DataInput.h"
#include "java/DataOutput.h"

std::unique_ptr<NBTTagCompound> CompressedStreamTools::readCompressed(const File &file) {
    gzFile gz = gzopen(file.toUtf8().c_str(), "rb");
    if (gz == nullptr) {
        throw std::ios_base::failure("Failed to open compressed NBT file");
    }

    std::string bytes;
    std::vector<char> buffer(8192);
    int read = 0;
    while ((read = gzread(gz, buffer.data(), static_cast<unsigned int>(buffer.size()))) > 0) {
        bytes.append(buffer.data(), static_cast<std::size_t>(read));
    }

    const int closeResult = gzclose(gz);
    if (read < 0 || closeResult != Z_OK) {
        throw std::ios_base::failure("Failed to read compressed NBT file");
    }

    std::istringstream stream(bytes, std::ios::binary);
    DataInput input(stream);
    std::unique_ptr<NBTBase> tag(NBTBase::readNamedTag(input));
    if (tag == nullptr || tag->getType() != 10) {
        throw std::ios_base::failure("Root tag must be a named compound tag");
    }

    return std::unique_ptr<NBTTagCompound>(static_cast<NBTTagCompound *>(tag.release()));
}

void CompressedStreamTools::writeCompressed(NBTTagCompound &compound, const File &file) {
    std::ostringstream stream(std::ios::binary);
    DataOutput output(stream);
    NBTBase::writeNamedTag(&compound, output);
    const std::string bytes = stream.str();

    gzFile gz = gzopen(file.toUtf8().c_str(), "wb");
    if (gz == nullptr) {
        throw std::ios_base::failure("Failed to open compressed NBT file");
    }

    const int written = gzwrite(gz, bytes.data(), static_cast<unsigned int>(bytes.size()));
    const int closeResult = gzclose(gz);
    if (written != static_cast<int>(bytes.size()) || closeResult != Z_OK) {
        throw std::ios_base::failure("Failed to write compressed NBT file");
    }
}
