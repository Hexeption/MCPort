//
// Created by Keir Davis on 24/06/2026.
//

#ifndef MCPORT_NETWORKING_H
#define MCPORT_NETWORKING_H

#include <filesystem>
#include <string>
#include <vector>

#include "java/Type.h"

namespace Networking {
    struct ParsedHttpUrl {
        std::string host;
        std::string port = "80";
        std::string path = "/";
    };

    std::string lowerCase(std::string value);

    bool startsWith(const std::string &value, const std::string &prefix);

    ParsedHttpUrl parseHttpUrl(const std::string &url);

    std::vector<ubyte_t> readFileBytes(const std::filesystem::path &path);

    std::vector<ubyte_t> downloadHttpBytes(const std::string &url);
}

#endif //MCPORT_NETWORKING_H
