#ifndef MCPORT_FILESYSTEM_H
#define MCPORT_FILESYSTEM_H

#include <vector>

#include "java/String.h"
#include "java/Type.h"

namespace OS::FileSystem {
    char_t separatorChar();

    char_t pathSeparatorChar();

    jstring join(const jstring &parent, const jstring &child);

    jstring name(const jstring &path);

    jstring parent(const jstring &path);

    bool isAbsolute(const jstring &path);

    jstring absolutePath(const jstring &path);

    jstring canonicalPath(const jstring &path);

    bool exists(const jstring &path);

    bool isDirectory(const jstring &path);

    bool isRegularFile(const jstring &path);

    bool createDirectory(const jstring &path);

    bool createDirectories(const jstring &path);

    bool remove(const jstring &path);

    bool rename(const jstring &from, const jstring &to);

    long_t fileSize(const jstring &path);

    long_t lastModifiedMillis(const jstring &path);

    std::vector<jstring> listFiles(const jstring &path);
}

#endif //MCPORT_FILESYSTEM_H
