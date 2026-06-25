//
// Created by Keir Davis on 24/06/2026.
//

#include "Networking.h"

#include <algorithm>
#include <array>
#include <cctype>
#include <fstream>
#include <stdexcept>
#include <string_view>

#if defined(_WIN32)
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

namespace {
#if defined(_WIN32)
    void closeSocket(const SOCKET socketFd) {
        closesocket(socketFd);
    }
#else
    void closeSocket(const int socketFd) {
        close(socketFd);
    }
#endif
}

namespace Networking {
    std::string lowerCase(std::string value) {
        std::ranges::transform(value, value.begin(), [](const unsigned char ch) {
            return static_cast<char>(std::tolower(ch));
        });
        return value;
    }

    bool startsWith(const std::string &value, const std::string &prefix) {
        return value.rfind(prefix, 0) == 0;
    }

    ParsedHttpUrl parseHttpUrl(const std::string &url) {
        constexpr std::string_view scheme = "http://";
        if (!startsWith(lowerCase(url), std::string(scheme))) {
            throw std::runtime_error("unsupported image URL: " + url);
        }

        ParsedHttpUrl parsed;
        const std::string withoutScheme = url.substr(scheme.size());
        const std::size_t slash = withoutScheme.find('/');
        const std::string authority = slash == std::string::npos ? withoutScheme : withoutScheme.substr(0, slash);
        parsed.path = slash == std::string::npos ? "/" : withoutScheme.substr(slash);

        const std::size_t colon = authority.rfind(':');
        if (colon == std::string::npos) {
            parsed.host = authority;
        } else {
            parsed.host = authority.substr(0, colon);
            parsed.port = authority.substr(colon + 1);
        }

        if (parsed.host.empty()) {
            throw std::runtime_error("missing image URL host: " + url);
        }

        return parsed;
    }

    std::vector<ubyte_t> readFileBytes(const std::filesystem::path &path) {
        std::ifstream stream(path, std::ios::binary);
        if (!stream) {
            throw std::runtime_error("failed to open image file: " + path.string());
        }

        return std::vector<ubyte_t>(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
    }

    std::vector<ubyte_t> downloadHttpBytes(const std::string &url) {
        const ParsedHttpUrl parsed = parseHttpUrl(url);

#if defined(_WIN32)
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            throw std::runtime_error("failed to initialize winsock");
        }
#endif

        addrinfo hints{};
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;

        addrinfo *results = nullptr;
        if (getaddrinfo(parsed.host.c_str(), parsed.port.c_str(), &hints, &results) != 0) {
#if defined(_WIN32)
            WSACleanup();
#endif
            throw std::runtime_error("failed to resolve image host: " + parsed.host);
        }

#if defined(_WIN32)
        SOCKET socketFd = INVALID_SOCKET;
#else
        int socketFd = -1;
#endif
        for (addrinfo *result = results; result != nullptr; result = result->ai_next) {
            socketFd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
#if defined(_WIN32)
            if (socketFd == INVALID_SOCKET) {
#else
            if (socketFd < 0) {
#endif
                continue;
            }

            if (connect(socketFd, result->ai_addr, static_cast<int>(result->ai_addrlen)) == 0) {
                break;
            }

            closeSocket(socketFd);
#if defined(_WIN32)
            socketFd = INVALID_SOCKET;
#else
            socketFd = -1;
#endif
        }

        freeaddrinfo(results);
#if defined(_WIN32)
        if (socketFd == INVALID_SOCKET) {
            WSACleanup();
#else
        if (socketFd < 0) {
#endif
            throw std::runtime_error("failed to connect to image host: " + parsed.host);
        }

        const std::string request = "GET " + parsed.path + " HTTP/1.0\r\nHost: " + parsed.host +
                                    "\r\nUser-Agent: MCPort\r\nConnection: close\r\n\r\n";
        const char *requestData = request.data();
        std::size_t remaining = request.size();
        while (remaining > 0) {
            const int sent = send(socketFd, requestData, static_cast<int>(remaining), 0);
            if (sent <= 0) {
                closeSocket(socketFd);
#if defined(_WIN32)
                WSACleanup();
#endif
                throw std::runtime_error("failed to send image request");
            }

            requestData += sent;
            remaining -= static_cast<std::size_t>(sent);
        }

        std::string response;
        std::array < char, 4096 > buffer{};
        for (;;) {
            const int received = recv(socketFd, buffer.data(), static_cast<int>(buffer.size()), 0);
            if (received <= 0) {
                break;
            }
            response.append(buffer.data(), static_cast<std::size_t>(received));
        }

        closeSocket(socketFd);
#if defined(_WIN32)
        WSACleanup();
#endif

        const std::size_t statusEnd = response.find("\r\n");
        if (statusEnd == std::string::npos) {
            throw std::runtime_error("invalid image HTTP response");
        }

        const std::string statusLine = response.substr(0, statusEnd);
        if (statusLine.find(" 404") != std::string::npos) {
            return {};
        }

        const std::size_t bodyStart = response.find("\r\n\r\n");
        if (bodyStart == std::string::npos) {
            throw std::runtime_error("image HTTP response has no body");
        }

        const char *body = response.data() + bodyStart + 4;
        const std::size_t bodySize = response.size() - bodyStart - 4;
        std::vector<ubyte_t> bodyBytes(bodySize);
        std::ranges::transform(body, body + bodySize, bodyBytes.begin(), [](const char value) {
            return static_cast<ubyte_t>(value);
        });
        return bodyBytes;
    }
}
