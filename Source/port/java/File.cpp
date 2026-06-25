#include "File.h"

#include "os/FileSystem.h"

const char_t File::separatorChar = OS::FileSystem::separatorChar();
const jstring File::separator = jstring(1, File::separatorChar);
const char_t File::pathSeparatorChar = OS::FileSystem::pathSeparatorChar();
const jstring File::pathSeparator = jstring(1, File::pathSeparatorChar);

File::File(const char *pathname) : path(String::fromUtf8(pathname == nullptr ? "" : pathname)) {
}

File::File(const std::string &pathname) : path(String::fromUtf8(pathname)) {
}

File::File(const jstring &pathname) : path(pathname) {
}

File::File(const File &parent, const jstring &child) : path(OS::FileSystem::join(parent.path, child)) {
}

File::File(const jstring &parent, const jstring &child) : path(OS::FileSystem::join(parent, child)) {
}

jstring File::getName() const {
    return OS::FileSystem::name(path);
}

jstring File::getParent() const {
    return OS::FileSystem::parent(path);
}

File File::getParentFile() const {
    return File(getParent());
}

jstring File::getPath() const {
    return path;
}

jstring File::getAbsolutePath() const {
    return OS::FileSystem::absolutePath(path);
}

File File::getAbsoluteFile() const {
    return File(getAbsolutePath());
}

jstring File::getCanonicalPath() const {
    return OS::FileSystem::canonicalPath(path);
}

File File::getCanonicalFile() const {
    return File(getCanonicalPath());
}

bool File::isAbsolute() const {
    return OS::FileSystem::isAbsolute(path);
}

bool File::exists() const {
    return OS::FileSystem::exists(path);
}

bool File::isDirectory() const {
    return OS::FileSystem::isDirectory(path);
}

bool File::isFile() const {
    return OS::FileSystem::isRegularFile(path);
}

bool File::mkdir() const {
    return OS::FileSystem::createDirectory(path);
}

bool File::mkdirs() const {
    return OS::FileSystem::createDirectories(path);
}

bool File::deleteFile() const {
    return OS::FileSystem::remove(path);
}

bool File::delete_() const {
    return deleteFile();
}

bool File::renameTo(const File &dest) const {
    return OS::FileSystem::rename(path, dest.path);
}

long_t File::length() const {
    return OS::FileSystem::fileSize(path);
}

long_t File::lastModified() const {
    return OS::FileSystem::lastModifiedMillis(path);
}

std::vector<File> File::listFiles() const {
    std::vector < File > files;
    for (const auto &filePath: OS::FileSystem::listFiles(path)) {
        files.emplace_back(filePath);
    }
    return files;
}

std::string File::toUtf8() const {
    return String::toUtf8(path);
}
