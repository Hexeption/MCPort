#include "os/FileSystem.h"

#include <filesystem>
#include <string>
#include <windows.h>

namespace {
    std::wstring toWide(const jstring &path) {
        return std::wstring(path.begin(), path.end());
    }

    jstring fromWide(const std::wstring &path) {
        return jstring(path.begin(), path.end());
    }

    std::filesystem::path toPath(const jstring &path) {
        return std::filesystem::path(toWide(path));
    }

    jstring fromPath(const std::filesystem::path &path) {
        return fromWide(path.wstring());
    }
}

namespace OS::FileSystem {
    char_t separatorChar() {
        return '\\';
    }

    char_t pathSeparatorChar() {
        return ';';
    }

    jstring join(const jstring &parent, const jstring &child) {
        if (parent.empty()) {
            return child;
        }
        return fromPath(toPath(parent) / toPath(child));
    }

    jstring name(const jstring &path) {
        return fromPath(toPath(path).filename());
    }

    jstring parent(const jstring &path) {
        return fromPath(toPath(path).parent_path());
    }

    bool isAbsolute(const jstring &path) {
        return toPath(path).is_absolute();
    }

    jstring absolutePath(const jstring &path) {
        std::error_code ec;
        const auto absolute = std::filesystem::absolute(toPath(path), ec);
        return ec ? path : fromPath(absolute);
    }

    jstring canonicalPath(const jstring &path) {
        std::error_code ec;
        auto canonical = std::filesystem::canonical(toPath(path), ec);
        if (!ec) {
            return fromPath(canonical);
        }

        canonical = std::filesystem::weakly_canonical(toPath(path), ec);
        return ec ? absolutePath(path) : fromPath(canonical);
    }

    bool exists(const jstring &path) {
        std::error_code ec;
        return std::filesystem::exists(toPath(path), ec);
    }

    bool isDirectory(const jstring &path) {
        std::error_code ec;
        return std::filesystem::is_directory(toPath(path), ec);
    }

    bool isRegularFile(const jstring &path) {
        std::error_code ec;
        return std::filesystem::is_regular_file(toPath(path), ec);
    }

    bool createDirectory(const jstring &path) {
        std::error_code ec;
        return std::filesystem::create_directory(toPath(path), ec);
    }

    bool createDirectories(const jstring &path) {
        std::error_code ec;
        return std::filesystem::create_directories(toPath(path), ec) || std::filesystem::is_directory(toPath(path), ec);
    }

    bool remove(const jstring &path) {
        std::error_code ec;
        return std::filesystem::remove(toPath(path), ec);
    }

    bool rename(const jstring &from, const jstring &to) {
        std::error_code ec;
        std::filesystem::rename(toPath(from), toPath(to), ec);
        return !ec;
    }

    long_t fileSize(const jstring &path) {
        std::error_code ec;
        const auto size = std::filesystem::file_size(toPath(path), ec);
        return ec ? 0 : static_cast<long_t>(size);
    }

    long_t lastModifiedMillis(const jstring &path) {
        WIN32_FILE_ATTRIBUTE_DATA data{};
        if (!GetFileAttributesExW(toWide(path).c_str(), GetFileExInfoStandard, &data)) {
            return 0;
        }

        ULARGE_INTEGER value{};
        value.LowPart = data.ftLastWriteTime.dwLowDateTime;
        value.HighPart = data.ftLastWriteTime.dwHighDateTime;
        return static_cast<long_t>((value.QuadPart - 116444736000000000ULL) / 10000ULL);
    }

    std::vector<jstring> listFiles(const jstring &path) {
        std::vector<jstring> files;
        std::error_code ec;
        for (const auto &entry: std::filesystem::directory_iterator(toPath(path), ec)) {
            files.push_back(fromPath(entry.path()));
        }
        return files;
    }
}
